#include <HSim/common.h>

namespace HSim
{

	template <typename T>
	class Frame
	{
	public:
		Frame() {}
		~Frame() {}

		Frame(size_t index_, T timeInterval_) : index(index_), timeInterval(timeInterval_) {}

		T elapsedTime()
		{
			return index * timeInterval;
		}

		void advance()
		{
			index++;
		}
		void advance(size_t delta)
		{
			index += delta;
		}

		Frame &operator++()
		{
			advance();
			return *this;
		}

		// Frame operator++(int)
		// {

		// }

	public:
		// index of this frame.
		size_t index = 0;
		// Time interval between two frames.
		T timeInterval = 1.0 / 60;
	};

	template <typename T>
	class Animation
	{
	public:
		Animation(){};
		~Animation(){};

	public:
		void update(Frame<T> frame) = 0;
	};

	template <typename T>
	class PhysicsAnimation : Animation<T>
	{
	public:
		PhysicsAnimation(){};
		~PhysicsAnimation(){};

	public:
		Frame<T> currentFrame;
		// fixed sub time steps
		size_t nmbSubTimeSteps = 1;
		T currentTime = 0.0;
	};

	template <typename T>
	using PhysicsAnimationPtr = std::shared_ptr<PhysicsAnimation<T>>;

} // namespace HSim
