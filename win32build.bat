@echo off
if /i "%1%"=="debug" set mode=Debug
if /i "%1%"=="release" set mode=Release
if /i "%1%"=="--help" goto help
if /i "%1%"=="/help" goto help
if "%1%"=="" goto error
if "%ONCE_GUARD%"=="ON" goto build
call vcvarsall.bat

:build
echo begin build with mode %mode%
python run_gyp
set ONCE_GUARD=ON
MSBuild os.sln /p:Configuration=%mode% /nologo
goto end

:help
echo win32build.bat [Debug/Release]
exit /b

:error
echo Error. see --help
exit /b

:end