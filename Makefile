# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/11 00:53:25 by onouakch          #+#    #+#              #
#    Updated: 2023/12/26 03:08:14 by heddahbi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

NAME	=	ircserv
CPP		=	c++
RM		=	rm -rf
FLAGS	=	-Wall -Wextra -Werror 

INCLUDES	=	./Inc/server.hpp \
				./Inc/Channel.hpp \
				./Inc/Client.hpp \
				./Inc/Command.hpp \
		
SRCS	=	$(wildcard ./Factory/*.cpp)

OBJS	=	$(SRCS:%.cpp=%.o)

all:	$(NAME)

$(NAME) : $(OBJS)
	$(CPP) $(FLAGS) $(OBJS) -o $@

%.o : %.cpp $(INCLUDES)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
