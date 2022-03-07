#ifndef ASPELL_HPP
# define ASPELL_HPP

class	ASpell
{
	private:
		std::string	name;
		std::string	effect;

	public:
		ASpell();
		ASpell(ASpell const &other);
		ASpell(std::string name, std::string effect);
		virtual ~ASpell();

		ASpell	&operator=(ASpell const &other);

		std::string	getName() const;
		std::string	getEffect() const;

		ASpell		*clone() const = 0;
};

#endif
