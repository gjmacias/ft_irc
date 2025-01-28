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
	std::string			splited_cmd;
	std::string			str;
	size_t				i;
	stm >> str;
	FindQ(cmd, str, splited_cmd);
	if (splited_cmd.empty())
		return (std::string("Quit"));
	if (splited_cmd[0] != ':')//if the message doesn't start with ':'
	{
		for (i = 0; i < splited_cmd.size(); i++)
		{
			if (splited_cmd[i] == ' ')
			{
				splited_cmd.erase(splited_cmd.begin() + i, splited_cmd.end());
				break;
			}
		}
		splited_cmd.insert(splited_cmd.begin(), ':');
	}
	return (splited_cmd);
}*/

void	Server::QuitCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	std::string	rpl;
	size_t		i;

	for (i = 0; i <channels.size(); i++)
	{
		if (channels[i].GetClient(fd))
		{
			channels[i].RemoveClient(fd);
			if (channels[i].GetClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + splited_cmd + "\r\n";
				channels[i].SendEveryone(rpl);
			}
		}
		else if (channels[i].GetAdmin(fd))
		{
			channels[i].RemoveAdmin(fd);
			if (channels[i].GetClientsNumber() == 0) //Buscar equivalente
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + splited_cmd +"\r\n";
				channels[i].SendEveryone(rpl);
			}
		}
	}
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	RemoveChannel(fd);
	RemoveClient(fd);
	RemoveFd(fd);
	close(fd);
}
