#ifndef RESPONSE_HPP
#define RESPONSE_HPP

typedef struct s_response
{
    std::string headers;
    int         body_file;
    bool        body;
}               response_t;
#endif 