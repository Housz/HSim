#pragma once

namespace HSim
{
	struct Frame
	{
		Frame();

		Frame(size_t index_, double timeInterval_);

		double elapsedTime();

		void advance();

		void advance(size_t delta);

		Frame &operator++();


		Frame operator++(int)
		{
			Frame result = *this;
			advance();
			return result;
		}

		// index of this frame.
		size_t index = 0;
		// Time interval between two frames.
		double timeInterval = 1.0 / 600;
	};

} // namespace HSim
