/*
	Part of the AthenaZero Chess Engine.

	This file contains code to hold a single perft result.

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

#ifndef ATHENAZERO_ENGINE_PERFT_RESULT
#define ATHENAZERO_ENGINE_PERFT_RESULT

#include "perftcount.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace ATHENAZEROENG
{
	/*
		Holds a single perft result.
	*/
	class PerftResult
	{
	public:
		/*
			Creates a new instance of the class.

			depth: The depth of the perft test.
			fen: The FEN position for the test.
			testName: The name of the test.
			timeTakenSconds: The time taken for the test in seconds.
		*/
		PerftResult(
			const int depth,
			const std::string& fen,
			const std::string& testName,
			const double timeTakenSconds);

		/*
			Gets whether this test passed (true) or failed (false).
		*/
		inline bool GetPassed()
		{
			return g_nodeCount.GetIsPassed() &&
				g_captureCount.GetIsPassed() &&
				g_enpassantCount.GetIsPassed() &&
				g_castleCount.GetIsPassed() &&
				g_promotionCount.GetIsPassed() &&
				g_checkCount.GetIsPassed() &&
				g_checkmateCount.GetIsPassed() &&
				g_setupPassed &&
				g_integrityCheckPassed;
		}

		/*
			Gets the depth in ply for this result.
		*/
		inline int GetDepth() { return g_depth; }

		/*
			Gets the FEN position used for the test.
		*/
		inline std::string GetFen() { return g_fen; }

		/*
			Gets the test name.
		*/
		inline std::string GetTestName() { return g_testName; }

		/*
			The node count
		*/
		inline PerftCount& NodeCount() { return g_nodeCount; }

		/*
			The capture count
		*/
		inline PerftCount& CaptureCount() { return g_captureCount; }

		/*
			The EnPassant count
		*/
		inline PerftCount& EnPassantCount() { return g_enpassantCount; }

		/*
			The Castle count
		*/
		inline PerftCount& CastleCount() { return g_castleCount; }

		/*
			The Promotion count
		*/
		inline PerftCount& PromotionCount() { return g_promotionCount; }

		/*
			The Check count
		*/
		inline PerftCount& CheckCount() { return g_checkCount; }

		/*
			The Checkmate count
		*/
		inline PerftCount& CheckmateCount() { return g_checkmateCount; }

		/*
			Sets whether the setup passed (true) or failed (false).
		*/
		inline void SetSetupPassed(bool value) { g_setupPassed = { value }; }

		/*
			Gets whether the setup passed (true) or failed (false).
		*/
		inline bool GetSetupPassed() { return g_setupPassed; };

		/*
			Sets whether the integrity check passed (true) or failed (false).
		*/
		inline void SetIntergityCheckPassed(bool value) { g_integrityCheckPassed = { value }; }

		/*
			Gets whether the integrity check passed (true) or failed (false).
		*/
		inline bool GetIntergityCheckPassed() { return g_integrityCheckPassed; };

		/*
			Gets the time taken in seconds to three decimal places.
		*/
		inline std::string GetTimeTaken()
		{
			std::stringstream result;

			//TODO: Why?
			//Disable C26444 as this causes issues in Visual Studio
#pragma warning (push)
#pragma warning (disable: 26444)	
			//TODO: Find out why needed?
			result.imbue(std::locale(""));
#pragma warning (pop)
			result << std::fixed << std::setprecision(3);
			result << g_timeTakenSconds << " second(s)";
			return result.str();
		}

		/*
			Gets the time taken for 1 node in Milliseconds, microseconds or nanoseconds as appropriate.

			Values will be in whole numbers only (rounded down).
		*/
		inline std::string GetTimeForOneNode()
		{
			long long nodeCount = g_nodeCount.GetActualCount();
			if (nodeCount < 1) return "N/A";

			double timeForOneNodeSeconds = g_timeTakenSconds / nodeCount;

			double result = static_cast<int>(timeForOneNodeSeconds * 1000000000.0); //Nanoseconds

			std::stringstream ss;

			if (result < 1000)
			{
				ss << result << " ns";
				return ss.str();
			}

			result = static_cast<int>(timeForOneNodeSeconds * 1000000.0); //Microseconds						
			if (result < 1000)
			{
				ss << result << " us";
				return ss.str();
			}

			result = static_cast<int>(timeForOneNodeSeconds * 1000.0); //Milliseconds			
			ss << result << " ms";
			return ss.str();
		}

		/*
			Gets nodes per second. Thousands are separated by commas and the value is rounded
			down to a whole number.
		*/
		inline std::string GetNodesPerSecond()
		{
			if (g_timeTakenSconds < 0.1) return "N/A";

			long long nps = static_cast<long long>(g_nodeCount.GetActualCount() / g_timeTakenSconds);

			std::stringstream result;

			//Disable scientific notation
			result.setf(std::ios::fixed, std::ios::floatfield);
			result.setf(std::ios::showpoint);

			//TODO: Why?
			//Disable C26444 as this causes issues in Visual Studio
#pragma warning (push)
#pragma warning (disable: 26444)
			//TODO: Find out why needed?
			result.imbue(std::locale(""));
#pragma warning (pop)
			result << nps << " NPS";
			return result.str();
		}

	private:
		std::string g_fen;
		std::string g_testName;
		int g_depth{ 0 };

		PerftCount g_nodeCount;
		PerftCount g_captureCount;
		PerftCount g_enpassantCount;
		PerftCount g_castleCount;
		PerftCount g_promotionCount;
		PerftCount g_checkCount;
		PerftCount g_checkmateCount;

		bool g_setupPassed{ false };
		bool g_integrityCheckPassed{ false };

		double g_timeTakenSconds{ 0.0 };
	};
}

#endif
