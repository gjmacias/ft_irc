#include "Server.hpp"


/*
###############################################################################
#								CLIENT INVOLVED								  #
###############################################################################
*/

void Server::ClientAuthentification(std::string cmd, int fd)
{
	Client		*client = GetClient(fd);
	size_t		position;

	cmd = cmd.substr(4);
	position = cmd.find_first_not_of("\t\v ");
	if(position == std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	cmd = cmd.substr(position);
	position = cmd.find_first_not_of("\t\v ");
	if(cmd[0] == ':')
		cmd.erase(cmd.begin());	
	else if(position != std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	if(!client->GetIsRegistered())
	{
		if(cmd == this->_password)
			client->SetIsRegistered(true);
		else
			SendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		SendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->GetNickName()), fd);
}

void Server::ClientNickname(std::string cmd, int fd)
{
	Client		*client = GetClient(fd);
	size_t		position;

	cmd = cmd.substr(4);
	position = cmd.find_first_not_of("\t\v ");
	if(position == std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	cmd = cmd.substr(position);
	position = cmd.find_first_not_of("\t\v ");
	if(cmd[0] == ':')
		cmd.erase(cmd.begin());	
	else if(position != std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}

	if (IsNickNameInUse(cmd) && client->GetNickName() != cmd)
	{
		if(client->GetNickName().empty())
			client->SetNickname("*");
	    SendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return;
	}
	if(!IsValidNickname(cmd))
	{
		SendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
	else
	{
		if (!(client->GetIsRegistered()))
			SendResponse(ERR_NOTREGISTERED(cmd), fd);
		else
		{
			//revisar// std::string oldnick = client->GetNickName();
			// client->SetNickname(cmd);
			// if(!oldnick.empty() && oldnick != cmd)
			// {
			// 	if(oldnick == "*" && !client->GetUserName().empty())
			// 	{
			// 		client->SetIsLogedInServer(true);
			// 		SendResponse(RPL_CONNECTED(client->GetNickName()), fd);
			// 		SendResponse(RPL_NICKCHANGE(client->GetNickName(), cmd), fd);
			// 	}
			// 	else
			// 		SendResponse(RPL_NICKCHANGE(oldnick,cmd), fd);
			// 	return;
			// }			
		}
	}
	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickName()), fd);
	}
}

void	Server::ClientUsername(std::vector<std::string> &splited_cmd, int fd)
{
	Client *client = GetClient(fd); 

	if(client && splited_cmd.size() < 5)
	{
		SendResponse(ERR_NOTENOUGHPARAM(client->GetNickName()), fd);
		return; 
	}
	if(!(client) || !(client->GetIsRegistered()))
		SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (client && !(client->GetUserName().empty()))
	{
		SendResponse(ERR_ALREADYREGISTERED(client->GetNickName()), fd);
		return;
	}
	else
		client->SetUsername(splited_cmd[1]);

	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickName()), fd);
	}
}
