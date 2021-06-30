#include "ClearColorTest.h"
#include <Renderer.h>
#include <ImGUI-1.60/imgui.h>

namespace test
{
	ClearColor::ClearColor()
		: clear_color{ 0.3f, 0.5f, 1.0f, 1.0f }
	{
		
	}
	ClearColor::ClearColor(float color[4])
		: clear_color{ color[0], color[1], color[2], color[3] }
	{

	}
	ClearColor::~ClearColor()
	{

	}

	void ClearColor::onUpdate(float delta_time)
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
}