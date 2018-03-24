// gcodeinspect.cpp : Defines the entry point for the console application.
//

#include "gcodeparse.h"
#include <vector>
#include <string>
#include <iostream>

int main()
{
	gcode::ParseResults results = gcode::parse_string(std::string("M18 S0\nG0 E-3.123"));
	std::cout << gcode::debug_to_string(results) << std::endl;
	system("pause");
    return 0;
}

