#include "../../../includes/conexion/Client.hpp"

/*
###############################################################################
#									REMOVE									  #
###############################################################################
*/
void Client::RmFromMyInvitedChannels(std::string channel_name)
{
	for (size_t i = 0; i < this->_myInviteChannels.size(); i++)
	{
		if (this->_myInviteChannels[i] == channel_name)
		{
			this->_myInviteChannels.erase(this->_myInviteChannels.begin() + i);
			return;
		}
	}
}

