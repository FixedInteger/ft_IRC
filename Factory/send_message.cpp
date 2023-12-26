/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 04:31:16 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/27 00:24:23 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"
int  user_parse(Client &client,std::string buffer)
{
    client.set_username (_return_cmd(buffer));
    std::string temp = buffer.substr(buffer.find(" ") + 1);
    std::string r;
    int i = 0;
    while (temp != "\r\n" && i < 3)
    {
        client.set_user_info_element(i, temp.substr(0, temp.find(" ")));
        temp = temp.substr(temp.find(" ") + 1);
        i++;
    }
    if (i < 4)
        r = temp;
    if (r.find(":") != std::string::npos)
        client.set_user_info_element(3, r.substr(r.find(":") + 1));
   else
        client.set_user_info_element(3, r);
    return (EXIT_SUCCESS);
}
void receive_message(int fd, Client &client, Server &server)
{
    char buff[1024] = {0};
    ssize_t bytes_read = recv(fd, buff, sizeof(buff), 0);
    if (bytes_read <= 0)
        return ;
    std::string buffer = buff;
    std::string command = buffer.substr(0, buffer.find(" "));
    std::string new_nickname = _return_cmd(buffer);
    if (command == "PASS")
    {
        client.set_password(_return_cmd(buffer));
       if(parse_pwd(client) == 0 && !client.get_is_auth())
            send_err(":Invalid password\r\n","464 *",fd,0);
        else if(client.get_is_auth() && parse_pwd(client) == 0 )
            return;
    }
    else
    {
        if (command == "NICK")
        {
           if(parse_nickname(new_nickname,fd) == 0)
                return ;
            check_inusenick(new_nickname,fd,server);
            client.set_nickname(new_nickname);
        }
        else if (command == "USER")
        {
            if(user_parse(client,buffer)== EXIT_FAILURE)
                return ;
            authentify(fd,client,server);
        }
    }
    std::cout << " : "<<buff;
}