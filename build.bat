@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

set GLFW_include=Libraries\glfw-3.4.bin.WIN64\include
set GLFW_lib=Libraries\glfw-3.4.bin.WIN64\lib-vc2022
set GLM_include=Libraries\glm

SET includes=/Isrc /I%VULKAN_SDK%/Include /I%GLFW_include% /I%GLM_include%
SET links=/link /LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib user32.lib /LIBPATH:%GLFW_lib% glfw3.lib
SET defines=/D DEBUG /D WINDOWS_BUILD

echo "Building main..."

cl /EHsc /Z7 /Fe"main" %includes% %defines% src/hello_triangle.cpp %links%