#include "Channel.hpp"

/*
###############################################################################
#									CHANGE									  #
###############################################################################
*/

bool Channel::ChangeClientToAdmin(std::string nickname)
{
	size_t i = 0;

	while (i < this->_clients.size())
	{
		if(this->_clients[i].GetNickName() == nickname)
			break;
		i++;
	}
	if( i < this->_clients.size())
	{
		this->_admins.push_back(this->_clients[i]);
		this->_clients.erase(i + this->_clients.begin());
		return true;
	}
	return false;
}

bool Channel::ChangeAdminToClient(std::string nickname)
{
	size_t i = 0;

	while (i < this->_admins.size())
	{
		if(this->_admins[i].GetNickName() == nickname)
			break;
		i++;
	}
	if(i < this->_admins.size())
	{
		this->_clients.push_back(this->_admins[i]);
		this->_admins.erase(i + this->_admins.begin());
		return true;
	}
	return false;

}
