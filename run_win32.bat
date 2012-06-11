@echo off
if not defined once_guard goto set_vars
:set_vars
call vcvarsall.bat

@echo on
python run_gyp

@echo off
set /A ONCE_GUARD=1

