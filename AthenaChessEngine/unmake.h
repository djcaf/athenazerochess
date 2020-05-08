/*
	Part of the Athena Chess Engine.

	This file contains code related to unmaking moves. Essentially holds the state
	so a move can be unmade quickly.
	
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

#pragma once

#include "board0x88lib.h"
#include "typedefs.h"
#include "piece.h"

namespace ATHENAENG
{
	class UnmakeItem
	{
	public:
		// *** Move *** \\

		//Main piece move from
		BoardIndex0x88 MovedFrom{ Null0x88Square };
		//Main piece move to
		BoardIndex0x88 MovedTo{ Null0x88Square };
		//Original piece from MovedFrom (for undoing promotion)
		Piece OriginalPiece;
		//Captured piece from MovedTo
		Piece CapturedPiece;

		//For castling (Rook move from)
		BoardIndex0x88 MovedFrom2{ Null0x88Square };
		//For castling (Rook move to)
		BoardIndex0x88 MovedTo2{ Null0x88Square };

		//For enpassant (square captured pawn was on)
		BoardIndex0x88 OtherReplaceSquare{ Null0x88Square };
		//For enpassant (actual pawn)
		Piece OtherReplacePiece;


		// *** State *** \\

		//The colour to move before the move is made
		int ColourToMove{ Piece::PieceColourWhite };

		//Castling state before move is made
		bool CanWhiteCastleKingSide{ true };
		bool CanWhiteCastleQueenSide{ true };
		bool CanBlackCastleKingSide{ true };
		bool CanBlackCastleQueenSide{ true };

		//Before move is made
		BoardIndex0x88 WhiteKingLocation0x88{ Null0x88Square };
		BoardIndex0x88 BlackKingLocation0x88{ Null0x88Square };
		BoardIndex0x88 EnpassantTargetSquare{ Null0x88Square };
		int HalfMoveClock{ 0 };
		int FullMoveNumber{ 1 };
	};
}