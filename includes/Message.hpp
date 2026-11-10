#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <cstddef>
# include <string>
# include <vector>

class Message
{
public:
	Message();
	explicit Message(const std::string &line);
	Message(const Message &other);
	Message &operator=(const Message &other);
	~Message();

	// Getters
	const std::string					&getPrefix() const;
	const std::string					&getCommand() const;
	const std::vector<std::string>		&getParams() const;
	const std::string					&getParam(size_t index) const;	// "" si hors limites
	size_t								paramCount() const;
	bool								hasTrailing() const;
	bool								isEmpty() const;				// pas de commande

private:
	std::string							_prefix;
	std::string							_command;   // en MAJUSCULES
	std::vector<std::string>			_params;    // le trailing est le dernier element
	bool								_hasTrailing;

	void parse(const std::string &line);
	static void skipSpaces(const std::string &s, size_t &i);
};

#endif