#include "Server.hpp"

/*
###############################################################################
#									TOPIC									  #
###############################################################################
*/
Revisar, no me queda claro
/*
std::string Server::tTopic()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Server::gettopic(std::string &input)
{
	size_t pos = input.find(":");
	if (pos == std::string::npos)
	{
		return "";
	}
	return input.substr(pos);
}

int Server::getpos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
		if (cmd[i] == ':' && (cmd[i - 1] == 32))
			return i;
	return -1;
}
*/

void	Server::TopicCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	size_t						pos;
	std::vector<std::string>	tmp;
	std::string 				nmch;
	if (splited_cmd = "TOPIC :" || splited_cmd.size() == 1)
	{
		SendError(461, fd, GetClient(fd)->GetNickName(), " :Not enough parameters\r\n");
		return ;
	}
	nmch = splited_cmd[1].substr(1);
	if (!GetChannel(nmch))
	{
		SendError(403, "#" + nmch, " :No such channel \r\n");
		return ;
	}
	if (!(GetChannel(nmch)->GetClient(fd)) && !(GetChannel(nmch)->GetAdmin(fd)))
	{
		SendError(442, "#" + nmch, " :You're not on that channel\r\n");
		return ;
	}
	if (splited_cmd.size() == 2)
	{
		if (GetChannel(nmch)->GetTopicName() == "")
		{
			SendResponse(": 331 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " : No topic is set\r\n");
			return ;
		}
		pos = GetChannel(nmch)->GetTopicName().find(":"); //Puede que esto de problemas con el spliteado.
		if (GetChannel(nmch)->GetTopicName() != "" && pos == std::string::npos)
		{
			SendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n");
			SendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(nmch)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n");
			return ;
		}
		else
		{
			pos = GetChannel(nmch)->GetTopicName().find(" ");
			if (pos == 0)
				GetChannel(nmch)->GetTopicName().erase(0, 1);
			SendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n");
			SendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(nmch)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n");
			return ;	
		}
	}
	if (splited_cmd.size() >= 3)
	{
		Revisar/*
		int pos = getpos(cmd);
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
			tmp.push_back(cmd.substr(getpos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{SendError(331, "#" + nmch, " :No topic is set\r\n");return;} // RPL_NOTOPIC (331) if no topic is set

		if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->GetClient(fd))
		{SendError(482, "#" + nmch, " :You're Not a channel operator\r\n");return;} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->GetAdmin(fd))
		{
			GetChannel(nmch)->SetTime(tTopic());
			GetChannel(nmch)->SetTopicName(tmp[2]);
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " :" + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			else
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			GetChannel(nmch)->sendTo_all(rpl);
		}
		else
		{
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
			{
				GetChannel(nmch)->SetTime(tTopic());
				GetChannel(nmch)->SetTopicName(tmp[2]);
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			else
			{
				size_t poss = tmp[2].find(" ");
				GetChannel(nmch)->SetTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':')
					tmp[2] = tmp[2].substr(1);
				GetChannel(nmch)->SetTopicName(tmp[2]);
				GetChannel(nmch)->SetTime(tTopic());
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			GetChannel(nmch)->sendTo_all(rpl);
		}
		*/
	}
}