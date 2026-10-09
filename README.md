# Robot War Game

A C++ battlefield simulation where multiple robot types (e.g., RoboCop, Terminator, UltimateRobot) take turns moving, scanning, firing, and eliminating each other on a grid-based arena.

## Tech Stack

- **Language:** C++  
- **Build Tooling:** g++ (CLI) / Code::Blocks project file included  
- **Core Concepts:** OOP, inheritance, polymorphism, linked lists, queues, file I/O

## Project Structure

```text
Robot-War-Game/
├── archive/
│   └── TC3L_G21_A2_AaronLim_LeeHongYi_VeniceGohL_WongHuiTing.zip
├── data/
│   ├── input/
│   │   ├── fileinput1.txt
│   │   ├── fileinput2.txt
│   │   ├── fileinput3.txt
│   │   └── fileinput4.txt
│   └── output/
│       ├── fileOutput1.txt
│       ├── fileOutput2.txt
│       ├── fileOutput3.txt
│       └── fileOutput4.txt
├── docs/
│   └── TC3L_G21_report.docx
├── include/
│   ├── Classheader.h
│   ├── Destroyedqueue.h
│   ├── Queue.h
│   ├── Reentryqueue.h
│   ├── Robot.h
│   └── RobotLinkedList.h
├── project/
│   ├── Robocopassignment.cbp
│   ├── Robocopassignment.depend
│   └── Robocopassignment.layout
└── src/
    └── main.cpp
```

## Setup and Run

### 1) Build

From the repository root:

```bash
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude src/main.cpp -o build/robot-war-game
```

### 2) Run

Use default paths (`data/input/fileinput1.txt` -> `data/output/fileOutput1.txt`):

```bash
./build/robot-war-game
```

Or provide custom input/output files:

```bash
./build/robot-war-game data/input/fileinput2.txt data/output/fileOutput2.txt
```
