#ifndef ATARGET_HPP
# define ATARGET_HPP

#include <iostream>
#include <string>
#include "ASpell.hpp"

class	ASpell;

class	ATarget
{
	private:
		std::string	type;

	pubic:
		ATarget();
		ATarget(ATarget const &other);
		ATarget(std::string type);
		virtual ~ATarget();

		ATarget	&operator=(ATarget const &other);
		
		std::string const	&getType() const;
		ATarget				*clone() const = 0;
		void				getHitBySpell(ASpell const &spell);
};

#endif
