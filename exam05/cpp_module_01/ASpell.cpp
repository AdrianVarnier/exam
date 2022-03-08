#include "ASpell.hpp"

ASpell::ASpell()
{
}

ASpell::ASpell(ASpell const &other) : name(other.name), effects(other.effects)
{
}

ASpell::ASpell(std::string name, std::string effects) : name(name), effects(effects)
{
}

ASpell::~ASpell()
{
}

ASpell	&ASpell::operator=(ASpell const &other)
{
	this->name = other.name;
	this->effects = other.effects;
	return (*this);
}

std::string	ASpell::getName() const
{
	return (this->name);
}

std::string	ASpell::getEffects() const
{
	return (this->effects);
}

void	ASpell::launch(ATarget const &target)
{
	target.getHitBySpell(this);
}
