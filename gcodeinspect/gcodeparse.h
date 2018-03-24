#pragma once

#include <vector>
#include <string>

namespace gcode {
	enum class ParseStatus {
		Success,
		BadSyntax,
	};

	struct Argument {
		char letter;
		double value;
	};

	struct Element {
		char letter;
		int number;
		std::vector<Argument> arguments;
	};

	struct ParseResults {
		std::vector<Element> elements;
		ParseStatus status;
	};

	ParseResults parse_string(std::string source);

	std::string debug_to_string(ParseStatus status);
	std::string debug_to_string(Argument argument);
	std::string debug_to_string(Element element);
	std::string debug_to_string(ParseResults results);
} // namespace gcode
