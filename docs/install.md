## dependencies
install [vcpkg](https://vcpkg.io/)

```
vcpkg install tbb:x64-windows
vcpkg install benchmark:x64-windows
```

## install

```
cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg_path>\scripts\buildsystems\vcpkg.cmake

cmake --build .\build\

cmake --build .\build\ --config=Release
```

## run

```

```