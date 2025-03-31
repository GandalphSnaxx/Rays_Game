@echo off

echo Configuring CMake...
@REM cmake -S . -b ./build
cmake --preset vcpkg

echo Compiling shaders...
cd src/shaders
call compile_shaders.bat
cd ../..

echo Building project...
cmake --build build