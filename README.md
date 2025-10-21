# 🎮 SFML Shape Bouncer  
**Assignment 1 – Game Programming (Memorial University of Newfoundland)**  

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white) ![SFML](https://img.shields.io/badge/SFML-FF6600?style=flat&logo=sfml&logoColor=white) ![ImGui](https://img.shields.io/badge/ImGui-3796EF?style=flat&logo=github&logoColor=white)

A C++/SFML application that reads shape descriptions from a configuration file, renders them on screen, and makes them **bounce off the window edges**. Includes a real-time **ImGui editor** for interactive shape manipulation.

---

## 📌 Description

This program:
- Loads **rectangles** and **circles** from a `config.txt` file;
- Draws each shape with its specified color, position, and velocity;
- Implements **boundary bouncing**: shapes reverse direction when hitting window edges;
- Displays each shape’s **name centered** on the shape using the specified font and color;
- Provides an **interactive ImGui interface** to:
  - List and select any shape;
  - Toggle visibility;
  - Adjust scale (0–4);
  - Modify X/Y velocity (–8 to 8);
  - Change color and name.

The application is designed to work with **any valid configuration file**, not just the provided example.

---

## 🛠️ Dependencies

- **C++20** or higher
- **SFML 3.0.2** (Simple and Fast Multimedia Library)
- **Dear ImGui v1.91.2** (integrated with SFML)
- **imgui-sfml v3.0** (to integrate SFML and ImGui)
- A compatible compiler (`g++`, `clang++`, etc.)
- **CMAKE 3.28.3**

---

## ▶️ Building and Running with CMake

This project uses **CMake** for easy and portable builds.

### Build steps

```bash
# Create a build directory
mkdir build && cd build

# Configure the project with CMake
cmake ..

# Compile the executable
cmake --build .

# Run the program (from the build directory)
./shape-bouncer
```

💡 Note: The application expects a config.txt file in the current working directory when you run it.  

## 🎮 Controls
Use your mouse to interact with the ImGui panel (on the right side of the window).
Close the window normally to exit.

## 📄 Configuration File Format (`config.txt`)

### Example:

```txt
Window 1024 768
Font assets/arial.ttf 16 255 255 255
Rectangle Player 100 100 2.0 1.5 0 255 0 80 40
Circle Enemy 500 300 -1.0 2.0 255 0 0 30
```

## 🧪 Recommended Tests
Add/remove shapes in config.txt;  
Test extreme velocities (e.g., ±8);  
Verify bouncing behavior near window edges with large shapes;  
Edit properties in real time via ImGui;  
Confirm text remains centered after scaling.  

## 🎓 Course Information
Course: Game Programming  
Institution: Memorial University of Newfoundland  
Instructor: [Dr. David Churchill](https://www.cs.mun.ca/~dchurchill/)  
Assignment: 1  

## 📚 Learning Resources
This assignment builds upon concepts taught in the official course lecture series:  
[Game Programming – Memorial University (YouTube Playlist)](https://www.youtube.com/watch?v=N4uMNj_hxvU&list=PL_xRyXins84_Sq7yZkxGP_MgYAH-Zo8Uu&index=1)
