@echo off
set glslc=C:\VulkanSDK\1.2.148.0\Bin\glslc.exe
set outPathPrefix=Shaders\Compiled\
set spirvExt=.spv

FOR %%i IN (Shaders\*.*) DO %glslc% %%i -o %outPathPrefix%%%~ni%%~xi%spirvExt%

pause