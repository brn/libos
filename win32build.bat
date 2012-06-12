@echo off
if "%ONCE_GUARD%"=="ON" goto build
call vcvarsall.bat

:build
@echo on
python run_gyp
@echo off
goto end

:end
set ONCE_GUARD=ON

