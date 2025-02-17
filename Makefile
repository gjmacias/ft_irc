
OS				:=	$(shell uname)
NAME 			=	ircserv
CC				= 	c++
FLAGS			=	$(F_STANDARDS) $(F_WARNINGS) $(F_DEBUG) $(F_DEPENDENCY) $(F_SANITIZERS)

F_STANDARDS		=	-std=c++98
F_WARNINGS		=	-Wall -Wextra -Werror -pedantic
F_DEBUG			=	-g3
F_DEPENDENCY	=	-MMD
F_SANITIZERS	=	#-fsanitize=address

###############################################################################
#									SRC										  #
###############################################################################

SRC			=	main.cpp \
				\
				conexion/Channels.cpp \
				conexion/Client.cpp \
				conexion/Server.cpp \
				\
				conexion/channelFunctions/AddChannel.cpp \
				conexion/channelFunctions/ChangeChannel.cpp \
				conexion/channelFunctions/RemoveChannel.cpp \
				conexion/channelFunctions/SenderChannel.cpp \
				conexion/channelFunctions/ValidationChannel.cpp \
				\
				conexion/clientFunctions/AddClient.cpp \
				conexion/clientFunctions/RemoveClient.cpp \
				conexion/clientFunctions/ValidationClient.cpp \
				\
				conexion/serverFunctions/AddServer.cpp \
				conexion/serverFunctions/ClientModifier.cpp \
				conexion/serverFunctions/ParseAndExecute.cpp \
				conexion/serverFunctions/RemoverServer.cpp \
				conexion/serverFunctions/SenderServer.cpp \
				conexion/serverFunctions/ValidationServer.cpp \
				\
				conexion/serverFunctions/commands/Invite.cpp \
				conexion/serverFunctions/commands/Join.cpp \
				conexion/serverFunctions/commands/Kick.cpp \
				conexion/serverFunctions/commands/Mode.cpp \
				conexion/serverFunctions/commands/Part.cpp \
				conexion/serverFunctions/commands/PrivateMessage.cpp \
				conexion/serverFunctions/commands/Quit.cpp \
				conexion/serverFunctions/commands/Topic.cpp \
				\
				functions/Splits.cpp \
				functions/UpperCase.cpp \
				functions/Validations.cpp \

OBJ			=	$(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))
DEPS		=	$(addprefix $(DPS_DIR), notdir($(SRC:.cpp=.d)))

###############################################################################
#									DIR_PATH								  #
###############################################################################

SRC_DIR		=	src/
OBJ_DIR		=	.obj/
DPS_DIR		=	.dps/

###############################################################################
#									LIBRARIES								  #
###############################################################################

LIB_PATH	=
LIB_FLAGS	=
LIB			=

ifeq ($(OS), Linux)
	LIB +=
else
	LIB +=
endif

###############################################################################
#									INCLUDES								  #
###############################################################################

INCS	=	-I 	includes/comunication \
			-I	includes/conexion \
			-I	includes/functions

ifeq ($(OS), Linux)
	INCS	+=
else
	INCS	+=
endif

###############################################################################
#									COLORS									  #
###############################################################################

DEF_COLOR		= 	\033[0m
GREEN 			= 	\033[38;5;46m
WHITE 			= 	\033[38;5;15m
GREY 			= 	\033[38;5;8m
ORANGE 			= 	\033[38;5;202m
RED 			= 	\033[38;5;160m

###############################################################################
#									RULES									  #
###############################################################################

all: make_dir $(NAME)
	@echo "" && echo "$(YELLOW)$(NAME) ready to use:$(DEF_COLOR)"

make_dir:
	@mkdir -p $(OBJ_DIR) $(DPS_DIR)
	@echo "$(GREEN)Compiling OBJECTS:$(DEF_COLOR)"



$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | make_dir
	@mkdir -p $(dir $@)
	@echo "$(GRAY)Compiling $< to $@ $(DEF_COLOR)"
	@$(CC) $(FLAGS) $(INCS) -c $< -o $@
	@mv $(basename $@).d $(DPS_DIR)

#					--------	MAKE LIBRARIES	--------						  #


#					--------	RULES PROGRAM	--------						  #

$(NAME): $(LIB) $(OBJ) Makefile
	@echo "$(MAGENTA)Compiling $(NAME)$(DEF_COLOR)"
	@$(CC) $(FLAGS) $(INCS) $(OBJ) -o $(NAME) 
	@echo "$(BLUE)IRCSERV ready to launch! $(DEF_COLOR)"

clean:
	@echo "$(RED)Removing ON $(NAME); OBJs and DEPs... $(DEF_COLOR)"
	@/bin/rm -rf $(OBJ_DIR) $(DPS_DIR)
	@echo "$(ORANGE)Done!$(DEF_COLOR)" && echo ""

fclean: clean
	@echo "$(RED)Removing execute $(NAME)... $(DEF_COLOR)"
	@/bin/rm -f $(NAME)
	@echo "$(ORANGE)Done!$(DEF_COLOR)" && echo ""
	
re:	clean all

###############################################################################
#									OTHERS									  #
###############################################################################

-include $(DEPS)
.PHONY: all clean fclean re		make_dir
