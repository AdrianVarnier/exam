#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

#include <string>
#include <map>
#include "ASpell.hpp"

class	SpellBook
{
	private:
		SpellBook(SpellBook const &other);
		SpellBook	&operator=(SpellBook const &other);

		std::map<std::string, ASpell *>	spells;

	public:
		SpellBook();
		virtual ~SpellBook();

		void	learnSpell(ASpell *spell);
		void	forgetSpell(std::string const &spell);
		ASpell	*createSpell(std::string const &spell) const;
};

#endif
