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

#include <imgui.h>

#include <cmath>
#include <array>
#include <initializer_list>
#include <exception>
#include <stdexcept>


template <typename Ty>
class rect4
{
public:

	rect4() :
		edges{ 0, 0, 0, 0 }
	{}

	rect4(const std::array<Ty, 4>& edges) :
		edges(edges)
	{}

	// left, bottom, right, top
	rect4(const Ty l, const Ty b, const Ty r, const Ty t) :
		edges{ l, b, r, t }
	{}
	
	rect4(const Ty width, const Ty height)
	{
		Ty op = static_cast<Ty>(2);
		edges[0] = -width / op;
		edges[1] = -height / op;
		edges[2] = width / op;
		edges[3] = height / op;
	}

	rect4(const glm::vec<2, Ty>& lt, const glm::vec<2, Ty>& rb)
	{
		edges[0] = lt.x;
		edges[1] = rb.y;
		edges[2] = rb.x;
		edges[3] = lt.y;
	}

	// left
	Ty l() const
	{
		return edges[0];
	}

	// bottom
	Ty b() const
	{
		return edges[1];
	}

	// right
	Ty r() const
	{
		return edges[2];
	}

	// top
	Ty t() const
	{
		return edges[3];
	}

	// set left
	void l(const Ty l)
	{
		edges[0] = l;
	}

	// set bottom
	void b(const Ty b)
	{
		edges[1] = b;
	}

	// set right
	void r(const Ty r)
	{
		edges[2] = r;
	}

	// set top
	void t(const Ty t)
	{
		edges[3] = t;
	}

	bool is_empty() const
	{
		bool empty = false;

		if (l() == 0 && b() == 0 && r() == 0 && t() == 0)
		{
			empty = true;
		}

		return empty;
	}

	Ty width(const bool use_abs = false) const
	{
		Ty width = 0;
		
		if (use_abs == false)
		{
			if (l() > r())
			{
				throw std::logic_error("left > right");
			}

			width = r() - l();
		}

		if (use_abs == true)
		{
			width = std::abs(r() - l());
		}
		
		return width;
	}

	Ty height(const bool use_abs = false) const
	{
		Ty height = 0;

		if (use_abs == false)
		{
			if (b() > t())
			{
				throw std::logic_error("bottom > top");
			}

			height = t() - b();
		}

		if (use_abs == true)
		{
			height = std::abs(t() - b());
		}

		return height;
	}

	glm::vec<2, Ty> size(const bool use_abs = false) const
	{
		return glm::vec<2, Ty>(width(use_abs), height(use_abs));
	}
	
	rect4 expand(const rect4& expand) const
	{
		rect4 expanded;
		expanded.l(l() - expand.l());
		expanded.b(b() - expand.b());
		expanded.r(r() + expand.r());
		expanded.t(t() + expand.t());
		return expanded;
	}

	rect4 reduce(const rect4& reduce) const
	{
		rect4 reduced;
		reduced.l(l() + reduce.l());
		reduced.b(b() + reduce.b());
		reduced.r(r() - reduce.r());
		reduced.t(t() - reduce.t());
		return reduced;
	}

	rect4 scale(const Ty factor) const
	{
		float half_width_diff = ((width() * factor) - width()) / static_cast<Ty>(2);
		float half_height_diff = ((height() * factor) - height()) / static_cast<Ty>(2);
		
		rect4 scaled = expand(rect4(half_width_diff, half_width_diff, half_height_diff, half_height_diff));
		return scaled;
	}

	glm::vec<2, Ty> center() const
	{
		return glm::vec<2, Ty>(l() + width() / static_cast<Ty>(2), b() + height() / static_cast<Ty>(2));
	}
	
	glm::vec<2, Ty> lb() const
	{
		return glm::vec<2, Ty>(l(), b());
	}

	glm::vec<2, Ty> rb() const
	{
		return glm::vec<2, Ty>(r(), b());
	}

	glm::vec<2, Ty> lt() const
	{
		return glm::vec<2, Ty>(l(), t());
	}

	glm::vec<2, Ty> rt() const
	{
		return glm::vec<2, Ty>(r(), t());
	}

private:
	
	std::array<Ty, 4> edges;
};

using rect4f = rect4<float>;



template <typename Ty>
class val4
{
public:

	val4() :
		val{ 0, 0, 0, 0 }
	{}

	val4(const std::array<Ty, 4>& val) :
		val(val)
	{}

	Ty Lenght(const size_t from, const size_t to) const
	{
		return val[to] - val[from];
	}

	Ty& operator[](size_t index)
	{
		return val[index];
	}

	Ty operator[](size_t index) const
	{
		return val[index];
	}

private:

	std::array<Ty, 4> val;
};

using val4f = val4<float>;

