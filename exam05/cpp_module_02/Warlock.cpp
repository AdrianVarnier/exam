#include "Warlock.hpp"

Warlock::Warlock(std::string name, std::string title) : name(name), title(title)
{
	std::cout << this->name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock()
{
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
	this->spellBook.learnSpell(spell);
}

void	Warlock::forgetSpell(std::string spell)
{
	this->spellBook.forgetSpell(spell);
}

void	Warlock::launchSpell(std::string spell, ATarget const &target)
{
	ASpell	*tmp = this->spellBook.createSpell(spell);
	if (tmp != NULL)
	{
		tmp->launch(target);
		delete tmp;
	}
}
