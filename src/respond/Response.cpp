#include "Response.hpp"

int Response::handle(Client &client)
{
    /* 
        request
           |
           |
           |
        V     V
    Error        check if request is valid
                |
                |
                |
                   V
                   check if request is GET, POST, DELETE
                     |
                     |
                     |
                        V
                        handle request
                        
    */
    return 0;
}

int Response::is_request_well_formed(Client &client, request_t &request)
{
    if (request.headers.find("Transfer-Encoding") != request.headers.end())
    {
        if (request.headers["Transfer-Encoding"] != "chunked")
            return this->send_error(client->get_sock(), 501);
    }
    if (request.headers.find("Content-Length") != request.headers.end() && request.headers.find("Transfer-Encoding") != request.headers.end()
        && request.method == "POST")
    {
        return this->send_error(client->get_sock(), 400);
    }
    if (this->containsAnyChar(request.path, ALLOWED_URL_CHAR) == true)
    {
        return this->send_error(client->get_sock(), 400);
    }
    if (request.method != "GET" && request.method != "POST" && request.method != "DELETE")
    {
        return this->send_error(client->get_sock(), 501);
    }
    if (request.http_version != "HTTP/1.1")
    {
        return this->send_error(client->get_sock(), 505);
    }
    if (request.path.size() > MAX_URL_SIZE)
    {
        return this->send_error(client->get_sock(), 414);
    }
    return 0;
}

int Response::send_error(int socket, int status)
{
    char *buffer = new char[1024];
    int sb;
    int status;

    std::string response =  "HTTP/1.1 "
                            + this->httpResponses[status] + "\r\n"
                          "Content-Type: text/html\r\n"
                          "\r\n";
    
    response += this->custom_error(this->httpResponses[status]);

    buffer = (char *)response.c_str();
    sb = ::send(socket, buffer, strlen(buffer), 0);
    return sb;
}


int int send(int socket, Client &client, Config &config)
{
    int sb = is_request_well_formed(client, client->get_request());
    if (sb != 0)
        return -1;
    return 0;
}

bool Response::containsAnyChar(const std::string& str, const std::string& charSet)
{
    for (std::string::size_type i = 0; i < charSet.length(); ++i)
    {
        if (str.find(charSet[i]) != std::string::npos)
        {
            return true; 
        }
    }
    return false;
}


Response::Response()
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

    this->fileExtensions.insert(std::make_pair(".txt", "Text File"));
    this->fileExtensions.insert(std::make_pair(".doc", "Microsoft Word Document"));
    this->fileExtensions.insert(std::make_pair(".pdf", "Portable Document Format"));
    this->fileExtensions.insert(std::make_pair(".jpg", "JPEG Image"));
    this->fileExtensions.insert(std::make_pair(".png", "Portable Network Graphics"));
    this->fileExtensions.insert(std::make_pair(".mp3", "MP3 Audio File"));
    this->fileExtensions.insert(std::make_pair(".mp4", "MP4 Video File"));
    this->fileExtensions.insert(std::make_pair(".exe", "Executable File"));
    this->fileExtensions.insert(std::make_pair(".zip", "Zip Archive"));
    this->fileExtensions.insert(std::make_pair(".csv", "Comma-Separated Values File"));
    this->fileExtensions.insert(std::make_pair(".html", "HTML File"));
    this->fileExtensions.insert(std::make_pair(".cpp", "C++ Source File"));
    this->fileExtensions.insert(std::make_pair(".java", "Java Source File"));
    this->fileExtensions.insert(std::make_pair(".py", "Python Script"));
    this->fileExtensions.insert(std::make_pair(".xls", "Microsoft Excel Spreadsheet"));
    this->fileExtensions.insert(std::make_pair(".ppt", "Microsoft PowerPoint Presentation"));
}

std::string Response::custom_error(const std::string& status) {
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
