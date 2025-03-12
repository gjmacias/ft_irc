#ifndef INFORMATION_HPP_
# define INFORMATION_HPP_

#include "Endline.hpp"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + ENDLINE)
#define RPL_UMODEIS(hostname, channelname, mode, user)  (":" + hostname + " MODE " + channelname + " " + mode + " " + user + ENDLINE)
#define RPL_CREATIONTIME(nickname, channelname, creationtime) (": 329 " + nickname + " " + channelname + " " + creationtime + ENDLINE)
#define RPL_CHANNELMODES(nickname, channelname, modes) (": 324 " + nickname + " " + channelname + " " + modes + ENDLINE)
#define RPL_CHANGEMODE(hostname, channelname, modesandarguments) (":" + hostname + " MODE " + channelname + " " + modesandarguments + ENDLINE)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + ENDLINE)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + ENDLINE)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + ENDLINE)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + ENDLINE)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " " + channelname + " :" + topic + "\r\n")

#endif

