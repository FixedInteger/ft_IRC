/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 04:31:16 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/27 10:20:57 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"

std::string parse(std::string buffer)
{
    std::string cmd;
    cmd = buffer.substr(buffer.find(" ") + 1, buffer.find("\r\n") - buffer.find(" ") - 1);
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '\r'), cmd.end());
    return(cmd);
}

int parse_pwd(std::string pwd)
{
    if(pwd.empty())
        return 2;
    return 0;
}
int parse_nickname(std::string nickname)
{
    if(nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-\n\r") != std::string::npos)
        return 1;
    if(nickname.empty())
        return 2;
    return 0;
}
int fetch_fd(std::set<std::string> &connected_clients,std::string nickname)
{
    for(std::set<std::string>::iterator it = connected_clients.begin(); it != connected_clients.end(); ++it)
        if((*it) == nickname)
            return 1;
    return 0;
}
int receive_message(Client &client,Server &server,int fd)
{
    char buff[1024] = {0};
    int read_bytes = recv(fd, buff, 1024, 0);
    if(read_bytes <= 0)
        return 0;
    std::string buffer(buff);
    if(buffer.find("PASS") != std::string::npos)
    {
        client.set_password(parse(buffer));
        if(parse_pwd(client.get_password()) == 2)
            send_err(":No password given\r\n","461 *",fd,0);
        else
            client.set_o(client.get_o() + 1);
        
    }
    else if(buffer.find("NICK") != std::string::npos)
    { 
        if(client.get_o() != 1)
            return 1;
        client.set_nickname(parse(buffer));
        if(parse_nickname(client.get_nickname()) == 1)
            send_err(":Invalid nickname\r\n","432 *",fd,0);
        else if(parse_nickname(client.get_nickname()) == 2)
            send_err(":No nickname given\r\n","431 *",fd,0);
        else if(fetch_fd(server.used_nicknames,client.get_nickname()) == 1)
            send_err(":Nickname is already in use\r\n","433 *",fd,0);
        else
            client.set_o(client.get_o() + 1);
        
    }
    else if(buffer.find("USER") != std::string::npos)
    {
        if(client.get_o() != 2)
            return 1;
        client.set_username(parse(buffer));
        std::cout << client.get_username() << std::endl;
        if(client.get_username().empty())
            send_err(":No username given\r\n","461 *",fd,0);
        else
            client.set_o(client.get_o() + 1);
    }
    return 1;
}
