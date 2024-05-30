#include <app/app.h>

const size_t N = 32;
HSim::Size3 SIZE(N, N, N);

HSim::LinearSystem3 sys;

int main()
{
	sys.resize(SIZE);
	
	return 0;
}