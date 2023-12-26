/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 01:13:20 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/26 06:58:32 by heddahbi         ###   ########.fr       */
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
    return(buffer.substr(buffer.find(" ") + 1, buffer.find("\r\n") - buffer.find(" ") - 1));
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
    if(nickname.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\^{}|`") == std::string::npos)
    {
        send_err(":Invalid nickname\r\n","432",fd,0);
        return 0;
    }
    if(nickname.find_first_of("  \t") != std::string::npos)
    {
        send_err(":Invalid nickname\r\n","432",fd,0);
        return 0;
    }
    if(nickname.empty() || nickname == "\n")
    {
        send_err(":No nickname given\r\n","431",fd,0);
        return 0;
    }
    return 1;
}
void check_inusenick(std::string nickname,int fd , Server &server,Client &client)
{
    if (server.used_nicknames.find(nickname) != server.used_nicknames.end()) 
    {
        server.connected_clients.erase(&client);
        send_err(":Nickname is already in use\r\n","433",fd,1);
        return;
    }
}
int parse_pwd(Client &client)
{
    if(client.get_password().find_first_of("  \t") != std::string::npos)
        return 0;
    if(client.get_password().empty() || client.get_password() == "\n")
        return 0;
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
                std::cout << "cl disconnected"<<std::endl;
                close(fd);
           }
           else if(fd == server_socket)
           {
                int client_socket = set_upnewconnx(server_socket);
                if(client_socket == -1)
                    ft_error("accept failed");
                else if(client_socket > 0)
                    setclient_queue(client_socket,i,evList,server.kq);
                server.connected_clients.insert( new Client(client_socket));
           }
           else 
            {
                for (std::set<Client*>::iterator it = server.connected_clients.begin(); it != server.connected_clients.end(); ++it)
                {
                    if((*it)->get_fd() == fd)
                    {
                        receive_message(fd,**it,server);
                        break;
                    }
                }
            
            }
        }
    }
    for (std::set<Client*>::iterator it = server.connected_clients.begin(); it != server.connected_clients.end(); ++it)
        delete *it;
    ft_close_all(server_socket,server.kq);
    return 0;
}