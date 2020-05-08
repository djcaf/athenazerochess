/*
	Part of the AthenaZero.

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

#include <sstream>

#include "board.h"
#include "move.h"
#include "piece.h"
#include "board0x88lib.h"
#include "constants.h"
#include "unmake.h"
#include "strings.h"

namespace ATHENAENG
{
	Board::Board()
	{
		//Create unmake move list
		g_UnmakeList = new UnmakeItem[InitialUnmakeMoveListCapacity];
		g_UnmakeLength = { 0 };
		g_UnmakeCapacity = { InitialUnmakeMoveListCapacity };

		//Setup the initial position
		NewGame();
	}

	Board::~Board()
	{
		delete[] g_UnmakeList;
	}

	void Board::NewGame()
	{
		ClearBoard();
		SetupPawns(true);
		SetupPawns(false);
		SetupPieces(true);
		SetupPieces(false);

		g_colourToMove = { Piece::PieceColourWhite };
		g_canWhiteCastleKingSide = { true };
		g_canWhiteCastleQueenSide = { true };
		g_canBlackCastleKingSide = { true };
		g_canBlackCastleQueenSide = { true };
		g_EnpassantTargetSquare = { Null0x88Square };
		g_WhiteKingLocation0x88 = { 0x04 };
		g_BlackKingLocation0x88 = { 0x74 };
		g_halfMoveClock = { 0 };
		g_fullMoveNumber = { 1 };
	}

	void Board::GeneratePseudoLegalMoves(Move* moves, int& moveCount)
	{
		moveCount = 0;

		for (int sq = 0; sq < BoardArrayFirstNotPiecePos; ++sq)
		{
			if (Is0x88SquareValid(sq))
			{
				Piece& p = g_board[sq];
				if (p.PieceColour == g_colourToMove)
				{
					switch (p.PieceType)
					{
					case Piece::PieceTypeRook:
						AddRookMoves(sq, moves, moveCount);
						break;
					case Piece::PieceTypeKnight:
						AddKnightMoves(sq, moves, moveCount);
						break;
					case Piece::PieceTypeBishop:
						AddBishopMoves(sq, moves, moveCount);
						break;
					case Piece::PieceTypeQueen:
						AddQueenMoves(sq, moves, moveCount);
						break;
					case Piece::PieceTypeKing:
						AddKingMoves(sq, moves, moveCount);
						break;
					case Piece::PieceTypePawn:
						AddPawnMoves(sq, moves, moveCount);
						break;
					}
				}
			}
		}
	}

	void Board::UpdateStateForMove(const Move& move)
	{
		int pieceType = g_board[move.MoveFrom].PieceType;
		int capturePieceType = g_board[move.MoveTo].PieceType;

		g_EnpassantTargetSquare = { Null0x88Square };

		bool resetHalfMoveClock = false;

		if (capturePieceType != Piece::PieceTypeNone)
		{
			resetHalfMoveClock = true;
		}

		if (pieceType == Piece::PieceTypeKing)
		{
			//King is moving

			//Deal with the following;
			//1. Update King Position
			//2. Update Whether Can Castle			

			if (g_colourToMove == Piece::PieceColourWhite)
			{
				g_WhiteKingLocation0x88 = move.MoveTo;
				g_canWhiteCastleKingSide = { false };
				g_canWhiteCastleQueenSide = { false };
			}
			else
			{
				g_BlackKingLocation0x88 = move.MoveTo;
				g_canBlackCastleKingSide = { false };
				g_canBlackCastleQueenSide = { false };
			}
		}
		else if (pieceType == Piece::PieceTypeRook)
		{
			//Rook moving, may affect castling
			switch (move.MoveFrom)
			{
			case 0x07:
				g_canWhiteCastleKingSide = { false };
				break;
			case 0x00:
				g_canWhiteCastleQueenSide = { false };
				break;
			case 0x77:
				g_canBlackCastleKingSide = { false };
				break;
			case 0x70:
				g_canBlackCastleQueenSide = { false };
				break;
			}
		}
		else if (pieceType == Piece::PieceTypePawn)
		{
			resetHalfMoveClock = true;

			int diff = 0x77 + move.MoveFrom - move.MoveTo;
			if (g_colourToMove == Piece::PieceColourWhite)
			{
				if (diff == 87)
				{
					//Double square move
					g_EnpassantTargetSquare = move.MoveTo - 16;
				}
			}
			else
			{
				if (diff == 151)
				{
					//Double square move
					g_EnpassantTargetSquare = move.MoveTo + 16;
				}
			}
		}

		if (capturePieceType == Piece::PieceTypeRook)
		{
			if (move.MoveTo == 0x07) g_canWhiteCastleKingSide = { false }; //Either captured black's King Side rook or the rook has moved anyway
			if (move.MoveTo == 0x00) g_canWhiteCastleQueenSide = { false }; //Either captured black's King Side rook or the rook has moved anyway
			if (move.MoveTo == 0x77) g_canBlackCastleKingSide = { false }; //Either captured white's Queen Side rook or the rook has moved anyway
			if (move.MoveTo == 0x70) g_canBlackCastleQueenSide = { false }; //Either captured white's Queen Side rook or the rook has moved anyway                
		}

		if (resetHalfMoveClock)
		{
			g_halfMoveClock = { 0 };
		}
		else
		{
			++g_halfMoveClock;
		}

		//Update side to move
		if (g_colourToMove == Piece::PieceColourWhite)
		{
			g_colourToMove = Piece::PieceColourBlack;
		}
		else
		{
			++g_fullMoveNumber;
			g_colourToMove = Piece::PieceColourWhite;
		}
	}

	bool Board::MakeMove(const Move& move)
	{
		if (!RecordStateToUnMake(move)) return false;
		UpdateStateForMove(move);

		//Remember g_colourToMove will now be the other player so is used in reverse below.
		//         If white has just moved this will now indicate black is to move, and vice-versa.		

		//Main piece
		if (move.PromoteTo == Piece::PieceTypeNone)
		{
			g_board[move.MoveTo].PieceType = g_board[move.MoveFrom].PieceType;
		}
		else
		{
			g_board[move.MoveTo].PieceType = move.PromoteTo;
		}
		g_board[move.MoveTo].PieceColour = g_board[move.MoveFrom].PieceColour;
		g_board[move.MoveFrom].PieceType = Piece::PieceTypeNone;

		//Rook if castling
		g_board[move.SecondaryMoveTo].PieceType = g_board[move.SecondaryMoveFrom].PieceType;
		g_board[move.SecondaryMoveTo].PieceColour = g_board[move.SecondaryMoveFrom].PieceColour;

		//Enpassant
		g_board[move.OtherSquareToClear].PieceType = Piece::PieceTypeNone;

		g_board[move.SecondaryMoveFrom].PieceType = Piece::PieceTypeNone;

		//Check testing				
		BoardIndex0x88 kingSideRookStartLocation;
		BoardIndex0x88 queenSideRookStartLocation;
		if (g_colourToMove == Piece::PieceColourWhite)
		{
			//Black moving (and white attacking) - Remember side to move is now reversed
			kingSideRookStartLocation = 0x77;
			queenSideRookStartLocation = 0x70;

			//g_BlackKingLocation0x88 was updated by UpdateStateForMove
			if (IsSquareAttacked(g_BlackKingLocation0x88, g_colourToMove))
			{
				//Cannot move into or leave in check
				UnMakeMove();
				return false;
			}
		}
		else
		{
			//White moving (and black attacking) - Remember side to move is now reversed
			kingSideRookStartLocation = 0x07;
			queenSideRookStartLocation = 0x00;

			//g_WhiteKingLocation0x88 was updated by UpdateStateForMove
			if (IsSquareAttacked(g_WhiteKingLocation0x88, g_colourToMove))
			{
				//Cannot move into or leave in check
				UnMakeMove();
				return false;
			}
		}

		bool isCastling = (move.SecondaryMoveFrom != Null0x88Square);
		if (isCastling)
		{
			if (IsSquareAttacked(move.MoveFrom, g_colourToMove)) //g_colourToMove has been reversed!
			{
				//Cannot castle out of check
				UnMakeMove();
				return false;
			}

			if (move.SecondaryMoveFrom == kingSideRookStartLocation)
			{
				//King Side
				//Check square one to the right, side to move is attacking!
				if (IsSquareAttacked(move.MoveFrom + 1, g_colourToMove)) //g_colourToMove has been reversed!
				{
					//Cannot castle through check
					UnMakeMove();
					return false;
				}
			}
			else if (move.SecondaryMoveFrom == queenSideRookStartLocation)
			{
				//Queen Side
				//Check square one to the right, side to move is attacking!
				if (IsSquareAttacked(move.MoveFrom - 1, g_colourToMove)) //g_colourToMove has been reversed!
				{
					//Cannot castle through check
					UnMakeMove();
					return false;
				}
			}
		}

		return true;
	}

	void Board::UnMakeMove()
	{
		--g_UnmakeLength;

		UnmakeItem& unmakeItem = g_UnmakeList[g_UnmakeLength];

		// *** Move *** \\
		
		g_board[unmakeItem.MovedFrom].PieceColour = unmakeItem.OriginalPiece.PieceColour;
		g_board[unmakeItem.MovedFrom].PieceType = unmakeItem.OriginalPiece.PieceType;

		g_board[unmakeItem.MovedTo].PieceColour = unmakeItem.CapturedPiece.PieceColour;
		g_board[unmakeItem.MovedTo].PieceType = unmakeItem.CapturedPiece.PieceType;

		g_board[unmakeItem.MovedFrom2].PieceColour = g_board[unmakeItem.MovedTo2].PieceColour;
		g_board[unmakeItem.MovedFrom2].PieceType = g_board[unmakeItem.MovedTo2].PieceType;

		g_board[unmakeItem.MovedTo2].PieceType = Piece::PieceTypeNone;

		g_board[unmakeItem.OtherReplaceSquare].PieceColour = unmakeItem.OtherReplacePiece.PieceColour;
		g_board[unmakeItem.OtherReplaceSquare].PieceType = unmakeItem.OtherReplacePiece.PieceType;

		// *** State *** \\

		g_colourToMove = unmakeItem.ColourToMove;
		g_canWhiteCastleKingSide = unmakeItem.CanWhiteCastleKingSide;
		g_canWhiteCastleQueenSide = unmakeItem.CanWhiteCastleQueenSide;
		g_canBlackCastleKingSide = unmakeItem.CanBlackCastleKingSide;
		g_canBlackCastleQueenSide = unmakeItem.CanBlackCastleQueenSide;
		g_WhiteKingLocation0x88 = unmakeItem.WhiteKingLocation0x88;
		g_BlackKingLocation0x88 = unmakeItem.BlackKingLocation0x88;
		g_EnpassantTargetSquare = unmakeItem.EnpassantTargetSquare;
		g_halfMoveClock = unmakeItem.HalfMoveClock;
		g_fullMoveNumber = unmakeItem.FullMoveNumber;
	}

	bool Board::RecordStateToUnMake(
		const Move& move)
	{
		if (g_UnmakeLength == g_UnmakeCapacity)
		{
			if (!IncreaseUnmakeListCapacity(g_UnmakeCapacity + UnmakeMoveListCapacityIncrement)) return false;
		}

		// *** Move *** \\
		
		UnmakeItem& unmakeItem = g_UnmakeList[g_UnmakeLength];

		unmakeItem.MovedFrom = move.MoveFrom;
		unmakeItem.MovedTo = move.MoveTo;
		unmakeItem.OriginalPiece.PieceColour = g_board[move.MoveFrom].PieceColour;
		unmakeItem.OriginalPiece.PieceType = g_board[move.MoveFrom].PieceType;
		unmakeItem.CapturedPiece.PieceColour = g_board[move.MoveTo].PieceColour;
		unmakeItem.CapturedPiece.PieceType = g_board[move.MoveTo].PieceType;
		unmakeItem.MovedFrom2 = move.SecondaryMoveFrom;
		unmakeItem.MovedTo2 = move.SecondaryMoveTo;
		unmakeItem.OtherReplaceSquare = move.OtherSquareToClear;
		unmakeItem.OtherReplacePiece.PieceColour = g_board[move.OtherSquareToClear].PieceColour;
		unmakeItem.OtherReplacePiece.PieceType = g_board[move.OtherSquareToClear].PieceType;

		// *** State *** \\

		unmakeItem.ColourToMove = g_colourToMove;
		unmakeItem.CanWhiteCastleKingSide = g_canWhiteCastleKingSide;
		unmakeItem.CanWhiteCastleQueenSide = g_canWhiteCastleQueenSide;
		unmakeItem.CanBlackCastleKingSide = g_canBlackCastleKingSide;
		unmakeItem.CanBlackCastleQueenSide = g_canBlackCastleQueenSide;
		unmakeItem.WhiteKingLocation0x88 = g_WhiteKingLocation0x88;
		unmakeItem.BlackKingLocation0x88 = g_BlackKingLocation0x88;
		unmakeItem.EnpassantTargetSquare = g_EnpassantTargetSquare;
		unmakeItem.HalfMoveClock = g_halfMoveClock;
		unmakeItem.FullMoveNumber = g_fullMoveNumber;

		++g_UnmakeLength;
		return true;
	}

	std::string Board::GetPositionAsFen()
	{
		std::stringstream result;

		for (int rank = 7; rank >= 0; --rank)
		{
			if (rank < 7) result << "/";
			int noPieceCount{ 0 };
			for (int file = 0; file <= 7; ++file)
			{
				Piece& piece = g_board[FileRankTo0x88(file, rank)];

				if (piece.PieceType == Piece::PieceTypeNone)
				{
					++noPieceCount;
				}
				else
				{
					if (noPieceCount > 0) result << noPieceCount;
					noPieceCount = 0;

					if (piece.PieceColour == Piece::PieceColourWhite)
					{
						//White
						switch (piece.PieceType)
						{
						case Piece::PieceTypeRook:
							result << "R";
							break;
						case Piece::PieceTypeKnight:
							result << "N";
							break;
						case Piece::PieceTypeBishop:
							result << "B";
							break;
						case Piece::PieceTypeQueen:
							result << "Q";
							break;
						case Piece::PieceTypeKing:
							result << "K";
							break;
						case Piece::PieceTypePawn:
							result << "P";
							break;
						}
					}
					else
					{
						//Black
						switch (piece.PieceType)
						{
						case Piece::PieceTypeRook:
							result << "r";
							break;
						case Piece::PieceTypeKnight:
							result << "n";
							break;
						case Piece::PieceTypeBishop:
							result << "b";
							break;
						case Piece::PieceTypeQueen:
							result << "q";
							break;
						case Piece::PieceTypeKing:
							result << "k";
							break;
						case Piece::PieceTypePawn:
							result << "p";
							break;
						}
					}
				}
			}
			if (noPieceCount > 0) result << noPieceCount;
		}

		if (g_colourToMove == Piece::PieceColourWhite)
		{
			result << " w ";
		}
		else
		{
			result << " b ";
		}

		if (!g_canWhiteCastleKingSide &&
			!g_canWhiteCastleQueenSide &&
			!g_canBlackCastleKingSide &&
			!g_canBlackCastleQueenSide)
		{
			result << "- ";
		}
		else
		{
			if (g_canWhiteCastleKingSide) result << "K";
			if (g_canWhiteCastleQueenSide) result << "Q";
			if (g_canBlackCastleKingSide) result << "k";
			if (g_canBlackCastleQueenSide) result << "q";
			result << " ";
		}

		if (g_EnpassantTargetSquare == Null0x88Square)
		{
			result << "- ";
		}
		else
		{
			char file = From0x88ToFileLetter(g_EnpassantTargetSquare);
			int rank = From0x88ToRank(g_EnpassantTargetSquare) + 1;
			result << file << rank << " ";
		}

		result << g_halfMoveClock << " ";

		result << g_fullMoveNumber;

		return result.str();
	}

	bool Board::SetPositionFromFen(std::string fen)
	{
		//TODO:
		NewGame();

		std::stringstream input;
		input << fen;
		//TODO: Why is the following not needed
		//input.seekg(0);		

		std::string fields;
		std::string activeColour;
		std::string castling;
		std::string enPassant;
		std::string fifth;
		std::string halfmove;
		std::string fullmove;
		input >> fields;
		input >> activeColour;
		input >> castling;
		input >> enPassant;
		input >> halfmove;
		input >> fullmove;

		if (fields == "") return false; //No need to call NewGame() here as so far no internal state is modified from standard
		if (activeColour == "") return false; //No need to call NewGame() here as so far no internal state is modified from standard
		if (castling == "") return false; //No need to call NewGame() here as so far no internal state is modified from standard
		if (enPassant == "") return false; //No need to call NewGame() here as so far no internal state is modified from standard		

		if (!ProcessFenFields(fields))
		{
			NewGame();
			return false;
		}

		if (activeColour == "w")
		{
			g_colourToMove = Piece::PieceColourWhite;
		}
		else if (activeColour == "b")
		{
			g_colourToMove = Piece::PieceColourBlack;
		}
		else
		{
			NewGame();
			return false;
		}

		g_canWhiteCastleKingSide = false;
		g_canWhiteCastleQueenSide = false;
		g_canBlackCastleKingSide = false;
		g_canBlackCastleQueenSide = false;

		if (castling != "-")
		{
			for (char c : castling)
			{
				switch (c)
				{
				case 'K':
					g_canWhiteCastleKingSide = true;
					break;
				case 'Q':
					g_canWhiteCastleQueenSide = true;
					break;
				case 'k':
					g_canBlackCastleKingSide = true;
					break;
				case 'q':
					g_canBlackCastleQueenSide = true;
					break;
				default:
					NewGame();
					return false;
				}
			}
		}

		if (enPassant != "-")
		{
			g_EnpassantTargetSquare = TextSquareTo0x88(enPassant);
			if (g_EnpassantTargetSquare == Null0x88Square)
			{
				NewGame();
				return false;
			}
		}

		//TODO: Validate
		if (halfmove != "")
		{
			if (!is_number(halfmove))
			{
				NewGame();
				return false;
			}
			//TODO: Improve
			if (halfmove.size() > 6)
			{
				NewGame();
				return false;
			}
			g_halfMoveClock = std::stoi(halfmove);
			if (g_halfMoveClock < 0)
			{
				NewGame();
				return false;
			}
		}

		if (fullmove != "")
		{
			//TODO: Validate
			if (!is_number(fullmove))
			{
				NewGame();
				return false;
			}
			//TODO: Improve
			if (fullmove.size() > 6)
			{
				NewGame();
				return false;
			}
			g_fullMoveNumber = std::stoi(fullmove);
			if (g_fullMoveNumber < 1)
			{
				NewGame();
				return false;
			}
		}

		if (!ValidatePosition())
		{
			NewGame();
			return false;
		}

		return true;
	}









	void Board::ClearBoard()
	{
		for (int sq = 0; sq < BoardArrayLength; ++sq)
		{
			g_board[sq].PieceType = Piece::PieceTypeNone;
			g_board[sq].PieceColour = Piece::PieceColourWhite;
		}
	}

	void Board::SetupPawns(bool setupWhite)
	{
		int rank;
		int pieceColour;

		if (setupWhite)
		{
			rank = { 1 };
			pieceColour = { Piece::PieceColourWhite };
		}
		else
		{
			rank = { 6 };
			pieceColour = { Piece::PieceColourBlack };
		}

		for (int file = 0; file < 8; ++file)
		{
			SetPieceOnSquare(file, rank, Piece::PieceTypePawn, pieceColour);
		}
	}

	void Board::SetupPieces(bool setupWhite)
	{
		int rank;
		int pieceColour;

		if (setupWhite)
		{
			rank = { 0 };
			pieceColour = { Piece::PieceColourWhite };
		}
		else
		{
			rank = { 7 };
			pieceColour = { Piece::PieceColourBlack };
		}

		SetPieceOnSquare(0, rank, Piece::PieceTypeRook, pieceColour);
		SetPieceOnSquare(1, rank, Piece::PieceTypeKnight, pieceColour);
		SetPieceOnSquare(2, rank, Piece::PieceTypeBishop, pieceColour);
		SetPieceOnSquare(3, rank, Piece::PieceTypeQueen, pieceColour);
		SetPieceOnSquare(4, rank, Piece::PieceTypeKing, pieceColour);
		SetPieceOnSquare(5, rank, Piece::PieceTypeBishop, pieceColour);
		SetPieceOnSquare(6, rank, Piece::PieceTypeKnight, pieceColour);
		SetPieceOnSquare(7, rank, Piece::PieceTypeRook, pieceColour);
	}

	void Board::SetPieceOnSquare(int file, int rank, int pieceType, int pieceColour)
	{
		BoardIndex0x88 sq = FileRankTo0x88(file, rank);
		g_board[sq].PieceType = pieceType;
		g_board[sq].PieceColour = pieceColour;
	}

	void Board::AddMove(
		const BoardIndex0x88 start,
		const BoardIndex0x88 target,
		const int promoteTo,
		const BoardIndex0x88 otherSquareToClear,
		const BoardIndex0x88 secondaryMoveFrom,
		const BoardIndex0x88 secondaryMoveTo,
		Move* moves,
		int& moveCount)
	{
		Move& move = moves[moveCount];
		move.MoveFrom = start;
		move.MoveTo = target;
		move.PromoteTo = promoteTo;
		move.OtherSquareToClear = otherSquareToClear;
		move.SecondaryMoveFrom = secondaryMoveFrom;
		move.SecondaryMoveTo = secondaryMoveTo;

		++moveCount;
	}

	void Board::AddBasicPieceMoves(
		BoardIndex0x88 start,
		int direction,
		int maxCount,
		Move* moves,
		int& moveCount)
	{
		BoardIndex0x88 pos = start + direction;
		int count = 0;

		while (Is0x88SquareValid(pos))
		{
			Piece& piece = g_board[pos];
			if (piece.PieceType == Piece::PieceTypeNone)
			{
				//Empty Square
				AddMove(
					start,
					pos,
					Piece::PieceTypeNone,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				++count;
				if (count == maxCount) break; //Find no more
				pos = pos + direction; //Next position
			}
			else if (piece.PieceColour != g_colourToMove)
			{
				//Capture
				AddMove(
					start,
					pos,
					Piece::PieceTypeNone,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				break; //Now stop - Do not pass through piece
			}
			else
			{
				//Cannot capture own piece - Stop
				break;
			}
		}
	}

	void Board::AddRookMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		AddBasicPieceMoves(start, BoardDirUp, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDown, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirRight, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirLeft, 8, moves, moveCount);
	}

	void Board::AddKnightMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		AddBasicPieceMoves(start, BoardDirKnight1, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight2, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight3, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight4, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight5, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight6, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight7, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirKnight8, 1, moves, moveCount);
	}

	void Board::AddBishopMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		AddBasicPieceMoves(start, BoardDirUpRight, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownLeft, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirUpLeft, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownRight, 8, moves, moveCount);
	}

	void Board::AddQueenMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		AddBasicPieceMoves(start, BoardDirUp, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDown, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirRight, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirLeft, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirUpRight, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownLeft, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirUpLeft, 8, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownRight, 8, moves, moveCount);
	}

	void Board::AddKingMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		AddBasicPieceMoves(start, BoardDirUp, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDown, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirRight, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirLeft, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirUpRight, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownLeft, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirUpLeft, 1, moves, moveCount);
		AddBasicPieceMoves(start, BoardDirDownRight, 1, moves, moveCount);

		//Castling
		if (g_colourToMove == Piece::PieceColourWhite)
		{
			//White
			if (g_canWhiteCastleKingSide)
			{
				if (g_board[0x05].PieceType == Piece::PieceTypeNone &&
					g_board[0x06].PieceType == Piece::PieceTypeNone)
				{
					//King and Rook must be in position for castling kingside.
					//Check tests are only tested on MakeMove()
					AddMove(
						0x04,
						0x06,
						Piece::PieceTypeNone,
						Null0x88Square,
						0x07,
						0x05,
						moves,
						moveCount);
				}
			}

			if (g_canWhiteCastleQueenSide)
			{
				if (g_board[0x03].PieceType == Piece::PieceTypeNone &&
					g_board[0x02].PieceType == Piece::PieceTypeNone &&
					g_board[0x01].PieceType == Piece::PieceTypeNone)
				{
					//King and Rook must be in position for castling queenside.
					//Check tests are only tested on MakeMove()
					AddMove(
						0x04,
						0x02,
						Piece::PieceTypeNone,
						Null0x88Square,
						0x00,
						0x03,
						moves,
						moveCount);
				}
			}
		}
		else
		{
			//Black
			if (g_canBlackCastleKingSide)
			{
				if (g_board[0x75].PieceType == Piece::PieceTypeNone &&
					g_board[0x76].PieceType == Piece::PieceTypeNone)
				{
					//King and Rook must be in position for castling kingside.
					//Check tests are only tested on MakeMove()
					AddMove(
						0x74,
						0x76,
						Piece::PieceTypeNone,
						Null0x88Square,
						0x77,
						0x75,
						moves,
						moveCount);
				}
			}

			if (g_canBlackCastleQueenSide)
			{
				if (g_board[0x73].PieceType == Piece::PieceTypeNone &&
					g_board[0x72].PieceType == Piece::PieceTypeNone &&
					g_board[0x71].PieceType == Piece::PieceTypeNone)
				{
					//King and Rook must be in position for castling queenside.
					//Check tests are only tested on MakeMove()
					AddMove(
						0x74,
						0x72,
						Piece::PieceTypeNone,
						Null0x88Square,
						0x70,
						0x73,
						moves,
						moveCount);
				}
			}
		}
	}

	void Board::AddPawnAdvance(
		const BoardIndex0x88 initial,
		const BoardIndex0x88 start,
		const int direction,
		const unsigned int promotionOperator,
		const unsigned int promotionComparison,
		bool isOnHomeSquare,
		Move* moves,
		int& moveCount)
	{
		BoardIndex0x88 target = start + direction;

		if (!Is0x88SquareValid(target)) return;

		if (g_board[target].PieceType == Piece::PieceTypeNone)
		{
			if ((target & promotionOperator) == promotionComparison)
			{
				//Promotion
				AddMove(
					initial,
					target,
					Piece::PieceTypeQueen,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					initial,
					target,
					Piece::PieceTypeRook,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					initial,
					target,
					Piece::PieceTypeBishop,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					initial,
					target,
					Piece::PieceTypeKnight,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
			}
			else
			{
				//No promotion
				AddMove(
					initial,
					target,
					Piece::PieceTypeNone,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
			}

			if (isOnHomeSquare)
			{
				AddPawnAdvance(
					initial,
					target,
					direction,
					promotionOperator,
					promotionComparison,
					false,
					moves,
					moveCount
				);
			}
		}
	}

	void Board::AddPawnCapture(
		const BoardIndex0x88 start,
		const int direction,
		const unsigned int promotionOperator,
		const unsigned int promotionComparison,
		Move* moves,
		int& moveCount)
	{
		BoardIndex0x88 target = start + direction;

		if (!Is0x88SquareValid(target)) return;

		if (g_board[target].PieceType != Piece::PieceTypeNone &&
			g_board[target].PieceColour != g_colourToMove)
		{
			if ((target & promotionOperator) == promotionComparison)
			{
				//Promotion
				AddMove(
					start,
					target,
					Piece::PieceTypeQueen,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					start,
					target,
					Piece::PieceTypeRook,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					start,
					target,
					Piece::PieceTypeBishop,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
				AddMove(
					start,
					target,
					Piece::PieceTypeKnight,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
			}
			else
			{
				//No promotion
				AddMove(
					start,
					target,
					Piece::PieceTypeNone,
					Null0x88Square,
					Null0x88Square,
					Null0x88Square,
					moves,
					moveCount);
			}
		}
	}

	void Board::AddPawnMoves(BoardIndex0x88 start, Move* moves, int& moveCount)
	{
		if (g_colourToMove == Piece::PieceColourWhite)
		{
			//White

			//Advance and promotion
			AddPawnAdvance(start, start, BoardDirPawnAdvanceSingleWhite, 0x70, 0x70, (start & 0xE0) == 0, moves, moveCount);
			//Capture
			AddPawnCapture(start, BoardDirPawnCaptureLeftWhite, 0x70, 0x70, moves, moveCount);
			AddPawnCapture(start, BoardDirPawnCaptureRightWhite, 0x70, 0x70, moves, moveCount);

			if (g_EnpassantTargetSquare != Null0x88Square)
			{
				BoardIndex0x88 pos = start + BoardDirPawnCaptureLeftWhite;
				if (pos == g_EnpassantTargetSquare)
				{
					AddMove(
						start,
						pos,
						Piece::PieceTypeNone,
						pos - 16,
						Null0x88Square,
						Null0x88Square,
						moves,
						moveCount);
				}

				pos = start + BoardDirPawnCaptureRightWhite;
				if (pos == g_EnpassantTargetSquare)
				{
					AddMove(
						start,
						pos,
						Piece::PieceTypeNone,
						pos - 16,
						Null0x88Square,
						Null0x88Square,
						moves,
						moveCount);
				}
			}
		}
		else
		{
			//Black

			//Advance and promotion
			AddPawnAdvance(start, start, BoardDirPawnAdvanceSingleBlack, 0xF0, 0x00, (start & 0x60) == 0x60, moves, moveCount);

			//Capture
			AddPawnCapture(start, BoardDirPawnCaptureRightBlack, 0xF0, 0x00, moves, moveCount);
			AddPawnCapture(start, BoardDirPawnCaptureLeftBlack, 0xF0, 0x00, moves, moveCount);

			if (g_EnpassantTargetSquare != Null0x88Square)
			{
				BoardIndex0x88 pos = start + BoardDirPawnCaptureRightBlack;
				if (pos == g_EnpassantTargetSquare)
				{
					AddMove(
						start,
						pos,
						Piece::PieceTypeNone,
						pos + 16,
						Null0x88Square,
						Null0x88Square,
						moves,
						moveCount);
				}

				pos = start + BoardDirPawnCaptureLeftBlack;
				if (pos == g_EnpassantTargetSquare)
				{
					AddMove(
						start,
						pos,
						Piece::PieceTypeNone,
						pos + 16,
						Null0x88Square,
						Null0x88Square,
						moves, moveCount);
				}
			}
		}
	}

	bool Board::IsSquareAttackedByStraightOrDiagonalAttackingPiece(
		const BoardIndex0x88 square,
		const int attackingColour,
		const int direction,
		const int singleDirectionPieceType)
	{
		BoardIndex0x88 target = square + direction;

		int count = 0;
		while (Is0x88SquareValid(target))
		{
			++count;

			Piece& piece = g_board[target];

			if (piece.PieceType != Piece::PieceTypeNone)
			{
				//There is a piece
				if (piece.PieceColour == attackingColour)
				{
					//A potentially attacking piece - check type
					switch (piece.PieceType)
					{
					case Piece::PieceTypeQueen:
						return true; //Attacked
					case Piece::PieceTypeKing:
						return count == 1; //In range so attacked
					default:
						return (piece.PieceType == singleDirectionPieceType);
					}
				}
				else
				{
					//A friendly piece so not attacking but potentially blocking
					return false;
				}
			} //Else, no piece so not attacking and can continue to the next square (if any)			

			target += direction;
		}

		return false; //No attack
	}

	bool Board::IsSquareAttackedByKnight(
		const BoardIndex0x88 square,
		const int attackingColour)
	{
		BoardIndex0x88 target = square + BoardDirKnight1;

		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight2;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}


		target = square + BoardDirKnight3;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight4;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight5;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight6;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight7;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + BoardDirKnight8;
		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];
			if (piece.PieceType == Piece::PieceTypeKnight &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		return false;
	}

	bool Board::IsAttackedByPawn(
		const BoardIndex0x88 square,
		const int attackingColour,
		const int direction1,
		const int direction2)
	{
		BoardIndex0x88 target = square + direction1;

		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];

			if (piece.PieceType == Piece::PieceTypePawn &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		target = square + direction2;

		if (Is0x88SquareValid(target))
		{
			Piece& piece = g_board[target];

			if (piece.PieceType == Piece::PieceTypePawn &&
				piece.PieceColour == attackingColour)
			{
				return true;
			}
		}

		return false;
	}

	bool Board::IsSquareAttacked(const BoardIndex0x88 square, const int attackingColour)
	{
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirUp,
			Piece::PieceTypeRook)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirDown,
			Piece::PieceTypeRook)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirRight,
			Piece::PieceTypeRook)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirLeft,
			Piece::PieceTypeRook)) return true;

		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirUpRight,
			Piece::PieceTypeBishop)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirDownLeft,
			Piece::PieceTypeBishop)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirUpLeft,
			Piece::PieceTypeBishop)) return true;
		if (IsSquareAttackedByStraightOrDiagonalAttackingPiece(
			square,
			attackingColour,
			BoardDirDownRight,
			Piece::PieceTypeBishop)) return true;

		if (IsSquareAttackedByKnight(square, attackingColour)) return true;

		if (attackingColour == Piece::PieceColourWhite)
		{
			//White attacking black
			if (IsAttackedByPawn(
				square,
				attackingColour,
				BoardDirTowardsAttackingWhitePawn1,
				BoardDirTowardsAttackingWhitePawn2)) return true;
		}
		else
		{
			//Black attacking white
			if (IsAttackedByPawn(
				square,
				attackingColour,
				BoardDirTowardsAttackingBlackPawn1,
				BoardDirTowardsAttackingBlackPawn2)) return true;
		}

		return false;
	}

	bool Board::ProcessFenFields(const std::string& fields)
	{
		ClearBoard();

		std::string s = fields;

		int rank = 7;

		size_t pos = s.find('/');

		std::string row;

		if (pos == std::string::npos)
		{
			row = s;
			s = "";
		}
		else
		{
			row = s.substr(0, pos);
			s.erase(0, pos + 1);
		}

		while (row.size() > 0)
		{
			if (rank < 0)
			{
				return false;
			}

			int file = { 0 };

			for (char c : row)
			{
				if (file > 7)
				{
					return false;
				}
				switch (c)
				{
				case 'K':
					SetPieceOnSquare(file, rank, Piece::PieceTypeKing, Piece::PieceColourWhite);
					g_WhiteKingLocation0x88 = FileRankTo0x88(file, rank);
					++file;
					break;
				case 'Q':
					SetPieceOnSquare(file, rank, Piece::PieceTypeQueen, Piece::PieceColourWhite);
					++file;
					break;
				case 'R':
					SetPieceOnSquare(file, rank, Piece::PieceTypeRook, Piece::PieceColourWhite);
					++file;
					break;
				case 'N':
					SetPieceOnSquare(file, rank, Piece::PieceTypeKnight, Piece::PieceColourWhite);
					++file;
					break;
				case 'B':
					SetPieceOnSquare(file, rank, Piece::PieceTypeBishop, Piece::PieceColourWhite);
					++file;
					break;
				case 'P':
					SetPieceOnSquare(file, rank, Piece::PieceTypePawn, Piece::PieceColourWhite);
					++file;
					break;
				case 'k':
					SetPieceOnSquare(file, rank, Piece::PieceTypeKing, Piece::PieceColourBlack);
					g_BlackKingLocation0x88 = FileRankTo0x88(file, rank);
					++file;
					break;
				case 'q':
					SetPieceOnSquare(file, rank, Piece::PieceTypeQueen, Piece::PieceColourBlack);
					++file;
					break;
				case 'r':
					SetPieceOnSquare(file, rank, Piece::PieceTypeRook, Piece::PieceColourBlack);
					++file;
					break;
				case 'n':
					SetPieceOnSquare(file, rank, Piece::PieceTypeKnight, Piece::PieceColourBlack);
					++file;
					break;
				case 'b':
					SetPieceOnSquare(file, rank, Piece::PieceTypeBishop, Piece::PieceColourBlack);
					++file;
					break;
				case 'p':
					SetPieceOnSquare(file, rank, Piece::PieceTypePawn, Piece::PieceColourBlack);
					++file;
					break;
				default:
					//TODO: OK - Make a library function
					if (c < '0' || c > '8')
					{
						return false;
					}
					std::stringstream ss;
					ss << c;
					int fileCount = std::stoi(ss.str());
					file += fileCount;
					break;
				}
			}

			if (file != 8)
			{
				return false; //Not enough/too many ranks
			}

			--rank;

			pos = s.find('/');

			if (pos == std::string::npos)
			{
				row = s;
				s = "";
			}
			else
			{
				row = s.substr(0, pos);
				s.erase(0, pos + 1);
			}
		}

		if (rank != -1)
		{
			return false; //Not enough/too many files
		}

		return true;
	}

	bool Board::ValidatePosition()
	{
		//Check:
		//   * There is one King per side
		//   * No pawns exist on ranks 1 or 8
		//   * The side not on move is not in check
		//   * Corrects Enpassant
		//   * Corrects castling rights

		int whiteKingCount = 0;
		int blackKingCount = 0;
		for (int file = 0; file < 8; ++file)
		{
			for (int rank = 0; rank < 8; ++rank)
			{
				BoardIndex0x88 sq = FileRankTo0x88(file, rank);
				int piece = g_board[sq].PieceType;
				int colour = g_board[sq].PieceColour;

				if (piece == Piece::PieceTypeKing)
				{
					if (colour == Piece::PieceColourWhite)
					{
						++whiteKingCount;
						if (whiteKingCount > 1)
						{
							return false;
						}
					}
					else
					{
						++blackKingCount;
						if (blackKingCount > 1)
						{
							return false;
						}
					}
				}
				else if (piece == Piece::PieceTypePawn)
				{
					if (rank == 0 || rank == 7)
					{
						return false;
					}
				}
			}
		}

		if (whiteKingCount != 1)
		{
			return false;
		}
		if (blackKingCount != 1)
		{
			return false;
		}

		if (g_colourToMove == Piece::PieceColourWhite)
		{
			//Black must not be in check
			if (IsSquareAttacked(g_BlackKingLocation0x88, Piece::PieceColourWhite))
			{
				return false;
			}
		}
		else
		{
			//White must not be in check
			if (IsSquareAttacked(g_WhiteKingLocation0x88, Piece::PieceColourBlack))
			{
				return false;
			}
		}

		if (g_EnpassantTargetSquare != Null0x88Square)
		{
			int rank = From0x88ToRank(g_EnpassantTargetSquare);
			int file = From0x88ToFile(g_EnpassantTargetSquare);
			if (rank == 2)
			{
				if (g_colourToMove == Piece::PieceColourWhite)
				{
					g_EnpassantTargetSquare = Null0x88Square;
				}
				++rank;
				BoardIndex0x88 sq = FileRankTo0x88(file, rank);
				if (g_board[sq].PieceColour != Piece::PieceColourWhite ||
					g_board[sq].PieceType != Piece::PieceTypePawn)
				{
					g_EnpassantTargetSquare = Null0x88Square;
				}
			}
			else if (rank == 5)
			{
				if (g_colourToMove == Piece::PieceColourBlack)
				{
					g_EnpassantTargetSquare = Null0x88Square;
				}
				--rank;
				BoardIndex0x88 sq = FileRankTo0x88(file, rank);
				if (g_board[sq].PieceColour != Piece::PieceColourBlack ||
					g_board[sq].PieceType != Piece::PieceTypePawn)
				{
					g_EnpassantTargetSquare = Null0x88Square;
				}
			}
			else
			{
				g_EnpassantTargetSquare = Null0x88Square;
			}
		}

		if (g_canWhiteCastleKingSide)
		{
			//Must be a white king on 0x04 and white Rook on 0x07
			if (g_board[0x04].PieceColour != Piece::PieceColourWhite ||
				g_board[0x04].PieceType != Piece::PieceTypeKing)
			{
				g_canWhiteCastleKingSide = false;
			}

			if (g_board[0x07].PieceColour != Piece::PieceColourWhite ||
				g_board[0x07].PieceType != Piece::PieceTypeRook)
			{
				g_canWhiteCastleKingSide = false;
			}
		}

		if (g_canWhiteCastleQueenSide)
		{
			//Must be a white king on 0x04 and white Rook on 0x00
			if (g_board[0x04].PieceColour != Piece::PieceColourWhite ||
				g_board[0x04].PieceType != Piece::PieceTypeKing)
			{
				g_canWhiteCastleQueenSide = false;
			}

			if (g_board[0x00].PieceColour != Piece::PieceColourWhite ||
				g_board[0x00].PieceType != Piece::PieceTypeRook)
			{
				g_canWhiteCastleQueenSide = false;
			}
		}

		if (g_canBlackCastleKingSide)
		{
			//Must be a black king on 0x74 and black Rook on 0x77
			if (g_board[0x74].PieceColour != Piece::PieceColourBlack ||
				g_board[0x74].PieceType != Piece::PieceTypeKing)
			{
				g_canBlackCastleKingSide = false;
			}

			if (g_board[0x77].PieceColour != Piece::PieceColourBlack ||
				g_board[0x77].PieceType != Piece::PieceTypeRook)
			{
				g_canBlackCastleKingSide = false;
			}
		}

		if (g_canBlackCastleQueenSide)
		{
			//Must be a black king on 0x74 and black Rook on 0x70
			//Must be a black king on 0x74 and black Rook on 0x77
			if (g_board[0x74].PieceColour != Piece::PieceColourBlack ||
				g_board[0x74].PieceType != Piece::PieceTypeKing)
			{
				g_canBlackCastleQueenSide = false;
			}

			if (g_board[0x70].PieceColour != Piece::PieceColourBlack ||
				g_board[0x70].PieceType != Piece::PieceTypeRook)
			{
				g_canBlackCastleQueenSide = false;
			}
		}

		return true;
	}

	bool Board::IsInCheck(int colour)
	{
		if (colour == 1) return false;
		return false;
	}

	bool Board::IncreaseUnmakeListCapacity(size_t newCapacity)
	{
		UnmakeItem* newUnmakeList = new UnmakeItem[newCapacity];
		if (newUnmakeList == nullptr) return false;

		for (size_t i = 0; i < g_UnmakeLength; ++i)
		{
			newUnmakeList[i] = g_UnmakeList[i];
		}

		delete[] g_UnmakeList;
		g_UnmakeList = newUnmakeList;
		g_UnmakeCapacity = { newCapacity };
		return true;
	}
}
