#pragma once

#include <vector>
#include <string>

class Preprocessor {
public:
	struct Definitions {
		std::vector<std::string> placeholder;
		std::vector<std::string> instruction;
	};
	Definitions definitions;
	void define(std::string name, std::string def);
};