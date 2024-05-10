#pragma once

#include <string>

namespace HSim
{

	struct Color
	{
		Color() {}

		Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

		Color(const Color &color_) : r(color_.r), g(color_.g), b(color_.b) {}

		template <typename U>
		Color(const std::initializer_list<U> &list) { set(list); }

		void set(float r_, float g_, float b_)
		{
			r = r_;
			g = g_;
			b = b_;
		}

		template <typename U>
		void set(const std::initializer_list<U> &list)
		{
			assert(list.size() == 3);

			auto iter = list.begin();

			r = static_cast<float>(*iter);
			g = static_cast<float>(*(++iter));
			b = static_cast<float>(*(++iter));
		}

		std::string serialize()
		{
			// todo
		}

		void deserialize(const std::string &source)
		{
			// todo
		}

	// data
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};

} // namespace HSim
