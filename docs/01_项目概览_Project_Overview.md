# 项目概览 (Project Overview)

## 简介 (Introduction)
CEImagesetEditor 是一个用于创建和编辑 CEGUI Imageset 文件的图形化工具。它基于 wxWidgets 框架开发，并集成了 CEGUI 渲染引擎。

## 技术栈 (Technology Stack)
- **编程语言**: C++
- **GUI 框架**: wxWidgets (2.6.0+)
- **渲染引擎**: CEGUI (0.5.0+)
- **构建系统**: Automake/Autoconf, Visual Studio Solution

## 目录结构 (Directory Structure)
- `src/`: 源代码文件 (.cpp)
- `inc/`: 头文件 (.h)
- `data/`: 资源文件
- `vc++*/`: Visual Studio 项目文件
- `Xcode/`: Xcode 项目文件

## 核心架构概览 (Architecture Overview)
项目采用文档-视图 (Document-View) 架构：
- **Document**: `EditorDocument` 负责管理数据（Imageset 定义、Regions）。
- **View**: `EditorView` 负责显示逻辑。
- **Frame**: `EditorFrame` 是主窗口容器。
