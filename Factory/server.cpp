/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 04:31:45 by heddahbi          #+#    #+#             */
/*   Updated: 2023/12/26 04:31:46 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../Inc/server.hpp"
int  set_upnewconnx(int server_socket)
{
    std::cout << "New client connected" << std::endl;
    struct sockaddr_in client_addr; // create socket for client
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
        return -1;
    return client_socket;
}

void set_upserverqueue(int server_socket, int kq)
{
   
    struct kevent evSet;
    EV_SET(&evSet, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL); // register server socket for read events
    int nev = kevent(kq, &evSet, 1, NULL, 0, NULL); // add server socket to the kqueue
    if (nev < 0)
        ft_error("kevent failed");
}

void setclient_queue(int client_socket,int i ,struct kevent evList[10],int kq)
{
    EV_SET(&evList[i], client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL); // register client socket for read events
    int nev = kevent(kq, &evList[i], 1, NULL, 0, NULL); // add client socket to the kqueue
    if (nev < 0)
        ft_error("kevent failed");
}
