@echo off
cd /d "%~dp0"
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -pedantic -Iheaders tests\testing.cpp src\food_menu_management.cpp src\token_lookup.cpp -o build\SmartCanteenTests.exe
if errorlevel 1 (
  echo Test build failed.
  pause
  exit /b 1
)
build\SmartCanteenTests.exe
pause
