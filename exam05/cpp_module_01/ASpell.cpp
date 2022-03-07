#include "ASpell.hpp"

ASpell::ASpell()
{
}

ASpell::ASpell(ASpell const &other) : name(other.name), effect(other.effect)
{
}

ASpell::ASpell(std::string name, std::string effect) : name(name), effect(effect)
{
}

ASpell::~ASpell()
{
}

ASpell	&ASpell::operator=(ASpell const &other)
{
	this->name = other.name;
	this->effect = other.effect;
	return (*this);
}

std::string	ASpell::getName() const
{
	return (this->name);
}

std::string	ASpell::getEffect() const
{
	return (this->effect);
}

void	ASpell::launch(ATarget const &target)
{
	target.getHitBySpell(this);
}
