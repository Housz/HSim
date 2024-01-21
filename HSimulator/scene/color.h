#pragma once 

#include <string>

namespace HSim
{
	
	struct Color
	{
		Color() {}
		~Color() {}

		Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}
		
		std::string serialize()
		{
			// todo
		}

		void deserialize(const std::string& source)
		{
			// todo
		}


		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};


	

} // namespace HSim
