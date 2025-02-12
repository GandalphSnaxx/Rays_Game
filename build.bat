@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

set GLFW_include=Libraries/glfw/include
set GLFW_lib=Libraries/glfw/build/src/Debug
set GLM_include=Libraries/glm
set C_STD_lib="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\lib\x64"
set VCRUNTIME_lib="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\lib\x64"
set VULKAN_LINK_CMD=/LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib user32.lib gdi32.lib shell32.lib

set includes=/Isrc /I%VULKAN_SDK%/Include /I%GLFW_include% /I%GLM_include%
set links=/link %VULKAN_LINK_CMD% /LIBPATH:%GLFW_lib% glfw3.lib /LIBPATH:%C_STD_lib% ucrt.lib /LIBPATH:%VCRUNTIME_lib% vcruntime.lib /NODEFAULTLIB:MSVCRTD
set defines=/D DEBUG /D WINDOWS_BUILD

echo "Compiling shaders..."

compile_shaders.bat

echo "Building main..."

echo Includes: %includes%
echo Links: %links%
echo Defines: %defines%

cl /std:c++17 /MD /EHsc /Z7 /Fe"main" %includes% %defines% src/hello_triangle.cpp %links%