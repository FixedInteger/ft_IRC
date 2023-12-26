# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 06:59:49 by heddahbi          #+#    #+#              #
#    Updated: 2023/12/26 07:00:21 by heddahbi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	=	ircserv
CPP		=	c++
RM		=	rm -rf
FLAGS	=	-Wall -Wextra -Werror -std=c++98 

INCLUDES = $(wildcard ./includes/*.hpp)
SRCS	 =	$(wildcard ./Factory/*.cpp)

OBJS	=	$(SRCS:%.cpp=%.o)

all:	$(NAME)

$(NAME) : $(OBJS) $(INCLUDES)
	$(CPP) $(FLAGS) $(OBJS) -o $@

%.o : %.cpp $(INCLUDES)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
