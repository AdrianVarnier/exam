#ifndef WARLOCK_HPP
# define WARLOCK_HPP

#include <string>
#include <iostream>
#include "ATarget.hpp"
#include "SpellBook.hpp"

class	Warlock
{
	private:
		Warlock();
		Warlock(Warlock const &other);
		Warlock	&operator=(Warlock const &other);

		std::string	name;
		std::string	title;
		SpellBook	spellBook;

	public:
		Warlock(std::string name, std::string title);
		virtual ~Warlock();

		std::string	getName() const;
		std::string	getTitle() const;
		void		setTitle(std::string title);
		void		introduce() const;
		void		learnSpell(ASpell *spell);
		void		forgetSpell(std::string spell);
		void		launchSpell(std::string spell, ATarget const &target);
};

#endif
