# 详细类参考 (Detailed Class Reference)

## 核心类架构 (Core Class Architecture)

### CEImagesetEditor (应用程序入口)

**继承关系**: `wxApp`

**职责**: 应用程序生命周期管理

#### 关键成员变量
- `wxDocManager* m_docManager`: 文档管理器，负责文档的创建、打开、关闭等操作

#### 关键方法

##### `bool OnInit()`
**功能**: 应用程序初始化入口点

**执行流程**:
1. 创建 `wxDocManager` 实例
2. 注册 PNG 图片处理器
3. 创建文档模板，关联 `EditorDocument` 和 `EditorView`
4. 限制最大同时打开文档数为 1
5. 创建主窗口 `EditorFrame`
6. 显示主窗口并附加 OpenGL 画布

**异常处理**: 捕获 `CEGUI::Exception` 并记录错误日志

##### `int OnExit()`
**功能**: 清理资源，销毁文档管理器

---

### EditorDocument (文档模型)

**继承关系**: `wxDocument`

**职责**: 
- 存储 Imageset 的所有数据（名称、图片文件、分辨率、Region 定义）
- 提供数据操作接口
- 负责文件的加载和保存

#### 关键成员变量

```cpp
wxString m_imagesetName;           // Imageset 名称
wxString m_imageFilename;          // 关联的图片文件路径
wxPoint m_nativeResolution;        // 原生分辨率 (默认 640x480)
bool m_autoScaled;                 // 是否启用自动缩放
mapNamedRegion m_mapSetRectangles; // Region 名称到矩形区域的映射
```

#### Region 管理

##### `void addRegion(const wxString& name, const wxRect& dim)`
添加新的图片区域定义。
- 存储到 `m_mapSetRectangles` 映射中
- 通知 View 更新界面
- 标记文档为已修改

##### `void setRegionArea(const wxString& name, const wxRect& dim, const bool evt_src)`
更新区域的矩形范围。
- `evt_src`: 标识事件来源，防止无限递归

##### `void deleteRegion(const wxString& name)`
删除指定的 Region。

##### `void renameRegion(const wxString& currentname, const wxString& newname)`
重命名 Region，会检查新名称是否已存在。

#### 文件操作

##### `bool OnOpenDocument(const wxString& filename)`
**加载流程**:
1. 计算相对于资源组的相对路径
2. 创建 `ImagesetHandler` SAX2 解析器
3. 使用 CEGUI XML Parser 解析文件
4. 捕获并记录解析异常
5. 更新所有 View
6. 标记文档为未修改状态

##### `bool OnSaveDocument(const wxString& filename)`
**保存流程**:
1. 调用 `generateRealImageset()` 生成临时 CEGUI Imageset
2. 使用 `ImagesetManager::writeImagesetToStream` 输出 XML
3. 销毁临时 Imageset
4. 标记文档为未修改状态

##### `CEGUI::Imageset* generateRealImageset()`
**功能**: 根据 Document 数据生成真实的 CEGUI Imageset 对象

**实现细节**:
- 如果同名 Imageset 已存在，先销毁
- 从图片文件创建 Imageset
- 设置自动缩放和原生分辨率
- 遍历 `m_mapSetRectangles`，为每个 Region 调用 `imageset.defineImage()`
- 删除 "full_image" 定义（如果存在）

---

### EditorView (视图层)

**继承关系**: `wxView`

**职责**: 连接 Document 和 UI 组件（Canvas、PropertiesPanel）

#### 事件响应方法

所有 `onXXX` 方法都是响应 Document 数据变化的回调：

- `onChangeImageFile()`: 更新 Canvas 显示的图片
- `onChangeImagesetName()`: 更新标题栏
- `onAddRegion()`: 在 Canvas 和 PropertiesPanel 中添加 Region
- `onSetRegionArea()`: 同步更新 Region 的矩形区域
- `onRenameRegion()`: 更新 UI 中的 Region 名称

#### 状态栏更新

- `updateMouseStatusBar()`: 显示鼠标当前坐标
- `updateDocStatusBar()`: 显示缩放比例和图片尺寸

---

### EditorFrame (主窗口)

**继承关系**: `wxDocParentFrame`

**职责**: 
- 管理应用程序的主窗口
- 组织 UI 布局（菜单栏、工具栏、分割窗口）

#### UI 组件层次

```
EditorFrame
├── MenuBar
├── ToolBar
└── wxSplitterWindow
    ├── EditorGLCanvas (左侧，OpenGL 渲染区)
    └── PropertiesPanel (右侧，属性面板)
```

#### 关键成员变量

```cpp
EditorDocument* m_document;            // 当前打开的文档
wxSplitterWindow* m_splitter;          // 分割窗口
PropertiesPanel* m_propsPanel;         // 属性面板
EditorGLCanvas* m_glcanvasImageset;    // OpenGL 画布
DialogResourceGroups* m_resGrpsEditor; // 资源组编辑对话框
wxColour m_backgroundColour;           // 背景颜色
```

#### 菜单事件处理

- `OnChangeBackgroundColour()`: 打开颜色选择对话框
- `OnEditResourceGroups()`: 打开资源组管理对话框

---

### EditorGLCanvas (OpenGL 渲染画布)

**继承关系**: `wxGLCanvas`

**职责**: 
- 使用 OpenGL 渲染 Imageset
- 处理鼠标和键盘交互
- 管理 CEGUI 系统的初始化和清理

#### CEGUI 系统管理

##### `void initialiseCEGUI()`
初始化 CEGUI 渲染器和系统单例。

##### `void cleanupCEGUI()`
清理 CEGUI 资源。

##### `bool resetCEGUI()`
重置 CEGUI 系统（例如分辨率改变时）。

##### `void createCEGUIWindows()`
创建用于显示 Imageset 的 CEGUI 窗口部件。

#### 渲染流程

##### `void Render()`
**渲染步骤**:
1. 设置 OpenGL 上下文
2. 清空颜色缓冲区
3. 调用 `renderImageset()` 渲染 Imageset
4. 调用 `CEGUI::System::getSingleton().renderGUI()`
5. 交换缓冲区

##### `void renderImageset()`
渲染当前 Imageset 和所有 ElasticBox（Region 边框）。

#### 交互功能

##### 鼠标事件
- `OnLeftDown()`: 选择 Region 或开始拖动
- `OnLeftUp()`: 结束拖动
- `OnMouseMotion()`: 拖动 Region 或边界
- `OnMouseWheel()`: 缩放视图

##### 键盘事件
- `OnKeyDown()`: 使用方向键移动选中的 Region
- `OnKeyUp()`: 停止移动

##### 滚动事件
- `OnScrollWin()`: 更新滚动条状态
- `updateScrollbars()`: 计算并更新滚动条
- `updateImageScrollPosition()`: 根据滚动条更新图像位置

#### 缩放功能

##### `void computeZoomFactor(int MouseWheelDirection)`
根据鼠标滚轮方向计算新的缩放因子：
- 向上滚动: `m_zoomFactor *= 1.1`
- 向下滚动: `m_zoomFactor /= 1.1`

---

### PropertiesPanel (属性面板)

**继承关系**: `wxPanel`

**职责**: 
- 显示和编辑 Imageset 的全局属性
- 管理 Region 列表（使用 wxGrid）
- 响应用户输入并更新 Document

#### UI 组件

```cpp
wxTextCtrl* m_imagesetNameTextCtrl;      // Imageset 名称输入框
wxTextCtrl* m_imageFilenameTextCtrl;     // 图片文件路径输入框
wxButton* m_imageFilenameBrowseButton;   // 浏览按钮
wxComboBox* m_nativeResolutionComboBox;  // 分辨率下拉框
wxCheckBox* m_autoScaleCheckbox;         // 自动缩放复选框
wxGrid* m_propertiesGrid;                // Region 定义表格
```

#### Grid 管理

##### Grid 列定义
| 列 | 内容 |
|---|---|
| 0 | Region 名称 |
| 1 | X 坐标 |
| 2 | Y 坐标 |
| 3 | 宽度 |
| 4 | 高度 |

##### Grid 事件处理

- `onGridCellChange()`: 单元格内容改变时同步到 Document
- `onGridCellSelect()`: 选中行时在 Canvas 中高亮对应 Region
- `onGridAddRow()`: 添加新 Region（自动生成唯一名称）
- `onGridDelRow()`: 删除选中的 Region

#### 数据同步

所有 `set*` 方法只更新 UI 显示，不修改 Document。  
所有 `on*` 事件处理方法则会调用 Document 的接口更新数据。

---

### ElasticBox (自定义 CEGUI 部件)

**继承关系**: `CEGUI::Window`

**职责**: 
- 表示 Imageset 中的一个 Region
- 提供视觉边框
- 支持拖动调整大小和位置

#### 特性
- 8 个拖动热点（4 角 + 4 边）
- 鼠标悬停时改变光标形状
- 激活/停用状态
- 自定义事件（Region 修改、激活、停用）

#### 事件

定义在 `ElasticBoxProperties.h` 中的自定义事件：
- `EventRegionModified`: Region 尺寸或位置改变
- `EventRegionActivated`: Region 被激活（选中）
- `EventRegionDeactivated`: Region 被停用

