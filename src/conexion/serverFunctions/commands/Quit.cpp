#include "Server.hpp"

/*
###############################################################################
#									Quit									  #
###############################################################################
*/
/*void	FindQ(std::string cmd, std::string tofind, std::string &str)
{
	size_t		i;
	std::string	tmp;

	for (i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] != ' ')
		{
			for (i = 0; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else
				tmp.clear();
		}
	}
	if (i < cmd.size())
		str = cmd.substr(i);
	for (i = 0; i < str.size() && str[i] == ' ' i++);
	str = str.substr(i);
}

std::string	SplitQuit(std::string cmd)
{
	std::istringstream	stm(cmd);
	std::string			reason;
	std::string			str;
	size_t				i;
	stm >> str;
	FindQ(cmd, str, reason);
	if (reason.empty())
		return (std::string("Quit"));
	if (reason[0] != ':')//if the message doesn't start with ':'
	{
		for (i = 0; i < reason.size(); i++)
		{
			if (reason[i] == ' ')
			{
				reason.erase(reason.begin() + i, reason.end());
				break;
			}
		}
		reason.insert(reason.begin(), ':');
	}
	return (reason);
}*/

void	Server::QuitCommand(std::vector<std::string> &splited_cmd, int &fd)
{
    (void)splited_cmd;
	(void)fd;
	std::vector<std::string>	reason;
	std::string	rpl;
	size_t		i;

	reason = splited_cmd;
	for (i = 0; i <channels.size(); i++)
	{
		if (channels[i].get_client(fd))
		{
			channels[i].remove_client(fd);
			if (channels[i].GetClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + reason + "\r\n";
				channels[i].sendTo_all(rpl);
			}
		}
		else if (channels[i].get_admin(fd))
		{
			channels[i].remove_admin(fd);
			if (channels[i].GetClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + reason +"\r\n";
				channels[i].sendTo_all(rpl);
			}
		}
	}
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	RemoveChannels(fd);
	RemoveClients(fd);
	RemoveFds(fd);
	close(fd);
}
