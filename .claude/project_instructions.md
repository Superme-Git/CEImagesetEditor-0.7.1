# Project Instructions & Rules

## 1. Technical Architecture Stack
- **IDE/Compiler**: Visual Studio 2013 (MSVC 12.0)
  - **Toolset**: v120
  - **Feature Set**: C++03 with partial C++11 support. 
    - *Supported*: `auto`, `nullptr`, `shared_ptr`, `unique_ptr`, `static_assert`, range-based for loops.
    - *Unsupported/Partial*: `constexpr`, complete move semantics in all STL containers, user-defined literals, `noexcept` (use `throw()`).
    - *Action*: Avoid modern C++14/17/20 features. Stick to C++11 subset compatible with MSVC 12.0.
- **GUI Framework**: wxWidgets 3.0.5
  - **Configuration**: Unicode, Static or Shared (check `vcvarsall.bat` environment).
  - **Conventions**: Use `wxString` for strings, `wxGLCanvas` for rendering.
- **Rendering Engine**: CEGUI 0.7.x (0.7.9 recommended)
  - **API**: Use 0.7.x API (e.g., `WindowManager::getSingleton()`). Do NOT use 0.8.x+ APIs (major breaking changes).
  - **Resources**: XML-based schemes, imagesets, layouts in `data/` folder.

## 2. Coding Standards
- **File Encoding**: **CRITICAL**: All source files (`.h`, `.cpp`) MUST be saved with **UTF-8 with BOM**.
  - Visual Studio 2013 requires BOM to correctly interpret source files containing non-ASCII characters (Chinese comments).
- **Comments**: 
  - Language: **Chinese (Simplified)**.
  - Style: Doxygen-compatible `/** ... */` for classes/methods, `//` for inline.
- **Naming**:
  - Classes: PascalCase (e.g., `EditorFrame`)
  - Methods: camelCase (e.g., `initialiseCEGUI`)
  - Members: `m_camelCase` or `d_camelCase` (CEGUI style), `m_` prefix (wxWidgets style). Follow file consistency.

## 3. Project Structure Rules
- `inc/`: Header files. Guard macros: `_CLASS_NAME_H_`.
- `src/`: Source files.
- `docs/`: Markdown documentation. Keep `index.md` updated as the entry point.
- `data/`: Runtime resources.

## 4. Workflows
- **Compilation**: Always ensure `vcvarsall.bat` setup logic is considered.
- **Documentation**: Update `docs/` artifacts when changing code logic.
