
OS				:=	$(shell uname)
NAME 			=	ircserv
CC				= 	c++
FLAGS			=	$(F_STANDARDS) $(F_WARNINGS) $(F_DEBUG) $(F_DEPENDENCY) $(F_SANITIZERS)

F_STANDARDS		=	-std=c++98
F_WARNINGS		=	-Wall -Wextra -Werror -pedantic
F_DEBUG			=	-g3
F_DEPENDENCY	=	-MMD -MP
F_SANITIZERS	=	-fsanitize=address

###############################################################################
#									SRC										  #
###############################################################################

SRC			=	src/main.cpp \
				\
				src/conexion/Server.cpp \
				src/conexion/Client.cpp \
				\
				src/conexion/serverFunctions/Authentifications.cpp \
				src/conexion/serverFunctions/Remover.cpp \
				src/conexion/serverFunctions/Sender.cpp \
				\
				src/conexion/serverFunctions/commands/Invite.cpp \
				src/conexion/serverFunctions/commands/Join.cpp \
				src/conexion/serverFunctions/commands/Kick.cpp \
				src/conexion/serverFunctions/commands/Mode.cpp \
				src/conexion/serverFunctions/commands/Quit.cpp \
				src/conexion/serverFunctions/commands/Topic.cpp \
				\
				src/functions/ParseAndExecute.cpp \
				src/functions/Splits.cpp \
				src/functions/Validations.cpp \

OBJ			= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEPS		= $(addprefix $(DPS_DIR), $(notdir $(SRC:.c=.d)))

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
#									INLUDES									  #
###############################################################################

INCS	= -I includes/

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
	@mkdir -p $(OBJ_DIR) $(DPS_DIR) $(UPL_DIR)
	@echo "$(GREEN)Compiling OBJECTS:$(DEF_COLOR)"



$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | make_dir
	@mkdir -p $(dir $@)
	@echo "$(GRAY)Compiling $< to $@ $(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@mv $(basename $@).d $(DPS_DIR)

#					--------	MAKE LIBRARIES	--------						  #


#					--------	RULES PROGRAM	--------						  #

$(NAME):  Makefile $(LIB) $(OBJ)
	@echo "$(MAGENTA)Compiling $(NAME)$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(INCS) $(OBJ) -o $(NAME) 
	@echo "$(BLUE)webserv ready to launch, use 80 as argument and connect in a browther as localhost.$(DEF_COLOR)"

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
