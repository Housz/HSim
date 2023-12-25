#include <HSim/animation.h>
#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class GridFluidSolver3 : public Animation<T>
	{

	public:
		GridFluidSolver3() {}
		~GridFluidSolver3() {}

	// setter, getter
	public:
		void setGravity(Vec3<T>);

	public:
		Vec3<T> _gravity;

	};

} // namespace HSim
