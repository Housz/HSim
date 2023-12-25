#include <HSim/grid_fluid_solver3.h>

using namespace HSim;

template <typename T>
void GridFluidSolver3<T>::setGravity(Vec3<T> g)
{
	_gravity = g;
}

template<typename T>
Vec3<T>& HSim::GridFluidSolver3<T>::getGravity()
{
	return _gravity;
}