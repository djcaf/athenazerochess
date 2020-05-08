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

#include "perftresult.h"
#include <string>

namespace ATHENAZEROENG
{
	PerftResult::PerftResult(
		const int depth,
		const std::string& fen,
		const std::string& testName,
		const double timeTakenSconds)
	{
		g_depth = { depth };
		g_fen = { fen };
		g_testName = { testName };
		g_timeTakenSconds = { timeTakenSconds };
	}
}
