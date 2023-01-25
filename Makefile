SRCS			=	./mandatory/childs.c	\
					./mandatory/ft_calloc.c	\
					./mandatory/ft_putstr_fd.c	\
					./mandatory/ft_split.c	\
					./mandatory/ft_strjoin.c	\
					./mandatory/ft_strnstr.c	\
					./mandatory/parsing.c	\
					./mandatory/pipex.c
OBJS			=	$(SRCS:.c=.o)

SRCS_BONUS		=	./bonus/exits.c	\
					./bonus/ft_calloc.c	\
					./mandatory/ft_putstr_fd.c	\
					./bonus/ft_split.c	\
					./bonus/ft_strjoin.c	\
					./bonus/ft_strnstr.c	\
					./bonus/parsing.c	\
					./bonus/pipes.c	\
					./bonus/pipex.c
OBJS_BONUS		=	$(SRCS_BONUS:.c=.o)

HEADER_SRCS		=	pipex.h pipex_bonus.h
HEADERS_DIR		=	includes
HEADERS			=	$(addprefix $(HEADERS_DIR), $(HEADER_SRCS))
CC				=	cc
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
LIBS			=	-L ./printf -lftprintf

NAME			=	pipex

GREEN = \033[1;32m
DEFAULT = \033[0m

%.o: %.c $(HEADERS) Makefile
	@$(CC) $(CFLAGS) -c $< -o $@ -gc5

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(CC) $(OBJS) $(LIBS) -o $(NAME)
				@echo "$(GREEN)$(NAME) compiled!$(DEFAULT)"

bonus:			$(OBJS_BONUS)
				@$(CC) $(OBJS_BONUS) $(LIBS) -o $(NAME)
				@echo "$(GREEN)$(NAME) bonus compiled!$(DEFAULT)"

clean:
				@$(RM) $(OBJS) $(OBJS_BONUS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re bonus