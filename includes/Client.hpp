/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcrochet <jcrochet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 15:39:23 by aronnet           #+#    #+#             */
/*   Updated: 2026/11/10 23:29:27 by jcrochet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstddef>
# include <string>

class Client
{
	private:
		static const size_t	MAX_RECV_BUFFER = 8192;		// une commande IRC fait <= 512 octets
		static const size_t	MAX_SEND_BUFFER = 65536;	// client trop lent => on le coupe

		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;

		bool		_authenticated;
		bool		_registered;
		bool		_closing;		// "a deconnecter en fin de tour de poll"

		std::string	_recvBuffer;
		std::string	_sendBuffer;

		// Interdits (C++98 : prives, jamais definis) => erreur de link/compil si on copie
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

	public:
		explicit Client(int fd);
		~Client();

		int					getFd() const;
		const std::string	&getNickname() const;
		const std::string	&getUsername() const;
		const std::string	&getRealname() const;
		const std::string	&getHostname() const;
		std::string			getPrefix() const;		// nick!user@host
		bool				isAuthenticated() const;
		bool				isRegistered() const;
		bool				isClosing() const;

		void	setNickname(const std::string &nick);
		void	setUsername(const std::string &user);
		void	setRealname(const std::string &real);
		void	setHostname(const std::string &host);
		void	setAuthenticated(bool value);
		void	setRegistered(bool value);
		void	markForDisconnect();

		void	appendToRecvBuffer(const std::string &data);	// debordement => _closing
		bool	extractLine(std::string &line);

		void	queueMessage(const std::string &msg);			// debordement => _closing
		bool	hasPendingData() const;
		const std::string	&getSendBuffer() const;
		void	consumeSendBuffer(size_t bytes);
};

#endif






















//#ifndef CLIENT_HPP
//# define CLIENT_HPP

//# include <string>

//class Client
//{
//	private:
//		int			_fd;
//		std::string	_nickname;
//		std::string	_username;
//		std::string	_realname;
//		std::string	_hostname;

//		bool		_authenticated;	// PASS ok
//		bool		_registered;	// PASS + NICK + USER recus

//		std::string	_recvBuffer;	// donnees recues, pas encore une commande complete
//		std::string	_sendBuffer;	// donnees a envoyer (si send() n'a pas tout pris)

//		Client();	// prive : on force Client(int fd)

//	public:
//		Client(int fd);
//		Client(const Client &other);
//		Client &operator=(const Client &other);
//		~Client();

//		// Getters
//		int					getFd() const;
//		const std::string	&getNickname() const;
//		const std::string	&getUsername() const;
//		const std::string	&getRealname() const;
//		const std::string	&getHostname() const;
//		bool				isAuthenticated() const;
//		bool				isRegistered() const;

//		// Setters
//		void	setNickname(const std::string &nick);
//		void	setUsername(const std::string &user);
//		void	setRealname(const std::string &real);
//		void	setHostname(const std::string &host);
//		void	setAuthenticated(bool value);
//		void	setRegistered(bool value);

//		// Buffer de reception
//		void	appendToRecvBuffer(const std::string &data);
//		bool	extractLine(std::string &line);	// true si une ligne complete a ete extraite

//		// Buffer d'envoi
//		void	queueMessage(const std::string &msg);
//		bool	hasPendingData() const;			// utile pour activer POLLOUT
//		const std::string	&getSendBuffer() const;
//		void	consumeSendBuffer(size_t bytes);	// retire ce que send() a reellement envoye
//};

//#endif