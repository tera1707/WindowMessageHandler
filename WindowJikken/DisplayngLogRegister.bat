@echo off
cd %~dp0
xcopy .\DisplayingLogStart.bat "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
pause

