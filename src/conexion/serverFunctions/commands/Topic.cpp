#include "Server.hpp"

/*
###############################################################################
#									TOPIC									  #
###############################################################################
*/

std::string Server::TimeTopic()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Server::GetTopic(std::string &input)
{
	size_t pos = input.find(":");
	if (pos == std::string::npos)
		return ("");
	return (input.substr(pos));
}

int Server::GetPos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
	{
		if (cmd[i] == ':' && (cmd[i - 1] == 32))
			return (i);
	}
	return (-1);
}

void	Server::TopicCommand(std::vector<std::string> &splited_cmd, std::string &cmd, int &fd)
{
	size_t						pos;
	std::vector<std::string>	tmp;
	std::string 				namechannel;
	if (splited_cmd.size() == 1)
	{
		SendError(461, fd, GetClient(fd)->GetNickname(), " :Not enough parameters\r\n");
		return ;
	}
	namechannel = splited_cmd[1].substr(1);
	if (!GetChannel(namechannel))
	{
		SendError(403, fd, "#" + namechannel, " :No such channel \r\n");
		return ;
	}
	if (!(GetChannel(namechannel)->GetClient(fd)) && !(GetChannel(namechannel)->GetAdmin(fd)))
	{
		SendError(442, fd, "#" + namechannel, " :You're not on that channel\r\n");
		return ;
	}
	if (splited_cmd.size() == 2)
	{
		if (GetChannel(namechannel)->GetTopicName() == "")
		{
			SendResponse(": 331 " + GetClient(fd)->GetNickname() + " " + "#" + namechannel + " : No topic is set\r\n", fd);
			return ;
		}
		pos = GetChannel(namechannel)->GetTopicName().find(":"); //Puede que esto de problemas con el spliteado.
		if (GetChannel(namechannel)->GetTopicName() != "" && pos == std::string::npos)
		{
			SendResponse(": 332 " + GetClient(fd)->GetNickname() + " " + "#" + namechannel + " " + GetChannel(namechannel)->GetTopicName() + "\r\n", fd);
			SendResponse(": 332 " + GetClient(fd)->GetNickname() + " " + "#" + namechannel + " " + GetClient(fd)->GetNickname() + " " + "\r\n", fd);
			return ;
		}
		else
		{
			pos = GetChannel(namechannel)->GetTopicName().find(" ");
			if (pos == 0)
				GetChannel(namechannel)->GetTopicName().erase(0, 1);
			SendResponse(": 332 " + GetClient(fd)->GetNickname() + " " + "#" + namechannel + " " + GetChannel(namechannel)->GetTopicName() + "\r\n", fd);
			SendResponse(": 332 " + GetClient(fd)->GetNickname() + " " + "#" + namechannel + " " + GetClient(fd)->GetNickname() + " " + "\r\n", fd);
			return ;	
		}
	}
	if (splited_cmd.size() >= 3)
	{
		int pos = GetPos(cmd);
		if (pos == -1 || splited_cmd[2][0] != ':')
		{
			tmp.push_back(splited_cmd[0]);
			tmp.push_back(splited_cmd[1]);
			tmp.push_back(splited_cmd[2]);
		}
		else
		{
			tmp.push_back(splited_cmd[0]);
			tmp.push_back(splited_cmd[1]);
			tmp.push_back(cmd.substr(GetPos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{
			SendError(331, fd, "#" + namechannel, " :No topic is set\r\n");
			return ;
		} // RPL_NOTOPIC (331) if no topic is set

		if (GetChannel(namechannel)->GetModesTopicRestriction() && GetChannel(namechannel)->GetClient(fd))
		{
			SendError(482, fd, "#" + namechannel, " :You're Not a channel operator\r\n");
			return ;
		} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (GetChannel(namechannel)->GetModesTopicRestriction() && GetChannel(namechannel)->GetAdmin(fd))
		{
			//GetChannel(namechannel)->SetTime(TimeTopic());
			GetChannel(namechannel)->SetTopicName(tmp[2]);
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
				rpl = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@localhost TOPIC #" + namechannel + " :" + GetChannel(namechannel)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			else
				rpl = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@localhost TOPIC #" + namechannel + " " + GetChannel(namechannel)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			GetChannel(namechannel)->SendEveryone(rpl);
		}
		else
		{
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
			{
				//GetChannel(namechannel)->SetTime(TimeTopic());
				GetChannel(namechannel)->SetTopicName(tmp[2]);
				rpl = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@localhost TOPIC #" + namechannel + " " + GetChannel(namechannel)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			else
			{
				size_t poss = tmp[2].find(" ");
				GetChannel(namechannel)->SetTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':')
					tmp[2] = tmp[2].substr(1);
				GetChannel(namechannel)->SetTopicName(tmp[2]);
				//GetChannel(namechannel)->SetTime(TimeTopic());
				rpl = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@localhost TOPIC #" + namechannel + " " + GetChannel(namechannel)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			GetChannel(namechannel)->SendEveryone(rpl);
		}
	}
}

