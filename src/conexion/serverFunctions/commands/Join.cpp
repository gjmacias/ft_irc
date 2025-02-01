#include "Server.hpp"

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

bool IsInvited(Client *cli, std::string ChName, int flag)
{
	if (cli->GetInviteChannel(ChName))
	{
		if (flag == 1)
			cli->RmChannelInvite(ChName);
		return (true);
	}
	return (false);
}

void	Server::ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->_channels[j].IsClientInChannel(GetClient(fd)->GetNickname()))
		return ;
	if (SearchForClients(GetClient(fd)->GetNickname()) >= 10)
	{
		SendError(405, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :You have joined too many _channels\r\n");
		return ;
	}
	if (!this->_channels[j].GetPassword().empty() && this->_channels[j].GetPassword() != token[i].second)
	{
		if (!IsInvited(GetClient(fd), token[i].first, 0))
		{
			SendErrorV2(475, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + token[i].first, " :Cannot join channel (+k) - bad key\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetModesInvitOnly())
	{
		if (!IsInvited(GetClient(fd), token[i].first, 1))
		{
			SendErrorV2(473, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + token[i].first, " :Cannot join channel (+i)\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetModesLimit() && this->_channels[j].CountAllClients() >= this->_channels[j].GetModesLimit())
	{
		SendErrorV2(471, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + token[i].first, " :Cannot join channel (+1)\r\n");
		return ;
	}
	//add Client to the channel
	Client	*Client = GetClient(fd);
	this->_channels[j].AddClient(Client);
	if(_channels[j].GetTopicName().empty())
		SendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->GetIpAdd(),token[i].first) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	else
		SendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->GetIpAdd(),token[i].first) + \
			RPL_TOPICIS(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].GetTopicName()) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	_channels[j].SendMeToAll(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->GetIpAdd(), token[i].first), fd);
}

void	Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(GetClient(fd)->GetNickname()) >= 10)
	{
		SendError(405, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :You have joined too many _channels\r\n");
		return ;
	}
	Channel	newChannel;
	newChannel.SetName(token[i].first);
	newChannel.AddAdmin(*GetClient(fd));
	this->_channels.push_back(newChannel);
	//notify taht the client join the channel
	SendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->GetIpAdd(),newChannel.GetName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickname(),newChannel.GetName(),newChannel.ListOfClients()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),newChannel.GetName()),fd);
}

void	Server::JoinCommand(std::vector<std::string> &splited_cmd, int &fd)
{
    (void)splited_cmd;
	bool	flag = false;
	std::vector < std::pair < std::string, std::string > > token;
    if (token.size() > 10)
    {
        SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Too many _channels\r\n");
        return ;
    }
    for (size_t i = 0; i < token.size(); i++)
    {
        if (this->_channels[j].GetName() == token[i].first)
        {
            ExistCh(token, i, j, fd);
            flag = true;
            break;
        }
    }
    if (!flag)
        NotExistCh(token, i, fd);
}
