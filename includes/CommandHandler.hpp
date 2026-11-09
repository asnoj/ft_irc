#include "Message.hpp"

class CommandHandler
{
public:
	CommandHandler(Server &server);
	void handle(Client &client, const std::string &line);

private:
	typedef void (CommandHandler::*Handler)(Client &, const Message &);
	Server &_server;
	std::map<std::string, Handler> _commands;

	void pass(Client &, const Message &);
	void nick(Client &, const Message &);
	void user(Client &, const Message &);
	void join(Client &, const Message &);
	void privmsg(Client &, const Message &);
	void kick(Client &, const Message &);
	void invite(Client &, const Message &);
	void topic(Client &, const Message &);
	void mode(Client &, const Message &);
};
