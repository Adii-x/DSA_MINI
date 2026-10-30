# Smart Canteen Pre-Order and Token System (C++17)

Beginner-friendly DSA mini-project. The program keeps data in memory only; data resets when the program exits. No file handling, database, priority orders, or priority queue.

## Four data structures
1. **Linked List** — custom linked list for food menu management.
2. **Queue** — `queue<int>` processes orders First In, First Out.
3. **Hash Table** — custom open-addressing hash table searches orders by token. No `unordered_map`.
4. **Stack** — `stack<CancellationRecord>` stores cancellation history, newest cancellation first.

The project uses `using namespace std;` and keeps the implementation in separate headers and source files.

## Features
- Display and add food items
- Place orders with multiple items
- Generate a unique token for each order
- Process orders in FIFO order
- Search order by token
- Cancel only pending orders and restore stock
- Display all orders and cancellation history
- Friendly input validation and formatted console output

## Run on Windows / VS Code
Install MinGW-w64 with `g++` on PATH. Open this folder in VS Code and run `run_windows.bat`. Run `run_tests.bat` for basic tests.

Manual compile command:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iheaders src/main_program.cpp src/food_menu_management.cpp src/token_lookup.cpp src/order_processing.cpp -o SmartCanteen
```

No file persistence is included intentionally.
