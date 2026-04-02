# Implementation Plan: Shape Renderer

**Feature:** 001-shape-renderer  
**Status:** Implemented (Retroactive Documentation)  
**Constitution Compliance:** Verified

---

## Executive Summary

This document describes the technical implementation of the Shape Renderer application, retroactively documenting the actual implementation while ensuring alignment with the project constitution and feature specification.

---

## Architecture Overview

### System Layers

```
┌─────────────────────────────────────┐
│         Presentation Layer          │
│    (main.cpp - ImGui + OpenGL)      │
├─────────────────────────────────────┤
│       Configuration Layer           │
│    (ConfigManager, ShapeLoader)     │
├─────────────────────────────────────┤
│        Business Logic Layer         │
│    (Shape, CircleShape, Rectangle)  │
└─────────────────────────────────────┘
```

### Component Diagram

```
main.cpp
├── ConfigManager (singleton)
│   ├── WindowConfig
│   ├── FontConfig
│   └── std::vector<Shape>
├── ShapeLoader (static utility)
│   └── Creates Shape instances from JSON
└── Shape Hierarchy
    ├── Shape (abstract base)
    ├── CircleShape
    └── RectangleShape
```

---

## Technical Decisions

### TD-001: Graphics Stack Selection
**Decision:** SDL2 + OpenGL 3.3 + Dear ImGui  
**Rationale:**
- SDL2 provides cross-platform window management and input
- OpenGL 3.3 offers hardware-accelerated rendering
- Dear ImGui integrates seamlessly with OpenGL
- All libraries are well-maintained and widely adopted

**Constitution Alignment:** Article II (Architecture), Article VI (Build System)

### TD-002: Shape Polymorphism
**Decision:** Abstract base class with virtual methods  
**Rationale:**
- Enables Open/Closed Principle for adding new shapes
- Clean separation between shape types
- Runtime polymorphism allows heterogeneous collections

**Constitution Alignment:** Article II (Modular Design)

### TD-003: Memory Management
**Decision:** std::unique_ptr for shape ownership  
**Rationale:**
- Automatic memory deallocation prevents leaks
- Clear ownership semantics
- No overhead compared to raw pointers

**Constitution Alignment:** Article I (Code Quality)

### TD-004: Configuration Storage
**Decision:** JSON with nlohmann/json library  
**Rationale:**
- Human-readable format
- Excellent C++ library support
- Easy to extend schema
- Handles nested structures naturally

**Constitution Alignment:** Article IV (Configuration)

### TD-005: Testing Framework
**Decision:** doctest with CTest integration  
**Rationale:**
- Header-only, minimal setup
- Compatible with CMake's CTest
- Fast compilation
- Similar API to Catch2

**Constitution Alignment:** Article I (Testing Standards), Article VI (Testing Integration)

---

## Data Model Implementation

### ShapeProperties Structure
```cpp
struct ShapeProperties {
    ImVec2 position;      // Screen coordinates
    float radius;         // Circle-specific
    float width, height;  // Rectangle-specific
    ImColor color;        // RGBA color
};
```

**Storage Strategy:** Properties embedded in concrete shape classes

### Configuration Schema v1.0
```json
{
  "window": {
    "title": "string",
    "width": "integer",
    "height": "integer"
  },
  "font": {
    "path": "string",
    "size": "float"
  },
  "shapes": [
    {
      "type": "circle|rectangle",
      "position": { "x": "float", "y": "float" },
      "radius": "float (optional)",
      "width": "float (optional)",
      "height": "float (optional)",
      "color": { "r": "float", "g": "float", "b": "float", "a": "float" }
    }
  ]
}
```

**Validation Rules:**
- Window dimensions must be > 0
- Color components must be in [0.0, 1.0]
- Shape type must be "circle" or "rectangle"
- Circles require radius; rectangles require width and height

---

## API Specifications

### Shape Interface

| Method | Signature | Description |
|--------|-----------|-------------|
| Destructor | `virtual ~Shape() = default` | Virtual destructor for proper cleanup |
| Render | `virtual void Render() const = 0` | Draw shape using ImGui |
| LoadFromJson | `virtual void LoadFromJson(const json&) = 0` | Populate from JSON |
| SaveToJson | `virtual json SaveToJson() const = 0` | Serialize to JSON |
| GetType | `virtual std::string GetType() const = 0` | Return shape type name |
| GetPosition | `virtual ImVec2 GetPosition() const = 0` | Get screen position |
| SetPosition | `virtual void SetPosition(const ImVec2&) = 0` | Set screen position |
| GetColor | `virtual ImColor GetColor() const = 0` | Get shape color |
| SetColor | `virtual void SetColor(const ImColor&) = 0` | Set shape color |

### ConfigManager Interface

| Method | Signature | Description |
|--------|-----------|-------------|
| GetInstance | `static ConfigManager& GetInstance()` | Singleton accessor |
| LoadConfig | `bool LoadConfig(const std::string& path)` | Load from file |
| SaveConfig | `bool SaveConfig(const std::string& path) const` | Save to file |
| GetWindowConfig | `const WindowConfig& GetWindowConfig() const` | Get window settings |
| GetFontConfig | `const FontConfig& GetFontConfig() const` | Get font settings |
| GetShapes | `const std::vector<std::unique_ptr<Shape>>& GetShapes() const` | Get all shapes |

### ShapeLoader Interface

| Method | Signature | Description |
|--------|-----------|-------------|
| CreateShape | `static std::unique_ptr<Shape> CreateShape(const std::string& type)` | Factory method |
| LoadShapesFromJson | `static std::vector<std::unique_ptr<Shape>> LoadShapesFromJson(const json&)` | Batch load |

---

## File Structure

```
/workspace
├── src/
│   ├── main.cpp              # Application entry point, UI rendering
│   ├── shape.hpp             # Shape base class definition
│   ├── shape.cpp             # Shape base class implementation
│   ├── circle_shape.hpp      # CircleShape definition
│   ├── circle_shape.cpp      # CircleShape implementation
│   ├── rectangle_shape.hpp   # RectangleShape definition
│   ├── rectangle_shape.cpp   # RectangleShape implementation
│   ├── config_manager.hpp    # ConfigManager definition
│   ├── config_manager.cpp    # ConfigManager implementation
│   ├── shape_loader.hpp      # ShapeLoader definition
│   └── shape_loader.cpp      # ShapeLoader implementation
├── test/
│   ├── CMakeLists.txt        # Test configuration
│   └── test_shapes.cpp       # Unit tests
├── CMakeLists.txt            # Main build configuration
├── memory/
│   └── constitution.md       # Project constitution
└── specs/
    └── 001-shape-renderer/
        ├── spec.md           # Feature specification
        └── plan.md           # This file
```

---

## Implementation Phases

### Phase 1: Core Infrastructure ✅
**Completed:** Base shape hierarchy and build system

**Deliverables:**
- [x] CMakeLists.txt with dependency management
- [x] Shape abstract base class
- [x] CircleShape implementation
- [x] RectangleShape implementation
- [x] Basic unit tests

**Constitution Gate:** Article I (Code Quality), Article VI (Build System)

### Phase 2: Configuration System ✅
**Completed:** Configuration loading and saving

**Deliverables:**
- [x] ConfigManager singleton
- [x] WindowConfig and FontConfig structures
- [x] JSON serialization/deserialization
- [x] Default configuration handling
- [x] Error handling for invalid files

**Constitution Gate:** Article IV (Configuration), Article V (Error Handling)

### Phase 3: User Interface ✅
**Completed:** ImGui-based interactive UI

**Deliverables:**
- [x] SDL2 window initialization
- [x] OpenGL context setup
- [x] ImGui integration
- [x] Shape property editors
- [x] Color picker widgets
- [x] Real-time preview

**Constitution Gate:** Article III (User Experience)

### Phase 4: Integration & Polish ✅
**Completed:** Full application integration

**Deliverables:**
- [x] Command-line argument parsing
- [x] Configuration persistence on exit
- [x] Documentation (Doxygen-style)
- [x] CTest integration
- [x] Cross-platform build verification

**Constitution Gate:** All articles verified

---

## Quality Gates

### Simplicity Gate (Article VII)
- ✅ Using single project structure
- ✅ No future-proofing or speculative features
- ✅ Each class has single responsibility

### Anti-Abstraction Gate (Article VIII)
- ✅ Using framework directly (SDL2, ImGui)
- ✅ Single model representation per shape
- ✅ No unnecessary indirection layers

### Testing Gate (Article I)
- ✅ All shape operations tested
- ✅ CTest enabled and configured
- ✅ Tests run in CI/CD pipeline

### Documentation Gate (Article I)
- ✅ All public APIs documented
- ✅ Comments explain "why", not "what"
- ✅ Documentation in English

---

## Complexity Tracking

Any deviations from simplicity principles must be documented here:

### Tracked Complexities
1. **Singleton Pattern in ConfigManager**
   - **Justification:** Global access required for configuration throughout application
   - **Alternative Considered:** Dependency injection rejected due to complexity
   - **Mitigation:** Thread-safe implementation with controlled access

2. **Polymorphic Shape Hierarchy**
   - **Justification:** Required for Open/Closed Principle
   - **Alternative Considered:** std::variant rejected for less extensibility
   - **Mitigation:** Clear interface documentation

---

## Dependencies

### Runtime Dependencies
| Library | Version | Purpose | License |
|---------|---------|---------|---------|
| SDL2 | 2.28+ | Windowing, Input | zlib |
| OpenGL | 3.3+ | Rendering | SGI B-2.0 |
| Dear ImGui | 1.90+ | UI Framework | MIT |

### Build-Time Dependencies
| Library | Version | Purpose | License |
|---------|---------|---------|---------|
| CMake | 3.16+ | Build System | BSD-3 |
| nlohmann/json | 3.11+ | JSON Parsing | MIT |
| doctest | 2.4+ | Testing | MIT |

### Installation (vcpkg)
```bash
vcpkg install sdl2 imgui[opengl-binding,sdl2-binding] nlohmann-json doctest
```

---

## Test Strategy

### Unit Tests (test_shapes.cpp)
- **TC-001:** Circle creation and property access
- **TC-002:** Rectangle creation and property access
- **TC-003:** Shape serialization to JSON
- **TC-004:** Shape deserialization from JSON
- **TC-005:** Polymorphic behavior verification

### Integration Tests
- **TC-101:** Configuration load/save cycle
- **TC-102:** Multiple shapes persistence
- **TC-103:** Invalid configuration handling

### Manual Tests
- **TC-201:** UI responsiveness at 60 FPS
- **TC-202:** Color picker real-time updates
- **TC-203:** Drag-and-drop shape positioning

---

## Performance Considerations

### Rendering Performance
- Target: 60 FPS (16.67ms per frame)
- ImGui batching minimizes draw calls
- No dynamic allocations in render loop

### Memory Usage
- Shapes stored in contiguous vector
- Smart pointers prevent leaks
- Configuration loaded once at startup

### Startup Time
- Target: < 100ms for configuration load
- JSON parsing optimized with nlohmann/json
- Font loading occurs during initialization

---

## Security Considerations

### Input Validation
- All user input validated before use
- File paths sanitized to prevent directory traversal
- Color values clamped to valid ranges

### Error Handling
- Exceptions caught at application boundaries
- Graceful degradation on configuration errors
- No sensitive data in error messages

---

## Maintenance Notes

### Extension Points
1. **New Shape Types:** Inherit from Shape, implement virtual methods
2. **Custom Properties:** Extend ShapeProperties structure
3. **Export Formats:** Add new serializer in ShapeLoader

### Known Limitations
- Only circles and rectangles supported (by design)
- No shape grouping or hierarchies
- Single window only

### Future Enhancements (Out of Scope)
- Triangle and polygon shapes
- Shape transformations (rotation, scaling)
- Layer management
- Export to image formats

---

## Success Criteria Verification

| Criterion | Status | Evidence |
|-----------|--------|----------|
| Builds without warnings | ✅ | CMake build logs |
| All tests pass | ✅ | CTest output |
| 60 FPS rendering | ✅ | ImGui frame timing |
| Configuration persistence | ✅ | Manual testing |
| Full documentation | ✅ | Doxygen generation |
| Constitution compliance | ✅ | This document |

---

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2024 | Development Team | Retroactive implementation plan |

---

## Appendices

### Appendix A: Example Configuration File
```json
{
  "window": {
    "title": "Shape Renderer",
    "width": 1280,
    "height": 720
  },
  "font": {
    "path": "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "size": 16.0
  },
  "shapes": [
    {
      "type": "circle",
      "position": { "x": 200, "y": 150 },
      "radius": 50,
      "color": { "r": 1.0, "g": 0.0, "b": 0.0, "a": 1.0 }
    },
    {
      "type": "rectangle",
      "position": { "x": 400, "y": 200 },
      "width": 100,
      "height": 80,
      "color": { "r": 0.0, "g": 1.0, "b": 0.0, "a": 1.0 }
    }
  ]
}
```

### Appendix B: Build Commands
```bash
# Configure
mkdir build && cd build
cmake ..

# Build
cmake --build .

# Run Tests
ctest --verbose

# Run Application
./shape_renderer [--config path/to/config.json]
```

### Appendix C: Compiler Flags
```cmake
-Wall -Wextra -Wpedantic -std=c++17
Debug: -g -O0 -DDEBUG
Release: -O3 -DNDEBUG
```
