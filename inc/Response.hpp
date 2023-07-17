#ifndef RESPONSE_HPP
#define RESPONSE_HPP

typedef struct s_response {
    std::string headers;
    int         body_file;
    bool        body;
    bool        init;
    bool        head_done;
    int         read_bytes;
} response_t;
#endif 