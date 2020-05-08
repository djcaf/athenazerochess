/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to pieces. This is a clas sthat holds details 
	about a piece (type and colour).
	
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

#ifndef ATHENAZERO_ENGINE_PIECE
#define ATHENAZERO_ENGINE_PIECE

namespace ATHENAENG
{
	class Piece
	{
	public:
		static constexpr int PieceTypeNone = 0;
		static constexpr int PieceTypeKing = 1;
		static constexpr int PieceTypeRook = 2;
		static constexpr int PieceTypeKnight = 4;
		static constexpr int PieceTypeBishop = 8;
		static constexpr int PieceTypeQueen = 16;
		static constexpr int PieceTypePawn = 32;

		static constexpr int PieceColourWhite = 64;
		static constexpr int PieceColourBlack = 128;

		/*
			The type of piece, should be one of:
			* PieceTypeNone
			* PieceTypeKing
			* PieceTypeRook
			* PieceTypeKnight
			* PieceTypeBishop
			* PieceTypeQueen
			* PieceTypePawn
		*/
		int PieceType{ PieceTypeNone };

		/*
			The piece colour, should be one of:
			* PieceColourWhite
			* PieceColourBlack
		*/
		int PieceColour{ PieceColourWhite };
	};
}

#endif
