#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include "Endline.hpp"

#define ERR_NEEDMODEPARM(channelname, mode) (": 696 #" + channelname + " * You must specify a parameter for the key mode. " + mode + ENDLINE)
#define ERR_INVALIDMODEPARM(channelname, mode) (": 696 #" + channelname + " Invalid mode parameter. " + mode + ENDLINE)
#define ERR_KEYSET(channelname) (": 467 #" + channelname + " Channel key already set. " + ENDLINE)
#define ERR_UNKNOWNMODE(nickname, channelname, mode) (": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + ENDLINE)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + ENDLINE)
#define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + ENDLINE)
#define ERR_NOTOPERATOR(channelname) (": 482 #" + channelname + " :You're not a channel operator" + ENDLINE)
#define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + ENDLINE)
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + ENDLINE)
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + ENDLINE)
#define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + ENDLINE)
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + ENDLINE)
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + ENDLINE)
#define ERR_ERRONEUSUSERNAME(username) (": 432 " + username + " :Erroneous username" + ENDLINE)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + ENDLINE)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + ENDLINE)

#endif

