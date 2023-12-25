#include <HSim/face_center_grid3.h>


int main()
{
	size_t n = 100;
	HSim::FaceCenterGrid3<float> fcg(n, n, n);

	std::cout << fcg.dataSizeU();
	std::cout << fcg.dataSizeV();
	std::cout << fcg.dataSizeW();

	std::cout << fcg.dataOriginU();
	std::cout << fcg.dataOriginV();
	std::cout << fcg.dataOriginW();
	
	std::cout << fcg.u(100, 99, 99) << std::endl;

	std::cout << fcg.dataAtCellCenter(99, 99, 99);
	std::cout << fcg.divergenceAtCellCenter(99, 99, 99);

	return 0;
}