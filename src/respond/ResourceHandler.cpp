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

int ResourceHandler::handle_request()
{
    int fd = check_request();

    if (fd != -1)
        return fd;
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
         if (it->get_server_name() == _client.get_request().headers["Host"])
         {
             it->sort_locations();
             return handle_location(*it , it->get_locations());
         }
    }
    return costum_error_page(404);
}

int ResourceHandler::handle_location(Server &server, std::vector<Location> &locations)
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

int ResourceHandler::handle_method(Server &server, Location &location)
{
    std::vector<std::string> methods = location.get_allowedMethods();
    std::string method = _client.get_request().method;

    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
    {
        if (*it == method)
            break ;
    }

    if (method == "GET")
        return get_file(server, location);
    else if (method == "DELETE")
        return delete_file(server, location);
    else if (method == "POST")
        return post_file(server, location);
    else
        return costum_error_page(405);
}

bool ResourceHandler::location_match(std::string location, std::string path)
{
    for (int i = 0; i < location.length(); i++)
    {
        if (location[i] != path[i])
            return false;
    }
    return true;
}

int ResourceHandler::get_file(Server const &server, Location const &location)
{

   
    return 0;
}

int ResourceHandler::delete_file(Server const &server, Location const &location)
{
 
    return 0;
}

int ResourceHandler::post_file(Server const &server, Location const &location)
{
   
    return 0;
}

int ResourceHandler::costum_error_page(int error_code)
{
    char buffer[1024] = {0};
    char filename[1024] = {0};
    std::string random = "/tmp/" + random_string(42) + ".html";
    int fd;

    std::string response =  "HTTP/1.1 "
                            + this->httpResponses[error_code] + "\r\n"
                          "Content-Type: text/html\r\n"
                          "\r\n";
    
    response += this->custom_error(this->httpResponses[error_code]);

    strncpy(buffer, response.c_str(), response.length());
    strncpy(filename, random.c_str() , strlen("/tmp/error.html"));
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    write(fd, buffer, strlen(buffer));
    return fd;
}

std::string ResourceHandler::custom_error(const std::string& status) {
    std::string htmlPage = "<html>\n";
    htmlPage += "<head>\n";
    htmlPage += "<title>Error " + status + "</title>\n";
    htmlPage += "<style>\n";
    htmlPage += "body {\n";
    htmlPage += "    font-family: Arial, sans-serif;\n";
    htmlPage += "    margin: 0;\n";
    htmlPage += "    padding: 0;\n";
    htmlPage += "    background-color: #f2f2f2;\n";
    htmlPage += "}\n";
    htmlPage += ".container {\n";
    htmlPage += "    width: 60%;\n";
    htmlPage += "    margin: 100px auto;\n";
    htmlPage += "    padding: 20px;\n";
    htmlPage += "    background-color: #fff;\n";
    htmlPage += "    border-radius: 5px;\n";
    htmlPage += "    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n";
    htmlPage += "}\n";
    htmlPage += "h1 {\n";
    htmlPage += "    color: #d8000c;\n";
    htmlPage += "}\n";
    htmlPage += "</style>\n";
    htmlPage += "</head>\n";
    htmlPage += "<body>\n";
    htmlPage += "<div class=\"container\">\n";
    htmlPage += "<h1>Error " + status + "</h1>\n";
    htmlPage += "</div>\n";
    htmlPage += "</body>\n";
    htmlPage += "</html>\n";

    return htmlPage;
}

int ResourceHandler::check_request( void ) {
    request_t request = this->_client.get_request();

    if (request.headers.find("Transfer-Encoding") != request.headers.end())
    {
        if (request.headers["Transfer-Encoding"] != "chunked")
            return this->costum_error_page(501);
    }
    if (request.headers.find("Content-Length") != request.headers.end() && request.headers.find("Transfer-Encoding") != request.headers.end()
        && request.method == "POST")
    {
        return this->costum_error_page(400);
    }
    // if (this->containsAnyChar(request.path, ALLOWED_URL_CHAR) == true)
    // {
    //     return this->costum_error_page(400);
    // }
    if (request.method != "GET" && request.method != "POST" && request.method != "DELETE")
    {
        return this->costum_error_page(501);
    }
    if (request.http_version != "HTTP/1.1")
    {
        return this->costum_error_page(505);
    }
    if (request.path.size() > MAX_URL_SIZE)
    {
        return this->costum_error_page(4);
    }
    return -1;
}

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