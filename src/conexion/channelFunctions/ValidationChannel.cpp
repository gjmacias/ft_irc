#include "Channel.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/

bool	Channel::IsModeCharActive(char character)
{
	for(size_t i = 0; i < this->_modes.size(); i++)
	{
		if(this->_modes[i].first == character)
			return this->_modes[i].second;
	}
	return false;
}

bool	Channel::IsClientInChannel(std::string nickname)
{
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if(this->_clients[i]->GetNickname() == nickname)
			return true;
	}
	for(size_t i = 0; i < this->_admins.size(); i++)
	{
		if(this->_admins[i]->GetNickname() == nickname)
			return true;
	}
	return false;
}