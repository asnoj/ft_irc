/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcrochet <jcrochet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 15:39:23 by aronnet           #+#    #+#             */
/*   Updated: 2026/11/10 23:12:08 by jcrochet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;

		bool		_authenticated;	// PASS ok
		bool		_registered;	// PASS + NICK + USER recus

		std::string	_recvBuffer;	// donnees recues, pas encore une commande complete
		std::string	_sendBuffer;	// donnees a envoyer (si send() n'a pas tout pris)

		Client();	// prive : on force Client(int fd)

	public:
		Client(int fd);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		// Getters
		int					getFd() const;
		const std::string	&getNickname() const;
		const std::string	&getUsername() const;
		const std::string	&getRealname() const;
		const std::string	&getHostname() const;
		bool				isAuthenticated() const;
		bool				isRegistered() const;

		// Setters
		void	setNickname(const std::string &nick);
		void	setUsername(const std::string &user);
		void	setRealname(const std::string &real);
		void	setHostname(const std::string &host);
		void	setAuthenticated(bool value);
		void	setRegistered(bool value);

		// Buffer de reception
		void	appendToRecvBuffer(const std::string &data);
		bool	extractLine(std::string &line);	// true si une ligne complete a ete extraite

		// Buffer d'envoi
		void	queueMessage(const std::string &msg);
		bool	hasPendingData() const;			// utile pour activer POLLOUT
		const std::string	&getSendBuffer() const;
		void	consumeSendBuffer(size_t bytes);	// retire ce que send() a reellement envoye
};

#endif