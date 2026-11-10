#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <cstddef>

class Client;

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;

		std::set<Client*>	_members;
		std::set<Client*>	_operators;	// sous-ensemble de _members
		std::set<Client*>	_invited;	// invites en attente, pas encore membres

		// Modes
		bool				_inviteOnly;		// +i
		bool				_topicRestricted;	// +t
		std::string			_key;				// +k : vide = pas de cle
		bool				_hasLimit;			// +l : actif ou non
		size_t				_limit;				// +l : la valeur

		Channel();	// prive : on force Channel(name)

	public:
		Channel(const std::string &name);
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);
		~Channel();

		// Membres
		bool					isMember(Client *client) const;
		void					addMember(Client *client);
		void					removeMember(Client *client);
		size_t					getMemberCount() const;
		const std::set<Client*>	&getMembers() const;

		// Operateurs
		bool	isOperator(Client *client) const;
		void	addOperator(Client *client);
		void	removeOperator(Client *client);

		// Invitations
		void	invite(Client *client);
		bool	isInvited(Client *client) const;

		// Infos
		const std::string	&getName() const;
		const std::string	&getTopic() const;
		void				setTopic(const std::string &topic);

		// Modes : getters
		bool				isInviteOnly() const;
		bool				isTopicRestricted() const;
		const std::string	&getKey() const;
		bool				hasLimit() const;
		size_t				getLimit() const;

		// Modes : setters
		void	setInviteOnly(bool value);
		void	setTopicRestricted(bool value);
		void	setKey(const std::string &key);	// "" = retirer la cle
		void	setLimit(size_t limit);			// active +l
		void	removeLimit();					// desactive +l

		// Logique
		int		canJoin(Client *client, const std::string &key) const;
		void	broadcast(const std::string &msg, Client *exclude = NULL);
};

#endif