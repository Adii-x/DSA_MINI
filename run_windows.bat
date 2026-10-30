@echo off
cd /d "%~dp0"
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -pedantic -Iheaders src\main_program.cpp src\food_menu_management.cpp src\token_lookup.cpp src\order_processing.cpp -o build\SmartCanteen.exe
if errorlevel 1 (
  echo Build failed. Check that g++ is installed and on PATH.
  pause
  exit /b 1
)
build\SmartCanteen.exe
pause
