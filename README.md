# CEImagesetEditor

![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)
![License](https://img.shields.io/badge/license-GPL-blue)
![CEGUI](https://img.shields.io/badge/dependency-CEGUI%200.5%2B-orange)
![wxWidgets](https://img.shields.io/badge/dependency-wxWidgets%202.6%2B-red)

**CEImagesetEditor** is a professional graphical tool designed for creating and editing [CEGUI](http://www.cegui.org.uk) Imageset files. Built with the robust **wxWidgets** framework, it offers a visual environment to manage image regions and properties, streamlining the workflow for GUI development in games and applications.

---

## 🚀 Key Features

*   **Visual Editing**: Intuitive interface to define and modify image regions on a texture.
*   **Real-time Preview**: Integrated CEGUI rendering to verify how imagesets look.
*   **Cross-Platform**: Compatible with Windows, Linux, and macOS.
*   **Format Support**: Fully supports standard CEGUI Imageset XML format.
*   **Auto-Scaling**: Tools to handle resolution scaling and native resolution settings.

## � Documentation

Comprehensive documentation is available in the `docs/` directory:

*   [**Project Overview**](docs/01_项目概览_Project_Overview.md): Architecture and technology stack.
*   [**Core Modules**](docs/02_核心核心模块_Core_Modules.md): Details on `EditorDocument` and application entry points.
*   [**Data Handling**](docs/03_数据处理_Data_Handling.md): How XML parsing and generation works.
*   [**UI Architecture**](docs/04_用户界面架构_UI_Architecture.md): Breakdown of the Document-View model and UI components.
*   [**API Reference**](docs/05_API_Reference.md): Auto-generated class and method reference.

> **Note**: You can regenerate the API documentation by running `update_docs.bat` in the project root.

## 🛠️ Installation & Build

### Prerequisites
*   **CEGUI**: Version 0.5.0 or later.
*   **wxWidgets**: Version 2.6.0 or later.
*   **C++ Compiler**: GCC (Linux/Mac) or Visual Studio (Windows).

### Linux / macOS (Autotools)

1.  Configure the build environment:
    ```bash
    ./configure
    ```
2.  Compile the source:
    ```bash
    make
    ```
3.  Install (optional):
    ```bash
    sudo make install
    ```

### Windows (Visual Studio)

Project files are provided for various Visual Studio versions:
*   **VS .NET 2003 (7.1)**: `vc++7.1/`
*   **VS 2005 (8.0)**: `vc++8/`
*   **VS 2008 (9.0)**: `vc++9/`

Open the appropriate solution file (`.sln`) and build the **Release** or **Debug** configuration.

## 📝 Usage

1.  Launch `CEImagesetEditor`.
2.  **Open** an existing `.imageset` file or **Create** a new one.
3.  Load the source image file (Texture).
4.  Use the mouse to draw/resize regions on the canvas.
5.  Set properties (Naming, Offsets) in the properties panel.
6.  **Save** your work.

## 🤝 Contributing

Contributions are welcome!
1.  Fork the repository.
2.  Create your feature branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

## � Authors

**Core Team:**
*   **Paul D Turner (CrazyEddie)** - Project co-ordination, Mac/Linux support.
*   **Frederico Jernimo (fjeronimo)** - Code lead, Win32 support.
*   **Tomas Lindquist Olsen (lindquist)** - Testing & premake support.

**Original Author:**
*   **Martin Fleurent (martignasse)**

For a full list of contributors, please refer to the [AUTHORS](AUTHORS) file.

## 📄 License

This project is licensed under the **GNU General Public License (GPL)**. See the [COPYING](COPYING) file for details.
