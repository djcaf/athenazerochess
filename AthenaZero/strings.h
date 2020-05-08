/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to strings. This is code that is used
	but not supported (yet) by the C++ standard library.
	
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

#ifndef ATHENAZERO_ENGINE_STRINGS
#define ATHENAZERO_ENGINE_STRINGS

#include <string>

namespace ATHENAZEROENG
{
	/*
		Checks if a string is numeric only.

		s: The string
		Returns: True if numeric, false oterwise
	*/
	bool is_number(const std::string& s);
}

#endif
