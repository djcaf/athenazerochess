/*
	Part of the Athena Chess Engine.

	This file contains code related to moves. Essentially this is a class that
	holds a moves from and to squares as well as whether it is a promotion.
	
	The Athena Chess Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	The Athena Chess Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>

#include "movelib.h"
#include "move.h"
#include "piece.h"
#include "board0x88lib.h"

namespace ATHENAENG
{
	std::string Move::GetMoveAsXBoardFormat()
	{
		if (!Is0x88SquareValid(MoveFrom)) return "";
		if (!Is0x88SquareValid(MoveTo)) return "";

		int moveFromFile = From0x88ToFile(MoveFrom);
		int moveFromRank = From0x88ToRank(MoveFrom);

		int moveToFile = From0x88ToFile(MoveTo);
		int moveToRank = From0x88ToRank(MoveTo);

		char fromFile = FileToCharacter(moveFromFile);
		int fromRank = moveFromRank + 1; //Was -1 on error so will now be 0 on error

		char toFile = FileToCharacter(moveToFile);
		int toRank = moveToRank + 1; //Was -1 on error so will now be 0 on error

		if (fromFile == '-') return "";
		if (fromRank == 0) return "";
		if (toFile == '-') return "";
		if (toRank == 0) return "";

		std::stringstream s;

		s << fromFile << fromRank << toFile << toRank;

		if (PromoteTo != Piece::PieceTypeNone)
		{
			switch (PromoteTo)
			{
			case Piece::PieceTypeQueen:
				s << "q";
				break;
			case Piece::PieceTypeRook:
				s << "r";
				break;
			case Piece::PieceTypeBishop:
				s << "b";
				break;
			case Piece::PieceTypeKnight:
				s << "n";
				break;
			default:
				return "";
			}
		}

		return s.str();
	}
}