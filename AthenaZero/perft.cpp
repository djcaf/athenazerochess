/*
	Part of the AthenaZero Chess Engine.

	This file contains code to run perft tests.

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

#include "perft.h"
#include "perftresults.h"
#include "perftresult.h"
#include "board.h"
#include "perftinternalstats.h"
#include "move.h"
#include "perfttest.h"
#include "perftcount.h"
#include "timer.h"
#include <string>
#include <iostream>

namespace ATHENAZEROENG
{
	Perft::Perft()
	{
		//Setup perft tests

		SetupPerftTestsInitialPosition();
		SetupPerftTestsPosition2();
		SetupPerftTestsPosition3();
		SetupPerftTestsPosition4(false);
		SetupPerftTestsPosition4(true);
		SetupPerftTestsPosition5();
		SetupPerftTestsPosition6();
	}

	PerftResults Perft::RunAllPerftTests(
		int maxDepth,
		bool stopOnFirstFailure)
	{
		PerftResults results;

		for (PerftTest perftTest : g_perftTests)
		{
			if (maxDepth <= 0 || perftTest.GetDepth() <= maxDepth)
			{
				PerftResult result = Perft::RunPerftTest(
					perftTest.GetDepth(),
					perftTest.GetFen(),
					perftTest.GetTestName());

				long long expectedNodes = perftTest.GetExpectedNodes();
				long long expectedCaptures = perftTest.GetExpectedCaptures();
				long long expectedEnPassant = perftTest.GetExpectedEnPassant();
				long long expectedCastles = perftTest.GetExpectedCastles();
				long long expectedPromotions = perftTest.GetExpectedPromotions();
				long long expectedChecks = perftTest.GetExpectedChecks();
				long long expectedCheckmates = perftTest.GetExpectedCheckmates();

				SetExpectedValue(expectedNodes, result.NodeCount(), false);
				SetExpectedValue(expectedCaptures, result.CaptureCount(), true);
				SetExpectedValue(expectedEnPassant, result.EnPassantCount(), true);
				SetExpectedValue(expectedCastles, result.CastleCount(), true);
				SetExpectedValue(expectedPromotions, result.PromotionCount(), true);
				SetExpectedValue(expectedChecks, result.CheckCount(), true);
				SetExpectedValue(expectedCheckmates, result.CheckmateCount(), true);

				//TODO: Remove
				result.CaptureCount().SetIsRecorded(false);
				result.EnPassantCount().SetIsRecorded(false);
				result.CastleCount().SetIsRecorded(false);
				result.PromotionCount().SetIsRecorded(false);
				result.CheckCount().SetIsRecorded(false);
				result.CheckmateCount().SetIsRecorded(false);

				results.AddResult(result);

				if (stopOnFirstFailure && !result.GetPassed())
				{
					break;
				}
			}
		}

		return results;
	}

	PerftResult Perft::RunPerftTest(const int depth, const std::string& fen, const std::string& testName)
	{
		Board board;
		if (!board.SetPositionFromFen(fen))
		{
			PerftResult result(depth, fen, testName, 0.0);
			result.SetSetupPassed(false);
			return result;
		}

		std::string initalPosition = board.GetPositionAsFen();

		PerftInternalStats stats;

		Timer timer;

		Search(board, stats, depth);

		double elapsedTimeSeconds = timer.ElapsedTimeSeconds();

		PerftResult result(depth, fen, testName, elapsedTimeSeconds);
		result.SetSetupPassed(true);

		std::string finalPosition = board.GetPositionAsFen();

		result.SetIntergityCheckPassed(initalPosition == finalPosition);

		result.NodeCount().SetActualCount(stats.Nodes);

		return result;
	}

	void Perft::Search(Board& board, PerftInternalStats& stats, int depth)
	{
		if (depth == 0)
		{
			++stats.Nodes;
			return;
		}

		Move moves[255];
		int moveCount = 0;

		board.GeneratePseudoLegalMoves(moves, moveCount);

		for (int i = 0; i < moveCount; ++i)
		{
			if (board.MakeMove(moves[i]))
			{
				Search(board, stats, depth - 1);
				board.UnMakeMove();
			}
		}
	}

	void Perft::SetupPerftTestsInitialPosition()
	{
		//rnbq1k1r/pp1P1ppp/2p5/8/1bB5/7P/PPP1NnP1/RNBQK2R w KQ - 1 2
		g_perftTests.push_back(PerftTest(1, "rnbq1k1r/pp1P1ppp/2p5/8/1bB5/7P/PPP1NnP1/RNBQK2R w KQ - 1 2", "Test Position", 8, 0, 0, 0, 0, 0, 0));
		g_perftTests.push_back(PerftTest(1, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 20, 0, 0, 0, 0, 0, 0));
		g_perftTests.push_back(PerftTest(2, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 400, 0, 0, 0, 0, 0, 0));
		g_perftTests.push_back(PerftTest(3, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 8902, 34, 0, 0, 0, 12, 0));
		g_perftTests.push_back(PerftTest(4, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 197281, 1576, 0, 0, 0, 469, 8));
		g_perftTests.push_back(PerftTest(5, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 4865609, 82719, 258, 0, 0, 27351, 347));
		g_perftTests.push_back(PerftTest(6, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Initial Position", 119060324, 2812008, 5248, 0, 0, 809099, 10828));
	}

	void Perft::SetupPerftTestsPosition2()
	{
		g_perftTests.push_back(PerftTest(1, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", "Position 2 [Kiwipete]", 48, 8, 0, 2, 0, 0, 0));
		g_perftTests.push_back(PerftTest(2, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", "Position 2 [Kiwipete]", 2039, 351, 1, 91, 0, 3, 0));
		g_perftTests.push_back(PerftTest(3, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", "Position 2 [Kiwipete]", 97862, 17102, 45, 3162, 0, 993, 1));
		g_perftTests.push_back(PerftTest(4, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", "Position 2 [Kiwipete]", 4085603, 757163, 1929, 128013, 15172, 25523, 43));
		g_perftTests.push_back(PerftTest(5, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", "Position 2 [Kiwipete]", 193690690, 35043416, 73365, 4993637, 8392, 3309887, 30171));
	}

	void Perft::SetupPerftTestsPosition3()
	{
		g_perftTests.push_back(PerftTest(1, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 14, 1, 0, 0, 0, 2, 0));
		g_perftTests.push_back(PerftTest(2, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 191, 14, 0, 0, 0, 10, 0));
		g_perftTests.push_back(PerftTest(3, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 2812, 209, 2, 0, 0, 267, 0));
		g_perftTests.push_back(PerftTest(4, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 43238, 3348, 123, 0, 0, 1680, 17));
		g_perftTests.push_back(PerftTest(5, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 674624, 52051, 1165, 0, 0, 52950, 0));
		g_perftTests.push_back(PerftTest(6, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 11030083, 940350, 33325, 0, 7552, 452473, 2733));
		g_perftTests.push_back(PerftTest(7, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", "Position 3", 178633661, 14519036, 294874, 0, 140024, 12797406, 87));
	}

	void Perft::SetupPerftTestsPosition4(const bool reversed)
	{
		std::string fen;
		std::string testName;

		if (reversed)
		{
			fen = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
			testName = "Position 4b";
		}
		else
		{
			fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
			testName = "Position 4a";
		}

		if (fen == "") return;

		g_perftTests.push_back(PerftTest(1, fen, testName, 6, 0, 0, 0, 0, 0, 0));
		g_perftTests.push_back(PerftTest(2, fen, testName, 264, 87, 0, 6, 48, 10, 0));
		g_perftTests.push_back(PerftTest(3, fen, testName, 9467, 1021, 4, 0, 120, 38, 22));
		g_perftTests.push_back(PerftTest(4, fen, testName, 422333, 131393, 0, 7795, 60032, 15492, 5));
		g_perftTests.push_back(PerftTest(5, fen, testName, 15833292, 2046173, 6512, 0, 329464, 200568, 50562));
		g_perftTests.push_back(PerftTest(6, fen, testName, 706045033, 210369132, 212, 10882006, 81102984, 26973664, 81076));
	}

	void Perft::SetupPerftTestsPosition5()
	{
		g_perftTests.push_back(PerftTest(1, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "Position 5", 44, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(2, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "Position 5", 1486, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(3, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "Position 5", 62379, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(4, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "Position 5", 2103487, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(5, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "Position 5", 89941194, -1, -1, -1, -1, -1, -1));
	}

	void Perft::SetupPerftTestsPosition6()
	{
		g_perftTests.push_back(PerftTest(1, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "Position 6", 46, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(2, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "Position 6", 2079, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(3, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "Position 6", 89890, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(4, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "Position 6", 3894594, -1, -1, -1, -1, -1, -1));
		g_perftTests.push_back(PerftTest(5, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "Position 6", 164075551, -1, -1, -1, -1, -1, -1));
	}

	void Perft::SetExpectedValue(long long expectedValue, PerftCount& perftCount, bool allowIgnore)
	{
		if (allowIgnore && expectedValue < 0)
		{
			perftCount.SetExpectedCount(0);
			perftCount.SetIsRecorded(false);
		}
		else
		{
			perftCount.SetExpectedCount(expectedValue);
			perftCount.SetIsRecorded(true);
		}
	}
}
