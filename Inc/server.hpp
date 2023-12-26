#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <set>
#include <cstring>
#include <cstdlib>
#include <cerrno>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <sys/time.h>
#include <map>

#define MAX_CLIENTS 10
#include "Client.hpp"

class Server
{
    public:
        int kq;
        int port;
        std::string pwd;
        int server_socket;
        struct sockaddr_in server_addr;
        std::set<std::string> used_nicknames;
        std::set<Client*> connected_clients;
         // set of connected clients
};


void ft_error(std::string str);
int parse_data(int ac ,char **av,int port);
void   ft_error(std::string str);
void check_port(int port);
int socket_create(void);
void bind_socket(int server_socket, struct sockaddr_in server_addr, int opt);
void listen_socket(int server_socket);
int  set_upnewconnx(int server_socket);
void set_upserverqueue(int server_socket, int kq);
void setclient_queue(int client_socket,int i ,struct kevent evList[10],int kq);
std::string parse_pwd(int ac ,char **av);
std::string parse_command(std::string command) ;
bool check_in(int c);
void send_err(std::string message, std::string code,int fd, int f);
void authentify(int fd,Client &client,Server &server);
int parse_pwd(Client &client);
void check_inusenick(std::string nickname,int fd , Server &serve,Client &client);
int parse_nickname(std::string nickname,int fd);
void send_err(std::string message, std::string code,int fd, int f);
std::string _return_cmd(std::string buffer);
void ft_close_all(int server_socket, int kq);
int ft_create_sever_socket(int port);
void receive_message(int fd, Client &client, Server &server);
#endif