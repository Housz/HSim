#pragma once

namespace HSim
{
	struct SimulatorFrame
	{
		SimulatorFrame();

		SimulatorFrame(size_t index_, double timeInterval_);

		double elapsedTime();

		void advance();

		void advance(size_t delta);

		SimulatorFrame &operator++();

		SimulatorFrame operator++(int)
		{
			SimulatorFrame result = *this;
			advance();
			return result;
		}

		// index of this frame.
		size_t index = 0;
		// Time interval between two frames.
		double timeInterval = 1.0 / 600;
	};

	using SimFrame = SimulatorFrame;
	using SFrame = SimulatorFrame;

} // namespace HSim
