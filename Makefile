SRCS			=	$(shell find sources -name "*.c")
OBJS			=	$(SRCS:.c=.o)

HEADERS			=	includes/
CC				=	cc
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
LIBS			=	-L ./printf -lftprintf

NAME			=	pipex

# %.o: %.c $(HEADERS) Makefile
# 	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

all:			$(NAME)

$(NAME):		$(SRCS)
				$(CC) $(SRCS) $(LIBS) -o $(NAME)

bonus: all

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re bonus