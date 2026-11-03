Contact Management System
A lightweight, console-based Contact Management System written in C++ (C++17) featuring persistent text file storage, robust data validation, and case-insensitive search capabilities.
---
Features
Add Contact: Validates inputs, formats phone numbers (7–15 digits), and prevents duplicate entries.
View All: Displays formatted contact cards along with a total count.
Search: Case-insensitive substring search across names and phone numbers.
Update: Edit existing details with validation and state rollback on file save failure.
Delete: Safe removal with confirmation prompts.
Sort: Alphabetical ordering by name using standard library algorithms.
File Persistence: Automatic read/write to `contacts.txt` using pipe-delimited format (`|`).
---
Project Structure
```text
.
├── ContactManagementSystem.cpp   # Main application source code
├── contacts.txt                  # Auto-generated database storage file
└── README.md                     # Project documentation
```
---
Prerequisites
C++ Compiler: GCC / MinGW, Clang, or MSVC supporting C++17 or higher.
Ensure `#include <sstream>` is included in the source file for string stream parsing.
---
Build & Run
Windows (PowerShell / Command Prompt)
Compile the program:
```cmd
   g++ -std=c++17 ContactManagementSystem.cpp -o contacts.exe
   ```
Run the application:
```cmd
   .\contacts.exe
   ```
Linux / macOS (Terminal)
Compile the program:
```bash
   g++ -std=c++17 ContactManagementSystem.cpp -o contacts
   ```
Run the application:
```bash
   ./contacts
   ```
---
Data Format
Saved contacts are stored in `contacts.txt` line-by-line:
```text
Name|Phone|Email|Address
```
Note: The delimiter `|` and newline characters are restricted in user input to prevent data corruption.
