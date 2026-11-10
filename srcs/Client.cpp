/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aronnet <aronnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 15:38:41 by aronnet           #+#    #+#             */
/*   Updated: 2026/11/09 16:19:46 by aronnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"


#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd),
	  _authenticated(false),
	  _registered(false)
{
}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_nickname = other._nickname;
		_username = other._username;
		_realname = other._realname;
		_hostname = other._hostname;
		_authenticated = other._authenticated;
		_registered = other._registered;
		_recvBuffer = other._recvBuffer;
		_sendBuffer = other._sendBuffer;
	}
	return (*this);
}

Client::~Client()
{
	// Le fd est ferme par le Server, pas ici
}

// Getters
int Client::getFd() const { return (_fd); }
const std::string &Client::getNickname() const { return (_nickname); }
const std::string &Client::getUsername() const { return (_username); }
const std::string &Client::getRealname() const { return (_realname); }
const std::string &Client::getHostname() const { return (_hostname); }
bool Client::isAuthenticated() const { return (_authenticated); }
bool Client::isRegistered() const { return (_registered); }

// Setters
void Client::setNickname(const std::string &nick) { _nickname = nick; }
void Client::setUsername(const std::string &user) { _username = user; }
void Client::setRealname(const std::string &real) { _realname = real; }
void Client::setHostname(const std::string &host) { _hostname = host; }
void Client::setAuthenticated(bool value) { _authenticated = value; }
void Client::setRegistered(bool value) { _registered = value; }

void Client::appendToRecvBuffer(const std::string &data) { _recvBuffer += data; }
void Client::queueMessage(const std::string &msg) { _sendBuffer += msg; }
bool Client::hasPendingData() const { return (!_sendBuffer.empty()); }
const std::string &Client::getSendBuffer() const { return (_sendBuffer); }

const std::string &Client::getSendBuffer() const
{
	return (_sendBuffer);
}
void Client::consumeSendBuffer(size_t bytes)
{
	_sendBuffer.erase(0, bytes);
}

bool Client::extractLine(std::string &line)
{
	size_t pos = _recvBuffer.find('\n');

	if (pos == std::string::npos)
		return (false);

	line = _recvBuffer.substr(0, pos);
	_recvBuffer.erase(0, pos + 1);

	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);

	return (true);
}