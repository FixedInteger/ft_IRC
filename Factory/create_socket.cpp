/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_socket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 04:31:54 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/26 04:31:55 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"
int ft_create_sever_socket(int port)
{
    int server_socket;
    struct sockaddr_in server_addr;
    server_socket = socket_create();
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(port); // Port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any IP address
    bind_socket(server_socket, server_addr,1);
    listen_socket(server_socket);
    return server_socket; 

}