#pragma once
#include <Structs.h>

#include <vector>
#include <functional>
#include <string>
using std::string;

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

	struct TestMenu : public Test
	{
		TestMenu(Test* current_test_ptr);

		void onImGuiRender() override;

		template<typename TestType>
		void add_test(const string& name)
		{
			this->tests.push_back(
				std::make_pair(name, []() {
					return new TestType{  };
				})
			);
		}

	private:
		Test*& current_test;
		std::vector<std::pair<string, std::function<Test* ()>>> tests;
	};

	struct ClearColor : public Test
	{
		ClearColor();
		ClearColor(float color[4]);
		//ClearColor(Vect4<float> color);

		void onRender() override;
		void onImGuiRender() override;
	private:
		// TODO: Vect4<float> color;
		float clear_color[4];
	};

	struct MVP : public Test
	{
		MVP(float res_x, float res_y);

		void onRender() override;
		void onImGuiRender() override;

	private:
		float x_range;
		float y_range;

		float x = 0.0f, y = 0.0f,
		width = 20.0f, height = 20.0f;
		/*colorRGBA<float> first_color{ 0.3f, 0.5f, 1.0f, 1.0f };
		colorRGBA<float> copy_color{ 1.0f, 0.5f, 1.0f, 1.0f };*/

	};
}