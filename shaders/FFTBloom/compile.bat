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

"%GLSLANG_VALIDATOR%" -V -R renderBox.vert -o renderBoxVert.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R renderBox.frag -o renderBoxFrag.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R skybox.vert -o skyboxVert.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R skybox.frag -o skyboxFrag.spv
if errorlevel 1 (
    set "COMPILE_EXIT_CODE=%ERRORLEVEL%"
    goto :compile_failed
)
"%GLSLANG_VALIDATOR%" -V -R bloom.comp -o bloomComp.spv
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
