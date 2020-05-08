/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to moves, such as code to convert betwee a file
	letter and its equivalent 0-based index.
	
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

#ifndef ATHENAZERO_ENGINE_MOVELIB
#define ATHENAZERO_ENGINE_MOVELIB

namespace ATHENAZEROENG
{
	/*
	Convertes the numeric file to its character equivalent.

	file: The file (0-7).
	Returns: The character (a-h, lowercase), or '-' if the file is invalid.
*/
	inline char FileToCharacter(int file)
	{
		switch (file)
		{
		case 0:
			return 'a';
		case 1:
			return 'b';
		case 2:
			return 'c';
		case 3:
			return 'd';
		case 4:
			return 'e';
		case 5:
			return 'f';
		case 6:
			return 'g';
		case 7:
			return 'h';
		default:
			return '-';
		}
	}

	/*
		Converts the character file to its numeric equivalent.

		character: The character file (a-h, case-insensitive).
		Returns: The file (0-7) or -1 if the character is invalid.
	*/
	inline int CharacterToFile(char character)
	{
		switch (character)
		{
		case 'a':
			return 0;
		case 'b':
			return 1;
		case 'c':
			return 2;
		case 'd':
			return 3;
		case 'e':
			return 4;
		case 'f':
			return 5;
		case 'g':
			return 6;
		case 'h':
			return 7;
		case 'A':
			return 0;
		case 'B':
			return 1;
		case 'C':
			return 2;
		case 'D':
			return 3;
		case 'E':
			return 4;
		case 'F':
			return 5;
		case 'G':
			return 6;
		case 'H':
			return 7;
		default:
			return -1;
		}
	}
}

#endif
