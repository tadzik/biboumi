#include <irc/irc_channel.hpp>
#include <algorithm>

IrcChannel::IrcChannel():
  joined(false),
  self(nullptr)
{
}

void IrcChannel::set_self(const std::string& name)
{
  this->self = std::make_unique<IrcUser>(name);
}

IrcUser* IrcChannel::add_user(const std::string& name,
                              const std::map<char, char>& prefix_to_mode)
{
  this->users.emplace_back(std::make_unique<IrcUser>(name, prefix_to_mode));
  return this->users.back().get();
}

IrcUser* IrcChannel::get_self() const
{
  return this->self.get();
}

IrcUser* IrcChannel::find_user(const std::string& name) const
{
  IrcUser user(name);
  for (const auto& u: this->users)
    {
      if (u->nick == user.nick)
        return u.get();
    }
  return nullptr;
}

void IrcChannel::remove_user(const IrcUser* user)
{
  this->users.erase(std::remove_if(this->users.begin(), this->users.end(),
                    [user](const std::unique_ptr<IrcUser>& u)
                    {
                      return user->nick == u->nick;
                    }), this->users.end());
}

void IrcChannel::remove_all_users()
{
  this->users.clear();
  this->self.reset();
}

DummyIrcChannel::DummyIrcChannel():
  IrcChannel(),
  joining(false)
{
}
