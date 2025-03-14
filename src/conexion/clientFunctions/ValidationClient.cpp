#include "../../../includes/conexion/Client.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/

bool Client::ImInvitedInChannel(std::string channel_name)
{
	for (size_t i = 0; i < this->_myInviteChannels.size(); i++)
	{
		if (this->_myInviteChannels[i] == channel_name)
			return true;
	}
	return false;
}

