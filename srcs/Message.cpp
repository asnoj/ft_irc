#include "Message.hpp"
#include <cctype>

Message::Message() : _hasTrailing(false) {}

Message::Message(const std::string &line) : _hasTrailing(false)
{
	parse(line);
}

Message::Message(const Message &o)
	: _prefix(o._prefix), _command(o._command),
	_params(o._params), _hasTrailing(o._hasTrailing) {}

Message &Message::operator=(const Message &o)
{
	if (this != &o)
	{
		_prefix = o._prefix;
		_command = o._command;
		_params = o._params;
		_hasTrailing = o._hasTrailing;
	}
	return *this;
}

Message::~Message() {}

const	std::string &Message::getPrefix() const  { return _prefix; }
const	std::string &Message::getCommand() const { return _command; }
const	std::vector<std::string> &Message::getParams() const { return _params; }
size_t	Message::paramCount() const  { return _params.size(); }
bool	Message::hasTrailing() const { return _hasTrailing; }
bool	Message::isEmpty() const { return _command.empty(); }

const std::string &Message::getParam(size_t index) const
{
	static const std::string empty;
	if (index >= _params.size())
		return empty;
	return _params[index];
}

void Message::skipSpaces(const std::string &s, size_t &i)
{
	while (i < s.size() && s[i] == ' ')
		++i;
}

void Message::parse(const std::string &line)
{
	size_t i = 0;
	const size_t n = line.size();
	size_t end;

	skipSpaces(line, i);

	// 1. prefix optionnel
	if (i < n && line[i] == ':')
 	{
		end = line.find(' ', i);
		if (end == std::string::npos)
		{
			_prefix = line.substr(i + 1);
			return;							// no command
		}
		_prefix = line.substr(i + 1, end - i - 1);
		i = end;
		skipSpaces(line, i);
	}

	// 2. command
	end = line.find(' ', i);
	if (end == std::string::npos)
		end = n;
	_command = line.substr(i, end - i);
	for (size_t k = 0; k < _command.size(); ++k)
		_command[k] = std::toupper(static_cast<unsigned char>(_command[k]));
	i = end;

	// 3. param
	while (i < n)
	{
		skipSpaces(line, i);
		if (i >= n)
			break;
		if (line[i] == ':')					// trailing : all the rest
		{
			_params.push_back(line.substr(i + 1));
			_hasTrailing = true;
			break;
		}
		end = line.find(' ', i);
		if (end == std::string::npos)
			end = n;
		_params.push_back(line.substr(i, end - i));
		i = end;
	}
}
