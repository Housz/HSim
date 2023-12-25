#include <HSim/grid_fluid_solver3.h>

using namespace HSim;

template <typename T>
void GridFluidSolver3<T>::setGravity(Vec3<T> g)
{
	_gravity = g;
}