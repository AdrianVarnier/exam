#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

#include "ATarget.hpp"
#include <map>
#include <string>

class	TargetGenerator 
{
	private:
		TargetGenerator(TargetGenerator const &other);
		TargetGenerator &operator=(TargetGenerator const &other);

		std::map<std::string, ATarget *>	targets;

		public:
			TargetGenerator();
			virtual	~TargetGenerator();

			void	learnTargetType(ATarget *);
			void	forgetTargetType(std::string const &target);
			ATarget	*createTarget(std::string const &target) const;
};

#endif
