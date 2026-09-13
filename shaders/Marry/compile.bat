@echo off
setlocal
set "GLSLANG_VALIDATOR=%~dp0..\..\third_party\VulkanSDK\Bin\glslangValidator.exe"
if not exist "%GLSLANG_VALIDATOR%" (
    echo Error: glslangValidator.exe not found at "%GLSLANG_VALIDATOR%".
    pause
    exit /b 1
)

pushd "%~dp0"
if errorlevel 1 (
    echo Error: failed to enter shader directory "%~dp0".
    pause
    exit /b 1
)

"%GLSLANG_VALIDATOR%" -V -R marryShader.vert -o marryVert.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R marryShader.frag -o marryFrag.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R shadowShader.vert -o shadowVert.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R shadowShader.frag -o shadowFrag.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R LVShader.vert -o LVVert.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R LVShader.frag -o LVFrag.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)

popd
pause
exit /b 0

:compile_failed
echo Error: shader compilation failed with exit code %COMPILE_EXIT_CODE%.
popd
pause
exit /b %COMPILE_EXIT_CODE%
