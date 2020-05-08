/*
	Part of the AthenaZero Chess Engine.

	This file contains general constants.
	
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

#ifndef ATHENAZERO_ENGINE_CONSTANTS
#define ATHENAZERO_ENGINE_CONSTANTS

namespace ATHENAZEROENG
{
	/*
		Holds the initial size of the unmake move list which is a basic
		array.

		Ideally set high so that it will always be big enough, increasing the
		size later is expensive.
	*/
	constexpr int InitialUnmakeMoveListCapacity = 255;
	/*
		If the unmake move list becomes full (probably unlikely)
		then it is increased in size by this amount.
	*/
	constexpr int UnmakeMoveListCapacityIncrement = 10;
}

#endif
