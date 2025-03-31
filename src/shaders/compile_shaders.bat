@echo off
setlocal enabledelayedexpansion

echo Compiling compute shaders...
for /r %%i in (*.comp) do (
  @REM echo Found file: %%i
  @REM Set the input to the found file
  set input_file=%%i
  @REM Set the output to the bin folder
  set output_file=!input_file:\shaders\=\shaders\bin\!.spv
  glslc "!input_file!" -o "!output_file!"
  echo Compiled compute shader: !input_file! to !output_file!
)

echo Compiling fragment shaders...
for /r %%i in (*.frag) do (
  @REM echo Found file: %%i
  @REM Set the input to the found file
  set input_file=%%i
  @REM Set the output to the bin folder
  set output_file=!input_file:\shaders\=\shaders\bin\!.spv
  glslc "!input_file!" -o "!output_file!"
  echo Compiled fragment shader: !input_file! to !output_file!
)

echo Compiling vertex shaders...
for /r %%i in (*.vert) do (
  @REM echo Found file: %%i
  @REM Set the input to the found file
  set input_file=%%i
  @REM Set the output to the bin folder
  set output_file=!input_file:\shaders\=\shaders\bin\!.spv
  glslc "!input_file!" -o "!output_file!"
  echo Compiled vertex shader: !input_file! to !output_file!
)

@REM pause