#include <iostream>
#include <fstream>
#include "Client.hpp"


std::string generate_filename() {
    std::string filename = "/tmp/";
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        filename += charset[rand() % charset.length()];
    }
    filename += ".txt";
    return filename;
}
void write_in_file(std::string filename, std::string buffer) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::app);
    file << buffer;
    file.close();
}
std::string trim(const std::string& str) 
{  
    std::string s = str;
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of('\r')+1);
    s.erase(s.find_last_not_of(' ')+1);
    return s;
}
int Client::get_port() {
    return _port;
}
static std::vector<std::string>    split_wordss(const std::string &str, char c) {
    std::vector<std::string> words;

    std::istringstream iss(str);
    while (iss)
    {
        std::string word;
        std::getline(iss, word, c);
        words.push_back(word);
    }
    return words;
}

static std::vector<std::string>    split_words(const std::string &str, char c) {
    std::vector<std::string> words;

    std::string::size_type pos;
    pos = str.find(c);
    if (pos == std::string::npos)
        exit(0);
    
    words.push_back(str.substr(0, pos));
    
    words.push_back(str.substr(pos + 1));
    return words;
}
static std::string find_key(std::map<std::string, std::string> map, std::string key) {
    std::map<std::string, std::string>::iterator it;
    it = map.find(key);
    if (it != map.end())
        return it->second;
    return "";
}

Client::Client(Config &config, int sock): _config(config) {
 
    _sock = sock;
}
Client::Client(Config &config, int port, int sock): _config(config){

    _request.body_lenght = 0;
    _request.body = "";
    _request.first_body = false;
    _port = port;
    _sock = sock;
    _buffer = "";
    _request.body_file = generate_filename();
}

request_t Client::get_request() {
    return _request;
}


std::string Client::get_buffer() {
    return _buffer;
}

int Client::get_sock() {
    return _sock;
}

void Client::parse_request() {
    std::vector<std::string> vectors;
    std::istringstream iss(_buffer);
    std::string line;
    bool firstline = false;
    _request.headerdone = false;
    while (std::getline(iss, line, '\n') && _request.headerdone == false)
    {
        if (firstline == false) {
            firstline = true;
            vectors = (split_wordss(line, ' '));
            _request.method = vectors.at(0);
            _request.path = vectors.at(1);
            _request.http_version = vectors.at(2);
            _request.http_version.erase(_request.http_version.size() - 1);
            continue;
        }
        if (line == "\r")
        {
            _request.headerdone = true;
            break ;
        }

        else if (_request.headerdone == false)
        {
            line.erase(line.size() - 1);
            vectors = split_words(line, ':');
            vectors.at(0) = trim(vectors.at(0));
            vectors.at(1) = trim(vectors.at(1));
            _request.headers.insert(std::pair<std::string, std::string>((vectors.at(0)), vectors.at(1)));
        }
    }
    if (_request.headers.find("Referer") != _request.headers.end())
    {
        std::string referer = _request.headers.find("Referer")->second;
        referer = referer.substr(referer.find("://") + 3, referer.size() - 1);
        referer = referer.substr(referer.find("/"), referer.size() - 1);
        _request.headers.find("Referer")->second = referer + _request.path;
    }
}


void Client::save_body(std::string &buffer, int &close_conn) {

    std::string body(buffer);
    if (find_key(_request.headers, "Content-Length") != "")
        {
            int size = std::stoi(find_key(_request.headers, "Content-Length"));
            if (_request.body_lenght  < size) {
                write_in_file(_request.body_file, body.substr(0, size - _request.body_lenght));
                std::ifstream in_file(_request.body_file, std::ios::binary);
                in_file.seekg(0, std::ios::end);
                int file_size = in_file.tellg();
                _request.body_lenght = file_size;
            }
            if (_request.body_lenght == size) {
                close_conn = true;
            }
        }
    else if(find_key(_request.headers, "Transfer-Encoding") == "chunked")
    {
        if (_request.body_lenght != 0) {
            if (body.size() < _request.body_lenght + 2)
            {
                write_in_file(_request.body_file, body);
                _request.body_lenght -= body.size();
            }
            else {
                write_in_file(_request.body_file, body.substr(0, _request.body_lenght));
                body = body.substr(_request.body_lenght + 2);
                _request.body_lenght = 0;

            }
        }
        if (_request.body_lenght == 0) {
            if (_request.body != "")
            {
                body = _request.body + body;
                _request.body = "";
            }
            if (body.find("\r\n") == std::string::npos) {
                _request.body = body;
                body = "";
            }
            while ( body.find("\r\n") != std::string::npos)
            {
                int size = 0;
                    
                std::string::size_type pos = body.find("\r\n");
                std::string size_str = body.substr(0, pos);
                try {
                    // std::cout << "size_str: " << size_str << std::endl;
        
                    _request.body_lenght = std::stoi(size_str, 0, 16);
                }
                catch (std::exception &e) {
                    std::cout << "error stoi" << std::endl;
                    std::cout << e.what() << std::endl;
                    close_conn = true;
                    break;
                }
                if(_request.body_lenght == 0)
                {
                    close_conn = true;
                    break;
                }
                body = body.substr(pos + 2);
                if (body.size() < _request.body_lenght + 2)
                {
                    write_in_file(_request.body_file, body);
                    _request.body_lenght -= body.size();
                    break;
                }
                write_in_file(_request.body_file, body.substr(0, _request.body_lenght));
                body = body.substr(_request.body_lenght + 2);
                _request.body_lenght = 0;
            }
        }
    }
    else
        close_conn = true;
}

bool Client::response() {
    char        buffer[CHUNKED_SIZE] = {0};
    bool        close_con = false;
    std::string str = "";
    int rc;

    if (_response.init == false) {
        _response = ResourceHandler(_config, *this).handle_request();
        _response.head_done = false;
    }

    if ( _response.head_done == false) {
        _response.head_done = true;
        str = _response.headers;
        send(_sock, str.c_str(), str.length(), 0);
        if (_response.body)
            return false;
        return true;
    }
    
    if ((rc = read(_response.body_file, buffer, CHUNKED_SIZE)) < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("read () failed");
            return true;
        }
    }
    if (rc == 0)
    {
        close_con = true;
        str = "0\r\n\r\n";
    }
    else
        str = to_hex(rc) + "\r\n" + std::string(buffer, rc) + "\r\n";
  
    send(_sock, str.c_str(), str.length(), 0);
    if (close_con == true)
        close(_response.body_file);
    
    return close_con;
}

std::string Client::to_hex(int nm) {
    std::string hex = "0123456789abcdef";
    std::string result;

    if (nm == 0)
        return "0";
    while (nm > 0)
    {
        result = hex[nm % 16] + result;
        nm /= 16;
    }
    return result;
}