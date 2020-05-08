/*
	Part of the AthenaZero Chess Engine.

	This file contains a class to hold the details about a single perft test to run.

	AthenaZero is free software: you can redistribute it and/or modify
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

#ifndef ATHENAZERO_ENGINE_PERFT_TEST
#define ATHENAZERO_ENGINE_PERFT_TEST

#include <string>

namespace ATHENAZEROENG
{
	/*
		Holds a single test to run.
	*/
	class PerftTest
	{
	public:
		/*
			Creates a new instance of the class.

			depth: The depth of the test.
			fen: The FEN starting position for the test.
			testName: The name of the test.
			 expectedNodes: The expected number of nodes.
			 expectedCaptures: The expected number of captures. Set to less than 0 to disable checking this item.
			 expectedEnPassant: The expected number of enpassant. Set to less than 0 to disable checking this item.
			 expectedCastles: The expected number of castles. Set to less than 0 to disable checking this item.
			 expectedPromotions: The expected number of promotions. Set to less than 0 to disable checking this item.
			 expectedChecks: The expected number of checks. Set to less than 0 to disable checking this item.
			 expectedCheckmates: The expected number of checkmates. Set to less than 0 to disable checking this item.
		*/
		PerftTest(
			const int depth,
			const std::string& fen,
			const std::string& testName,
			const long long expectedNodes,
			const long long expectedCaptures,
			const long long expectedEnPassant,
			const long long expectedCastles,
			const long long expectedPromotions,
			const long long expectedChecks,
			const long long expectedCheckmates);

		/*
			Gets the depth for the test.
		*/
		inline int GetDepth() { return g_depth; }

		/*
			Gets the FEn for the test starting position.
		*/
		inline std::string GetFen() { return g_fen; }

		/*
			Gets the test's name.
		*/
		inline std::string GetTestName() { return g_testName; }

		/*
			Gets the expected nodes.
		*/
		inline long long GetExpectedNodes() { return g_expectedNodes; }

		/*
			Gets the expected captures (or -1 if this item is ignored).
		*/
		inline long long GetExpectedCaptures() { return g_expectedCaptures; }

		/*
			Gets the expected enpassant (or -1 if this item is ignored).
		*/
		inline long long GetExpectedEnPassant() { return g_expectedEnPassant; }

		/*
			Gets the expected castles (or -1 if this item is ignored).
		*/
		inline long long GetExpectedCastles() { return g_expectedCastles; }

		/*
			Gets the expected promotions (or -1 if this item is ignored).
		*/
		inline long long GetExpectedPromotions() { return g_expectedPromotions; }

		/*
			Gets the expected checks (or -1 if this item is ignored).
		*/
		inline long long GetExpectedChecks() { return g_expectedChecks; }

		/*
			Gets the expected checkmates (or -1 if this item is ignored).
		*/
		inline long long GetExpectedCheckmates() { return g_expectedCheckmates; }

	private:
		int g_depth{ 0 };
		std::string g_fen;
		std::string g_testName;
		long long g_expectedNodes{ 0 };
		long long g_expectedCaptures{ 0 };
		long long g_expectedEnPassant{ 0 };
		long long g_expectedCastles{ 0 };
		long long g_expectedPromotions{ 0 };
		long long g_expectedChecks{ 0 };
		long long g_expectedCheckmates{ 0 };
	};
}

#endif
