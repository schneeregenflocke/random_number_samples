/*
random_samples
Copyright(c) 2020 Marco Peyer

This program is free software; you can redistribute itand /or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.

See <https://www.gnu.org/licenses/gpl-2.0.txt>.
*/


#ifndef RANDOMDATES_H
#define RANDOMDATES_H


#include "RandomNumbers.h"
#include "Files.h"


#define BOOST_DATE_TIME_NO_LIB
#include <boost\date_time\gregorian\gregorian_types.hpp>
using namespace boost::gregorian;


class RandomDatePeriods
{
public:
	
	std::vector<date_period> intervals;

	RandomDatePeriods(date_period generateininterval, double probabilitystart, double probabilityend)
	{
		geometric_distribution dist0(probabilitystart);
		geometric_distribution dist1(probabilityend);

		RandomNumberGenerator<geometric_distribution> generator0(dist0);
		RandomNumberGenerator<geometric_distribution> generator1(dist1);

		bool run = true;
		date current_begin_date = generateininterval.begin();

		while (run)
		{
			current_begin_date += date_duration(generator0.RandomNumber());
			date current_end_date = current_begin_date + date_duration(generator1.RandomNumber());

			if (current_end_date < generateininterval.end())
			{
				intervals.push_back(date_period(current_begin_date, current_end_date));
				current_begin_date = current_end_date;
			}
			else
			{
				run = false;
			}
		}
	}

	void WriteToFile(std::ofstream& file, char seperator = '\t')
	{
		for (auto iter = intervals.cbegin(); iter != intervals.cend(); ++iter)
		{
			file << iter->begin().day().as_number() << "." << iter->begin().month().as_number() << "." << iter->begin().year();
			file << '\t';
			file << iter->end().day().as_number() << "." << iter->end().month().as_number() << "." << iter->end().year();
			file << '\n';
		}
	}
};

#endif // RANDOMDATES_H
