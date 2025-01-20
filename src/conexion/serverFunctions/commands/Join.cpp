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
	std::isstringstream			iss(cmd);

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
	for (size_t i = 0; i < token.size(); i++)
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
        NoExistCh(token, i, fd);
}
