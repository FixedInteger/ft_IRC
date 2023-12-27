#include "../Inc/server.hpp"

void check_o(std::set<std::string> &used_nicknames,Client &client,int fd,Server &server)
{
    if(used_nicknames.find(client.get_nickname()) != used_nicknames.end() && client.get_o() == 3)
	{
		server.connected_clients.erase(&client);
        send_err(":Nickname is already in use\r\n","433 * ",fd,1);
	}
     
}
int authentify(int fd,Client &client,Server &server)
{
	if(client.get_o() != 3)
		return 0;
	check_o(server.used_nicknames,client,fd,server);
	server.used_nicknames.insert(client.get_nickname());
	client.set_is_auth(true);
	send_message("Welcome to the Internet Relay Network " + client.get_nickname(),"* ",fd,1);
	return 1;
    
}



