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

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <filesystem>




class FileOutput
{
public:
	
	FileOutput(const std::string& name) :
		filepath(name)
	{
		file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		OpenFile();
	}

	~FileOutput()
	{
		Close();
	}

	template<typename T> 
	std::ofstream& operator<<(const T& value)
	{
		file << value;
		return file;
	}

	void Close()
	{
		try
		{
			file.close();
		}
		catch (std::ofstream::failure& e)
		{
			std::cerr << e.what() << '\n';
		}
	}

	std::filesystem::path GetFilepath()
	{
		return filepath.filename();
	}

private:

	template<typename T = double> 
	void OpenFile()
	{
		try
		{
			file.open(filepath, std::ios::trunc);

			file << std::fixed;
			file << std::setprecision(std::numeric_limits<T>::digits10);
		}
		catch (std::ofstream::failure& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	
	std::filesystem::path filepath;
	std::ofstream file;
};

