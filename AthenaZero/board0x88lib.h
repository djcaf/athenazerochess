/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to 0x88 board representation tasks such as
	converting between the 0x88 representation and ranks and files.
	
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

#ifndef ATHENAZERO_ENGINE_BOARD0X88LIB
#define ATHENAZERO_ENGINE_BOARD0X88LIB

#include "typedefs.h"
#include "movelib.h"

#include <string>
#include <sstream>

namespace ATHENAZEROENG
{
	//Can be accessed in the board as it is an valid array index even though not a valid square - Avoids some branch statements.
	static constexpr BoardIndex0x88 Null0x88Square = 0x88;

	/*
			Converts a rank and file to the 0x88 board index.

			file: The file from 0-7.
			rank: The rank from 0-7.

			Returns: The 0x88 square.
		*/
	inline BoardIndex0x88 FileRankTo0x88(int file, int rank)
	{
		return 16 * rank + file;
	}

	/*
		Converts a 0x88 board index to the file.

		sq0x88: The 0x88 square.

		Returns: The file (0-7).
	*/
	inline int From0x88ToFile(BoardIndex0x88 sq0x88)
	{
		return sq0x88 & 7;
	}

	/*
		Converts a 0x88 board index to the file.

		sq0x88: The 0x88 square.

		Returns: The file (a-h). If for some reason the sq0x88 value is not valie then return -.
	*/
	inline char From0x88ToFileLetter(BoardIndex0x88 sq0x88)
	{
		int file = From0x88ToFile(sq0x88);

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
		Converts a 0x88 board index to the rank.

		sq0x88: The 0x88 square.

		Returns: The rank (0-7).
	*/
	inline int From0x88ToRank(BoardIndex0x88 sq0x88)
	{
		return sq0x88 >> 4;
	}

	/*
		Determines if the specified square is valid.

		sq0x88: The 0x88 square to check.

		returns: True if valid, false otherwise.
	*/
	inline bool Is0x88SquareValid(BoardIndex0x88 sq0x88)
	{
		return (sq0x88 & Null0x88Square) == 0;
	}

	/*
		Converts a text square (e.g. e4) to the 0x88 version or returns
		Null0x88Square if the square is not valid.

		move: The square (e.g. e4).
		Returns: The 0x88 square or Null0x88Square on error.
	*/
	inline BoardIndex0x88 TextSquareTo0x88(std::string square)
	{
		if (square.size() != 2) return Null0x88Square;
		char file = square[0];
		char rank = square[1];
		int file07 = CharacterToFile(file);
		if (file07 == -1) return Null0x88Square;

		//TODO: OK - Make a library function
		if (rank < '1' || rank > '8') return Null0x88Square;
		std::stringstream ss;
		ss << rank;
		int rank07 = std::stoi(ss.str()) - 1;

		return FileRankTo0x88(file07, rank07);
	}

	/*
		Converts an 0x88 square to a text square (e.g. e4). Returns
		"" on error.

		square: The 0x88 square.
		Returns: The square (e.g. e4) or "" on error.
	*/
	inline std::string Sq0x88ToTextSquare(BoardIndex0x88 square)
	{
		if (!Is0x88SquareValid(square)) return "";
		int file07 = From0x88ToFile(square);
		int rank07 = From0x88ToRank(square);

		char file = FileToCharacter(file07);
		if (file == '-') return "";

		std::stringstream ss;
		ss << file << (rank07 + 1);

		return ss.str();
	}
}

#endif
