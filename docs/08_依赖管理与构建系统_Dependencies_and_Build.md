# 依赖管理与构建系统 (Dependencies and Build System)

## 外部依赖 (External Dependencies)

### CEGUI (Crazy Eddie's GUI System)

**版本要求**: 0.5.0 或更高

**使用的模块**:
- `CEGUI::System`: 核心系统单例
- `CEGUI::OpenGLRenderer`: OpenGL 渲染器后端
- `CEGUI::ImagesetManager`: Imageset 资源管理
- `CEGUI::XMLParser`: XML 解析器接口
- `CEGUI::DefaultResourceProvider`: 资源路径管理
- `CEGUI::Window`: 窗口部件基类（ElasticBox 继承自它）
- `CEGUI::FrameWindow`: 帧窗口（提供 SizingLocation 枚举）

**关键头文件**:
```cpp
#include <CEGUI.h>
#include <CEGUISystem.h>
#include <CEGUIImageset.h>
#include <CEGUIXMLParser.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
```

**链接库**:
- `CEGUIBase-0.lib` (或 `.a` 在 Linux 上)
- `CEGUIOpenGLRenderer-0.lib`

---

### wxWidgets

**版本要求**: 2.6.0 或更高

**使用的模块**:
- **Core**: `wxApp`, `wxFrame`, `wxPanel`, `wxSplitterWindow`
- **OpenGL**: `wxGLCanvas` (需要启用 `--with-opengl` 编译 wxWidgets)
- **Doc/View**: `wxDocument`, `wxView`, `wxDocManager`, `wxDocTemplate`
- **Controls**: `wxGrid`, `wxTextCtrl`, `wxComboBox`, `wxCheckBox`, `wxButton`
- **Events**: `wxCommandEvent`, `wxMouseEvent`, `wxKeyEvent`, `wxPaintEvent`
- **Utilities**: `wxLogError`, `wxBusyCursor`, `wxWindowDisabler`, `wxColourDialog`

**关键头文件**:
```cpp
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/docview.h>
#include <wx/grid.h>
#include <wx/splitter.h>
```

**链接库 (Windows)**:
- Debug: `wxmsw26d_gl.lib`, `wxmsw26d_core.lib`, `wxbase26d.lib`
- Release: `wxmsw26_gl.lib`, `wxmsw26_core.lib`, `wxbase26.lib`

**链接库 (Linux)**:
```bash
wx-config --libs --gl-libs
```

---

### OpenGL

**版本**: OpenGL 1.1+ (基本渲染功能)

**使用的函数**:
- `glClear()`: 清空缓冲区
- `glViewport()`: 设置视口
- `glMatrixMode()`, `glLoadIdentity()`: 矩阵操作（通过 CEGUI 间接使用）

**链接库**:
- Windows: `opengl32.lib`, `glu32.lib`
- Linux: `-lGL`, `-lGLU`

---

### C++ 标准库

**使用的组件**:
- `<fstream>`: 文件流操作（保存 Imageset）
- `<map>`: `std::map` 用于 Region 映射
- `<string>`: CEGUI::String

---

## 构建系统 (Build Systems)

### 1. Autotools (Linux/macOS)

#### 核心文件

**configure.ac**
```autoconf
AC_INIT([CEImagesetEditor], [0.7.1])
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_PROG_LIBTOOL

# 检查 CEGUI
PKG_CHECK_MODULES([CEGUI], [CEGUI-0 >= 0.5.0])

# 检查 wxWidgets
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG([2.6.0], [wxWin=1])

if test "$wxWin" != 1; then
    AC_MSG_ERROR([wxWidgets not found!])
fi

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
```

**Makefile.am** (根目录)
```makefile
SUBDIRS = src
```

**src/Makefile.am**
```makefile
bin_PROGRAMS = CEImagesetEditor

CEImagesetEditor_SOURCES = \
    CEImagesetEditor.cpp \
    EditorDocument.cpp \
    EditorFrame.cpp \
    EditorGLCanvas.cpp \
    EditorView.cpp \
    PropertiesPanel.cpp \
    ElasticBox.cpp \
    ImagesetHandler.cpp \
    DialogAbout.cpp \
    DialogResourceGroups.cpp

CEImagesetEditor_CPPFLAGS = $(CEGUI_CFLAGS) $(WX_CPPFLAGS)
CEImagesetEditor_LDADD = $(CEGUI_LIBS) $(WX_LIBS)
```

#### 构建命令

```bash
./configure --prefix=/usr/local
make
sudo make install
```

#### 常用配置选项

- `--with-wx-config=/path/to/wx-config`: 指定 wxWidgets 配置脚本
- `--enable-debug`: 启用调试符号
- `CPPFLAGS="-I/custom/include"`: 自定义头文件路径
- `LDFLAGS="-L/custom/lib"`: 自定义库文件路径

---

### 2. Visual Studio (Windows)

#### 项目结构

```
vc++7.1/               # Visual Studio .NET 2003
├── CEImagesetEditor.sln
└── CEImagesetEditor.vcproj

vc++8/                 # Visual Studio 2005
├── CEImagesetEditor.sln
└── CEImagesetEditor.vcproj

vc++9/                 # Visual Studio 2008
├── CEImagesetEditor.sln
└── CEImagesetEditor.vcproj
```

#### 项目配置

**包含目录** (Additional Include Directories):
```
$(CEGUI_HOME)/include
$(WXWIN)/include
$(WXWIN)/lib/vc_lib/mswud  # Debug Unicode
```

**库目录** (Additional Library Directories):
```
$(CEGUI_HOME)/lib
$(WXWIN)/lib/vc_lib
```

**链接库** (Additional Dependencies):
```
CEGUIBase-0_d.lib
CEGUIOpenGLRenderer-0_d.lib
wxmsw28ud_gl.lib
wxmsw28ud_core.lib
wxbase28ud.lib
opengl32.lib
glu32.lib
```

**预处理器定义** (Preprocessor Definitions):
- Debug: `_DEBUG`, `__WXDEBUG__`, `WXUSINGDLL`
- Release: `NDEBUG`, `WXUSINGDLL`

**运行时库**:
- Debug: Multi-threaded Debug DLL (`/MDd`)
- Release: Multi-threaded DLL (`/MD`)

---

### 3. Xcode (macOS)

项目文件位于 `Xcode/` 目录。

#### 框架依赖
- `OpenGL.framework`
- `CEGUI.framework` (需要从源码构建)
- wxWidgets (通过 Homebrew 或源码安装)

#### Build Settings

**Header Search Paths**:
```
/usr/local/include
/usr/local/include/CEGUI-0
$(WXWIN)/include
```

**Library Search Paths**:
```
/usr/local/lib
```

**Other Linker Flags**:
```
-lCEGUIBase-0
-lCEGUIOpenGLRenderer-0
`wx-config --libs --gl-libs`
```

---

## 依赖版本兼容性 (Dependency Compatibility Matrix)

| CEImagesetEditor | CEGUI | wxWidgets | 编译器 |
|---|---|---|---|
| 0.7.1 | 0.5.x - 0.7.x | 2.6.x - 2.8.x | GCC 4.x, MSVC 2003-2008 |

> **注意**: CEGUI 0.8+ 有重大 API 变化，不兼容。

---

## 常见构建问题 (Common Build Issues)

### 问题 1: 找不到 CEGUI 头文件

**错误信息**:
```
fatal error: CEGUI.h: No such file or directory
```

**解决方案**:
```bash
# Linux
export PKG_CONFIG_PATH=/path/to/cegui/lib/pkgconfig:$PKG_CONFIG_PATH
./configure

# Windows
# 在项目属性中添加 CEGUI 的 include 路径
```

### 问题 2: wxWidgets 链接错误

**错误信息**:
```
undefined reference to `wxApp::OnInit()'
```

**解决方案**:
确保链接了所有必需的 wxWidgets 库：
```bash
# 查看需要的库
wx-config --libs --gl-libs
```

### 问题 3: OpenGL 上下文创建失败

**错误信息**:
```
CEGUI::RendererException: Failed to create OpenGL context
```

**解决方案**:
- 确保显卡驱动程序支持 OpenGL
- 在 X11 环境下确保 GLX 可用
- Windows 下检查是否安装了正确的 OpenGL 运行时

### 问题 4: CEGUI 0.8+ 不兼容

**解决方案**:
本项目针对 CEGUI 0.5-0.7 开发。如需支持 0.8+，需要：
1. 更新 API 调用（例如 `CEGUI::System::create()` 替代 `new CEGUI::System()`）
2. 使用新的资源管理系统
3. 更新窗口部件创建方式

---

## 资源文件管理 (Resource Management)

### 资源组 (Resource Groups)

CEGUI 使用资源组来组织文件。编辑器通过 `DefaultResourceProvider` 管理：

```cpp
CEGUI::DefaultResourceProvider* rp =
    static_cast<CEGUI::DefaultResourceProvider*>(
        CEGUI::System::getSingleton().getResourceProvider());

rp->setResourceGroupDirectory("imagesets", "./imagesets/");
rp->setResourceGroupDirectory("schemes", "./schemes/");
```

### 资源组配置对话框

`DialogResourceGroups` 允许用户：
- 添加/删除资源组
- 设置资源组目录
- 指定默认资源组

配置保存在注册表（Windows）或配置文件（Linux）。

