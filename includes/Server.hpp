#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>
# include <vector>
# include <poll.h>
# include <signal.h>
# include "CommandHandler.hpp"

class Client;
class Channel;

class Server
{
	public:
		Server(int port, const std::string &password);
		~Server();

		void		run();								// boucle principale
		static void	signalHandler(int sig);				// leve le flag _stop

		// ---- Contrat avec la partie B ----
		const std::string	&getPassword() const;
		Client				*findClientByNick(const std::string &nick);
		Channel				*findChannel(const std::string &name);
		Channel				*createChannel(const std::string &name);
		void				removeChannel(const std::string &name);
		void				removeClientFromAllChannels(Client &c);

	private:
		int							_port;
		std::string					_password;
		int							_listenFd;
		std::map<int, Client*>		_clients;		// cle = fd
		std::map<std::string, Channel*>	_channels;	// cle = nom en minuscules
		CommandHandler				_handler;

		static volatile sig_atomic_t	_stop;

		Server(const Server &);					// interdits
		Server &operator=(const Server &);

		void	setupSocket();
		void	buildPollList(std::vector<pollfd> &fds) const;
		void	acceptClient();
		void	readFromClient(Client &c);
		void	writeToClient(Client &c);
		void	removeClosingClients();
		void	removeClient(int fd);
};

#endif