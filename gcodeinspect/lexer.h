#pragma once

#include <vector>
#include <sstream>

namespace gcode {
	namespace lexer {
		enum class LexemeType { Letter, Number, LineBreak };

		struct Lexeme {
			LexemeType type;
			union {
				char letter;
				float number;
			};
		};

		std::vector<Lexeme> lex_stream(std::stringstream);
		std::vector<Lexeme> lex_string(std::string);
	}
}
