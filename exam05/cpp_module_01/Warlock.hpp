#ifndef WARLOCK_HPP
# define WARLOCK_HPP

#include <string>
#include <iostream>

class	Warlock
{
	private:
		Warlock();
		Warlock(Warlock const &other);
		Warlock	&operator=(Warlock const &other);

		std::string	name;
		std::string	title;

	public:
		Warlock(std::string name, std::string title);
		virtual ~Warlock();

		std::string	getName() const;
		std::string	getTitle() const;
		void		setTitle(std::string title);
		void		introduce() const;
};

#endif
