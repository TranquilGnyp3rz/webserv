#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler(Config &config, Client &client) : _client(client), _servers(config.get_servers())
{

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

    /* tmp sort procedure */
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        it->sort_locations();
    }
}

response_t ResourceHandler::handle_request()
{
    // int fd; // = check_request();
    // response_t response;

    // if (fd != -1)
    //     return fd;
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
         if (it->get_server_name() == _client.get_request().headers["Host"] && _client.get_port() == it->get_port())
         {
             return handle_location(*it , it->get_locations());
         }
    }
    return costum_error_page(404);
}

response_t ResourceHandler::handle_location(Server &server, std::vector<Location> &locations)
{
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (location_match(it->get_locationName(), _client.get_request().path))
        {
            return handle_method(server, *it);
        }
    }
    return costum_error_page(404);
}

response_t ResourceHandler::handle_method(Server &server, Location &location)
{
    if (_client.get_request().method == "GET") {
        if (location.isMethodAllowed("GET" ) == false)
            return costum_error_page(405);
        return get_file(server, location);
    }
    else if (_client.get_request().method == "DELETE") {
        if (location.isMethodAllowed("DELETE" ) == false)
            return costum_error_page(405);
        return delete_file(server, location);
    }
    else
        return costum_error_page(405);
}

bool ResourceHandler::location_match(std::string location, std::string path)
{
    std::string tmp = path.substr(0, location.length());
    if (tmp == location && (path[location.length()] == '/' || path[location.length()] == '\0'))
        return true;
    return false;
}

response_t ResourceHandler::get_directory(Server  &server, Location  &location)
{
    std::string directoryPath = location.get_root() + location.get_locationName();
    std::string html;
    std::string htmlPath = "/tmp/" + random_string(10) + ".html";
    std::vector<std::string> files;
    response_t response;
    DIR* dir;
    int fd;
    struct dirent* entry;

    dir = opendir(directoryPath.c_str());
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
        html += "<li><a href=\"" + location.get_locationName() + "/" + *it + "\">" +  *it + "</a></li>\n";
    }
    html += "</ul>\n";
    html += "</body>\n";
    html += "</html>\n";

    fd = open(htmlPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        return costum_error_page(500);
    }
    if (write(fd, html.c_str(), html.length()) == -1) {
        return costum_error_page(500);
    }
    close(fd);
    fd = open(htmlPath.c_str(), O_RDONLY);
    if (fd == -1) {
        return costum_error_page(500);
    }
    response.body_file = fd;
    response.headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\ncontent-length: " + std::to_string(get_file_size(fd)) + "\r\n\r\n";
    return response; 
}

response_t ResourceHandler::get_file(Server  &server, Location  &location)
{
    std::string file_path;
    response_t response;
    response.body = true;
    if (_client.get_request().path == location.get_locationName()) {
        std::string index = location.get_root() + location.get_locationName() + "/";
        if (location.get_index() != "")
            index += location.get_index();
        else
            index += "index.html";
       if (access(index.c_str(), R_OK) != -1) {
            int fd = open(index.c_str(), O_RDONLY);
            if (fd == -1)
                return costum_error_page(404);
            response.body_file = fd;
            response.headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\ncontent-length: " + std::to_string(get_file_size(fd)) + "\r\n\r\n";
            return response;
        }
        else if (location.get_autoIndex() == "on") {
            return get_directory(server, location);
        }
        else {
            return costum_error_page(404);
        }
       }

    file_path = location.get_root() + _client.get_request().path; 
    int fd = open(file_path.c_str(), O_RDONLY);
    if (fd == -1)
        return costum_error_page(404);
    response.body_file = fd;
    response.headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\ncontent-length: " + std::to_string(get_file_size(fd)) + "\r\n\r\n";
    return response;
}

response_t ResourceHandler::delete_file(Server  &server, Location  &location) {
    std::string file_path = location.get_root() + _client.get_request().path;
    response_t response;
    response.body = false;

    if (remove(file_path.c_str()) != 0)
        return costum_error_page(404);
    response.headers = "HTTP/1.1 204 OK\r\n\r\n";
    return response;
}


response_t ResourceHandler::costum_error_page(int error_code)
{
    response_t response;
    response.body = true;
    char buffer[60000] = {0};
    char filename[1024] = {0};
    std::string random = "/tmp/" + random_string(5);
    random  += ".html";
    std::cout << "random: " << random << std::endl;
    int fd;

    std::string error = this->custom_error(this->httpResponses[error_code]);
   
    response.headers = "HTTP/1.1 " + std::to_string(error_code) + " OK\r\nContent-Type: text/html\r\ncontent-length: " + std::to_string(error.length()) + "\r\n\r\n";
    strncpy(buffer, error.c_str(), error.length());
    strncpy(filename, random.c_str() , strlen("/tmp/error.html"));
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int writen = write(fd, buffer, strlen(buffer));
    std::cout << "writen :" << writen <<  "buffer :" << buffer << std::endl;
    close(fd);
    fd = open(filename, O_RDONLY);
    std::cout << "Resource : fd: " << fd << std::endl;
    response.body_file = fd;
    return response;
}

std::string ResourceHandler::custom_error(const std::string& status) {
    std::string htmlPage = "<html><head><title>Error " + status + "</title><style>body{font-family:'Courier New',monospace;margin:0;padding:0;background-color:#000;}.container{width:60%;margin:100px auto;padding:20px;background-color:#001a00;border-radius:5px;box-shadow:0 0 10px rgba(0,255,0,0.3);animation:glitch 2s infinite;}@keyframes glitch{0%{transform:translate(0);}20%{transform:translate(-2px,-2px);}40%{transform:translate(2px,2px);}60%{transform:translate(-2px,-2px);}80%{transform:translate(2px,2px);}100%{transform:translate(0);}}h1{color:#00ff00;text-align:center;text-transform:uppercase;letter-spacing:2px;font-size:32px;text-shadow:0 0 10px #00ff00,0 0 20px #00ff00,0 0 30px #00ff00;animation:glitch-text 2s infinite;}@keyframes glitch-text{0%{transform:translate(0);}20%{transform:translate(-2px,-2px);}40%{transform:translate(2px,2px);}60%{transform:translate(-2px,-2px);}80%{transform:translate(2px,2px);}100%{transform:translate(0);}}</style></head><body><div class=\"container\"><h1>Error " + status + "</h1></div></body></html>";
    return htmlPage;
}

// reponse_t ResourceHandler::check_request( void ) {
//     request_t request = this->_client.get_request();

//     if (request.headers.find("Transfer-Encoding") != request.headers.end())
//     {
//         if (request.headers["Transfer-Encoding"] != "chunked")
//             return this->costum_error_page(501);
//     }
//     if (request.headers.find("Content-Length") != request.headers.end() && request.headers.find("Transfer-Encoding") != request.headers.end()
//         && request.method == "POST")
//     {
//         return this->costum_error_page(400);
//     }
//     // if (this->containsAnyChar(request.path, ALLOWED_URL_CHAR) == true)
//     // {
//     //     return this->costum_error_page(400);
//     // }
//     if (request.method != "GET" && request.method != "POST" && request.method != "DELETE")
//     {
//         return this->costum_error_page(501);
//     }
//     if (request.http_version != "HTTP/1.1")
//     {
//         return this->costum_error_page(505);
//     }
//     if (request.path.size() > MAX_URL_SIZE)
//     {
//         return this->costum_error_page(4);
//     }
//     return -1;
// }

std::string ResourceHandler::random_string( size_t length )
{
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

int ResourceHandler::get_file_size(int fd)
{
    struct stat stat_buf;

    fstat(fd, &stat_buf);
    return stat_buf.st_size;
}
