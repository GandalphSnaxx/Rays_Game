

set shader_compiler=%VULKAN_SDK%/Bin/glslc.exe
set shader_path=/src/shaders/

%shader_compiler% %shader_path%vertex/shader.vert -o vert.spv
%shader_compiler% %shader_path%fragment/shader.frag -o frag.spv
pause