# Global Time Zone Application

## Overview

This is a C++ console-based application that allows users to:

- View current time in different time zones.
- Convert time between different time zones.
- Toggle Daylight Saving Time (DST) handling.
- Add and manage hotspot cities (Admin only).
- Authenticate as an admin to access additional features.

The project utilizes the ICU (International Components for Unicode) library along with C++ standard time functions and CSV file streams to store hotspot cities.

---

## Features

- Supports over **600** time zones globally, powered by the ICU library.
- Handles DST (Daylight Saving Time) adjustments.
- Admin authentication to manage hotspot cities.
- Uses ICU library for accurate timezone conversions.
- Stores hotspot cities using CSV file streams for persistence.

---

## Installation Guide

### 1. Install vcpkg (if not already installed)

```sh
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
```

### 2. Install ICU library using vcpkg

```sh
vcpkg install icu:x64-windows
```

(For 32-bit systems, use `icu:x86-windows` instead.)

### 3. Integrate vcpkg with Visual Studio (Optional)

```sh
vcpkg integrate install
```

---

## How to Compile and Run

### Using g++ (MinGW-w64)

```sh
g++ -o timezone_app main.cpp TimeZoneManager.cpp UserManager.cpp -std=c++17 -I"C:\vcpkg\installed\x64-windows\include" -L"C:\vcpkg\installed\x64-windows\lib" -licuuc -licuin -licudt
./timezone_app
```

### Using Visual Studio

1. Open Visual Studio and create a new C++ Console Application.
2. Add `main.cpp`, `TimeZoneManager.cpp`, and `UserManager.cpp` to the project.
3. Configure `vcpkg` by setting the include and library directories in project properties.
4. Build and run the project.

---

## Usage Instructions

1. Run the program.
2. Select options from the menu to:
   - View current time in different time zones.
   - Convert time between time zones.
   - Toggle DST handling.
   - Login as an admin to manage hotspot cities.
3. Follow the on-screen prompts to navigate the application.

---

## Author

Developed by **Veteran (Somling Vijay Hajji)**

---

## Contributions

Contributions are welcome! Feel free to fork the repository and submit pull requests.

---

