#include "SpellBook.hpp"

SpellBook::SpellBook()
{
}

SpellBook::~SpellBook()
{
	std::map<std::string, ASpell *>::iterator	it = this->spells.begin();
	std::map<std::string, ASpell *>::iterator	end = this->spells.end();

	for( ; it != end; it++)
		delete it->second;
	this->spells.clear();
}

void	SpellBook::learnSpell(ASpell *spell)
{
	if (this->spells.find(spell->getName()) == this->spells.end())
		this->spells.insert(std::pair<std::string, ASpell *>(spell->getName(), spell));
}

void	SpellBook::forgetSpell(std::string const &spell)
{
	if (this->spells.find(spell) != this->spells.end())
	{
		delete this->spells.find(spell)->second;
		this->spells.erase(spell);
	}
}

ASpell	*SpellBook::createSpell(std::string const &spell) const
{
	if (this->spells.find(spell) != this->spells.end())
		return (this->spells.find(spell)->second->clone());
	else
		return (NULL);
}
