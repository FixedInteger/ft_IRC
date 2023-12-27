/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 01:13:20 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/27 10:11:33 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"
#include <sstream>


void ft_close_all(int server_socket, int kq)
{
    close(server_socket);
    close(kq);
}

std::string _return_cmd(std::string buffer)
{
    std::string cmd;
    cmd = buffer.substr(buffer.find(" ") + 1, buffer.find("\r\n") - buffer.find(" ") - 1);
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '\r'), cmd.end());
    return(cmd);
}
#include <iostream>  // Include necessary headers

void send_message( std::string message, std::string nickname, int fd, int code)
{
    char buff[1024] = {0};
    std::string irc_message;
    
    irc_message = ":localhost" + std::to_string(code).append(" ") + nickname.append(" ") + message.append("\r\n");
    strcpy(buff, irc_message.c_str());
    int send_bytes = send(fd, buff, strlen(buff), 0);
    if (send_bytes < 0)
        return ;
}

void send_err(std::string message, std::string code,int fd, int f)
{
    std::string ll = ":localhost " + code.append(" ") + message.append(" ");
    int send_bytes = send(fd,ll.c_str(),ll.size(),0);
    if(send_bytes < 0)
        return ;
    if (f == 1)
        close(fd);
}

int parse_nickname(std::string nickname,int fd)
{
   if(nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-\n\r") != std::string::npos)
    {
        send_err(":Invalid nickname\r\n","432 ",fd,0);
        return 0;
    }
   
    if(nickname.find_first_of("  \t") != std::string::npos)
    {
        send_err(":Invalid nickname\r\n","432 * ",fd,0);
        return 0;
    }
    return 1;
}
int  check_inusenick(std::string nickname, Server &server)
{
    if (server.used_nicknames.find(nickname) != server.used_nicknames.end()) 
        return 0;
    return 1;
}
int parse_pwd(Client &client,Server &server)
{
    if(strcmp(client.get_password().c_str(),server.pwd.c_str()) != 0)
    {
        send_err(":Invalid password\r\n","464 * ",client.get_fd(),0);
        return 0;
    }
    return 1;

}

int main(int ac, char** av) 
{
    Server server;
    server.kq = kqueue();
    server.port = parse_data(ac,av,server.port);
    server.pwd = parse_pwd(ac,av);
    int server_socket = ft_create_sever_socket(server.port);
    struct kevent evList[10];
    std::cout << "IRC Server listening on port " << server.port << std::endl;
    set_upserverqueue(server_socket,server.kq);
    while (1)
    {
        int nev = kevent(server.kq, NULL, 0, evList, 10, NULL); // wait for events
        if (nev < 0)
            ft_error("kevent failed");
        for (int i = 0; i < nev; i++)
        {
           int fd = (int)evList[i].ident;
          if((int)evList[i].flags & EV_EOF)
           {
                for(std::set<Client*>::iterator it = server.connected_clients.begin(); it != server.connected_clients.end(); ++it)
                {
                    if((*it)->get_fd() == fd)
                    {
                        std::cout << "client " << (*it)->get_nickname() << " disconnected" << std::endl;
                        server.used_nicknames.erase((*it)->get_nickname());
                        server.connected_clients.erase(*it);
                        break;
                    }
                }
                std::cout << "client  disconnected"<<std::endl;
                close(fd);
           }
              else if (fd == server_socket)
              {
                 int client_socket = set_upnewconnx(server_socket);
                 if (client_socket == -1)
                      ft_error("accept failed");
                 Client *client = new Client(client_socket);
                 setclient_queue(client_socket,i,evList,server.kq);
                 server.connected_clients.insert(client);
                 server.used_nicknames.insert(client->get_nickname());
              }
              else if ((int)evList[i].flags & EVFILT_READ) 
              {
                    for(std::set<Client*>::iterator it = server.connected_clients.begin(); it != server.connected_clients.end(); ++it)
                    {
                        if((*it)->get_fd() == fd)
                        {
                            if(receive_message(**it,server,fd) == 0)
                            {
                                std::cout << "client " << (*it)->get_nickname() << " disconnected" << std::endl;
                                server.used_nicknames.erase((*it)->get_nickname());
                                server.connected_clients.erase(*it);
                                break;
                            }
                            if((*it)->get_o() == 3)
                                authentify(fd,**it,server);
                            break;
                        }
                        else
                            continue;
                    }
              }
              else
                {
                    std::cout << "Unexpected event" << std::endl;
                    exit(EXIT_FAILURE);
                }           
        }
    }
    for (std::set<Client*>::iterator it = server.connected_clients.begin(); it != server.connected_clients.end(); ++it)
        delete *it;
    ft_close_all(server_socket,server.kq);
    return 0;
}