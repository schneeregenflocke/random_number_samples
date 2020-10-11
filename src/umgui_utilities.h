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

#include "rect4.h"

#include "glfw_include.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>





class TransformCoordinateSystem
{
public:
	TransformCoordinateSystem()

	{}

	void SetAspectRatio(const rect4f& canvas_rect, const rect4f& plot_rect, const val4f& axes)
	{
		this->plot_rect = plot_rect;
		this->canvas_rect = canvas_rect;
		this->axes = axes;
		
		aspect_ratio = plot_rect.Size(true) / Vec2(axes.Lenght(0, 1), axes.Lenght(2, 3));
	}

	void SetCanvasSizeAndOrigin(Vec2 origin, Vec2 scrolling)
	{
		this->origin = origin;
		this->scrolling = scrolling;
	}

	val4f GetScrolledAxes()
	{
		Vec2 scaled_scrolling = ScaleToPlot(scrolling);

		val4f scrolled_axes;
		scrolled_axes[0] = axes[0] - scaled_scrolling.x;
		scrolled_axes[1] = axes[1] - scaled_scrolling.x;
		scrolled_axes[2] = axes[2] + scaled_scrolling.y;
		scrolled_axes[3] = axes[3] + scaled_scrolling.y;

		return scrolled_axes;
	}

	Vec2 TransformToCanvas(const Vec2& point) const
	{
		Vec2 scaled_point = ScaleToCanvas(point);
		float scaled_x_axis_begin = ScaleXToCanvas(axes[0]);
		float scaled_y_axis_begin = ScaleYToCanvas(axes[2]);

		float axes_x_gap = plot_rect.L() - canvas_rect.L();
		float axes_y_gap = std::abs(plot_rect.B() - canvas_rect.B());

		Vec2 result;
		result.x = origin.x + axes_x_gap - scaled_x_axis_begin + scaled_point.x;
		result.y = origin.y + canvas_rect.Height(true) - axes_y_gap + scaled_y_axis_begin - scaled_point.y;

		return result;
	}


	Vec2 ScaleToCanvas(const Vec2& value) const
	{
		Vec2 result;
		result.x = aspect_ratio.x * value.x;
		result.y = aspect_ratio.y * value.y;

		return result;
	}

	float ScaleXToCanvas(const float value) const
	{
		return aspect_ratio.x * value;
	}

	float ScaleYToCanvas(const float value) const
	{
		return aspect_ratio.y * value;
	}

	Vec2 ScaleToPlot(const Vec2& value) const
	{
		Vec2 result;
		result.x = (1.0f / aspect_ratio.x) * value.x;
		result.y = (1.0f / aspect_ratio.y) * value.y;
		return result;
	}

	float ScaleXToPlot(const float value) const
	{
		return (1.0f / aspect_ratio.x) * value;
	}

	float ScaleYToPlot(const float value) const
	{
		return (1.0f / aspect_ratio.y) * value;
	}

private:

	Vec2 aspect_ratio;

	Vec2 origin;
	Vec2 scrolling;

	rect4f canvas_rect;
	rect4f plot_rect;

	val4f axes;
};

class TransformCoordinateSystemInterface
{
public:

	val4f GetScrolledAxes()
	{
		return transform_coordinate_system.GetScrolledAxes();
	}

	TransformCoordinateSystem GetTransformedCoordianteSystem()
	{
		return transform_coordinate_system;
	}

protected:

	TransformCoordinateSystem transform_coordinate_system;
};

