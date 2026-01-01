# CEImagesetEditor VS 2013 编译快速指南

> **前提**: wxWidgets 3.0.5 已编译完成 ✅  
> **位置**: `E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5`

---

## 快速步骤 (2-3 小时)

### 1. ✅ 转换项目 (已完成)

**状态**: 项目已成功转换为 VS 2013 格式

**转换结果**:
- ✅ 项目文件: `.vcproj` → `.vcxproj`
- ✅ 平台工具集: v90 (VS 2008) → v120 (VS 2013)
- ✅ 解决方案文件已更新
- ✅ 原始文件已备份到 `Backup/` 目录

**下一步**: 继续步骤 2 配置项目

### 2. ✅ 配置项目 (已完成 - 库引用)

**状态**: wxWidgets 库引用已更新为 3.0.5

**已完成的修改**:
- ✅ Debug 配置: `wxbase28ud.lib` → `wxbase30ud.lib`
- ✅ Debug 配置: `wxmsw28ud_*.lib` → `wxmsw30ud_*.lib`
- ✅ Release 配置: `wxbase28u.lib` → `wxbase30u.lib`
- ✅ Release 配置: `wxmsw28u_*.lib` → `wxmsw30u_*.lib`

**仍需配置** (如果使用环境变量):

确保环境变量指向正确路径:
```batch
WXWIDGETS = E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5
CEGUI = <你的CEGUI路径>
CE_IMAGESET_EDITOR = E:\CEImagesetEditor-0.7.1
```

或者在 VS 2013 项目属性中手动设置绝对路径(参考下方原始说明)。

---

<details>
<summary>📋 原始配置说明(如需手动配置点击展开)</summary>

#### 打开项目属性 (Alt+F7)

**所有配置** → **C/C++** → **常规** → **附加包含目录**:
```
E:\CEImagesetEditor-0.7.1\inc
E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5\include
E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5\include\msvc
$(CEGUI)\cegui\include
```

**所有配置** → **链接器** → **常规** → **附加库目录**:
```
E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5\lib\vc_lib
$(CEGUI)\lib
```

**Debug 配置** → **链接器** → **输入** → **附加依赖项**:
```
OpenGL32.lib
GLU32.lib
comctl32.lib
rpcrt4.lib
CEGUIBase_d.lib
CEGUIOpenGLRenderer_d.lib
wxbase30ud.lib
wxmsw30ud_core.lib
wxmsw30ud_gl.lib
wxmsw30ud_adv.lib
```

**Release 配置** → **链接器** → **输入** → **附加依赖项**:
```
OpenGL32.lib
GLU32.lib
comctl32.lib
rpcrt4.lib
CEGUIBase.lib
CEGUIOpenGLRenderer.lib
wxbase30u.lib
wxmsw30u_core.lib
wxmsw30u_gl.lib
wxmsw30u_adv.lib
```

</details>

**下一步**: 继续步骤 3 修改代码

### 3. ✅ 修改代码 (已完成)

**状态**: wxGLCanvas 代码已更新为 wxWidgets 3.0.5 兼容

**已完成的修改**:

1. **`inc\EditorGLCanvas.h`** - 添加成员变量:
   ```cpp
   wxGLContext* m_context;  // OpenGL 上下文
   ```

2. **`src\EditorGLCanvas.cpp`** - 构造函数修改:
   ```cpp
   // 旧: wxGLCanvas(parent, static_cast<const wxGLContext*>(0), -1, ...)
   // 新: wxGLCanvas(parent, wxID_ANY, NULL, ...)
   
   // 添加上下文创建
   m_context = new wxGLContext(this);
   SetCurrent(*m_context);
   ```

3. **`src\EditorGLCanvas.cpp`** - 析构函数修改:
   ```cpp
   delete m_context;  // 清理 OpenGL 上下文
   ```

**下一步**: 继续步骤 4 编译项目

---

<details>
<summary>📋 原始修改说明(点击展开查看详细代码)</summary>

#### 文件: `src\EditorGLCanvas.cpp` (第 71-72 行)

**修改前**:
```cpp
EditorGLCanvas::EditorGLCanvas(EditorView* v, wxWindow* parent, 
                               const wxPoint& pos, const wxSize& size) :
    wxGLCanvas(parent, static_cast<const wxGLContext*>(0), -1, 
               pos, size, wxSUNKEN_BORDER | wxVSCROLL | wxHSCROLL),
```

**修改后**:
```cpp
EditorGLCanvas::EditorGLCanvas(EditorView* v, wxWindow* parent, 
                               const wxPoint& pos, const wxSize& size) :
    wxGLCanvas(parent, wxID_ANY, NULL, 
               pos, size, wxSUNKEN_BORDER | wxVSCROLL | wxHSCROLL),
```

#### 添加 OpenGL 上下文管理

**在 `inc\EditorGLCanvas.h` 中添加**:
```cpp
private:
    wxGLContext* m_context;  // 添加这一行
```

**在 `src\EditorGLCanvas.cpp` 构造函数中**:
```cpp
EditorGLCanvas::EditorGLCanvas(...) : wxGLCanvas(...) {
    // 在 SetCurrent() 之前添加
    m_context = new wxGLContext(this);
    SetCurrent(*m_context);
    
    // ... 原有代码
}
```

**在析构函数中**:
```cpp
EditorGLCanvas::~EditorGLCanvas() {
    delete m_context;  // 添加这一行
    cleanupCEGUI();
}
```

### 4. 环境配置 (重要)

由于 VS 2013 安装在 `D:` 盘,请在命令行中先运行环境脚本:

```batch
:: 初始化 VS 2013 编译环境 (x86)
"D:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
```

**已验证的关键路径**:
- **VS 安装根目录**: `D:\Program Files (x86)\Microsoft Visual Studio 12.0\`
- **编译器 (cl.exe)**: `D:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\`
- **MSBuild**: `C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe`
- **Windows SDK**: `C:\Program Files (x86)\Windows Kits\8.1\`

### 5. 编译 (30 分钟)

```batch
:: 确保已运行 vcvarsall.bat
:: 进入项目目录
cd E:\CEImagesetEditor-0.7.1\vc++9

:: 命令行编译 (推荐)
msbuild CEImagesetEditor.sln /p:Configuration=Debug /p:Platform=Win32
msbuild CEImagesetEditor.sln /p:Configuration=Release /p:Platform=Win32

:: 或者打开 IDE 编译
start CEImagesetEditor.sln
```

**常见错误**:
- `LNK2019`: 检查库路径和库名是否正确
- `C2065`: 检查包含目录是否正确
- `C2664`: wxGLCanvas 构造函数参数错误

### 6. 运行测试 (15 分钟)

```batch
:: 复制 CEGUI DLL
copy $(CEGUI)\bin\*.dll bin\debug\

:: 复制数据文件
xcopy data bin\debug\data /S /I /Y

:: 运行
bin\debug\CEImagesetEditor_d.exe
```

---

## 环境变量设置

建议设置环境变量简化配置:

```batch
:: 系统环境变量
CEGUI = C:\path\to\CEGUI-0.7.9
```

或在项目属性表中设置用户宏。

---

## 验证清单

- [ ] VS 2013 已安装
- [ ] wxWidgets 3.0.5 库文件存在 (`E:\CEImagesetEditor-0.7.1\wxWidgets-3.0.5\lib\vc_lib\wx*.lib`)
- [ ] CEGUI 0.7.x 已准备
- [ ] 项目已转换为 VS 2013 格式
- [ ] 包含目录已更新
- [ ] 库目录已更新
- [ ] 链接库已更新
- [ ] wxGLCanvas 代码已修改
- [ ] 编译成功
- [ ] 程序可运行

---

## 需要帮助?

查看详细文档: [VS 2013 + wxWidgets 3.0.5 兼容性评估报告](10_VS2013编译指南_VS2013_Build_Guide.md)
