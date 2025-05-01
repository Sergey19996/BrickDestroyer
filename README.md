# BrickDestroyer

**BrickDestroyer** is a simple 2D Android game project written in C++ using the Android SDK and NDK. The game is built from scratch and demonstrates the use of several software design patterns and fundamental game development principles.

## 📱 Platform

- **Android**
- Supported architecture: `arm64-v8a`

## 🛠 Technologies Used

- **C++**
- **Android Studio**
- **Android NDK**
- **OpenGL ES** for rendering
- **CMake** for build configuration
- **Java/Kotlin** (minimal use, only for NDK integration)

## 📁 Project Structure

- `BrickDestroyer/app/src/main/cpp/` — core C++ game logic
- `BrickDestroyer/app/src/main/cpp/extern/` — external libraries compiled for `arm64-v8a`

## 🎮 Features

- 2D rendering with OpenGL ES
- User input and UI interaction
- Game state management (active game, menu, win screen)
- C++ game logic integration with the Android platform

## 🧱 Design Patterns Used

- **Singleton** — for global resource and manager access
- **Observer** — to connect UI with gameplay logic
- **State Pattern** — to manage game states (active, menu, win)
- **Command / Callback** — to pass user actions from UI to the game

## 🔧 Build Instructions

1. Make sure Android Studio with NDK support is installed.
2. Open the project in Android Studio.
3. Ensure that the `arm64-v8a` architecture is selected in the build settings.
4. Run the project on a real device or emulator.

## 📌 Notes

- The game is in development; more levels and mechanics may be added.
- Thanks to the C++ core, the project is potentially portable to other platforms.
