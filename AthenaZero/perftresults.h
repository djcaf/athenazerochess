/*
	Part of the AthenaZero Chess Engine.

	This file contains code to hold multiple single perft results.

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

#ifndef ATHENAZERO_ENGINE_PERFT_RESULTS
#define ATHENAZERO_ENGINE_PERFT_RESULTS

#include <vector>

#include "perftresult.h"

namespace ATHENAZEROENG
{
	class PerftResults
	{
	public:
		/*
			Adds a result to the collection.

			result: The result to add.
		*/
		void AddResult(const PerftResult& result);

		/*
			Gets the number of results in the collection.
		*/
		inline size_t GetCount() { return _results.size(); }

		/*
			Gets the result from the collection.

			i: The 0-based index of the result to get.
		*/
		inline PerftResult& GetResult(size_t i) { return _results.at(i); }

	private:
		std::vector<PerftResult> _results;
	};
}

#endif
