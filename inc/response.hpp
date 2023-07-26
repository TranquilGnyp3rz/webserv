#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#define CHUNKED_SIZE 60000
#include <cstdlib>

typedef struct response_t {
    std::string headers;
    char        buffer[CHUNKED_SIZE];
    int         body_file;
    bool        body;
    bool        init;
    bool        head_done;
    int         read_bytes;
    bool        cgi;
    pid_t       cgi_pid;
    std::string cgi_response_file_name;
    std::string str;

    response_t(){
        headers = "";
        str = "";
        body = false;
        body_file = 0;
        head_done = false;
        init = false;
        read_bytes = 0;
        cgi = false;
        cgi_pid = -1;
        cgi_response_file_name = "";
        bzero(buffer, CHUNKED_SIZE);
    }
} response_t;
#endif 