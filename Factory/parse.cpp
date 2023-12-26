/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 04:31:20 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/26 04:31:21 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"

void ft_error(std::string str)
{
    std::cerr << str << std::endl;
    exit(1);
}

void check_port(int port)
{
    if (port <= 1023 || port > 65535)
        ft_error("Please enter a valid Port");
}
int parse_data(int ac ,char **av,int port)
{
     if (ac != 3)
        ft_error("Usage: ./ircserv <port> <pwd>");
    else
    {
        if (av[1])
        {
            port = atoi(av[1]);
            check_port(port);
        }
        if(av[2])
        {
            std::string pwd = av[2];
            pwd = pwd.append("\n");
        }
        
        else
            ft_error("Please provide Port");
    }
    return port;
}
std::string parse_pwd(int ac ,char **av)
{
    std::string pwd;
    if (ac != 3)
        ft_error("Usage: ./ircserv <port> <pwd>");
    else
    {
        if (av[1])
        {
            int port = atoi(av[1]);
            check_port(port);
        }
        if(av[2])
            pwd = av[2];
    
        else
            ft_error("Please provide Port");
    }
    return pwd;
}


bool check_in(int c)
{
    return(c == '\n' || c == '\r');
}
std::string parse_command(std::string command) 
{
	int start = 0;
	while (start < (int)command.size() && (std::isspace(command[start])|| check_in(command[start])))
		start++;
	  int end = command.size();
	while (end > start &&( std::isspace(command[end - 1])||check_in(command[end -1])))
		end--;
	if (start < end) 
		return command.substr(start, end - start).append("\r\n");
	else 
		return "";
}