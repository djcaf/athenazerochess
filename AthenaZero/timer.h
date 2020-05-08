/*
	Part of the AthenaZero Chess Engine.

	This file contains timing code.

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

#ifndef ATHENAZERO_ENGINE_TIMER
#define ATHENAZERO_ENGINE_TIMER

#include <chrono>

namespace ATHENAZEROENG
{
	class Timer
	{
	public:
		Timer() : g_startTime(clock_t::now()) { }

		void Reset()
		{
			g_startTime = clock_t::now();
		}

		double ElapsedTimeSeconds() const
		{
			return std::chrono::duration_cast<second_t>(clock_t::now() - g_startTime).count();
		}

	private:
		using clock_t = std::chrono::high_resolution_clock;
		using second_t = std::chrono::duration<double, std::ratio<1>>;

		std::chrono::time_point<clock_t> g_startTime;
	};
}

#endif