#include "Client.hpp"

/*
###############################################################################
#									REMOVE									  #
###############################################################################
*/
void Client::RmFromMyChannels(std::string &channel_name)
{
	for (size_t i = 0; i < this->_myChannels.size(); i++)
	{
		if (this->_myChannels[i] == channel_name)
		{
			this->_myChannels.erase(this->_myChannels.begin() + i);
			return;
		}
	}
}
