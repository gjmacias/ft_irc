#include "Server.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/
bool	Server::IsRegisteredAndLoged(int fd)
{
	if (GetClient(fd) && GetClient(fd)->GetIsRegistered() && !(GetClient(fd)->GetNickName().empty()) 
		&& !(GetClient(fd)->GetUserName().empty()) && GetClient(fd)->GetNickName() != "*"
		&& GetClient(fd)->GetIsLogedInServer())
		return true;
	return false;
}

bool	Server::IsOnlyRegistered(Client *client)
{
	if (client && client->GetIsRegistered() && !(client->GetUserName().empty()) && 
		!(client->GetNickName().empty()) && client->GetNickName() != "*" && !(client->GetIsLogedInServer()))
		return true;
	return false;
}

bool Server::IsValidNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!(std::isalnum(nickname[i])) && nickname[i] != '_')
			return false;
	}
	return true;
}

bool Server::IsNickNameInUse(std::string& nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].GetNickName() == nickname)
			return true;
	}
	return false;
}