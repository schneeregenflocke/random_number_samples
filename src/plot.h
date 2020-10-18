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

#include "histogram.h"
#include "transform.h"

#include <boost/math/distributions.hpp>
namespace math = boost::math;



#include <vector>
#include <array>
#include <string>
#include <tuple>


class Plot : public TransformCoordinateSystemInterface
{
public:

	Plot() :
		scrolling(0, 0),
		axes({ -5.0f, 5.0f, -0.25f, 1.0f }),
		grid_gaps({ 1.0f, 0.25f }),
		curve_color(ImColor(.191f, .526f, .805f, 1.f)),
		histogram_color(ImColor(.944f, .791f, .663f, .534f))
	{}

	val4f GetAxes() const
	{
		return this->axes;
	}

	void SetAxes(const val4f& axes)
	{
		this->axes = axes;
	}

	std::array<float, 2> GetGridGaps() const
	{
		return this->grid_gaps;
	}

	void SetGridGaps(const std::array<float, 2>& grid_gaps)
	{
		this->grid_gaps = grid_gaps;
	}


	void SetCanvasRect()
	{
		const glm::vec2 cursor_screen_pos = ImGui::GetCursorScreenPos();
		const glm::vec2 content_region_avail = ImGui::GetContentRegionAvail();
		
		canvas_rect = rect4f(cursor_screen_pos, cursor_screen_pos + content_region_avail);
	}

	void SetPlotRect(const float axis_gap_back, const float axis_gap_front)
	{
		this->axis_gap_back = axis_gap_back;
		this->axis_gap_front = axis_gap_front;

		plot_rect = rect4f(canvas_rect.lt() + glm::vec2(axis_gap_back * 2, axis_gap_back), canvas_rect.rb() + glm::vec2(-axis_gap_back, -axis_gap_back * 2));
	}

	void ProceedInteraction()
	{
		ImGui::InvisibleButton("canvas", canvas_rect.size(true), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		
		is_hovered = ImGui::IsItemHovered();
		is_active = ImGui::IsItemActive();

		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			scrolling.x += ImGui::GetIO().MouseDelta.x;
			scrolling.y += ImGui::GetIO().MouseDelta.y;
		}

		origin = glm::vec2(canvas_rect.l() + scrolling.x, canvas_rect.t() + scrolling.y);
	}

	void ProceedAxes()
	{
		transform_coordinate_system.SetAspectRatio(canvas_rect, plot_rect, axes);
		
		transform_coordinate_system.SetCanvasSizeAndOrigin(origin, scrolling);

		////////////////////////////////////////////////////////////////////////////////

		x_axisline_p0 = glm::vec2(canvas_rect.l() + axis_gap_back * 2, canvas_rect.b() - axis_gap_back);
		x_axisline_p1 = glm::vec2(canvas_rect.r() - axis_gap_back, canvas_rect.b() - axis_gap_back);

		y_axisline_p0 = glm::vec2(canvas_rect.l() + axis_gap_back, canvas_rect.b() - axis_gap_back * 2);
		y_axisline_p1 = glm::vec2(canvas_rect.l() + axis_gap_back, canvas_rect.t() + axis_gap_back);

	}

	void ProceedGrid()
	{
		vertical_grid.clear();
		horizontal_grid.clear();

		x_axis_ticks.clear();
		y_axis_ticks.clear();

		x_axis_labels.clear();
		y_axis_labels.clear();

		
		const float tick_lenght = 10;
		const int decimal_places = 2;

		glm::vec2 grid_step = transform_coordinate_system.ScaleToCanvas(glm::vec2(grid_gaps[0], grid_gaps[1]));
		float magic_x = fmodf(scrolling.x, grid_step.x);

		val4f scrolled_axes = transform_coordinate_system.GetScrolledAxes();

		float x = plot_rect.l() + magic_x;

		if (magic_x < 0)
		{
			x += grid_step.x;
		}

		for (; x <= plot_rect.r(); x += grid_step.x)
		{
			std::array<glm::vec2, 2> current_line = { glm::vec2(x, plot_rect.t()) , glm::vec2(x, plot_rect.b()) };
			vertical_grid.push_back(current_line);

			std::array<glm::vec2, 2> current_tick = { glm::vec2(x, x_axisline_p0.y) , glm::vec2(x, x_axisline_p0.y + tick_lenght) };
			x_axis_ticks.push_back(current_tick);

			float x_value = transform_coordinate_system.ScaleXToPlot(x - plot_rect.l()) + scrolled_axes[0];
			std::stringstream stream;
			stream << std::fixed << std::setprecision(decimal_places) << x_value;
			std::tuple<glm::vec2, std::string> current_label = std::make_tuple(current_tick[1], stream.str());
			x_axis_labels.push_back(current_label);
		}

		float magic_y = fmodf(-scrolling.y, grid_step.y);

		float y = plot_rect.b() - magic_y;

		if (magic_y < 0)
		{
			y -= grid_step.y;
		}

		for (; y >= plot_rect.t(); y -= grid_step.y)
		{
			std::array<glm::vec2, 2> current_line = { glm::vec2(plot_rect.l(), y), glm::vec2(plot_rect.r(), y) };
			horizontal_grid.push_back(current_line);

			std::array<glm::vec2, 2> current_tick = { glm::vec2(y_axisline_p0.x, y), glm::vec2(y_axisline_p0.x - tick_lenght, y) };
			x_axis_ticks.push_back(current_tick);

			float y_value = scrolled_axes[2] - transform_coordinate_system.ScaleYToPlot(y - plot_rect.b());
			std::stringstream stream;
			stream << std::fixed << std::setprecision(decimal_places) << y_value;
			std::tuple<glm::vec2, std::string> current_label = std::make_tuple(current_tick[1], stream.str());
			y_axis_labels.push_back(current_label);
		}
	}

	template<typename Ty>
	void SetPlotCurve(const Ty distribution)
	{
		val4f scrolled_axes = transform_coordinate_system.GetScrolledAxes();

		const size_t steps = 1000;
		const float step_size = scrolled_axes.Lenght(0, 1) / static_cast<float>(steps);

		std::vector<glm::vec2> pdf_curve(steps);

		for (size_t index = 0; index < steps; ++index)
		{
			const float float_index = static_cast<float>(index);

			const float current_x_value = scrolled_axes[0] + float_index * step_size;
			const float current_y_value = math::pdf(distribution, current_x_value);

			pdf_curve[index] = glm::vec2(current_x_value, current_y_value);
		}

		transformed_curve.resize(pdf_curve.size());
		size_t counter = 0;
		for (const auto& curve_point : pdf_curve)
		{
			transformed_curve[counter] = transform_coordinate_system.TransformToCanvas(curve_point);
			++counter;
		}
	}


	void SetHistogram(const histogram::histogram<std::tuple<histogram::axis::regular<>>>& histogram)
	{
		bin_array.clear();

		for (histogram::axis::index_type index = 1; index < histogram.size() - 1; ++index)
		{
			const float bin_lower = histogram.axis(0).bin(index).lower();
			const float bin_upper = histogram.axis(0).bin(index).upper();
			const float histogram_width = bin_upper - bin_lower;
			const float bin_height = histogram[index] / histogram_width;
			
			const glm::vec2 p0 = transform_coordinate_system.TransformToCanvas(glm::vec2(bin_lower, bin_height));
			const glm::vec2 p1 = transform_coordinate_system.TransformToCanvas(glm::vec2(bin_upper, 0.f));

			bin_array.push_back({ p0, p1 });
		}
	}

	void Draw()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		draw_list->AddRectFilled(canvas_rect.lt(), canvas_rect.rb(), ImColor(1.0f, 1.0f, 1.0f, 1.0f));
		draw_list->AddRect(canvas_rect.lt(), canvas_rect.rb(), ImColor(1.0f, 1.0f, 1.0f, 1.0f));
		draw_list->AddRect(plot_rect.lt(), plot_rect.rb(), ImColor(0.0f, 0.0f, 0.0f, 0.5f));


		ImColor axis_color(0.4f, 0.4f, 1.0f, 1.0f);

		ImFont* font = ImGui::GetIO().Fonts->Fonts[0];
		

		const float font_size = 14.f;
		const float tick_gap = 2.f;
		
		for (const auto& label : x_axis_labels)
		{
			auto pos = std::get<0>(label);
			auto text = std::get<1>(label);

			glm::vec2 font_size_vec = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text.c_str());
			pos.x = pos.x - (font_size_vec.x * 0.5f);
			pos.y = pos.y + tick_gap;

			draw_list->AddText(font, font_size, pos, axis_color, text.c_str());
		}

		for (const auto& label : y_axis_labels)
		{
			auto pos = std::get<0>(label);
			auto text = std::get<1>(label);

			glm::vec2 font_size_vec = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text.c_str());
			pos.x = pos.x - (font_size_vec.x + tick_gap);
			pos.y = pos.y - (font_size_vec.y * 0.5f);

			draw_list->AddText(font, font_size, pos, axis_color, text.c_str());
		}

		draw_list->PushClipRect(canvas_rect.lt(), canvas_rect.rb(), false);


		draw_list->AddLine(x_axisline_p0, x_axisline_p1, axis_color, 2.0f);
		draw_list->AddLine(y_axisline_p0, y_axisline_p1, axis_color, 2.0f);


		for (const auto& line : x_axis_ticks)
		{
			draw_list->AddLine(line[0], line[1], axis_color, 2);
		}

		for (const auto& line : y_axis_ticks)
		{
			draw_list->AddLine(line[0], line[1], axis_color, 2);
		}

		draw_list->PopClipRect();

		draw_list->PushClipRect(plot_rect.lt(), plot_rect.rb(), false);

		ImColor grid_lines_color(0.8f, 0.8f, 0.8f, 0.5f);

		for (const auto& line : vertical_grid)
		{
			draw_list->AddLine(line[0], line[1], grid_lines_color);
		}

		for (const auto& line : horizontal_grid)
		{
			draw_list->AddLine(line[0], line[1], grid_lines_color);
		}


		for (const auto& bin : bin_array)
		{
			draw_list->AddRect(bin[0], bin[1], histogram_color);
			draw_list->AddRectFilled(bin[0], bin[1], histogram_color);
		}

		draw_list->AddPolyline(transformed_curve.data(), transformed_curve.size(), curve_color, false, 2.0f);

		
		draw_list->PopClipRect();
	}

	ImColor curve_color;
	ImColor histogram_color;

private:

	rect4f canvas_rect;
	rect4f plot_rect;

	float axis_gap_back;
	float axis_gap_front;


	bool is_hovered;
	bool is_active;

	glm::vec2 scrolling;
	glm::vec2 origin;

	val4f axes;
	std::array<float, 2> grid_gaps;

	glm::vec2 x_axisline_p0;
	glm::vec2 x_axisline_p1;

	glm::vec2 y_axisline_p0;
	glm::vec2 y_axisline_p1;

	glm::vec2 cs_origin_x_axis;
	glm::vec2 cs_origin_y_axis;

	std::vector<std::array<glm::vec2, 2>> vertical_grid;
	std::vector<std::array<glm::vec2, 2>> horizontal_grid;

	std::vector<std::array<glm::vec2, 2>> x_axis_ticks;
	std::vector<std::array<glm::vec2, 2>> y_axis_ticks;

	std::vector<std::tuple<glm::vec2, std::string>> x_axis_labels;
	std::vector<std::tuple<glm::vec2, std::string>> y_axis_labels;

	std::vector<ImVec2> transformed_curve;
	std::vector<std::array<glm::vec2, 2>> bin_array;
};
