#include "Warlock.hpp"

Warlock::Warlock(std::string name, std::string title) : name(name), title(title)
{
	std::cout << this->name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock()
{
	std::map<std::string, ASpell *>::iterator	it = this->spells.begin();
	std::map<std::string, ASpell *>::iterator	end = this->spells.end();

	for( ; it != end; it++)
		delete it->second;
	this->spells.clear();
	std::cout << this->name << ": My job here is done!" << std::endl;
}

std::string	Warlock::getName() const
{
	return (this->name);
}

std::string	Warlock::getTitle() const
{
	return (this->title);
}

void	Warlock::setTitle(std::string title)
{
	this->title = title;
}

void	Warlock::introduce() const
{
	std::cout << this->name << ": I am " << this->name << ", " << this->title << "!" << std::endl;
}

void	Warlock::learnSpell(ASpell *spell)
{
	if (spells.find(spell->getName()) == this->spells.end())
		spells.insert(std::pair<std::string, ASpell *>(spell->getName(), spell));
}

void	Warlock::forgetSpell(std::string spell)
{
	if (spells.find(spell) != this->spells.end())
	{
		delete spells.find(spell)->second;
		spells.erase(spell);
	}
}

void	Warlock::launchSpell(std::string spell, ATarget const &target)
{
	if (spells.find(spell) != this->spells.end())
		spells.find(spell)->second->launch(target);
}
