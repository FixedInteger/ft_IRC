#include "../Inc/server.hpp"

void check_o(std::set<std::string> &used_nicknames,Client &client,int fd,Server &server)
{
    if(used_nicknames.find(client.get_nickname()) != used_nicknames.end())
	{
		server.connected_clients.erase(&client);
        send_err(":Nickname is already in use\r\n","433 * ",fd,1);
	}
     
}
void authentify(int fd,Client &client,Server &server)
{
    if(client.get_nickname().empty() || client.get_username().empty()|| client.get_user_info()[0].empty())
        return;
    else
	{
		std::string message;
		client.get_password().empty()  ? client.set_is_auth(false) : client.set_is_auth(true);
		check_o(server.used_nicknames,client,fd,server);
		if(client.get_is_auth() && (strcmp(parse_command(server.pwd).c_str(),parse_command(client.get_password()).c_str()) == 0  || strcmp(server.pwd.append("\n").c_str(),client.get_password().append("\n").c_str()) == 0))
		{
			send_err(" : Welcome to the Internet Relay Network \r\n","001 * ",fd , 0); // RPL_WELCOME
			server.used_nicknames.insert(client.get_nickname());
		}
		else
		{
			server.connected_clients.erase(&client);
			send_err(": Password incorrect \r\n","464 * ",fd,1);
		}
	}
    
}