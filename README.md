# HSim
A toy framework for learning computer graphics simulation algorithms.


reference: [Fluid Engine Dev - Jet](https://github.com/doyubkim/fluid-engine-dev)


---------------

vcpkg install tbb:x64-windows
vcpkg install benchmark:x64-windows

cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg_path>\scripts\buildsystems\vcpkg.cmake4
cmake --build .\build\