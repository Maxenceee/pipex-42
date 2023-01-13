SRCS			=	$(shell find mandatory -name "*.c")
OBJS			=	$(SRCS:.c=.o)

SRCS_BONUS		=	$(shell find bonus -name "*.c")
OBJS_BONUS		=	$(SRCS_BONUS:.c=.o)

HEADER_SRCS		=	pipex.h pipex_bonus.h
HEADERS_DIR		=	includes
HEADERS			=	$(addprefix $(HEADERS_DIR), $(HEADER_SRCS))
CC				=	cc
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
LIBS			=	-L ./printf -lftprintf

NAME			=	pipex

%.o: %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $< -o $@ -gc5

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(CC) $(OBJS) $(LIBS) -o $(NAME)
				@echo "$(GREEN)$(NAME) compiled!$(DEFAULT)"

bonus:			$(OBJS_BONUS)
				@$(CC) $(OBJS_BONUS) $(LIBS) -o $(NAME)
				@echo "$(GREEN)$(NAME) bonus compiled!$(DEFAULT)"

clean:
				$(RM) $(OBJS) $(OBJS_BONUS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re bonus