/*
	Part of the AthenaZero Chess Engine.

	This file contains the engine's entry point.

	The AthenaZero Chess Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AthenaZero is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
	bool exit = false;
	while (!exit)
	{
		std::cout << "ATHENA> ";
		std::string command;
		if (!std::getline(std::cin, command)) return 1;

		bool validCommand = false;

		if (command == "exit")
		{
			validCommand = true;
			exit = true;
		}
		
		if (!validCommand)
		{
			std::cout << "Unknown command '" << command << "'" << std::endl;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
