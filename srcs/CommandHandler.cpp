#include "../includes/CommandHandler.hpp"

// CommandHandler.cpp
void CommandHandler::handle(Client &client, const std::string &line)
{
	Message msg(line);
	if (msg.isEmpty())
		return;

	std::map<std::string, Handler>::iterator it = _commands.find(msg.getCommand());
	if (it == _commands.end())
	{
		// ERR_UNKNOWNCOMMAND (421) or ignor
		return;
	}
	const std::string &cmd = msg.getCommand();
	if (!client.isRegistered()
		&& cmd != "PASS" && cmd != "NICK" && cmd != "USER"
		&& cmd != "CAP"  && cmd != "QUIT")
	{
		// ERR_NOTREGISTERED (451)
		return;
	}
	(this->*(it->second))(client, msg);
	}