# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/10 13:51:54 by wricky-t          #+#    #+#              #
#    Updated: 2023/02/20 17:51:26 by wricky-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------------------------------------#
#   INGREDIENTS                                                                #
#------------------------------------------------------------------------------#

NAME		:= philo

CC			:= gcc -g3

CFLAGS		:= -Wall -Werror -Wextra -pthread

ifeq ($(DB), 1)
	CFLAGS += -fsanitize=thread
	# CFLAGS += -fsanitize=address
endif

SRC_PATH	:= .

OBJ_PATH	:= objs

INCLUDES	:= $(SRC_PATH)/*.h

INCFLAGS	:= -I $(INCLUDES)

RM			:= rm -rf

NORM		:= norminette

INSTR		:= "./philo 👥 💀 😋 😴 [🔢]"

#------------------------------------------------------------------------------#
#   PROGRAM'S SCRS                                                             #
#------------------------------------------------------------------------------#

SRCS		:= $(wildcard $(SRC_PATH)/*.c)

OBJS		:= $(SRCS:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)

#------------------------------------------------------------------------------#
#   STYLING                                                                    #
#------------------------------------------------------------------------------#

GR			:= \033[1;92m

BL			:= \033[1;34m

WH			:= \033[1;37m

UWH			:= \033[4;37m

YL			:= \033[1;33m

DF			:= \033[0m

#------------------------------------------------------------------------------#
#   RECIPE                                                                     #
#------------------------------------------------------------------------------#

all: $(NAME)
	@clear
	@echo ""
	@echo "                       $(GR)⣿⣿"
	@echo "                      ⣠⣿⣿⣄"
	@echo "                     ⢰⣿⣿⣿⣿⡆"
	@echo "                     ⠘⢻⣿⣿⡟⠃$(DF)"
	@echo "          $(GR) ⢀⣶⣶⡀$(DF)    ⢀⣀⣀⣈⣉⣉⣁⣀⣀⡀     $(GR)⣰⣶⣆"
	@echo "            ⣿⣿ $(DF) ⢠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⡄ $(GR) ⢰⣿⡆"
	@echo "          ⢠⣿⣿⣿⣷$(DF)  ⠉⠛⠿⠿⣿⣿⣿⣿⣿⣿⠿⠿⠛⠉  $(GR)⣼⣿⣿⣿⣧"
	@echo "          ⠸⢿⣿⣿⠿$(DF)   ⣷    $(GR)⣠⣄$(DF)    ⣾   $(GR)⠿⣿⣿⣿⠿"
	@echo "            ⣿⣿$(DF)    ⠉    $(GR)⣿⣿$(DF)    ⠉    $(GR)⣿⣿⣿"
	@echo "            ⠻⠟$(DF)        $(GR)⣾⣿⣿⣷ $(DF)       $(GR)⠘⠿⠃"
	@echo "                     $(GR)⢸⣿⣿⣿⣿⡇$(DF)"
	@echo "                     $(GR)⠈⢹⣿⣿⡏⠁$(DF)"
	@echo "                       $(GR)⣿⣿$(DF)"
	@echo ""
	@echo "$(WH)Welcome philosophers! How many will be dining today?"
	@echo ""
	@echo "       $(INSTR)$(DF)"
	@echo ""

ifeq ($(DB), 1)
	@echo ""
	@echo "               🐛 DEBUG MODE 🐛"
	@echo ""
endif

help:
	@clear
	@echo "$(WH)$(INSTR)"
	@echo ""
	@echo "ARGUMENTS:"
	@echo ""
	@echo "  👥: Number of philo      (1 - 200)"
	@echo "  💀: Time to die          (in ms)"
	@echo "  😋: Time to eat          (in ms)"
	@echo "  😴: Time to sleep        (in ms)"
	@echo "  🔢: Number of simulation $(YL)(OPTIONAL)"
	@echo ""
	@echo "Note: Positive numeric argument only!"
	@echo ""
	@echo "$(WH)To check if there's a $(GR)data race$(WH), compile with DB=1"
	@echo "$(DF)"

$(NAME): $(OBJS)
	@echo "$(YL)🧙 Preparing table for the philosophers...$(DF)"
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(WH)🍳 Cooking $(UWH)$(notdir $<)$(DF)"

clean:
	@clear
	@$(RM) $(OBJ_PATH)
	@echo "$(BL)🧻 Cleaning off the table...$(DF)"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) *.dSYM

re: fclean all

norm: $(SRCS)
	@clear
	@$(NORM) $(SRCS) $(INCLUDES)

val: all
	@valgrind --leak-check=full --show-leak-kinds=definite,possible --log-file=leaks.txt ./philo 4 410 200 200

.PHONY:
	clean fclean re
