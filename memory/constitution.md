# Project Constitution

## Core Principles

This document establishes the governing principles for the Shape Renderer project. All development decisions must align with these principles.

---

## Article I: Code Quality

### 1.1 Clean Code Standards
- All code must be self-documenting with clear naming conventions
- Functions should have a single responsibility
- Classes should follow the Single Responsibility Principle (SRP)
- Code duplication must be minimized through abstraction

### 1.2 Documentation Requirements
- All public APIs must have Doxygen-style documentation
- Comments must explain "why", not "what"
- Documentation must be kept in sync with implementation
- All comments must be in English

### 1.3 Testing Standards
- All features must have corresponding tests using CTest
- Test coverage should focus on critical paths
- Tests must be deterministic and repeatable
- Test names should clearly describe the expected behavior

---

## Article II: Architecture

### 2.1 Modular Design
- Shape types must be implemented as separate classes
- Configuration management must be isolated from business logic
- The system must support adding new shape types without modifying existing code (Open/Closed Principle)

### 2.2 Dependency Management
- Dependencies must be explicitly declared in CMakeLists.txt
- External libraries must be versioned and pinned
- Circular dependencies are prohibited

### 2.3 Layer Separation
- Business logic (shapes) must be independent of UI (ImGui)
- Configuration loading must be separate from shape rendering
- Data structures must be distinct from their presentation

---

## Article III: User Experience

### 3.1 Interface Consistency
- All UI elements must follow ImGui best practices
- Color pickers must provide immediate visual feedback
- Property editors must validate input in real-time
- Window layout must be intuitive and customizable

### 3.2 Performance Requirements
- UI must maintain 60 FPS during normal operation
- Shape rendering must not block the main thread
- Configuration changes must apply immediately without restart

---

## Article IV: Configuration

### 4.1 File Format
- Configuration must use JSON format
- All configuration keys must be snake_case
- Default values must be provided for all optional settings
- Invalid configuration files must be handled gracefully

### 4.2 Validation
- Configuration values must be validated on load
- Out-of-range values must be clamped or rejected with clear error messages
- Missing required fields must use sensible defaults

---

## Article V: Error Handling

### 5.1 Exception Safety
- All exceptions must be caught at appropriate boundaries
- Error messages must be clear and actionable
- The application must never crash due to invalid user input

### 5.2 Logging
- Errors must be logged to stderr
- Warning conditions should be logged but not halt execution
- Debug information must be available in debug builds

---

## Article VI: Build System

### 6.1 CMake Standards
- Must use modern CMake (target-based approach)
- Compiler warnings must be enabled (-Wall -Wextra)
- Build must support both Debug and Release configurations

### 6.2 Testing Integration
- CTest must be enabled for all test targets
- Tests must run automatically in CI/CD pipelines
- Test failures must prevent merges to main branch

---

## Article VII: Version Control

### 7.1 Commit Messages
- Commits must follow conventional commit format
- Each commit should represent a logical unit of change
- Commit messages must be in English

### 7.2 Branch Strategy
- Feature branches must be prefixed with `feature/`
- Bug fixes must be prefixed with `fix/`
- Documentation changes must be prefixed with `docs/`

---

## Article VIII: Thread Safety

### 8.1 Concurrency Model
- UI operations must occur on the main thread only
- Shared resources must be protected with appropriate synchronization
- Lock contention must be minimized

---

## Article IX: Maintenance

### 9.1 Technical Debt
- TODO comments must include issue references when possible
- Technical debt must be tracked and addressed iteratively
- Refactoring opportunities must be documented

### 9.2 Deprecation Policy
- Deprecated APIs must be marked with [[deprecated]]
- Deprecation notices must include migration guidance
- Removed APIs must have at least one release cycle of deprecation

---

## Enforcement

This constitution is enforced through:
1. Code review processes
2. Automated linting and formatting
3. CI/CD pipeline checks
4. Regular architecture reviews

Any deviation from these principles must be documented and justified in the commit message or pull request description.
