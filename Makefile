############################################################################################################
# Variables

NAME			=	pipex
BONUS			=	pipex_bonus

# Dependencies
LIBFT_DIR		=	./libft
LIBFT			=	$(LIBFT_DIR)/libft.a

# Sources
COMMON_SRC =	pipex_utils.c pipex_split.c
PIPEX_SRC =		pipex.c $(COMMON_SRC)
PIPEX_BONUS_SRC =	pipex_bonus.c pipex_utils_bonus.c $(COMMON_SRC)

# Objects
OBJ_DIR	=		./obs
PIPEX_OBJ =		$(addprefix $(OBJ_DIR)/, $(PIPEX_SRC:.c=.o))
PIPEX_BONUS_OBJ =	$(addprefix $(OBJ_DIR)/, $(PIPEX_BONUS_SRC:.c=.o))

# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
MKDIR = mkdir -p

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
NC = \033[0m # No color

############################################################################################################
# Minishell Rules
all: ascii_art
bonus: ascii_art_bonus


ascii_art:
	@if ! $(MAKE) -q $(NAME); then \
		printf "\033[0;35m"; \
		printf "██████╗ ██╗██████╗ ███████╗██╗  ██╗\n"; \
		printf "██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝\n"; \
		printf "██████╔╝██║██████╔╝█████╗   ╚███╔╝ \n"; \
		printf "██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗ \n"; \
		printf "██║     ██║██║     ███████╗██╔╝ ██╗\n"; \
		printf "╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝\n"; \
		printf "			  MRUBAL-C\033[0m\n"; \
		echo "$(YELLOW)\nCreating program...$(GREEN)"; \
		i=0; \
		while [ $$i -lt 30 ]; do \
			echo -n "█"; \
			sleep 0.05; \
			i=$$((i + 1)); \
		done; \
		$(MAKE) -s $(NAME); \
	else \
		echo "$(GREEN)[$(NAME)] is already up to date.$(NC)"; \
	fi

ascii_art_bonus:
	@if ! $(MAKE) -q $(BONUS); then \
		printf "\033[0;35m"; \
		printf "██████╗ ██╗██████╗ ███████╗██╗  ██╗\n"; \
		printf "██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝\n"; \
		printf "██████╔╝██║██████╔╝█████╗   ╚███╔╝ \n"; \
		printf "██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗ \n"; \
		printf "██║     ██║██║     ███████╗██╔╝ ██╗\n"; \
		printf "╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝\n"; \
		printf "			  MRUBAL-C\033[0m\n"; \
		echo "$(YELLOW)\nCreating program...$(GREEN)"; \
		i=0; \
		while [ $$i -lt 30 ]; do \
			echo -n "█"; \
			sleep 0.05; \
			i=$$((i + 1)); \
		done; \
		$(MAKE) -s $(BONUS); \
	else \
		echo "$(GREEN)[$(BONUS)] is already up to date.$(NC)"; \
	fi
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(PIPEX_OBJ)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(PIPEX_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "$(GREEN)Program created 👍: $(NAME)$(NC)"

$(BONUS): $(PIPEX_BONUS_OBJ)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(PIPEX_BONUS_OBJ) -L$(LIBFT_DIR) -lft -o $(BONUS)
	@echo "$(GREEN)Program created 👍: $(BONUS)$(NC)"

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean -s
	@echo "$(RED)Removing object files... 🗑️$(NC)"
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean -s
	@echo "$(RED)Removing program... 🧹$(NC)"
	@$(RM) $(NAME)
	@$(RM) $(BONUS)
	@echo "$(GREEN)Full clean complete 🗑️$(NC)"

re: fclean all

rebonus: fclean bonus

.PHONY: all bonus clean fclean re rebonus
############################################################################################################
