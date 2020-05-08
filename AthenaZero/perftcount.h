/*
	Part of the AthenaZero Chess Engine.

	This file contains code to hold details about a perft run (i.e. counts and pass/fail).

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

#ifndef ATHENAZERO_ENGINE_PERFT_COUNT
#define ATHENAZERO_ENGINE_PERFT_COUNT

namespace ATHENAZEROENG
{
	/*
	Holds an expected count, actual count and pass/fail.
	*/
	class PerftCount
	{
	public:
		/*
			Gets true if the value is recorded, false otherwise.
		*/
		inline bool GetIsRecorded() { return g_isRecorded; }

		/*
			Gets the expected count. Will always be 0 if GetIsRecorded() is false
		*/
		inline long long GetExpectedCount() { return g_expectedCount; }

		/*
			Gets the actual count.
		*/
		inline long long GetActualCount() { return g_actualCount; }

		/*
			Sets whether the value is recorded/applicable (true) or not (false).
		*/
		inline void SetIsRecorded(const bool value) { g_isRecorded = value; }

		/*
			Gets the expected count. Will always be 0 if GetIsRecorded() is false
		*/
		inline void SetExpectedCount(const long long value) { g_expectedCount = value; }

		/*
			Gets the actual count.
		*/
		inline void SetActualCount(const long long value) { g_actualCount = value; }

		/*
			Returns true if passed (or GetIsRecorded() is false), false if failed. Basically compares
			the expected to actual and checks they are equal.
		*/
		inline bool GetIsPassed() { return !g_isRecorded || g_expectedCount == g_actualCount; }
	private:
		bool g_isRecorded{ false };
		long long g_expectedCount{ 0 };
		long long g_actualCount{ 0 };
	};
}

#endif
