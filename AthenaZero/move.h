/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to moves. Essentially this is a class that
	holds a moves from and to squares as well as whether it is a promotion.
	
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

#pragma once

#include <string>

#include "piece.h"
#include "typedefs.h"
#include "board0x88lib.h"

namespace ATHENAENG
{
	class Move
	{
	public:
		/*
			The starting square (0x88 format)
		*/
		BoardIndex0x88 MoveFrom{ Null0x88Square };

		/*
			The destination square (0x88 format)
		*/
		BoardIndex0x88 MoveTo{ Null0x88Square };

		/*
			The promote to. Must be one of:
				* Piece::PieceTypeNone
				* Piece::PieceTypeRook
				* Piece::PieceTypeKnight
				* Piece::PieceTypeBishop
				* Piece::PieceTypeQueen
		*/
		int PromoteTo{ Piece::PieceTypeNone };

		/*
			If set when the move is made this square is also cleared.

			Used for Enpassant captures, set to the pawn that is actually captured.

			Can always clear even if Null0x88Square as that is a valid array index, just not
			a valid 0x88 square
		*/
		BoardIndex0x88 OtherSquareToClear{ Null0x88Square };

		/*
			The starting square for the secondary move (0x88 format).

			Used to move te rook when castling.

			If this and SecondaryMoveTo are Null0x88Square then can just copy to self as
			although an invalid 0x88 square it is a valid array index.
		*/
		BoardIndex0x88 SecondaryMoveFrom{ Null0x88Square };

		/*
			The destination square for the secondary move  (0x88 format)

			Used to move te rook when castling.

			If this and SecondaryMoveTo are Null0x88Square then can just copy to self as
			although an invalid 0x88 square it is a valid array index.
		*/
		BoardIndex0x88 SecondaryMoveTo{ Null0x88Square };

		/*
			Gets the move in the standard format.

			The format is e1e4, e7e8q for promotion. For castling then
			the move will be the king move, e.g. e1g1

			Returns: The move in standard format. If there is an invalid value then
					 returns a blank string.
		*/
		std::string GetMoveAsStandardFormat();
	};
}
