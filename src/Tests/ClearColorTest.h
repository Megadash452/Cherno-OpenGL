#pragma once
#include "Test.h"
#include <Structs.h>

namespace test
{
	struct ClearColor : public Test
	{
		ClearColor();
		ClearColor(float color[4]);
		//ClearColor(Vect4<float> color);
		~ClearColor() override;

		void onUpdate(float delta_time) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		// TODO: Vect4<float> color;
		float clear_color[4];
	};
}