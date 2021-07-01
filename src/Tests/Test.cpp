#include "Test.h"
#include <Renderer.h>
#include <ImGUI-1.60/imgui.h>

namespace test
{
	TestMenu::TestMenu(Test* current_test_ptr)
		:current_test(current_test_ptr)
	{
	}
	void TestMenu::onImGuiRender()
	{
		for (auto& test : this->tests)
			if (ImGui::Button(test.first.c_str()))
				this->current_test = test.second();
	}


	ClearColor::ClearColor()
		: clear_color{ 0.3f, 0.5f, 1.0f, 1.0f }
	{

	}
	ClearColor::ClearColor(float color[4])
		: clear_color{ color[0], color[1], color[2], color[3] }
	{
	}
	void ClearColor::onRender()
	{
		GLCALL(glClearColor(this->clear_color[0], this->clear_color[1], this->clear_color[2], this->clear_color[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}
	void ClearColor::onImGuiRender()
	{
		ImGui::Text("CLEAR COLOR");
		ImGui::ColorEdit4("Clear Color", this->clear_color);
	}

	MVP::MVP(float res_x, float res_y)
		: x_range(res_x - width), y_range(res_y - height)
	{
	}
	void MVP::onRender()
	{
	}
	void MVP::onImGuiRender()
	{
		/*ImGui::Text("Color");
		ImGui::ColorEdit4("RGBA Color", (float*)&first_color);
		ImGui::SliderFloat("Opacity", &first_color.w, 0.0f, 1.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &first_position.x, 0.0f, x_range);
		ImGui::SliderFloat("Y", &first_position.y, 0.0f, y_range);
		ImGui::SliderFloat("Z", &first_position.z, 0.0f, 1.0f);

		ImGui::Text("Copy Position");
		ImGui::SliderFloat("x", &copy_position.x, 0.0f, (float)(Resolution.x - width));
		ImGui::SliderFloat("y", &copy_position.y, 0.0f, (float)(Resolution.y - height));
		ImGui::SliderFloat("z", &copy_position.z, 0.0f, 1.0f);*/
	}
}