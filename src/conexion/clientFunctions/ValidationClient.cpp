#include "Client.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/

bool Client::ImInChannel(std::string &channel_name)
{
	for (size_t i = 0; i < this->_myChannels.size(); i++)
	{
		if (this->_myChannels[i] == channel_name)
			return true;
	}
	return false;
}
