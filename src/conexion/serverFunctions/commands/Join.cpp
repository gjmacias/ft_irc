#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									Join									  #
###############################################################################
*/

int	Server::SearchForClients(std::string nickname)
{
	int		count;
	size_t	i;

	count = 0;
	for (i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].IsClientInChannel(nickname))
			count++;
	}
	return (count);
}

void	Server::ExistCh(std::string &channelname, std::string &password, int j, int fd)
{
	Client* Client = GetClient(fd);

	if (this->_channels[j].IsClientInChannel(Client->GetNickname()))
		return ;
	if (this->_channels[j].GetModesInvitOnly() && !(Client->ImInvitedInChannel(channelname)))
	{
		SendErrorV2(473, Client->GetFd(), Client->GetNickname(), channelname, " :Cannot join channel (+i)\r\n");
		return;
	}
	if (this->_channels[j].GetModesChannelKey() && this->_channels[j].GetPassword() != password)
	{
		SendErrorV2(475, Client->GetFd(), Client->GetNickname(), channelname, " :Cannot join channel (+k) - bad key\r\n");
		return ;
	}
	if (this->_channels[j].GetModesLimit() && this->_channels[j].CountAllClients() >= this->_channels[j].GetLimitNumber())
	{
		SendErrorV2(471, Client->GetFd(), Client->GetNickname(), channelname, " :Cannot join channel (+1)\r\n");
		return ;
	}
	//add Client to the channel
	this->_channels[j].AddClient(Client);
	//Remove if its invited
	if (Client->ImInvitedInChannel(channelname))
		Client->RmFromMyInvitedChannels(channelname);
	//Messsages
	if(_channels[j].GetTopicName().empty())
		SendResponse(RPL_JOINMSG(Client->GetHostname(), Client->GetIPaddress(), channelname) + \
			RPL_NAMREPLY(Client->GetNickname(), _channels[j].GetName(), _channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(Client->GetNickname(), _channels[j].GetName()), fd);
	else
		SendResponse(RPL_JOINMSG(Client->GetHostname(), Client->GetIPaddress(),channelname) + \
			RPL_TOPICIS(Client->GetNickname(), _channels[j].GetName(), _channels[j].GetTopicName()) + \
			RPL_NAMREPLY(Client->GetNickname(), _channels[j].GetName(), _channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(Client->GetNickname(), _channels[j].GetName()), fd);
	_channels[j].SendMeToAll(fd, RPL_JOINMSG(Client->GetHostname(), Client->GetIPaddress(), channelname));
}

void	Server::NotExistCh(std::string channelname, std::string password, int fd)
{
	Channel	newChannel;

	if (!IsValidChannelname(channelname))
	{
		SendResponse("Bad channelname", fd);
		return;
	}
	newChannel.SetName(channelname);
	newChannel.AddAdmin(GetClient(fd));
	if (!password.empty())
	{
		newChannel.SetModesChannelKey(true);
		newChannel.SetPassword(password);
	}
	AddChannel(newChannel);
	//notify taht the clientent join the channel
	SendResponse(RPL_JOINMSG(GetClient(fd)->GetHostname(),GetClient(fd)->GetIPaddress(),newChannel.GetName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickname(),newChannel.GetName(),newChannel.ListOfClients()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),newChannel.GetName()),fd);
}

void	Server::JoinCommand(std::vector<std::string> &splited_cmd, int fd)
{
	bool						flag = false;
	std::vector<std::string>	list_channels;
	std::vector<std::string>	list_passwords;
	std::string					tmp = "";

	if (splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return ;
	}
	list_channels = split_delimeter(splited_cmd[1], ',');
	if (splited_cmd.size() >= 3)
		list_passwords = split_delimeter(splited_cmd[2], ',');
    if (list_channels.size() > 10)
    {
        SendError(GetClient(fd)->GetFd(), 407, GetClient(fd)->GetNickname(), " :Too many _channels\r\n");
        return ;
    }
    for (size_t i = 0; i < list_channels.size(); i++)
    {
		if (i < list_passwords.size())
			tmp = list_passwords[i];
		else
			tmp = "";
		for (size_t j = 0; j < this->_channels.size(); j++)
		{
			if (this->_channels[j].GetName() == list_channels[i])
			{
				ExistCh(list_channels[i], tmp, j, fd);
				flag = true;
				break;
			}
		}
		if (!flag)
			NotExistCh(list_channels[i], tmp, fd);
    }
}

