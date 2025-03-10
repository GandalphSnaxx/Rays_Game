@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

set GLFW_include=Libraries/glfw/include
set GLM_include=Libraries/glm
set STB_include=Libraries/stb
set FlagField_include=Libraries/FlagField/include
set Validator_include=Libraries/Validator/include

set GLFW_lib=Libraries/glfw/build/src/Debug
set C_STD_lib="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\lib\x64"
set VCRUNTIME_lib="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\lib\x64"

set GLFW_LINK_CMD=/LIBPATH:%GLFW_lib% glfw3.lib
set C_STD_LINK_CMD=/LIBPATH:%C_STD_lib% ucrt.lib
set VULKAN_LINK_CMD=/LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib user32.lib gdi32.lib shell32.lib
set VCRUNTIME_LINK_CMD=/LIBPATH:%VCRUNTIME_lib% vcruntime.lib

set includes=/Isrc /I%VULKAN_SDK%/Include /I%GLFW_include% /I%GLM_include% /I%STB_include% /I%FlagField_include% /I%Validator_include%
set links=/link %VULKAN_LINK_CMD% %GLFW_LINK_CMD% %C_STD_LINK_CMD% %VCRUNTIME_LINK_CMD% /NODEFAULTLIB:MSVCRTD
set defines=/D DEBUG /D WINDOWS_BUILD

set CFLAGS=/std:c++17 /MD /EHsc /Z7 /Fo"src\objs\\" /Fd"src\pdbs\\" /Fe"main.exe"

setlocal enabledelayedexpansion
set cpps="src\hello_triangle.cpp" "Libraries/Validator/src/return_t.cpp"

for /R src/vulkan %%a in (*.cpp) do (
  set cpps=!cpps! "%%a"
)
endlocal & set cpps=%cpps%

echo "Compiling shaders..."

call compile_shaders.bat

echo "Building main..."

echo C flags: %CFLAGS%
echo Includes: %includes%
echo Links: %links%
echo Defines: %defines%
echo Cpp Files: %cpps%

cl %CFLAGS% %includes% %defines% "src/main.cpp" %cpps% %links%