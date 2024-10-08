#ifndef RESPONSE_HPP
#define RESPONSE_HPP

typedef struct s_response {
    std::string headers;
    int         body_file;
    bool        body;
    bool        init;
    bool        head_done;
    int         read_bytes;
    bool        cgi;
    bool        cgi_response;
    pid_t       cgi_pid;
    std::string cgi_response_file_name;
} response_t;
#endif 