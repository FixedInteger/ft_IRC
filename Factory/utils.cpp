/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 06:25:35 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/26 02:54:43 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/server.hpp"

int socket_create(void)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        ft_error("Socket Creation Failed");
    if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1) // set server socket to non-blocking
        ft_error("fcntl failed");

    return server_socket;
}

void bind_socket(int server_socket, struct sockaddr_in server_addr ,int opt)
{
    int setsockopt_status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&opt, sizeof(opt));
    if (setsockopt_status == -1)
    {
        ft_error("setsockopt failed");
        close(server_socket);
    }
    int bind_status = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_status == -1)
    {
        ft_error("Binding Failed");
        close(server_socket);
    }
}
void listen_socket(int server_socket)
{
    int listen_status = listen(server_socket, 5); // listen for connections
    if (listen_status == -1)
    {
        ft_error("Listening Failed");
        close(server_socket);
    }
}