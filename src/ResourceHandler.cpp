#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler(Config &config, Client &client) : _client(client), _servers(config.get_servers()) {

    this->httpResponses.insert(std::make_pair(100, "100 Continue"));
    this->httpResponses.insert(std::make_pair(101, "101 Switching Protocols"));
    this->httpResponses.insert(std::make_pair(200, "200 OK"));
    this->httpResponses.insert(std::make_pair(201, "201 Created"));
    this->httpResponses.insert(std::make_pair(202, "202 Accepted"));
    this->httpResponses.insert(std::make_pair(203, "203 Non-Authoritative Information"));
    this->httpResponses.insert(std::make_pair(204, "204 No Content"));
    this->httpResponses.insert(std::make_pair(205, "205 Reset Content"));
    this->httpResponses.insert(std::make_pair(206, "206 Partial Content"));
    this->httpResponses.insert(std::make_pair(300, "300 Multiple Choices"));
    this->httpResponses.insert(std::make_pair(301, "301 Moved Permanently"));
    this->httpResponses.insert(std::make_pair(302, "302 Found"));
    this->httpResponses.insert(std::make_pair(303, "303 See Other"));
    this->httpResponses.insert(std::make_pair(304, "304 Not Modified"));
    this->httpResponses.insert(std::make_pair(305, "305 Use Proxy"));
    this->httpResponses.insert(std::make_pair(307, "307 Temporary Redirect"));
    this->httpResponses.insert(std::make_pair(400, "400 Bad Request"));
    this->httpResponses.insert(std::make_pair(401, "401 Unauthorized"));
    this->httpResponses.insert(std::make_pair(402, "402 Payment Required"));
    this->httpResponses.insert(std::make_pair(403, "403 Forbidden"));
    this->httpResponses.insert(std::make_pair(404, "404 Not Found"));
    this->httpResponses.insert(std::make_pair(405, "405 Method Not Allowed"));
    this->httpResponses.insert(std::make_pair(406, "406 Not Acceptable"));
    this->httpResponses.insert(std::make_pair(407, "407 Proxy Authentication Required"));
    this->httpResponses.insert(std::make_pair(408, "408 Request Timeout"));
    this->httpResponses.insert(std::make_pair(409, "409 Conflict"));
    this->httpResponses.insert(std::make_pair(410, "410 Gone"));
    this->httpResponses.insert(std::make_pair(411, "411 Length Required"));
    this->httpResponses.insert(std::make_pair(412, "412 Precondition Failed"));
    this->httpResponses.insert(std::make_pair(413, "413 Request Entity Too Large"));
    this->httpResponses.insert(std::make_pair(414, "414 Request-URI Too Long"));
    this->httpResponses.insert(std::make_pair(415, "415 Unsupported Media Type"));
    this->httpResponses.insert(std::make_pair(416, "416 Requested Range Not Satisfiable"));
    this->httpResponses.insert(std::make_pair(417, "417 Expectation Failed"));
    this->httpResponses.insert(std::make_pair(500, "500 Internal Server Error"));
    this->httpResponses.insert(std::make_pair(501, "501 Not Implemented"));
    this->httpResponses.insert(std::make_pair(502, "502 Bad Gateway"));
    this->httpResponses.insert(std::make_pair(503, "503 Service Unavailable"));
    this->httpResponses.insert(std::make_pair(504, "504 Gateway Timeout"));
    this->httpResponses.insert(std::make_pair(505, "505 HTTP Version Not Supported"));

    this->_mimeTypes["aac"] = "audio/aac";
    this->_mimeTypes["abw"] = "application/x-abiword";
    this->_mimeTypes["arc"] = "application/octet-stream";
    this->_mimeTypes["avi"] = "video/x-msvideo";
    this->_mimeTypes["azw"] = "application/vnd.amazon.ebook";
    this->_mimeTypes["bin"] = "application/octet-stream";
    this->_mimeTypes["bz"] = "application/x-bzip";
    this->_mimeTypes["bz2"] = "application/x-bzip2";
    this->_mimeTypes["csh"] = "application/x-csh";
    this->_mimeTypes["css"] = "text/css";
    this->_mimeTypes["csv"] = "text/csv";
    this->_mimeTypes["doc"] = "application/msword";
    this->_mimeTypes["epub"] = "application/epub+zip";
    this->_mimeTypes["gif"] = "image/gif";
    this->_mimeTypes["htm"] = "text/html";
    this->_mimeTypes["html"] = "text/html";
    this->_mimeTypes["ico"] = "image/x-icon";
    this->_mimeTypes["ics"] = "text/calendar";
    this->_mimeTypes["jar"] = "application/java-archive";
    this->_mimeTypes["jpeg"] = "image/jpeg";
    this->_mimeTypes["jpg"] = "image/jpeg";
    this->_mimeTypes["js"] = "application/javascript";
    this->_mimeTypes["json"] = "application/json";
    this->_mimeTypes["mid"] = "audio/midi";
    this->_mimeTypes["midi"] = "audio/midi";
    this->_mimeTypes["mpeg"] = "video/mpeg";
    this->_mimeTypes["mpkg"] = "application/vnd.apple.installer+xml";
    this->_mimeTypes["odp"] = "application/vnd.oasis.opendocument.presentation";
    this->_mimeTypes["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    this->_mimeTypes["odt"] = "application/vnd.oasis.opendocument.text";
    this->_mimeTypes["oga"] = "audio/ogg";
    this->_mimeTypes["ogv"] = "video/ogg";
    this->_mimeTypes["ogx"] = "application/ogg";
    this->_mimeTypes["pdf"] = "application/pdf";
    this->_mimeTypes["ppt"] = "application/vnd.ms-powerpoint";
    this->_mimeTypes["rar"] = "application/x-rar-compressed";
    this->_mimeTypes["rtf"] = "application/rtf";
    this->_mimeTypes["sh"] = "application/x-sh";
    this->_mimeTypes["svg"] = "image/svg+xml";
    this->_mimeTypes["swf"] = "application/x-shockwave-flash";
    this->_mimeTypes["tar"] = "application/x-tar";
    this->_mimeTypes["tif"] = "image/tiff";
    this->_mimeTypes["tiff"] = "image/tiff";
    this->_mimeTypes["ttf"] = "font/ttf";
    this->_mimeTypes["vsd"] = "application/vnd.visio";
    this->_mimeTypes["wav"] = "audio/x-wav";
    this->_mimeTypes["weba"] = "audio/webm";
    this->_mimeTypes["webm"] = "video/webm";
    this->_mimeTypes["mp4"] = "video/mp4";
    this->_mimeTypes["webp"] = "image/webp";
    this->_mimeTypes["woff"] = "font/woff";
    this->_mimeTypes["woff2"] = "font/woff2";
    this->_mimeTypes["xhtml"] = "application/xhtml+xml";
    this->_mimeTypes["xls"] = "application/vnd.ms-excel";
    this->_mimeTypes["xml"] = "application/xml";
    this->_mimeTypes["xul"] = "application/vnd.mozilla.xul+xml";
    this->_mimeTypes["zip"] = "application/zip";
    this->_mimeTypes["3gp"] = "video/3gpp";
    this->_mimeTypes["audio/3gpp"] = "audio/3gpp";
    this->_mimeTypes["3g2"] = "video/3gpp2";
    this->_mimeTypes["audio/3gpp2"] = "audio/3gpp2";
    this->_mimeTypes["7z"] = "application/x-7z-compressed";
}

response_t ResourceHandler::handle_request() {
    if (_client.get_bad_request() != 0)
        return dynamic_page(_client.get_bad_request(), false, _servers[0]);
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if (it->get_server_name() == _client.get_request().headers["Host"] && _client.get_port() == it->get_port())
        {
            return handle_location(*it , it->get_locations());
        }
        if (it->get_server_name() + ":" + it->get_listen() == _client.get_request().headers["Host"] && _client.get_port() == it->get_port())
        {
            return handle_location(*it , it->get_locations());
        }
    }
    return dynamic_page(404, false, _servers[0]);
}

response_t ResourceHandler::handle_location(Server &server, std::vector<Location> &locations) {
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (location_match(it->get_locationName(), _client.get_request().path))
        {
             _target = it->get_root() + '/' + _client.get_request().path.substr(it->get_locationName().length(), _client.get_request().path.length());
             size_t pos = _target.find('?');
             if (pos != std::string::npos)
                _target = _target.substr(0, pos);
             std::cout << "Requested file or directory : " << _target << std::endl;
             return handle_method(server, *it);
        }
    }
    std::cout << "no location found" << std::endl;
    return dynamic_page(404, true, server);
}

bool ResourceHandler::location_match(std::string location, std::string path) {
    std::string tmp = path.substr(0, location.length());
    if (location == "/" && path[0] == '/')
        return true;
    if (tmp == location && (path[location.length()] == '/' || path[location.length()] == '\0'))
            return true;
    return false;
}

response_t ResourceHandler::handle_method(Server &server, Location &location) {
    if (location.isMethodAllowed(_client.get_request().method) == false)
        return dynamic_page(405, true, server);
    
    if (location.get_redirection() != "") {
           response_t response;
           response.init = true;
           response.body = false;
           response.headers = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + location.get_redirection() + "\r\n\r\n";
           return response;
    }
    if (_client.get_request().method == "GET") {
        return GET(server, location);   
    } else if (_client.get_request().method == "DELETE") {
            return DELETE(server, location);
    } else if (_client.get_request().method == "POST") {
        return POST(server, location);
    }
    return dynamic_page(405, true, server);
}

response_t ResourceHandler::GET(Server &server, Location &location) {
    response_t response;
    struct stat s;
    response.init = true;
    response.body = true;

    if (stat(_target.c_str(),&s) != 0) {
        if (errno == ENOENT)
            return dynamic_page(404, true, server);
        else if (errno == EACCES)
            return dynamic_page(403, true, server);
        else
            return dynamic_page(500, true, server);
    }

    if (to_cgi(server, _target))
        return handler_cgi(server, location, _target);

    if (s.st_mode & S_IFDIR) {
        std::string index = _target;
    
        if (location.get_index() != "") {
            return get_index(server, location);
        }

        index += (_target.back() != '/')? "/index.html" : "index.html";
        if (access(index.c_str(), R_OK) != -1) {
            int fd = open(index.c_str(), O_RDONLY);
            if (fd == -1)
                return dynamic_page(500, true, server);
            response.body_file = fd;
            response.headers = generate_headers("200", _client.get_request().method, index, fd);
            return response;
        }
        else if (location.get_autoIndex() == "on") {
            return get_directory(server, location);
        }
        else {
            return dynamic_page(404, true, server);
        }
    }

    int fd = open(_target.c_str(), O_RDONLY);
    if (fd == -1)
        return dynamic_page(500, true, server);

    response.body_file = fd;
    response.headers = generate_headers("200", _client.get_request().method, _target, fd);
    return response;
}

response_t ResourceHandler::POST(Server  &server, Location  &location)
{
    (void)server;
    (void)location;

    if (_client.get_request().body_lenght > std::atoi(server.get_clientMaxBodySize().c_str()))
        return dynamic_page(413, true, server);
    if (to_cgi(server, _target))
        return handler_cgi(server, location, _target);
    return dynamic_page(405, true, server);
}

response_t ResourceHandler::get_index(Server &server, Location &location)
{
    response_t response;

    response.init = true;
    response.body = true;

    std::string index = _target;
    index += (_target.back() != '/' && location.get_index()[0] != '/')? "/" + location.get_index() : location.get_index();

    if (to_cgi(server, index))
        return handler_cgi(server, location, index);
    
    int fd = open(index.c_str(), O_RDONLY);
    if (fd == -1)
        return dynamic_page(500, true, server);

    response.body_file = fd;
    response.headers = generate_headers("200", _client.get_request().method, index, fd);
    return response;
}

response_t ResourceHandler::get_directory(Server  &server, Location  &location) {
    std::string html;
    std::string htmlPath = "/tmp/" + random_string(15) + ".html";
    std::vector<std::string> files;
    response_t response;

    response.init = true;
    response.body = true;

    DIR* dir;
    int fd;
    struct dirent* entry;

    dir = opendir(_target.c_str());
    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                files.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
    html += "<!DOCTYPE html>\n";
    html += "<html>\n";
    html += "<head>\n";
    html += "<title>File List</title>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<h1>Autoindex</h1>\n";
    html += "<ul>\n";
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
        std::string a;
        a += "<li><a href=\"";
        a += location.get_locationName();
        a += _target.substr(location.get_root().length() + 1, std::string::npos);
        if (a[a.length() - 1] != '/')
            a += "/";
        a += *it + "\">" +  *it + "</a></li>\n";
        html += a;
    }
    html += "</ul>\n";
    html += "</body>\n";
    html += "</html>\n";

    fd = open(htmlPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        return dynamic_page(500, true, server);
    }
    if (write(fd, html.c_str(), html.length()) == -1) {
        return dynamic_page(500, true, server);
    }
    close(fd);
    fd = open(htmlPath.c_str(), O_RDONLY);
    if (fd == -1) {
        return dynamic_page(500, true, server);
    }
    response.body_file = fd;
    response.headers = generate_headers("200", _client.get_request().method, htmlPath, fd);
    return response;
}

bool can_delete(const char* path)
{
    struct stat path_info;
    if (stat(path, &path_info) != 0) 
        return false;

    if (S_ISDIR(path_info.st_mode))
    {
        if (access(path, R_OK | W_OK | X_OK) == 0)
            return true;
    }
    else
    {
        if (access(path, R_OK | W_OK) == 0)
            return true;
    }
    return false;
}

int delete_file(const char* path, const struct stat* sb, int typeflag, struct FTW* ftwbuf)
{
    (void) sb;
    (void) typeflag;
    (void) ftwbuf;
    if (remove(path) == -1) 
    {
        std::cerr << "Error deleting file/directory: " << strerror(errno) << std::endl;

    } else {
        std::cout << "Deleted file/directory: " << path << std::endl;
    }

    return 0;
}

response_t ResourceHandler::DELETE(Server  &server, Location  &location) {
    response_t response;

    (void)server;
    (void)location;
    response.init = true; 
    response.body = false;
    std::string path = _target;

    if (access(path.c_str(), F_OK) == -1)
    {
        response.headers = "HTTP/1.1 404 Not Found\r\n\r\n";
        return response;
    }
    struct stat path_info;
    if (stat(path.c_str(), &path_info) != 0)
    {
        response.headers = "HTTP/1.1 404 Not Found\r\n\r\n";
        return response;
    }
    else
    {
        if (S_ISDIR(path_info.st_mode))
        {
            if(path[path.size() - 1] != '/')
            {
                response.headers = "HTTP/1.1 409 Conflict\r\n\r\n";
                return response;
            }
            if(can_delete(path.c_str()))
            {
                if (nftw(path.c_str(), delete_file, 64, FTW_DEPTH | FTW_PHYS) == -1)
                {
                    response.headers = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
                    return response;
                }
                else
                {
                    response.headers = "HTTP/1.1 204 No Content\r\n\r\n";
                    return response;
                }
            }
            else
            {
                response.headers = "HTTP/1.1 403 Forbidden\r\n\r\n";
                return response;
            }
        } 
        else
        {
            if (can_delete(path.c_str()) == false)
            {
                response.headers = "HTTP/1.1 403 Forbidden\r\n\r\n";
                return response;
            }

            if (remove(path.c_str()) != 0)
            {
                response.headers = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
                return response;
            }
        }

    }
    response.headers = "HTTP/1.1 204 No Content\r\n\r\n";
    return response;
}

response_t ResourceHandler::handler_cgi(Server  &server, Location  &location, std::string script_path) {
    response_t response;

    response.init = true;
    response.cgi = true;

    response.cgi_response_file_name = "/tmp/" + random_string(15) + ".cgi";

    response.body_file = open(response.cgi_response_file_name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (response.body_file == -1)
        return dynamic_page(500, true, server);

    response.cgi_pid = fork();
    if (response.cgi_pid == -1)
        return dynamic_page(500, true, server);

    /*** Child Process ***/
    if (response.cgi_pid == 0)
    {
        char **env = set_cgi_envv(server, location, script_path);
        char *bin = get_cgi_bin(server, location, script_path);
        const char *argv[] = { bin , _target.c_str(), NULL};
        request_t request = _client.get_request();
        int input_fd;
    
        if (_client.get_request().method == "POST")
        {
            input_fd = open(request.body_file.c_str(), O_RDONLY);
            if (input_fd == -1)
                exit(1);

            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        dup2(response.body_file, STDOUT_FILENO);
        close(response.body_file);

        execve(bin, (char **)argv, (char **)env);
        exit(1);
    }

    close(response.body_file);
    return response;
}

char       **ResourceHandler::set_cgi_envv(Server  &server, Location  &location, std::string script_path) {
    std::map<std::string, std::string> headers = _client.get_request().headers;
    std::map<std::string, std::string> cgi_headers;

    cgi_headers["SERVER_SOFTWARE"] = "webserv";
    cgi_headers["SERVER_NAME"] = server.get_server_name();
    cgi_headers["GATEWAY_INTERFACE"] = "CGI/1.1";
    cgi_headers["SERVER_PROTOCOL"] = "HTTP/1.1";
    cgi_headers["SERVER_PORT"] = server.get_listen();
    cgi_headers["REQUEST_METHOD"] = _client.get_request().method;
    cgi_headers["DOCUMENT_ROOT"] = location.get_root();
    cgi_headers["PATH_INFO"] = _client.get_request().path.substr(0, _client.get_request().path.rfind('/'));
    cgi_headers["SCRIPT_NAME"] = _client.get_request().path.substr(_client.get_request().path.rfind('/') + 1, _client.get_request().path.find('?'));
    cgi_headers["SCRIPT_FILENAME"] = script_path;
    cgi_headers["DOCUMENT_URI"] = _client.get_request().path;
    cgi_headers["PATH"] = std::string(std::getenv("PATH"));
    cgi_headers["REQUEST_URI"] = _client.get_request().path;
    cgi_headers["UPLOAD_DIR"] = server.get_uploadPath();
    cgi_headers["QUERY_STRING"] = _client.get_request().path.substr(_client.get_request().path.find('?') + 1, std::string::npos);
    cgi_headers["REDIRECT_STATUS"] = "200";
    cgi_headers["CONTENT_LENGTH"] = _client.get_request().headers["Content-Length"];
    cgi_headers["CONTENT_TYPE"] = _client.get_request().headers["Content-Type"];
    
    return convert_map_to_cgi_envv(headers, cgi_headers);
}

char    *ResourceHandler::get_cgi_bin(Server &server, Location &location, std::string script_path) {
    (void)location;
    std::vector<std::string> &_cgipath = server.get_cgipath();
    std::vector<std::string> &_cgiext = server.get_cgiextension();
    std::string etc = script_path.substr(script_path.rfind('.'));
    int i = 0;

    for (std::vector<std::string>::iterator it = _cgiext.begin(); it != _cgiext.end(); it++)
    {
        if (*it == etc)
             break ;
        i++;
    }
    int j = 0;
    for (std::vector<std::string>::iterator it = _cgipath.begin(); it != _cgipath.end(); it++)
    {
        if (j == i)
            return strdup(it->c_str());
        j++;
    }
    return nullptr;
}

std::string ResourceHandler::string_upper_copy(std::string str)
{
    std::string tmp = str;
    
    for (int i = 0; i < (int)tmp.length(); i++)
    {
        tmp[i] = toupper(tmp[i]);
    }

    return tmp;
}

char        **ResourceHandler::convert_map_to_cgi_envv(std::map<std::string, std::string> &headers, std::map<std::string, std::string> &headers2)
{    
    char **env = new char*[headers.size() + headers2.size() + 1];
    int i = 0;

    for (std::map< std::string, std::string>::iterator it = headers2.begin(); it != headers2.end(); it++) {
        std::string tmp = it->first + "=" + it->second;
        env[i] = new char[tmp.length() + 1];
        strncpy(env[i], tmp.c_str(), tmp.length());
        env[i][tmp.length()] = '\0';
        i++; 
    }
    
    for (std::map< std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        std::string tmp = "HTTP_" + string_upper_copy(it->first) + "=" + string_upper_copy(it->second);
        env[i] = new char[tmp.length() + 1];
        strncpy(env[i], tmp.c_str(), tmp.length());
        env[i][tmp.length()] = '\0';
        i++; 
    }
    
    env[i] = nullptr;
    return env;
}

bool    ResourceHandler::to_cgi(Server &server, std::string path)
{
    std::vector<std::string> &_cgiext = server.get_cgiextension();
    std::string etc = path.substr(path.rfind('.'));
    int i = 0;

    for (std::vector<std::string>::iterator it = _cgiext.begin(); it != _cgiext.end(); it++)
    {
        if (*it == etc)
            return true;
        i++;
    }
    return false;
}

response_t ResourceHandler::dynamic_page(int status, bool config, Server &server) {
    response_t response;

    response.init = true;
    response.body = true;


    char buffer[60000] = {0};
    char filename[1024] = {0};
    int fd;

    if (config == false) {
        std::string random = "/tmp/" + random_string(5) + std::to_string(status) + ".html";
        std::string error = this->generate_page(this->httpResponses[status]);
        response.headers = "HTTP/1.1 " + std::to_string(status) + " OK\r\nContent-Type: text/html\r\ncontent-length: " + std::to_string(error.length()) + "\r\n\r\n";
        strncpy(buffer, error.c_str(), error.length());
        strncpy(filename, random.c_str() , random.length());
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        write(fd, buffer, strlen(buffer));
        close(fd);
        fd = open(filename, O_RDONLY);
        response.body_file = fd;
        return response;
    }

    for (std::vector<std::pair<int, std::string> >::iterator it = server.get_error_pages().begin(); it != server.get_error_pages().end(); it++) {
        if (it->first == status)
        {
            strncpy(filename, it->second.c_str() , it->second.length());
            fd = open(filename, O_RDONLY);
            if (fd == -1)
            {
                return dynamic_page(status, false, server);
            }
            response.body_file = fd;
            response.headers = generate_headers(std::to_string(status), _client.get_request().method, filename, fd);
            return response;
        }
    }
    return dynamic_page(status, false, server);
}

std::string ResourceHandler::generate_page(const std::string& status) {
    std::string htmlPage =
        "<html>\n"
        "<head>\n"
        "    <title>Error " + status + "</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
        "            margin: 0;\n"
        "            padding: 0;\n"
        "            background-color: #f1f1f1;\n"
        "        }\n"
        "        .container {\n"
        "            width: 60%;\n"
        "            margin: 100px auto;\n"
        "            padding: 20px;\n"
        "            background-color: #ffffff;\n"
        "            border-radius: 5px;\n"
        "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n"
        "        }\n"
        "        h1 {\n"
        "            color: #555555;\n"
        "            text-align: center;\n"
        "            text-transform: uppercase;\n"
        "            letter-spacing: 2px;\n"
        "            font-size: 32px;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <h1>Error " + status + "</h1>\n"
        "    </div>\n"
        "</body>\n"
        "</html>";

    return htmlPage;
}


std::string ResourceHandler::random_string( size_t length) {
    srand((unsigned) time(NULL) * getpid());
    char set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string str;
    str.reserve(length);
    while (length--)
    {
        str += set[rand() % (sizeof(set) - 1)];
    }
    return str;
}

int ResourceHandler::get_file_size(int fd) {
    struct stat stat_buf;

    fstat(fd, &stat_buf);
    return stat_buf.st_size;
}

std::string ResourceHandler::get_mime_type(std::string path) {
    size_t pos = path.find_last_of(".");

    if (pos == std::string::npos)
        return "text/plain";
    std::string extension = path.substr(pos + 1);
    if (this->_mimeTypes.find(extension) == this->_mimeTypes.end())
        return "text/plain";
    std::string mime_type = this->_mimeTypes[extension];
    return mime_type;
}

std::string ResourceHandler::get_last_modified(std::string path) {
    struct stat attrib;
    std::string last_modified;

    stat(path.c_str(), &attrib);
    last_modified = ctime(&attrib.st_mtime);
    last_modified.erase(last_modified.end() - 1);
    return last_modified;
}

std::string ResourceHandler::get_date() {
    time_t now = time(0);
    std::string date = ctime(&now);
    date.erase(date.end() - 1);
    return date;
}

std::string ResourceHandler::get_headers(std::map<std::string, std::string> &headers) {
    std::string response_headers;

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    {
        response_headers += it->first + ": " + it->second + "\r\n";
    }
    return response_headers;
}

std::string ResourceHandler::generate_headers(std::string status, std::string method, std::string request_target, int fd) {
    (void)method;
    std::string headers = "HTTP/1.1 " + status + "\r\n";
    headers += "Date: " + this->get_date() + "\r\n";
    headers += "Server: webserv\r\n";
    headers += "Content-Type: " + this->get_mime_type(request_target) + "\r\n";
    headers += "Content-Length: " + std::to_string(get_file_size(fd)) + "\r\n";
    headers += "Last-Modified: " + this->get_last_modified(request_target) + "\r\n";
    headers += "\r\n";
    return headers;
}
