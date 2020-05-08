/*
	Part of the AthenaZero Chess Engine.

	This file contains code to run perft tests.

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

#ifndef ATHENAZERO_ENGINE_PERFT
#define ATHENAZERO_ENGINE_PERFT

#include <string>
#include <fstream>

#include "perftresults.h"
#include "board.h"
#include "perftinternalstats.h"
#include "perfttest.h"
#include <vector>

namespace ATHENAZEROENG
{
	class Perft
	{
	public:
		/*
			Creates a new instance of the class.
		*/
		Perft();
		/*
			Runs all perft tests.

			maxDepth: The maximum depth. Set to 0 (or less) to run all depths.
			stopOnFirstFailure: If true then stops running the tests then the first test fails.
			Returns: The reslts. An empty collection is returned on error or if there are no tests to run.
		*/
		PerftResults RunAllPerftTests(
			int maxDepth,
			bool stopOnFirstFailure);
	private:
		std::ofstream* g_logfile;

		std::vector<PerftTest> g_perftTests;

		/*
			Runs a perft test.

			depth: The depth in ply.
			fen: The starting position.
			testName: The name of the test.

			Returns: The result. On error returns a blank class which will set nodes (expected and actual) to 0.
		*/
		PerftResult RunPerftTest(const int depth, const std::string& fen, const std::string& testName);

		/*
			Performs the recursive searc.

			board: The board set to the correct perft starting position.
			stats: The stats.
			depth: The depth to search to.
		*/
		void Search(Board& board, PerftInternalStats& stats, int depth);

		/*
			Sets up the perft tests from the initial position.
		*/
		void SetupPerftTestsInitialPosition();

		/*
			Sets up the perft tests for position 2 (Kiwipete)
		*/
		void SetupPerftTestsPosition2();

		/*
			Sets up the perft tests for position 3
		*/
		void SetupPerftTestsPosition3();

		/*
			Sets up the perft tests for position 4 a and b

			reversed: True to use reveresed FEN which has the same results. False to use the non-reversed FEN.
		*/
		void SetupPerftTestsPosition4(const bool reversed);

		/*
			Sets up the perft tests for position 5
		*/
		void SetupPerftTestsPosition5();

		/*
			Sets up the perft tests for position 6
		*/
		void SetupPerftTestsPosition6();

		/*
			Setes the expected values.

			expectedValue: The expected value.
			perftCount: The perft count.
			allowIgnore: True to allow the value to be ignored, false otherwise.
		*/
		void SetExpectedValue(long long expectedValue, PerftCount& perftCount, bool allowIgnore);
	};
}

#endif
