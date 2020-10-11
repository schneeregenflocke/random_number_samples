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

#pragma once

#include <vector>
#include <random>
#include <immintrin.h>


class rdrand32_Engine
{
public:

	rdrand32_Engine()
	{}

	using result_type = unsigned int;

	static constexpr result_type(min)() 
	{
		return 0;
	}

	static constexpr result_type(max)() 
	{
		return static_cast<result_type>(-1);
	}

	//use -mrdrnd compiler option for g++
	result_type operator()() 
	{
		result_type random;
		_rdrand32_step(&random);
		return random;
	}

	rdrand32_Engine(const rdrand32_Engine&) = delete;
	rdrand32_Engine& operator=(const rdrand32_Engine&) = delete;
};


template<typename T>
class RandomNumberGenerator
{
public:
	
	using U = typename T::result_type;
	
	RandomNumberGenerator(const T& distribution) :
		distribution(distribution) 
	{}

	U GenerateRandomNumber()
	{
		return distribution(rdrand);
	}

private:
	rdrand32_Engine rdrand;
	T distribution;
};

