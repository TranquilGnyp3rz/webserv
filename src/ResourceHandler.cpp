#include "ResourceHandler.hpp"
#include <cstring>

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
    

    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        it->sort_locations();
    }

}

response_t ResourceHandler::handle_request() {

    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if (it->get_server_name() + ":" + it->get_listen() == _client.get_request().headers["Host"] && _client.get_port() == it->get_port())
        {
            std::cout << "location :" << it2->get_locationName() << std::endl;
        }
         if (it->get_server_name() + ":" + it->get_listen() == _client.get_request().headers["Host"] && _client.get_port() == it->get_port())
         {
             return handle_location(*it , it->get_locations());
         }
         std::cout << "server name :" << _client.get_request().headers["Host"] << std::endl;
    }
    std::cout << "no server found" << std::endl;
    return dynamic_page(404, false, _servers[0]);
}

response_t ResourceHandler::handle_location(Server &server, std::vector<Location> &locations) {
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (location_match(it->get_locationName(), _client.get_request().path))
        {
             _target = it->get_root() + '/';
             _target += _client.get_request().path.substr(it->get_locationName().length(), _client.get_request().path.length());
             std::cout << "Target : " << _target << std::endl;
             return handle_method(server, *it);
        }
    }
    std::cout << "no location found" << std::endl;
    return dynamic_page(404, true, server);
}

bool ResourceHandler::location_match(std::string location, std::string path) {
    std::string tmp = path.substr(0, location.length());
    std::cout << "location :" << location << std::endl;
    std::cout << "path :" << path << std::endl;
    if (tmp == location && (path[location.length()] == '/' || path[location.length()] == '\0'))
            return true;
    return false;
}

response_t ResourceHandler::handle_method(Server &server, Location &location) {
    if (location.isMethodAllowed(_client.get_request().method) == false)
        return dynamic_page(405, true, server);

    if (_client.get_request().method == "GET") {
        if (location.get_redirection() != "") {
            response_t response;
            response.init = true;
            response.body = false;
            response.cgi_response = false;
            response.headers = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + location.get_redirection() + "\r\n\r\n";
            return response;
        }
        return get_file(server, location);
    }
    else if (_client.get_request().method == "DELETE") {
        struct stat s;
        if (stat(_target.c_str(),&s) != 0)
            return dynamic_page(500, true, server);
        if (s.st_mode & S_IFDIR)
            return dynamic_page(405, true, server);
        else
            return delete_file(server, location);
    }

    return dynamic_page(405, true, server);
}

response_t ResourceHandler::get_file(Server &server, Location &location) {
    response_t response;
    struct stat s;
    response.init = true;
    response.body = true;
    response.cgi_response = false;
    
    if (stat(_target.c_str(),&s) != 0){
        return dynamic_page(404, true, server);
    }
    // std::cout << "target : " << _target << std::endl;
    if (s.st_mode & S_IFDIR) {
        // std::cout << "Is directory" << std::endl;
        std::string index = _target;
        if (location.get_index() != "")
        {
            if (_target.back() != '/')
                index += "/";
            index += location.get_index();
            int fd = open(index.c_str(), O_RDONLY);
            if (fd == -1)
                return dynamic_page(500, true, server);
            response.body_file = fd;
            response.headers = generate_headers("200", _client.get_request().method, index, fd);
            return response;
        }
       
        if (_target.back() != '/')
            index += "/";
        index += "index.html";
        // std::cout << std::endl;
        // std::cout << "index :" << index << std::endl;
        // std::cout << std::endl;
        if (access(index.c_str(), R_OK) != -1)
        {
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
    if (access(_target.c_str(), R_OK) == -1)
        return dynamic_page(404, true, server);

    // std::cout << "target : ------------------- " << _target << std::endl;
    int fd = open(_target.c_str(), O_RDONLY);
    if (fd == -1)
        return dynamic_page(500, true, server);

    response.body_file = fd;
    response.headers = generate_headers("200", _client.get_request().method, _target, fd);
    return response;
}

response_t ResourceHandler::get_directory(Server  &server, Location  &location) {
    std::string html;
    std::string htmlPath = "/tmp/" + random_string(15) + ".html";
    std::vector<std::string> files;
    response_t response;

    response.init = true;
    response.body = true;
    response.cgi_response = false;

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
        if (location.get_locationName() == "/")
        {
            html += "<li><a href=\"" +  _target.substr(location.get_root().length() + 1) + "/" + *it + "\">" +  *it + "</a></li>\n";
            std::string print = "<li><a href=\"" +  _target.substr(location.get_root().length() + 1) + "/" + *it + "\">" +  *it + "</a></li>\n";
            std::cout << print << std::endl;
        }
        else
        {
            html += "<li><a href=\"" + location.get_locationName() + _target.substr(location.get_root().length()) + "/" + *it + "\">" +  *it + "</a></li>\n";
             std::string print = "<li><a href=\"" +  _target.substr(location.get_root().length()) + "/" + *it + "\">" +  *it + "</a></li>\n";
            std::cout << print << std::endl;
        }
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
    std::cout << "get dir done -------------------------" << std::endl;
    return response;
}

response_t ResourceHandler::delete_file(Server  &server, Location  &location) {
    response_t response;

    response.init = true; 
    response.body = false;
    response.cgi_response = false;

    if (std::remove(_target.c_str()) != 0)
        response.headers = "HTTP/1.1 404 OK\r\n\r\n";
    else
        response.headers = "HTTP/1.1 204 OK\r\n\r\n";
    return response;
}


response_t ResourceHandler::dynamic_page(int status, bool config, Server &server) {
    response_t response;

    response.init = true;
    response.body = true;
    response.cgi_response = false;

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
        int writen = write(fd, buffer, strlen(buffer));
        // std::cout << "writen :" << writen <<  "buffer :" << buffer << std::endl;
        close(fd);
        fd = open(filename, O_RDONLY);
        // std::cout << "Resource : fd: " << fd << std::endl;
        response.body_file = fd;
    } else {
        std::vector<std::pair<int, std::string> >::iterator it;
        for (it = server.get_error_pages().begin(); it != server.get_error_pages().end(); it++) {
            if (it->first == status)
            {
                strncpy(filename, it->second.c_str() , it->second.length());
                fd = open(filename, O_RDONLY);
                if (fd == -1)
                    return dynamic_page(500, false, server);
                response.body_file = fd;
                response.headers = generate_headers(std::to_string(status), _client.get_request().method, filename, fd);
                return response;
            }
        }
        if (it == server.get_error_pages().end())
            return dynamic_page(status, false, server);
    }

    return response;
}

std::string ResourceHandler::generate_page(const std::string& status) {
    std::string htmlPage = "<html><head><title>Error " + status + "</title><style>body{font-family:'Courier New',monospace;margin:0;padding:0;background-color:#000;}.container{width:60%;margin:100px auto;padding:20px;background-color:#001a00;border-radius:5px;box-shadow:0 0 10px rgba(0,255,0,0.3);animation:glitch 2s infinite;}@keyframes glitch{0%{transform:translate(0);}20%{transform:translate(-2px,-2px);}40%{transform:translate(2px,2px);}60%{transform:translate(-2px,-2px);}80%{transform:translate(2px,2px);}100%{transform:translate(0);}}h1{color:#00ff00;text-align:center;text-transform:uppercase;letter-spacing:2px;font-size:32px;text-shadow:0 0 10px #00ff00,0 0 20px #00ff00,0 0 30px #00ff00;animation:glitch-text 2s infinite;}@keyframes glitch-text{0%{transform:translate(0);}20%{transform:translate(-2px,-2px);}40%{transform:translate(2px,2px);}60%{transform:translate(-2px,-2px);}80%{transform:translate(2px,2px);}100%{transform:translate(0);}}</style></head><body><div class=\"container\"><h1>Error " + status + "</h1></div></body></html>";
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
    std::string headers = "HTTP/1.1 " + status + "\r\n";
    headers += "Date: " + this->get_date() + "\r\n";
    headers += "Server: webserv\r\n";
    headers += "Content-Type: " + this->get_mime_type(request_target) + "\r\n";
    headers += "Content-Length: " + std::to_string(get_file_size(fd)) + "\r\n";
    headers += "Last-Modified: " + this->get_last_modified(request_target) + "\r\n";
    headers += "\r\n";
    return headers;
}

response_t ResourceHandler::handler_cgi(Server  &server, Location  &location, std::string script_path)
{
    response_t response;
    std::string path = script_path;

    script_path = script_path.substr(0, script_path.find('?'));
    // std::cout << "script_path :" << script_path << std::endl;
    response.cgi = true;
    response.cgi_response = true;
    response.head_done = true;
    response.cgi_response_file_name = "/tmp/" + random_string(15) + ".cgi";
    
    response.body_file = open(response.cgi_response_file_name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    
    if (response.body_file == -1)
        return dynamic_page(500, true, server);
    
    response.cgi_pid = fork();
    
    if (response.cgi_pid == -1)
        return dynamic_page(500, true, server);
    if (response.cgi_pid == 0)
    {
        // std::cout << "cgi is forked " << std::endl;
        char **env = set_cgi_envv(server, location, script_path);
        char *bin = get_cgi_bin(server, location, script_path);
        const char *argv[] = { bin , script_path.c_str(), NULL};
        request_t request = _client.get_request();
        int input_fd = open(request.body_file.c_str(), O_RDONLY);
        if (input_fd == -1)
            exit(1);
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
        dup2(response.body_file, STDOUT_FILENO);
        close(response.body_file);
        execve(bin, (char **)argv, (char **)env);
        exit(1);
    }
    return response;
}

char       **ResourceHandler::set_cgi_envv(Server  &server, Location  &location, std::string script_path)
{
    std::map<std::string, std::string> env_map = _client.get_request().headers;

    env_map["SERVER_SOFTWARE"] = "webserv";
    env_map["SERVER_NAME"] = server.get_server_name();
    env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
    env_map["SERVER_PORT"] = server.get_listen();
    env_map["REQUEST_METHOD"] = _client.get_request().method;
    env_map["PATH_INFO"] = _client.get_request().path;
    env_map["PATH_TRANSLATED"] = script_path;
    env_map["SCRIPT_NAME"] = script_path;
    env_map["UPLOAD_DIR"] = server.get_uploadPath();
    env_map["SCRIPT_FILENAME"] = script_path;

    return convert_map_to_cgi_envv(env_map);
}

char        * ResourceHandler::get_cgi_bin(Server &server, Location &location, std::string script_path)
{
    std::vector<std::string> &_cgipath = server.get_cgipath();
    std::vector<std::string> &_cgiext = server.get_cgiextension();
    int i = script_path.rfind('.');
    if (i == std::string::npos)
        return nullptr;
    std::string ext = script_path.substr(i);
    if (std::find(_cgiext.begin(), _cgiext.end(), ext) == _cgiext.end())
        return nullptr;
    
    return strdup(_cgipath[0].c_str());
}

bool    ResourceHandler::to_cgi(std::string filepath)
{
    int i = filepath.rfind('.');
    int j = filepath.find('?');
    if (i == std::string::npos)
    {
        return false;
    }
    if (j != std::string::npos)
    {
        std::string tmp = filepath.substr(0, j);
        std::string ext = tmp.substr(i);
        if (ext == ".py" || ext == ".php")
            return true;
    }
    std::string ext = filepath.substr(i);
    if (ext == ".py" || ext == ".php")
        return true;
    return false;
}

char        **ResourceHandler::convert_map_to_cgi_envv(std::map<std::string, std::string> &headers)
{
    char **env = new char*[headers.size() + 1];
    int i = 0;

    for (std::map< std::string, std::string>::iterator it; it != headers.end(); it++) {
        std::string tmp = "HTTP_" + string_upper_copy(it->first) + "=" + string_upper_copy(it->second);
        env[i] = new char[tmp.length() + 1];
        strncpy(env[i], tmp.c_str(), tmp.length());
        env[i][tmp.length()] = '\0';
        i++; 
    }
    env[i] = nullptr;
    return env;
}

std::string ResourceHandler::string_upper_copy(std::string str)
{
    std::string tmp = str;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    return tmp;
}

std::string ResourceHandler::get_filepath(Server &server, Location &location, std::string path)
{
    std::string requested_file;
    int i = path.find('?');
    if (i == std::string::npos)
        requested_file = path.substr(location.get_locationName().length(), path.length());
    else
        requested_file = path.substr(location.get_locationName().length(), i - location.get_locationName().length());
    /*
        filepath should be according to the subject
    */
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << "requested_file :" << location.get_root() + requested_file << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    return location.get_root() + requested_file;
}