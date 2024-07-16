NAME = libft.a
CC = cc
FLAGS = -Wall -Wextra -Werror# -g -fsanitize=address
AR = ar -rcs
RM = rm -f

SRC_DIR = src
OBJ_DIR = obj
GNL_DIR = getnextline/src
PRINTF_DIR = ft_printf/src

SRC = $(SRC_DIR)/ft_isalpha.c \
		$(SRC_DIR)/ft_isdigit.c \
		$(SRC_DIR)/ft_isalnum.c \
		$(SRC_DIR)/ft_isascii.c \
		$(SRC_DIR)/ft_isprint.c \
		$(SRC_DIR)/ft_strlen.c \
		$(SRC_DIR)/ft_memset.c \
		$(SRC_DIR)/ft_bzero.c \
		$(SRC_DIR)/ft_memcpy.c \
		$(SRC_DIR)/ft_memmove.c \
		$(SRC_DIR)/ft_strlcpy.c \
		$(SRC_DIR)/ft_strlcat.c \
		$(SRC_DIR)/ft_toupper.c \
		$(SRC_DIR)/ft_tolower.c \
		$(SRC_DIR)/ft_strchr.c \
		$(SRC_DIR)/ft_strrchr.c \
		$(SRC_DIR)/ft_strncmp.c \
		$(SRC_DIR)/ft_memchr.c \
		$(SRC_DIR)/ft_memcmp.c \
		$(SRC_DIR)/ft_strnstr.c \
		$(SRC_DIR)/ft_atoi.c \
		$(SRC_DIR)/ft_calloc.c \
		$(SRC_DIR)/ft_strdup.c \
		$(SRC_DIR)/ft_substr.c \
		$(SRC_DIR)/ft_strjoin.c \
		$(SRC_DIR)/ft_strtrim.c \
		$(SRC_DIR)/ft_split.c \
		$(SRC_DIR)/ft_itoa.c \
		$(SRC_DIR)/ft_strmapi.c \
		$(SRC_DIR)/ft_striteri.c \
		$(SRC_DIR)/ft_putchar_fd.c \
		$(SRC_DIR)/ft_putstr_fd.c \
		$(SRC_DIR)/ft_putendl_fd.c \
		$(SRC_DIR)/ft_putnbr_fd.c \
		$(SRC_DIR)/ft_lstnew_bonus.c \
		$(SRC_DIR)/ft_lstadd_front_bonus.c \
		$(SRC_DIR)/ft_lstsize_bonus.c \
		$(SRC_DIR)/ft_lstlast_bonus.c \
		$(SRC_DIR)/ft_lstadd_back_bonus.c \
		$(SRC_DIR)/ft_lstdelone_bonus.c \
		$(SRC_DIR)/ft_lstclear_bonus.c \
		$(SRC_DIR)/ft_lstiter_bonus.c \
		$(SRC_DIR)/ft_lstmap_bonus.c \
		$(GNL_DIR)/get_next_line.c \
		$(GNL_DIR)/get_next_line_utils.c \
		$(PRINTF_DIR)/ft_printf.c \
		$(PRINTF_DIR)/conversions/ft_putchar_printf.c \
		$(PRINTF_DIR)/conversions/ft_puthex_printf.c \
		$(PRINTF_DIR)/conversions/ft_putnbr_printf.c \
		$(PRINTF_DIR)/conversions/ft_putptr_printf.c \
		$(PRINTF_DIR)/conversions/ft_putstr_printf.c \
		$(PRINTF_DIR)/conversions/ft_putuns_printf.c

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC)) \
	$(patsubst $(GNL_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC)) \
	$(patsubst $(PRINTF_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC)) \
	$(patsubst $(PRINTF_DIR)/conversions/%.c, $(OBJ_DIR)/%.o, $(SRC)) \

all: $(NAME)

clean:
	@-$(RM) -r $(OBJ_DIR)
	@printf "Removed objects\n"
	
fclean: clean
	@-$(RM) $(NAME)
	@printf "Removed executables\n"

re: fclean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@printf "Building $@\n"
	@$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c
	@mkdir -p $(@D)
	@printf "Building $@\n"
	@$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PRINTF_DIR)/%.c
	@mkdir -p $(@D)
	@printf "Building $@\n"
	@$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PRINTF_DIR)/conversions/%.c
	@mkdir -p $(@D)
	@printf "Building $@\n"
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ) libft.h getnextline/get_next_line.h ft_printf/ft_printf.h
	@printf "Creating library...\n"
	@$(AR) $(NAME) $(OBJ) $^
	@printf "Done.\n"

.PHONY: all clean fclean re bonus