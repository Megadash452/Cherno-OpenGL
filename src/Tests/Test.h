#pragma once

namespace test
{
	struct Test
	{
		Test() {}
		virtual ~Test() {}

		virtual void onUpdate(float delta_time) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}
	};
}