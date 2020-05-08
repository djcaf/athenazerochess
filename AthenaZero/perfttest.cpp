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

#include "perfttest.h"

namespace ATHENAZEROENG
{
	PerftTest::PerftTest(
		const int depth,
		const std::string& fen,
		const std::string& testName,
		const long long expectedNodes,
		const long long expectedCaptures,
		const long long expectedEnPassant,
		const long long expectedCastles,
		const long long expectedPromotions,
		const long long expectedChecks,
		const long long expectedCheckmates)
	{
		g_depth = { depth };
		g_fen = { fen };
		g_testName = { testName };
		g_expectedNodes = { expectedNodes };
		g_expectedCaptures = { expectedCaptures };
		g_expectedEnPassant = { expectedEnPassant };
		g_expectedCastles = { expectedCastles };
		g_expectedPromotions = { expectedPromotions };
		g_expectedChecks = { expectedChecks };
		g_expectedCheckmates = { expectedCheckmates };

		if (g_expectedNodes < 0) g_expectedNodes = 0;
		if (g_expectedCaptures < -1) g_expectedCaptures = -1;
		if (g_expectedEnPassant < -1) g_expectedEnPassant = -1;
		if (g_expectedCastles < -1) g_expectedCastles = -1;
		if (g_expectedPromotions < -1) g_expectedPromotions = -1;
		if (g_expectedChecks < -1) g_expectedChecks = -1;
		if (g_expectedCheckmates < -1) g_expectedCheckmates = -1;
	}
}
