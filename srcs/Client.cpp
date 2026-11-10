#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd), _authenticated(false), _registered(false), _closing(false)
{
}

Client::~Client() {}		// le fd est ferme par le Server, pas ici

std::string Client::getPrefix() const
{
	return (_nickname + "!" + _username + "@" + _hostname);
}

bool Client::isClosing() const { return (_closing); }
void Client::markForDisconnect() { _closing = true; }

void Client::appendToRecvBuffer(const std::string &data)
{
	// Le Server vide le buffer ligne par ligne apres chaque recv :
	// ici il ne reste normalement qu'UNE ligne incomplete.
	if (_recvBuffer.size() + data.size() > MAX_RECV_BUFFER)
	{
		_closing = true;
		return;
	}
	_recvBuffer += data;
}

void Client::queueMessage(const std::string &msg)
{
	if (_sendBuffer.size() + msg.size() > MAX_SEND_BUFFER)
	{
		_closing = true;
		return;
	}
	_sendBuffer += msg;
}
