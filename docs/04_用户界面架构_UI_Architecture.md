# 用户界面架构 (UI Architecture)

## 布局概览 (Layout Overview)
主界面由 `EditorFrame` 管理，包含：
- **菜单栏 (MenuBar)**: 文件操作、编辑等。
- **工具栏 (ToolBar)**: 常用操作快捷方式。
- **渲染区 (Canvas)**: `EditorGLCanvas` 显示 Imageset 和边框。
- **属性面板 (Properties)**: `PropertiesPanel` 编辑选定 Region 的属性。

## 关键类 (Key Classes)

### `EditorFrame`
主窗口类，负责组装界面组件。
- 包含 `EditorGLCanvas` 和 `PropertiesPanel`。
- 处理菜单命令（如 Zoom, Background toggle）。

### `EditorGLCanvas`
基于 OpenGL 的画布，用于实时渲染 Imageset。
- 负责绘制背景图片。
- 负责绘制 Region 的边框（ElasticBox）。
- 处理鼠标交互（选择、移动、缩放 Region）。

### `EditorView`
连接 Document 和 Window。 Inherits `wxView`.
- 监听 Document 的变化。
- 更新 Canvas 的显示。
