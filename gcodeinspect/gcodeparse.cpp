// gcodeparse.cpp : Defines the exported functions for the DLL application.
//

#include "gcodeparse.h"

#include <vector>
#include <string>
#include <sstream>

#include "lexer.h"

namespace gcode {
	ParseResults parse_string(std::string source)
	{
		std::vector<lexer::Lexeme> lexed_results = lexer::lex_string(source);
		std::vector<Element> elements;

		int index = 0;

		while (index < lexed_results.size()) {
			lexer::Lexeme lexeme = lexed_results[index];
			index++;

			// Entry point of the line parsing.
			// It is a parse error to start with a number.
			if (lexeme.type == lexer::LexemeType::Number) {
				ParseResults result;
				result.status = ParseStatus::BadSyntax;

				return result;
			}
			// If it's a line break just continue on.
			else if (lexeme.type == lexer::LexemeType::LineBreak) {
				continue;
			}
			// If it's a letter, start parsing a gcode line.
			else if (lexeme.type == lexer::LexemeType::Letter) {
				Element element;
				element.letter = lexeme.letter;

				// Expect a number immediately thereafter.
				lexer::Lexeme next_lexeme = lexed_results[index++];

				if (next_lexeme.type == lexer::LexemeType::Number) {
					element.number = next_lexeme.number;
				}
				else {
					ParseResults result;
					result.status = ParseStatus::BadSyntax;

					return result;
				}

				// Now parse arguments.
				while (index < lexed_results.size()) {
					if (lexed_results[index].type == lexer::LexemeType::LineBreak) {
						break;
					}
					else {
						lexer::Lexeme letter_lexeme = lexed_results[index];
						lexer::Lexeme number_lexeme = lexed_results[index + 1];

						Argument argument;
						argument.letter = letter_lexeme.letter;
						argument.value = number_lexeme.number;

						element.arguments.push_back(argument);

						index += 2;
					}
				}

				elements.push_back(element);
			}
		}

		ParseResults result;
		result.status = ParseStatus::Success;
		result.elements = elements;

		return result;
	}

	std::string debug_to_string(ParseStatus status)
	{
		return std::string(status == ParseStatus::Success ? "Success" : "BadSyntax");
	}

	std::string debug_to_string(Argument argument)
	{
		std::ostringstream output;
		output << "Argument ";
		output << argument.letter;
		output << argument.value;
		return output.str();
	}

	std::string debug_to_string(Element element)
	{
		std::ostringstream output;
		output << "Element ";
		output << element.letter;
		output << element.number;
		output << " { ";

		for (Argument argument : element.arguments) {
			output << debug_to_string(argument);
			output << ", ";
		}

		output << " } ";
		return output.str();
	}

	std::string debug_to_string(ParseResults results)
	{
		std::ostringstream output;
		output << "Status: ";
		output << debug_to_string(results.status);
		output << "; Elements: \n";

		for (Element element : results.elements) {
			output << "\t";
			output << debug_to_string(element);
			output << "\n";
		}

		return output.str();
	}
} // namespace gcode
