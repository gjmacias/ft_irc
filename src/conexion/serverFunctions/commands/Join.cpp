#include "Server.hpp"

/*
###############################################################################
#									Join									  #
###############################################################################
*/
int	Server::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
	std::vector<std::string>	tmp;
	std::string					ChStr;
	std::string					PassStr;
	std::string					buff;
	std::istringstream			iss(cmd);

	while(iss >> cmd)
		tmp.push_back(cmd);
	if (tmp.size() < 2)
	{
		token.clear();
		return (0);
	}
	tmp.erase(tmp.begin());
	ChStr = tmp[0];
	tmp.erase(tmp.begin());
	if (!tmp.empty())
	{
		PassStr = tmp[0];
		tmp.clear();
	}
	for (size_t i = 0; i < ChStr.size(); i++)
	{
		if (ChStr[i] == ',')
		{
			token.push_back(std::make_pair(buff, ""));
			buff.clear();
		}
		else
			buff += ChStr[i];
	}
	token.push_back(std::make_pair(buff, ""));
	if (!PassStr.empty())
	{
		size_t	j = 0;
		size_t	i;
		buff.clear();
		for (i = 0; i < PassStr.size(); i++)
		{
			if (PassStr[i] == ',')
			{
				token[j].second = buff;
				j++;
				buff.clear();
			}
			else
				buff += PassStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++) //erase the empty channel names
	{
		if (token[i].first.empty())
			token.erase(token.begin() + i--);
	}
	for (size_t i = 0; i < token.size(); i++)
	{
		if (*(token[i].first.begin()) != '#')
		{
			senderror(403, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :No such channel\r\n");
			token.erase(token.begin() + i--):
		}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return (1);
}

int	Server::SearchForClients(std::string nickname)
{
	int		count;
	size_t	i;

	count = 0;
	for (i = 0; i < tjois->channels.size(); i++)
	{
		if (this->channels[i].GetClientInChannel(nickname))
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
	if (this->channels[j].GetClientInChannel(GetClient(fd)->GetNickName())) // if the client is already registered
		return ;
	if (SearchForClients(GetClient(fd)->GetNickName()) >= 10) // if the client is already in 10 channels
	{
		senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
		return ;
	}
	if (!this->channels[j].GetPassword().empty() && this->channels[j].GetPassword() != token[i].second)
	{
		if (!IsInvited(GetClient(fd), token[i].first, 0))
		{
			senderror(475, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n");
			return ;
		}
	}
	if (this->channels[j].GetInviteOnly()) // if the channel is invit only
	{
		if (!IsInvited(GetClient(fd), token[i].first, 1))
		{
			senderror(473, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n");
			return ;
		}
	}
	if (this->channels[j].GetLimit() && this->channels[j].GetClientsNumber() >= this->channels[j].GetLimit())
	{
		senderror(471, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+1)\r\n");
		return ;
	}
	//add Client to the channel
	Client	*cli = GetClient(fd);
	this->channels[j].add_client(*cli);
	if(channels[j].GetTopicName().empty())
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),token[i].first) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickName(),channels[j].GetName(),channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),channels[j].GetName()),fd);
	else
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),token[i].first) + \
			RPL_TOPICIS(GetClient(fd)->GetNickName(),channels[j].GetName(),channels[j].GetTopicName()) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickName(),channels[j].GetName(),channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),channels[j].GetName()),fd);
	channels[j].sendTo_all(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first), fd);
}

void	Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(GetClient(fd)->GetNickName()) >= 10)
	{
		senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
		return ;
	}
	Channel	newChannel;
	newChannel.SetName(token[i].first);
	newChannel.add_admin(*GetClient(fd));
	newChannel.set_createiontime();
	this->channels.push_back(newChannel);
	//notify taht the client join the channel
	_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),newChannel.GetName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickName(),newChannel.GetName(),newChannel.clientChannel_list()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),newChannel.GetName()),fd);
}

void	Server::JoinCommand(std::string &cmd, int &fd)
{
    std::vector < std::pair < std::string, std::string >> token;
    if (!SplitJoin(token, cmd, fd))
    {
        senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n");
        return ;
    }
    if (token.size() > 10)
    {
        senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many channels\r\n");
        return ;
    }
    for (size_t i = 0; i < token.size(); i++)
    {
        if (this->channels[j].GetName() == token[i].first)
        {
            ExistCh(token, i, j, fd);
            flag = true;
            break;
        }
    }
    if (!flag)
        NotExistCh(token, i, fd);
}
