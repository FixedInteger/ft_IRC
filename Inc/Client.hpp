#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"

class Client
{
        int fd;
        bool is_auth ;
        std::string password;
        std::string  user_info[4];
        std::string username;
        std::string nickname;
    public:
        Client(int fd) : fd(fd) {};
        int get_fd() const { return fd; }
        std::string get_nickname() const { return nickname; }
        std::string get_username() const { return username; }
        std::string get_password() const { return password; }
        std::string *get_user_info() { return user_info; }
        bool get_is_auth() const { return is_auth; }
        void set_nickname(std::string nickname) { this->nickname = nickname; }
        void set_username(std::string username) { this->username = username; }
        void set_password(std::string password) { this->password = password; }
        void set_user_info_element(int index, std::string value) { user_info[index] = value; }
        void set_is_auth(bool is_auth) { this->is_auth = is_auth; }
        bool operator<(const Client &rhs) const { return fd < rhs.fd; }
        Client();
        ~Client();
};
#endif