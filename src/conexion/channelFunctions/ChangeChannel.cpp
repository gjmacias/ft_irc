#include "../../../includes/conexion/Channel.hpp"

/*
###############################################################################
#									CHANGE									  #
###############################################################################
*/

bool Channel::ChangeClientToAdmin(std::string nickname)
{
	for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->GetNickname() == nickname)
		{
			this->_admins.push_back(*it);
			this->_clients.erase(it);
			return true;
		}
	}
	return false;
}

bool Channel::ChangeAdminToClient(std::string nickname)
{
	for (std::vector<Client*>::iterator it = this->_admins.begin(); it != this->_admins.end(); ++it)
	{
		if ((*it)->GetNickname() == nickname)
		{
			this->_clients.push_back(*it);
			this->_admins.erase(it);
			return true;
		}
	}
	return false;
}

