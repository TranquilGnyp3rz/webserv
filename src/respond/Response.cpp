#include "Response.hpp"

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