vcpkg install benchmark:x64-windows

vcpkg install tbb:x64-windows

cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg_path>\scripts\buildsystems\vcpkg.cmake

cmake --build .\build\


google benchmark

.\build\tests\Debug\benchmark.exe --benchmark_time_unit={ns|us|ms|s}
