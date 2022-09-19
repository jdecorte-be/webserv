#include "../server/server.hpp"
#include "../parsing/requete.hpp"
#include "../parsing/webserv.hpp"

bool is_request_done(char *request, size_t header_size, size_t sizereq)
{
    size_t sizebody = sizereq - header_size;

	char *body = strstr(request, "\r\n\r\n");
	if (!body)
		return false;
	body += 4;
	if (strnstr(request, "chunked", sizereq - sizebody))
	{
		if (strstr(body, "\r\n\r\n"))
			return true;
		return false;
	}
	else if (strnstr(request, "Content-Length", sizereq - sizebody))
	{
		char *start = strnstr(request, "Content-Length: ", sizereq - sizebody) + 16;
		char *end = strstr(start, "\r\n");
		char *len = strndup(start, end - start);
		int len_i = atoi(len);
		free(len);
		if ((size_t)len_i <= sizebody)
			return true;
		return false;
	}
	else if (strnstr(request, "boundary=", sizereq - sizebody))
	{
		if (strstr(body, "\r\n\r\n"))
			return true;
		return false;
	}
	return true;
}

void Server::addtowait(int socket, fd_set *set)
{
    FD_SET(socket, set);
    if(socket > max_fd)
        max_fd = socket;
}

void Server::selectfd(fd_set *read, fd_set *write)
{
    int r = 0;
    if((r = select(max_fd + 1, read, write, 0, 0)) < 0)
        exit(-1);
    else if (r == 0)
        std::cout << "Select time out" << std::endl;
    writeSet = *write;
    readSet = *read;
}


void Server::waitClient()
{
    fd_set read;
    fd_set write;

    FD_ZERO(&read);
    FD_ZERO(&write);
    for(size_t i = 0; i < sockets.size(); i++) // Set fd of server
        addtowait(sockets[i].getServerSocket(), &read);
    for(size_t i = 0; i < clients.size(); i++) // Set fd of server
        addtowait(clients[i].getClientSocket(), &read);
    selectfd(&read, &write);

}

void Server::acceptClient()
{
    sockaddr_in addrclient;
    socklen_t clientSize = sizeof(addrclient);

    for(size_t i = 0; i < sockets.size(); i++)
    {
        if(FD_ISSET(sockets[i].getServerSocket(), &readSet))
        {
            Client client;
            client.init(i);
            client.setSocketClient(accept(sockets[i].getServerSocket(), (struct sockaddr *)&addrclient, &clientSize));
            clients.push_back(client);
            if(client.getClientSocket() < 0)
            {
                perror("Connect");
                exit(-1);
            }
            std::cout << colors::green << "New connection !" << std::endl;//Tim
        }
    }
}

void Server::handleRequest()
{
    for(size_t i = 0; i < clients.size(); i++)
    {
        if(FD_ISSET(clients[i].getClientSocket(), &readSet))
        {

            size_t Reqsize = recv(clients[i].getClientSocket(), clients[i].request, MAX_REQUEST_SIZE, 0);
            clients[i].requestSize += Reqsize;

            for (size_t size = 0; size < Reqsize; size++)
                clients[i].final_request.push_back(clients[i].request[size]);

            int header_size = clients[i].final_request.find("\r\n\r\n", 0);
            header_size += 4;

            if (Reqsize < 0)
            {
                std::cout << "Recv failed !" << std::endl;
                showError(500, clients[i]);
                kill_client(clients[i]);
                i--;
            }
            else if(Reqsize == 0)
            {
                std::cout << colors::on_bright_red << "Connection is closed !" << colors::on_grey << std::endl;
                kill_client(clients[i]);
                i--;
            }
            else if(is_request_done((char *)clients[i].final_request.c_str(), header_size, clients[i].requestSize))
            {
                std::cout << colors::bright_cyan << "== New Request ! ==" << std::endl;
                Requete requete((char*)clients[i].final_request.c_str());
                int ret = -1;
                if ((ret = requete.check_tim()) != -1)
                {
                    showError(ret, clients[i]);
                    if(kill_client(clients[i], requete))
                        i--;
                    continue;       
                }
                std::string urlrcv = requete.getUrl();
                size_t pos;
                if((pos = urlrcv.rfind("?")) != std::string::npos)
                {
                    query = urlrcv.substr(pos, urlrcv.size());
                    urlrcv = urlrcv.substr(0, pos);
                }
                if(requete.getLen() != std::string::npos && requete.getLen() > (size_t)stoi(servers[clients[i].getNServer()]->getBody()))
                {
                    showError(413, clients[i]);
                    if(kill_client(clients[i], requete))
                        i--;
                    continue;
                }
                
                loc = getLocation(urlrcv, clients[i].getNServer());

                if(!((loc == NULL) ? is_allowed(servers[clients[i].getNServer()]->getMethod(),
                    requete.getMethod()) : is_allowed(loc->getMethod(), requete.getMethod()))
                        && urlrcv.find("cgi_bin") == std::string::npos)
                {
                    std::cout << "Unautorised Method " << requete.getMethod() << " !" << std::endl;
                    showError(405, clients[i]);
                    if(kill_client(clients[i], requete))
                        i--;
                    continue;
                }
                if (is_cgi(urlrcv))
                {
                    std::cout << colors::blue << "CGI Start !" << colors::grey << std::endl;

                    std::string urlsend = getRootPatch(urlrcv, clients[i].getNServer());
                    std::string rescgi = execCGI(urlsend, envp, requete, servers[clients[i].getNServer()]);
                    if(rescgi.empty())
                        showError(404, clients[i]);

                    rescgi = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n" + rescgi;
                    int r = send(clients[i].getClientSocket(), rescgi.c_str(), rescgi.size(), 0);
                    if(r < 0)
                        showError(500, clients[i]);
                    else if (r == 0)
                        showError(400, clients[i]);
                }
                else
                {
                    if(loc && !loc->getRedir().empty())
                        do_redir(clients[i], loc->getRedir());
                    else if (requete.getMethod() == "GET")
                        getMethod(clients[i], urlrcv, requete);
                    else if (requete.getMethod() == "POST")
                        postMethod(clients[i], urlrcv, requete);
                    else if (requete.getMethod() == "DELETE")
                        deleteMethod(clients[i], urlrcv);
                }
                if(kill_client(clients[i], requete))
                    i--;
                if(i <= 0)
                {
                    clients[i].requestSize = 0;
                    bzero(clients[i].request, MAX_REQUEST_SIZE);
                }
            }
        }
    }
    usleep(10000);
}

void Server::getMethod(Client &client, std::string urlrcv, Requete req)
{
    (void)req;
    std::cout << colors::bright_yellow << "GET Method !" << std::endl;

    if(urlrcv.size() >= 64)
    {
        showError(414, client);
        return ;
    }
    struct stat path_stat;

    std::string urlsend = getRootPatch(urlrcv, client.getNServer());

    if(loc && !(loc->getIndex().empty()) && (strcmp(urlrcv.c_str(), \
        loc->getDir().c_str()) == 0))
    {
        showPage(client, loc->getRoot() + loc->getIndex(), 200);
        return ;
    }

    FILE *fd = fopen(urlsend.c_str(), "rb");
    stat(urlsend.c_str(), &path_stat);

    if(fd == NULL)
    {
        std::cout << colors::on_bright_blue << "Ressource not find : "<< urlsend << colors::on_grey << std::endl;
        showError(404, client);
    }
    else
    {
        if(S_ISDIR(path_stat.st_mode))
        {
            std::cout << colors::on_bright_blue << "File is a directory !" << colors::on_grey << std::endl;

            if(strcmp(urlrcv.c_str(), "/") == 0)
                showPage(client, urlsend + servers[client.getNServer()]->getIndex(), 200);
            else if(servers[client.getNServer()]->getListing() == "on" || (loc && loc->getListing() == "on"))
                rep_listing(client.getClientSocket(), urlrcv, urlsend, client);
            else
                showError(404, client);
        }
        else
            showPage(client, urlsend, 200);
        fclose(fd);
    }
}

void Server::deleteMethod(Client &client, std::string urlrcv)
{
    std::cout << colors::bright_yellow << "DELETE Method !" << std::endl;
    std::string urlsend = getRootPatch(urlrcv, client.getNServer());


    FILE *fd = fopen(urlsend.c_str(), "r");
    if(!fd)
    {
        showError(404, client);
        return ;
    }
    fclose(fd);
    std::remove(urlsend.c_str());

    std::string tosend = "HTTP/1.1 200 OK\n";
    int ret = send(client.getClientSocket() ,tosend.c_str(), tosend.size(), 0);
    if(ret < 0)
        showError(500, client);
    else if (ret == 0)
		showError(400, client);
    std::cout << colors::green << urlsend << " as been delete !" << std::endl;
}

void Server::postMethod(Client client, std::string url, Requete req)
{

    if(req.getHeader()["Transfer-Encoding"] == "chunked")
    {
        showError(411, client);
        return;
    }
    std::string urlsend = getRootPatch(url, client.getNServer());
	struct stat buf;
	lstat(urlsend.c_str(), &buf);

    if(S_ISDIR(buf.st_mode))
    {

        std::string name;
        size_t start = 0;
        size_t end = 0;
        std::string body = req.getFullBody();
        std::string file;
        if(!(req.getHeader()["Content-Type"].empty()) && !(req.getBoundary().empty()))
        {
            std::cout << colors::on_cyan <<  "Post in directory : " << colors::on_grey << colors::green << std::endl;
            for(int i = 0; true; i++)
            {
                if((start = body.find("name=\"", start)) == std::string::npos)
                    break ;
                start += 6;
                if((end = body.find("\"", start)) == std::string::npos)
                    break ;
                name = body.substr(start, end - start);
                std::cout << "+ " + name << std::endl;

                if((start = body.find("\r\n\r\n", end)) == std::string::npos)
                    break ;
                start += 4;
                if((end = body.find(req.getBoundary(), start)) == std::string::npos)
                    break ;
                
                file = body.substr(start, end - start - 4);

                if(!writewithpoll(urlsend + "/" + name, client, file))
                    break ;

                if(body[end + req.getBoundary().size()] == '-')
                    break ;
            }
        }
        else
        {
            showError(400, client);
            return ;
        }
    }
    else
    {
        std::cout << "Post in file" << std::endl;
        if(!writewithpoll(urlsend, client, req))
            return ;
    }
    if(req.getLen() == 0)
        showPage(client, "", 204);
    else
        showPage(client, "", 201);
}

