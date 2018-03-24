#include "lexer.h"

#include <vector>
#include <sstream>

namespace gcode {
	namespace lexer {
		std::vector<Lexeme> lex_line(std::string line) {
			std::vector<Lexeme> result;

			// Skip the line if it's empty; there's nothing to do here.
			if (line.length() == 0) {
				return result;
			}

			unsigned int index = 0;

			while (index < line.length()) {
				char c = line[index];
				index++;

				// Comment.
				// Immediately abort further parsing of this line.
				if (c == ';') {
					break;
				}

				// If between 65-90 or 97-122, letter.
				if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
					Lexeme lexeme;
					lexeme.type = LexemeType::Letter;
					lexeme.letter = c;
					result.push_back(lexeme);
				}
				// If between 48-57 (0-9), digit. Start number parsing.
				// This assumes that Gcode numbers always start with a digit / negative sign.
				else if ((c >= 48 && c <= 57) || c == '-') {
					std::string digits;
					bool seenDecimal = false;

					digits.push_back(c);

					while (index < line.length()) {
						char numberChar = line[index];

						if (numberChar == '.') {
							if (!seenDecimal) {
								digits.push_back(numberChar);
							}
							else {
								break;
							}
						}
						else if (numberChar >= 48 && numberChar <= 57) {
							digits.push_back(numberChar);
						}
						else {
							break;
						}

						index++;
					}

					float number = stof(digits);
					Lexeme lexeme;
					lexeme.type = LexemeType::Number;
					lexeme.number = number;
					result.push_back(lexeme);
				}
			}

			// Add a newline lexeme.
			Lexeme lexeme;
			lexeme.type = LexemeType::LineBreak;
			result.push_back(lexeme);

			return result;
		}

		std::vector<Lexeme> lex_stream(std::stringstream input) {
			std::vector<Lexeme> result;
			std::string line;

			while (std::getline(input, line)) {
				auto line_lexemes = lex_line(line);
				result.reserve(line_lexemes.size());
				result.insert(result.end(), line_lexemes.begin(), line_lexemes.end());
			}

			return result;
		}

		std::vector<Lexeme> lex_string(std::string input) {
			return lex_stream(std::stringstream(input));
		}
	}
}
