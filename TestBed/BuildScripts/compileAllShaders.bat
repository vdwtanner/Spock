@echo off
set glslc=C:\VulkanSDK\1.2.148.0\Bin\glslc.exe
set outPathPrefix=%1\Compiled\
set spirvExt=.spv

FOR %%i IN (%1\*.*) DO (
  echo Compiling shader: %%i
  %glslc% %%i -o %outPathPrefix%%%~ni%%~xi%spirvExt%
)