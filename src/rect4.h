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

#include <exception>
#include <cmath>
#include <initializer_list>
#include <array>
#include <stdexcept>



class Vec2 : public ImVec2
{
public:
	Vec2() {}

	Vec2(const ImVec2& vec2) :
		ImVec2(vec2)
	{}

	Vec2(float x, float y) :
		ImVec2(x, y)
	{}

	Vec2 operator+(const Vec2& op) const
	{
		return Vec2(op.x + x, op.y + y);
	}

	Vec2 operator-(const Vec2& op) const
	{
		return Vec2(x - op.x, y - op.y);
	}

	Vec2 operator*(const Vec2& op) const
	{
		return Vec2(op.x * x, op.y * y);
	}

	Vec2 operator/(const Vec2& op) const
	{
		return Vec2(x / op.x, y / op.y);
	}

	Vec2 operator*(const float op) const
	{
		return Vec2(op * x, op * y);
	}

	Vec2 operator/(const float op) const
	{
		return Vec2(x / op, y / op);
	}

	/*float GetIntervalLenght() const
	{
		return y - x;
	}*/

	/*float InterpolateInterval(const float value) const
	{
		return x + GetIntervalLenght() * value;
	}*/
};


template <typename Ty0>
class val4
{
public:

	val4() :
		val{ 0, 0, 0, 0 }
	{}

	val4(const std::array<Ty0, 4>& val) :
		val(val)
	{}

	Ty0 Lenght(const size_t from, const size_t to) const
	{
		return val[to] - val[from];
	}

	Ty0& operator[](size_t index)
	{
		return val[index];
	}

	Ty0 operator[](size_t index) const
	{
		return val[index];
	}

private:

	std::array<Ty0, 4> val;
};

using val4f = val4<float>;


template <typename Ty0>
class rect4
{
public:
	rect4() :
		l(0),
		b(0),
		r(0),
		t(0)
	{}

	// Left, Bottom, Right, Top
	rect4(const Ty0 l, const Ty0 b, const Ty0 r, const Ty0 t) :
		l(l),
		b(b),
		r(r),
		t(t)
	{}
	
	rect4(const Ty0 width, const Ty0 height)
	{
		Ty0 operand = static_cast<Ty0>(2);
		l = -width / operand;
		b = -height / operand;
		r = width / operand;
		t = height / operand;
	}

	rect4(const Vec2& lt, const Vec2& rb)
	{
		l = lt.x;
		b = rb.y;
		r = rb.x;
		t = lt.y;
	}

	// Left
	Ty0 L() const
	{
		return l;
	}

	// Bottom
	Ty0 B() const
	{
		return b;
	}

	// Right
	Ty0 R() const
	{
		return r;
	}

	// Top
	Ty0 T() const
	{
		return t;
	}

	// Set Left
	void L(const Ty0 l)
	{
		this->l = l;
	}

	// Set Bottom
	void B(const Ty0 b)
	{
		this->b = b;
	}

	// Set Right
	void R(const Ty0 r)
	{
		this->r = r;
	}

	// Set Top
	void T(const Ty0 t)
	{
		this->t = t;
	}

	Ty0 Width(bool use_abs = false) const
	{
		Ty0 width = 0;
		
		if (use_abs == false)
		{
			if (l > r)
			{
				throw std::logic_error("left > right");
			}

			width = r - l;
		}

		if (use_abs == true)
		{
			width = std::abs(r - l);
		}
		
		return width;
	}

	bool IsEmpty()
	{
		bool is_empty = false;
		if (l == 0 && b == 0 && r == 0 && t == 0)
		{
			is_empty = true;
		}

		return is_empty;
	}

	Ty0 Height(bool use_abs = false) const
	{
		Ty0 height = 0;

		if (use_abs == false)
		{
			if (b > t)
			{
				throw std::logic_error("bottom > top");
			}

			height = t - b;
		}

		if (use_abs == true)
		{
			height = std::abs(t - b);
		}

		return height;
	}

	Vec2 Size(bool use_abs = false) const
	{
		return Vec2(Width(use_abs), Height(use_abs));
	}
	
	rect4 Expand(const rect4& expand) const
	{
		rect4 expanded;
		expanded.L(l - expand.l);
		expanded.B(b - expand.b);
		expanded.R(r + expand.r);
		expanded.T(t + expand.t);
		return expanded;
	}

	rect4 Reduce(const rect4& reduce) const
	{
		rect4 reduced;
		reduced.L(l + reduce.l);
		reduced.B(b + reduce.b);
		reduced.R(r - reduce.r);
		reduced.T(t - reduce.t);
		return reduced;
	}

	rect4 Scale(const Ty0 factor) const
	{
		float half_width_diff = ((Width() * factor) - Width()) / static_cast<Ty0>(2);
		float half_height_diff = ((Height() * factor) - Height()) / static_cast<Ty0>(2);
		
		rect4 scaled = Expand(rect4(half_width_diff, half_width_diff, half_height_diff, half_height_diff));
		return scaled;
	}

	Vec2 Center() const
	{
		return Vec2(l + Width() / static_cast<Ty0>(2), b + Height() / static_cast<Ty0>(2));
	}
	
	Vec2 LB() const
	{
		return Vec2(l, b);
	}
	Vec2 RB() const
	{
		return Vec2(r, b);
	}
	Vec2 LT() const
	{
		return Vec2(l, t);
	}
	Vec2 RT() const
	{
		return Vec2(r, t);
	}

private:
	Ty0 l;
	Ty0 b;
	Ty0 r;
	Ty0 t;
};

using rect4f = rect4<float>;


