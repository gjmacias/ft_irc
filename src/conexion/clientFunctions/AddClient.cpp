#include "../../../includes/conexion/Client.hpp"

/*
###############################################################################
#									ADDERS									  #
###############################################################################
*/
void Client::AddToMyInvitedChannels(std::string channel_name)
{
	if (!ImInvitedInChannel(channel_name))
	_myInviteChannels.push_back(channel_name);
}

