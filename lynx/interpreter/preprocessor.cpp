#include "preprocessor.h"

void Preprocessor::define(std::string name, std::string def)
{
	definitions.placeholder.push_back(name);
	definitions.instruction.push_back(def);
}