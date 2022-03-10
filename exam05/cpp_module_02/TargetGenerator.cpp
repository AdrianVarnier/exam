#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator()
{
}

TargetGenerator::~TargetGenerator()
{
	std::map<std::string, ATarget *>::iterator	it = this->targets.begin();
	std::map<std::string, ATarget *>::iterator	end = this->targets.end();

	for( ; it != end; it++)
		delete it->second;
	this->targets.clear();
}

void	TargetGenerator::learnTargetType(ATarget *target)
{
	if (this->targets.find(target->getType()) == this->targets.end())
		this->targets.insert(std::pair<std::string, ATarget *>(target->getType(), target));
}

void	TargetGenerator::forgetTargetType(std::string const &target)
{
	if (this->targets.find(target) != this->targets.end())
	{
		delete this->targets.find(target)->second;
		this->targets.erase(target);
	}
}

ATarget	*TargetGenerator::createTarget(std::string const &target) const
{
	if (this->targets.find(target) != this->targets.end())
		return (this->targets.find(target)->second->clone());
	else
		return (NULL);
}
