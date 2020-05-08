/*
	Part of the AthenaZero Chess Engine.

	This file contains code related to generating legal moves and understanding the
	rules of chess.

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

#include "piece.h"
#include "move.h"
#include "typedefs.h"
#include "board0x88lib.h"
#include "unmake.h"

namespace ATHENAENG
{


	class Board
	{
	public:
		/*
			Creates a new instance of the class and initialises a new game starting at the standard chess
			starting position.
		*/
		Board();

		/*
			Releases resources.
		*/
		~Board();

		/*
			Resets to the start of a new game starting at the standard chess
			starting position.
		*/
		void NewGame();

		/*
			Generates pseudo legal moves. These are moves that are valid, but may leave the player
			in check or castling may not be possible due to check
			or obstruction. Calling MakeMove() with one of these moves will do these tests.

			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: Initially set to 0 by this method. When the method returns
					   this will contain the count of moves added to the moves
					   array.
		*/
		void GeneratePseudoLegalMoves(Move* moves, int& moveCount);

		/*
			Makes the specified move. Only pass a move returned by GeneratePseudoLegalMoves()
			to avoid undefined behaviour.

			move: The move.

			Returns: True if the move is ok, false if it is not valid (i.e.
					 leave the king in check or castling not possible).
		*/
		bool MakeMove(const Move& move);

		/*
			Unmakes a move. Does no validation so do NOT call when there are
			no moves to unmake, will cause an overflow and thus undefined
			behaviour.
		*/
		void UnMakeMove();

		/*
			Gets the FEN (Forsyth–Edwards Notation) for the current position.

			Returns: The FEN position.
		*/
		std::string GetPositionAsFen();

		/*
			Sets the positon from the FEN (Forsyth–Edwards Notation) for a position.

			fen: The FEN positon to set.

			Returns: True if valid, false otherwise. If false is returned then the game will be set to the start
					 of a new game using the standard chess starting position.
		*/
		bool SetPositionFromFen(std::string fen);

		/*
			Gets the current colour to move.
			Piece::PieceColourWhite or Piece::PieceColourBlack.
		*/
		inline int GetColourToMove() const
		{
			return g_colourToMove;
		}

		/*
			Returns true if white has not lost it potential to castle king-side, false
			otherwise.
		*/
		inline bool GetCanWhiteCastleKingSide() const
		{
			return g_canWhiteCastleKingSide;
		}

		/*
			Returns true if white has not lost it potential to castle queen-side, false
			otherwise.
		*/
		inline bool GetCanWhiteCastleQueenSide() const
		{
			return g_canWhiteCastleQueenSide;
		}

		/*
			Returns true if black has not lost it potential to castle king-side, false
			otherwise.
		*/
		inline bool GetCanBlackCastleKingSide() const
		{
			return g_canBlackCastleKingSide;
		}

		/*
			Returns true if black has not lost it potential to castle queen-side, false
			otherwise.
		*/
		inline bool GetCanBlackCastleQueenSide() const
		{
			return g_canBlackCastleQueenSide;
		}

		/*
			Gets the white king location in 0x88 format.
		*/
		inline BoardIndex0x88 GetWhiteKingLocation0x88() const
		{
			return g_WhiteKingLocation0x88;
		}

		/*
			Gets the black king location in 0x88 format.
		*/
		inline BoardIndex0x88 GetBlackKingLocation0x8() const
		{
			return g_BlackKingLocation0x88;
		}

		/*
			Gets the enpassant target square. i.e. if the last move was a double pawn move then this is the square immeidately behind
			the pawn (0x88 format), Null0x88Square otherwise.
		*/
		inline BoardIndex0x88 GetEnpassantTargetSquare() const
		{
			return g_EnpassantTargetSquare;
		}

		/*
			Gets the half move clock. Starts at 1 and increments ater each move. Resets to 0 after a pwn move or capture.
		*/
		inline int GetHalfMoveClock() const
		{
			return g_halfMoveClock;
		}

		/*
			Gets the full move number. Starts at 1 and increments after black's move.
		*/
		inline int GetFullMoveNumber() const
		{
			return g_fullMoveNumber;
		}

		/*
			Gets the piece type on the specified square.

			Returns one of;
				* PieceTypeKing
				* PieceTypeQueen
				* PieceTypeBishop
				* PieceTypeRook
				* PieceTypeKnight
				* PieceTypePawn
				* PieceTypeNone

			Does no validation.

			square: The square.
		*/
		inline int GetSquarePieceType(BoardIndex0x88 square) const
		{
			return g_board[square].PieceType;
		}

		/*
			Gets the piece colour on the specified square.

			Returns one of;
				* PieceColourWhite
				* PieceColourBlack

			Does no validation.

			square: The square.
		*/
		inline int GetSquarePieceColour(BoardIndex0x88 square) const
		{
			return g_board[square].PieceColour;
		}
	private:
		//0-136 (136 = 0x88), so Null0x88Square actually exists in the array - Avoids some branch statements
		static constexpr int BoardArrayLength = 137;

		//Will be no valid square at or above this - used to itterate through all valid squares
		static constexpr int BoardArrayFirstNotPiecePos = 120;

		static constexpr int MaxValid0x88Location = 0x77;

		//Note directions are all from whites perspective
		static constexpr int BoardDirUp = 16;
		static constexpr int BoardDirRight = 1;
		static constexpr int BoardDirDown = -16;
		static constexpr int BoardDirLeft = -1;

		static constexpr int BoardDirUpRight = 17;
		static constexpr int BoardDirDownRight = -15;
		static constexpr int BoardDirDownLeft = -17;
		static constexpr int BoardDirUpLeft = 15;

		static constexpr int BoardDirKnight1 = 14;
		static constexpr int BoardDirKnight2 = 31;
		static constexpr int BoardDirKnight3 = 33;
		static constexpr int BoardDirKnight4 = 18;
		static constexpr int BoardDirKnight5 = -14;
		static constexpr int BoardDirKnight6 = -31;
		static constexpr int BoardDirKnight7 = -33;
		static constexpr int BoardDirKnight8 = -18;

		static constexpr int BoardDirPawnAdvanceSingleWhite = 16;
		static constexpr int BoardDirPawnCaptureRightWhite = 17;
		static constexpr int BoardDirPawnCaptureLeftWhite = 15;

		static constexpr int BoardDirPawnAdvanceSingleBlack = -16;
		static constexpr int BoardDirPawnCaptureRightBlack = -15;
		static constexpr int BoardDirPawnCaptureLeftBlack = -17;

		static constexpr int BoardDirTowardsAttackingWhitePawn1 = -17;
		static constexpr int BoardDirTowardsAttackingWhitePawn2 = -15;

		static constexpr int BoardDirTowardsAttackingBlackPawn1 = 17;
		static constexpr int BoardDirTowardsAttackingBlackPawn2 = 15;

		Piece g_board[BoardArrayLength];

		/*
			The current side (colour) to move.
			* Piece::PieceColourWhite
			* Piece::PieceColourBlack
		*/
		int g_colourToMove{ Piece::PieceColourWhite };

		bool g_canWhiteCastleKingSide{ true };
		bool g_canWhiteCastleQueenSide{ true };
		bool g_canBlackCastleKingSide{ true };
		bool g_canBlackCastleQueenSide{ true };

		BoardIndex0x88 g_WhiteKingLocation0x88{ Null0x88Square };
		BoardIndex0x88 g_BlackKingLocation0x88{ Null0x88Square };

		BoardIndex0x88 g_EnpassantTargetSquare{ Null0x88Square };

		//Starts at zero. Inceremented after every ply, reset to 0 after a pawn move or capture.
		int g_halfMoveClock{ 0 };

		//Starts at 1 and increments after black's move.
		int g_fullMoveNumber{ 1 };

		UnmakeItem* g_UnmakeList;
		size_t g_UnmakeLength{ 0 };
		size_t g_UnmakeCapacity{ 0 };

		/*
			Records the curret state so it can be undone with UnMakeMove().

			move: The move being made.

			Returns: True on success or false on failure.
		*/
		bool RecordStateToUnMake(const Move& move);

		/*
			Updates all status values for the board basedon the move.

			Updates the following:
				* Colour/Side to Move
				* Castling rights
				* Enpassant Target Square
				* King location
		*/
		void UpdateStateForMove(const Move& move);

		/*
			Ensures the entire board is clear (i.e. contains no pieces).
		*/
		void ClearBoard();

		/*
			Adds all pawns at their initial default positions.

			setupWhite: If true add white pawns, false ass black pawns.
		*/
		void SetupPawns(bool setupWhite);

		/*
			Adds all pieces at their initial default positions.

			setupWhite: If true add white pieces, false ass black pawns.
		*/
		void SetupPieces(bool setupWhite);

		/*
			Sets the specified square to a piece. Note: does not set king locations.

			file: the file (0-7)
			rank: the rank (0-7)
			pieceType: The piece type, must be one of:
				* Piece::PieceTypeNone
				* Piece::PieceTypeKing
				* Piece::PieceTypeRook
				* Piece::PieceTypeKnight
				* Piece::PieceTypeBishop
				* Piece::PieceTypeQueen
				* Piece::PieceTypePawn
			pieceColour: The piece colour, must be one of:
				* Piece::PieceColourWhite
				* Piece::PieceColourBlack
		*/
		void SetPieceOnSquare(int file, int rank, int pieceType, int pieceColour);

		/*
			Adds moves to the move list.

			start: The starting square (0x88).
			target: The starting square (0x88).
			promoteTo: The piece to promote to. Must be one of:
				* Piece::PieceTypeNone
				* Piece::PieceTypeRook
				* Piece::PieceTypeKnight
				* Piece::PieceTypeBishop
				* Piece::PieceTypeQueen
			otherSquareToClear: If set when the move is made this
								square is also cleared. Used for Enpassant
								captures, set to the pawn that is actually
								captured.
			secondaryMoveFrom: The starting square for the secondary move (0x88 format).
							   Used to move te rook when castling.
			secondaryMoveTo: The destination square for the secondary move (0x88 format).
							 Used to move te rook when castling.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddMove(
			const BoardIndex0x88 start,
			const BoardIndex0x88 target,
			const int promoteTo,
			const BoardIndex0x88 otherSquareToClear,
			const BoardIndex0x88 secondaryMoveFrom,
			const BoardIndex0x88 secondaryMoveTo,
			Move* moves,
			int& moveCount);

		/*
			Adds basic piece moves that capture normally (i.e. not pawns).

			Can be used for any piece that is not a pawn (Rook, Knight, Bishop, King and Queen).

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			direction: The 0x88 direction. This is added to start to get the next square
					   in the desired direction. Then added again for the next square
					   and so on.
			maxCount: The maximum number of squares to move. Generally set to
					  8 for Rook, Bishop and Queen. Set to 1
					  for King and Knight.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddBasicPieceMoves(
			BoardIndex0x88 start,
			int direction,
			int maxCount,
			Move* moves,
			int& moveCount);

		/*
			Adds add rook moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddRookMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds add knight moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddKnightMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds add bishop moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddBishopMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds add queen moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddQueenMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds add king moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddKingMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds add pawn moves.

			Always adds for the current colour to move (relevant when it comes to capturing).

			start: The 0x88 position to start at.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddPawnMoves(BoardIndex0x88 start, Move* moves, int& moveCount);

		/*
			Adds a pawn move.

			initial: The initial position. Used for double move. First move this is the same as start. For the second move
					 it is the original position.
			start: The starting square (ox88)
			direction: +16 for white, -16 for black
			promotionOperator: Used to detect the promotion square.
							   * Set to 0x70 for white
							   * Set to 0xF0 for black
			promotionComparison: Used to detect the promotion square.
							   * Set to 0x70 for white
							   * Set to 0x00 to black
			isOnHomeSquare: True if the pawn is initially on its home square.
							False otherwise.
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddPawnAdvance(
			const BoardIndex0x88 initial,
			const BoardIndex0x88 start,
			const int direction,
			const unsigned int promotionOperator,
			const unsigned int promotionComparison,
			bool isOnHomeSquare,
			Move* moves,
			int& moveCount);

		/*
			Adds a pawn capture

			start: The starting square (ox88)
			direction: +15/+17 for white, -15/-17 for black
			promotionOperator: Used to detect the promotion square.
							   * Set to 0x70 for white
							   * Set to 0xF0 for black
			promotionComparison: Used to detect the promotion square.
							   * Set to 0x70 for white
							   * Set to 0x00 to black
			moves: An array. Should be at least 238 in length to avoid
				   potential overflows.
			moveCount: When the method returns
					   this will contain the count of moves added to the moves
					   array. Note: Is NOT initialised to 0 (i.e. keeps its current value
					   when this method is called). It is simply modified (i.e. added to)
					   by this method.
		*/
		void AddPawnCapture(
			const BoardIndex0x88 start,
			const int direction,
			const unsigned int promotionOperator,
			const unsigned int promotionComparison,
			Move* moves,
			int& moveCount);

		/*
			Determines if the specified square is attacked by a Queen, King or (Rook/Bishop)

			square: The attacked aquare.
			attackingColour: The side attacking. Must be one of:
				* Piece::PieceColourWhite
				* Piece::PieceColourBlack
			direction: The 0x88 direction of attack.
			singleDirectionPieceType: Should be Rook ir Bishop.
									  This method always checks Queen and King (diagonal and straight)
									  but only checks Rook or Bishop (depending on the direction), as specified by this
									  parameter. Must be one of:
										* Piece::PieceTypeRook
										* Piece::PieceTypeBishop
			Returns: True if the square is attacked, false otherwise.
		*/
		bool IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			const BoardIndex0x88 square,
			const int attackingColour,
			const int direction,
			const int singleDirectionPieceType);

		bool IsSquareAttackedByKnight(
			const BoardIndex0x88 square,
			const int attackingColour);

		/*
			Determines if a square is attacked by a pawn.

			square: The attacked aquare.
			attackingColour: The side attacking. Must be one of:
				* Piece::PieceColourWhite
				* Piece::PieceColourBlack
			direction1: First direction towards the attacker from the attacked piece.
						NOTE: This is the OPPOSITE direction that the piece will attack in.
			direction2: First direction towards the attacker from the attacked piece.
						NOTE: This is the OPPOSITE direction that the piece will attack in.
		*/
		bool IsAttackedByPawn(
			const BoardIndex0x88 square,
			const int attackingColour,
			const int direction1,
			const int direction2);

		/*
			Determines if a square is attacked.

			square: The attacked square.
			attackingColour: The side attacking. Must be one of:
				* Piece::PieceColourWhite
				* Piece::PieceColourBlack
			Returns: True if the square is attacked, false otherwise.
		*/
		bool IsSquareAttacked(const BoardIndex0x88 square, const int attackingColour);

		/*
			Processes the field section of a FEN (Forsyth–Edwards Notation) position.

			fields: The fields portion of the FEN string.
			Returns: True on success or false pn failure.
		*/
		bool ProcessFenFields(const std::string& fields);

		/*
			Checks if the position is valid.

			Checks the following:
				* There is one King per side
				* No pawns exist on ranks 1 or 8
				* The side not on move is not in check
		*/
		bool ValidatePosition();

		/*
			Returns true if the specified side is in check.

			colour: The colour to check.
			Returns: True if in check, false otherwise.
		*/
		bool IsInCheck(int colour);

		/*
			Increases the unmake move list capacity.

			newCapacity: The new capacity. Setting this to the same (or less) than the current capacity results
					 in undefined behaviour.
			Returns: True on success or false on failure
		*/
		bool IncreaseUnmakeListCapacity(size_t newCapacity);
	};
}
