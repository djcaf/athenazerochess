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

#include "perft.h"
#include "perftresults.h"

using namespace ATHENAZEROENG;

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
		else if (command == "perft")
		{
			validCommand = true;
			Perft perft;
			PerftResults results = perft.RunAllPerftTests(0, false);
			std::cout << "Result Count: " << results.GetCount() << std::endl << std::endl;

			int passed = 0;
			int failed = 0;

			for (size_t i = 0; i < results.GetCount(); ++i)
			{
				PerftResult result = results.GetResult(i);
				std::cout << "Test: " << result.GetTestName() << ", Depth: " << result.GetDepth() << std::endl;
				std::cout << "   Result: ";
				if (result.GetPassed())
				{
					std::cout << "PASSED" << std::endl;
				}
				else
				{
					std::cout << "FAILED" << std::endl;
				}
				std::cout << "   FEN: " << result.GetFen() << std::endl;
				std::cout << "   Setup: ";
				if (result.GetSetupPassed())
				{
					std::cout << "PASSED" << std::endl;

					std::cout << "   Integrity: ";
					if (result.GetIntergityCheckPassed())
					{
						std::cout << "PASSED" << std::endl;

						std::cout << "   Nodes: ";

						if (result.NodeCount().GetIsPassed())
						{
							std::cout << "PASSED" << std::endl;
							++passed;
						}
						else
						{
							std::cout << "FAILED" << std::endl;
							++failed;
						}

						std::stringstream nodeResults;
						//TODO: Why?
						//Disable C26444 as this causes issues in Visual Studio
#pragma warning (push)
#pragma warning (disable: 26444)	
						//TODO: Find out why needed?
						nodeResults.imbue(std::locale(""));
#pragma warning (pop)

						//Disable scientific notation
						nodeResults.setf(std::ios::fixed, std::ios::floatfield);
						nodeResults.setf(std::ios::showpoint);

						nodeResults << "      Expected: " << result.NodeCount().GetExpectedCount() << "\n";
						nodeResults << "      Actual: " << result.NodeCount().GetActualCount();
						std::cout << nodeResults.str() << std::endl;

						std::cout << "Total Time: " << result.GetTimeTaken() << std::endl;
						std::cout << "Rate: " << result.GetNodesPerSecond() << std::endl;
						std::cout << "Node Time: " << result.GetTimeForOneNode() << std::endl;
					}
					else
					{
						std::cout << "FAILED" << std::endl;
						++failed;
					}
				}
				else
				{
					std::cout << "FAILED" << std::endl;
					++failed;
				}
				std::cout << std::endl << std::endl;
			}

			std::cout << "Passed: " << passed << std::endl;
			std::cout << "Failed: " << failed << std::endl;
			std::cout << "Total: " << results.GetCount() << std::endl;
			if (failed == 0)
			{
				std::cout << " *** PASSED ***" << std::endl;
			}
			else
			{
				std::cout << " *** FAILED ***" << std::endl;
			}
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
