/*
	Part of the AthenaZero Chess Engine.

	This file contains code to hold internal stats used when running the perft tests.

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

#ifndef ATHENAZERO_ENGINE_PERFT_INTERNAL_STATS
#define ATHENAZERO_ENGINE_PERFT_INTERNAL_STATS

namespace ATHENAZEROENG
{
	class PerftInternalStats
	{
	public:
		long long Nodes{ 0 };
		long long Captures{ 0 };
		long long Enpassant{ 0 };
		long long Castles{ 0 };
		long long Promotions{ 0 };
		long long Checks{ 0 };
		long long Checkmates{ 0 };
	};
}

#endif
