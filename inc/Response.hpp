#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <map>

class response
{
    private:
        std::map<int, std::string> httpResponses{
    {100, "Continue"},
    {101, "Switching Protocols"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {307, "Temporary Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Request Entity Too Large"},
    {414, "Request-URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Requested Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"}
    };
    std::map<std::string, std::string> fileExtensions{
    {".txt", "Text File"},
    {".doc", "Microsoft Word Document"},
    {".pdf", "Portable Document Format"},
    {".jpg", "JPEG Image"},
    {".png", "Portable Network Graphics"},
    {".mp3", "MP3 Audio File"},
    {".mp4", "MP4 Video File"},
    {".exe", "Executable File"},
    {".zip", "Zip Archive"},
    {".csv", "Comma-Separated Values File"},
    {".html", "HTML File"},
    {".cpp", "C++ Source File"},
    {".java", "Java Source File"},
    {".py", "Python Script"},
    {".xls", "Microsoft Excel Spreadsheet"},
    {".ppt", "Microsoft PowerPoint Presentation"}
    };
    public:
        
        std::string Response::custom_error(const std::string& status);
};
#endif 