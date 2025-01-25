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
	(void)splited_cmd;
	(void)fd;
	size_t						pos;
	std::vector<std::string>	scmd = splited_cmd;
	std::vector<std::string>	tmp;

	if (scmd = "TOPIC :")
	{
		senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
		return ;
	}
	if (scmd.size() == 1)
	{
		senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
		return ;
	}
	std::string nmch = scmd[1].substr(1);
	if (!GetChannel(nmch))
	{
		senderror(403, "#" + nmch, fd, " :No such channel \r\n");
		return ;
	}
	if (!(GetChannel(nmch)->get_client(fd)) && !(GetChannel(nmch)->get_admin(fd)))
	{
		senderror(442, "#" + nmch, fd, " :You're not on that channel\r\n");
		return ;
	}
	if (scmd.size() == 2)
	{
		if (GetChannel(nmch)->GetTopicName() == "")
		{
			_sendResponse(": 331 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " : No topic is set\r\n");
			return ;
		}
		pos = GetChannel(nmch)->GetTopicName().find(":"); //Puede que esto de problemas con el spliteado.
		if (GetChannel(nmch)->GetTopicName() != "" && pos == std::string::npos)
		{
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(nmch)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd);
			return ;
		}
		else
		{
			pos = GetChannel(nmch)->GetTopicName().find(" ");
			if (pos == 0)
				GetChannel(nmch)->GetTopicName().erase(0, 1);
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(nmch)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd);
			return ;	
		}
	}
	if (scmd.size() >= 3)
	{
		Revisar/*
		int pos = getpos(cmd);
		if (pos == -1 || scmd[2][0] != ':')
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(scmd[2]);
		}
		else
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(cmd.substr(getpos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{senderror(331, "#" + nmch, fd, " :No topic is set\r\n");return;} // RPL_NOTOPIC (331) if no topic is set

		if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->get_client(fd))
		{senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n");return;} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->get_admin(fd))
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