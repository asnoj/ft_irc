#include "../includes/CommandHandler.hpp"

CommandHandler::CommandHandler(Server &server) : _server(server)
{
	_commands["PASS"]    = &CommandHandler::pass;
	_commands["NICK"]    = &CommandHandler::nick;
	_commands["USER"]    = &CommandHandler::user;
	_commands["JOIN"]    = &CommandHandler::join;
	_commands["PRIVMSG"] = &CommandHandler::privmsg;
	_commands["KICK"]    = &CommandHandler::kick;
	_commands["INVITE"]  = &CommandHandler::invite;
	_commands["TOPIC"]   = &CommandHandler::topic;
	_commands["MODE"]    = &CommandHandler::mode;
	_commands["CAP"]     = &CommandHandler::cap;
	_commands["PING"]    = &CommandHandler::ping;
	_commands["QUIT"]    = &CommandHandler::quit;
	_commands["PART"]    = &CommandHandler::part;
}

// CommandHandler.cpp
void CommandHandler::sendNumeric(Client &c, const std::string &code,
								 const std::string &params, const std::string &text)
{
	std::string out = ":";
	out += SERVER_NAME;
	out += " " + code + " ";
	out += c.getNickname().empty() ? "*" : c.getNickname();	// "*" tant qu'il n'a pas de nick
	if (!params.empty())
		out += " " + params;
	out += " :" + text + "\r\n";
	c.queueMessage(out);	// on ne fait QUE mettre en file : le send() se fera sur POLLOUT
}

void CommandHandler::handle(Client &client, const std::string &line)
{
	if (client.isClosing())			// deja condamne : on ignore ses commandes
		return;

	Message msg(line);
	if (msg.isEmpty())
		return;

	const std::string &cmd = msg.getCommand();
	std::map<std::string, Handler>::iterator it = _commands.find(cmd);
	if (it == _commands.end())
	{
		sendNumeric(client, ERR_UNKNOWNCOMMAND, cmd, "Unknown command");
		return;
	}
	if (!client.isRegistered()
		&& cmd != "PASS" && cmd != "NICK" && cmd != "USER"
		&& cmd != "CAP"  && cmd != "QUIT" && cmd != "PING")
	{
		sendNumeric(client, ERR_NOTREGISTERED, "", "You have not registered");
		return;
	}
	(this->*(it->second))(client, msg);
}
