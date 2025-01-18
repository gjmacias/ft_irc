#include "Channel.hpp"

/*
###############################################################################
#									CHANGE									  #
###############################################################################
*/

bool Channel::ChangeClientToAdmin(std::string nickname)
{
	for(size_t i = 0; i < clients.size(); i++)
	{
		if(clients[i].GetNickName() == nickname)
			break;
	}
	if(i < clients.size())
	{
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool Channel::ChangeAdminToClient(std::string nickname)
{
	for(size_t i = 0; i < admins.size(); i++)
	{
		if(admins[i].GetNickName() == nickname)
			break;
	}
	if(i < admins.size())
	{
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;

}
