#include "Channel.hpp"
#include "Client.hpp"

// ---- Forme canonique ----

Channel::Channel(const std::string &name)
	: _name(name),
	  _inviteOnly(false),
	  _topicRestricted(false),
	  _hasLimit(false),
	  _limit(0)
{
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		_name = other._name;
		_topic = other._topic;
		_members = other._members;
		_operators = other._operators;
		_invited = other._invited;
		_inviteOnly = other._inviteOnly;
		_topicRestricted = other._topicRestricted;
		_key = other._key;
		_hasLimit = other._hasLimit;
		_limit = other._limit;
	}
	return (*this);
}

Channel::~Channel()
{
	// Les Client* appartiennent au Server : on ne delete rien ici
}

// ---- Membres ----

bool Channel::isMember(Client *client) const
{
	return (_members.find(client) != _members.end());
}

void Channel::addMember(Client *client)
{
	_members.insert(client);
	_invited.erase(client);	// l'invitation est consommee une fois dedans
}

void Channel::removeMember(Client *client)
{
	_members.erase(client);
	_operators.erase(client);	// un kicke / parti ne reste pas op
	_invited.erase(client);
}

size_t Channel::getMemberCount() const { return (_members.size()); }
const std::set<Client*> &Channel::getMembers() const { return (_members); }

// ---- Operateurs ----

bool Channel::isOperator(Client *client) const
{
	return (_operators.find(client) != _operators.end());
}

void Channel::addOperator(Client *client)
{
	if (isMember(client))	// un op est forcement membre
		_operators.insert(client);
}

void Channel::removeOperator(Client *client)
{
	_operators.erase(client);
}

// ---- Invitations ----

void Channel::invite(Client *client)
{
	_invited.insert(client);
}

bool Channel::isInvited(Client *client) const
{
	return (_invited.find(client) != _invited.end());
}

// ---- Infos ----

const std::string &Channel::getName() const { return (_name); }
const std::string &Channel::getTopic() const { return (_topic); }
void Channel::setTopic(const std::string &topic) { _topic = topic; }

// ---- Modes ----

bool Channel::isInviteOnly() const { return (_inviteOnly); }
bool Channel::isTopicRestricted() const { return (_topicRestricted); }
const std::string &Channel::getKey() const { return (_key); }
bool Channel::hasLimit() const { return (_hasLimit); }
size_t Channel::getLimit() const { return (_limit); }

void Channel::setInviteOnly(bool value) { _inviteOnly = value; }
void Channel::setTopicRestricted(bool value) { _topicRestricted = value; }
void Channel::setKey(const std::string &key) { _key = key; }
void Channel::setLimit(size_t limit) { _hasLimit = true; _limit = limit; }
void Channel::removeLimit() { _hasLimit = false; _limit = 0; }

// ---- Logique ----

int Channel::canJoin(Client *client, const std::string &key) const
{
	if (_inviteOnly && _invited.find(client) == _invited.end())
		return (473);	// ERR_INVITEONLYCHAN

	if (!_key.empty() && _key != key)
		return (475);	// ERR_BADCHANNELKEY

	if (_hasLimit && _members.size() >= _limit)
		return (471);	// ERR_CHANNELISFULL

	return (0);
}

void Channel::broadcast(const std::string &msg, Client *exclude)
{
	std::set<Client*>::const_iterator it;

	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (*it != exclude)
			(*it)->queueMessage(msg);
	}
}