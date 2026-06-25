CPP = c++
# d'inclure directement les headers
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

# --- MANDATORY (SERVEUR) ---
NAME = ircserv

SRCS =  ./mandatory/src/main.cpp \
        ./mandatory/src/Utils.cpp \
        ./mandatory/src/Server.cpp \
        ./mandatory/src/Client.cpp \
        ./mandatory/src/Authentication.cpp \
        ./mandatory/src/Join.cpp \
        ./mandatory/src/Invite.cpp \
        ./mandatory/src/Topic.cpp \
        ./mandatory/src/Privmsg.cpp \
        ./mandatory/src/Mode.cpp \
        ./mandatory/src/Kick.cpp \
        ./mandatory/src/parse_message.cpp \
        ./mandatory/src/Whois.cpp \
        ./mandatory/src/Channel.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = ./mandatory/includes/Utils.hpp \
          ./mandatory/includes/Server.hpp \
          ./mandatory/includes/Client.hpp \
          ./mandatory/includes/Message.hpp \
          ./mandatory/includes/IrcReply.hpp \
          ./mandatory/includes/Channel.hpp

# --- BONUS (BOT) ---
NAME_BOT = ircbot

SRCS_BOT = ./bonus/src/bot_main.cpp \
           ./bonus/src/Bot.cpp ./bonus/src/Utils.cpp

OBJS_BOT = $(SRCS_BOT:.cpp=.o)

HEADERS_BOT = ./bonus/includes/Bot.hpp ./bonus/includes/Utils.hpp

# Colors 
GREEN  = \033[92;5;118m
YELLOW = \033[93;5;226m
GRAY   = \033[33;2;37m
RESET  = \033[0m

# --------------------------------------------------------------------
all: $(NAME)

# --- COMPILATION MANDATORY ---
$(NAME): $(OBJS)
	@printf "$(GRAY) - Compiling ircserv...\n$(RESET)"
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN) - Executable ircserv ready.\n$(RESET)"

# Regle generique pour le dossier mandatory
./mandatory/src/%.o: ./mandatory/src/%.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

# --- LA REGLE BONUS ---
# bonus: $(NAME) $(NAME_BOT)
bonus:  $(NAME_BOT)

$(NAME_BOT): $(OBJS_BOT)
	@printf "$(GRAY) - Compiling ircbot...\n$(RESET)"
	@$(CPP) $(CPPFLAGS) $(OBJS_BOT) -o $(NAME_BOT)
	@printf "$(GREEN) - Executable ircbot ready.\n$(RESET)"

# Regle generique mise a jour pour le dossier bonus/src/
./bonus/src/%.o: ./bonus/src/%.cpp $(HEADERS_BOT)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

# --------------------------------------------------------------------
clean:
	@rm -f $(OBJS) $(OBJS_BOT)
	@printf "$(YELLOW) - Objects removed.\n$(RESET)"

fclean: clean
	@rm -f $(NAME) $(NAME_BOT)
	@printf "$(YELLOW) - Executables removed.\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus

#kin mochkil f compilation f bonus ok