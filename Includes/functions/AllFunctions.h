#ifndef FUNCTIONS_H_
# define FUNCTIONS_H_

# include <vector>
# include <string>
# include <sstream>


//		Validations
bool 						isPortValid(std::string port);

//		ServerUtils
//		- Parse:
void						parse_and_exec_cmd(std::string &cmd, int fd)
//		- Utils:
std::vector<std::string>	split_recivedBuffer(std::string str);
std::vector<std::string>	split_cmd(std::string& cmd)
bool						notregistered(int fd)


#endif