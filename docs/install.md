## dependencies
install [vcpkg](https://vcpkg.io/)

```
vcpkg install tbb:x64-windows
vcpkg install benchmark:x64-windows
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows
vcpkg install stb:x64-windows
vcpkg install tinyobjloader:x64-windows
vcpkg.exe install imgui[core,glfw-binding,opengl3-binding]:x64-windows  --recurse
```

## install

```
cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg_path>\scripts\buildsystems\vcpkg.cmake

cmake --build .\build\

cmake --build .\build\ --config=Release
```

## run

```
cd .\build\out\bin\Debug\ 
.\xxxx.exe
cd .\build\out\bin\Release\    
.\xxxx.exe
```