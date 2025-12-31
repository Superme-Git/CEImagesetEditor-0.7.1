# 工作流程与事件处理 (Workflows and Event Handling)

## 应用程序启动流程 (Application Startup Flow)

```mermaid
sequenceDiagram
    participant User
    participant CEImagesetEditor
    participant wxDocManager
    participant EditorFrame
    participant EditorGLCanvas
    participant CEGUI

    User->>CEImagesetEditor: 启动程序
    CEImagesetEditor->>CEImagesetEditor: OnInit()
    CEImagesetEditor->>wxDocManager: new wxDocManager()
    CEImagesetEditor->>CEImagesetEditor: 注册 wxDocTemplate
    CEImagesetEditor->>EditorFrame: new EditorFrame()
    EditorFrame->>EditorFrame: 创建菜单栏和工具栏
    EditorFrame->>EditorFrame: 创建分割窗口
    CEImagesetEditor->>EditorFrame: Show(true)
    CEImagesetEditor->>EditorFrame: AttachCanvas()
    EditorFrame->>EditorGLCanvas: new EditorGLCanvas()
    EditorGLCanvas->>CEGUI: initialiseCEGUI()
    CEGUI-->>EditorGLCanvas: 初始化完成
    EditorGLCanvas-->>EditorFrame: Canvas 已附加
    EditorFrame-->>User: 显示主窗口
```

### 关键步骤说明

1. **文档模板注册**: 将 `.xml` 和 `.imageset` 文件关联到 `EditorDocument` 和 `EditorView`
2. **最大文档数限制**: `SetMaxDocsOpen(1)` 确保单文档界面（SDI）
3. **延迟 Canvas 创建**: 必须在窗口显示后才能初始化 OpenGL 上下文

---

## 打开文件流程 (Open File Workflow)

```mermaid
sequenceDiagram
    participant User
    participant wxDocManager
    participant EditorDocument
    participant ImagesetHandler
    participant CEGUIXMLParser
    participant EditorView

    User->>wxDocManager: File > Open
    wxDocManager->>EditorDocument: OnOpenDocument(filename)
    EditorDocument->>ImagesetHandler: new ImagesetHandler(this)
    EditorDocument->>CEGUIXMLParser: parseXMLFile(handler, filename)
    
    Note over CEGUIXMLParser,ImagesetHandler: SAX2 解析开始
    
    CEGUIXMLParser->>ImagesetHandler: elementStart("Imageset", attrs)
    ImagesetHandler->>EditorDocument: setImagesetName(...)
    ImagesetHandler->>EditorDocument: setImageFilename(...)
    ImagesetHandler->>EditorDocument: setNativeResolution(...)
    
    loop 每个 Image 元素
        CEGUIXMLParser->>ImagesetHandler: elementStart("Image", attrs)
        ImagesetHandler->>EditorDocument: addRegion(name, rect)
    end
    
    CEGUIXMLParser-->>EditorDocument: 解析完成
    EditorDocument->>EditorDocument: UpdateAllViews()
    EditorDocument->>EditorView: OnUpdate()
    EditorView->>EditorView: 更新 Canvas 和 PropertiesPanel
    EditorView-->>User: 显示 Imageset
```

### ImagesetHandler 的角色

`ImagesetHandler` 是一个 **SAX2 事件处理器**，它实现了 `CEGUI::XMLHandler` 接口：

- **elementStart()**: 当识别到 XML 标签时被调用
  - `<Imageset>`: 提取 Name, Imagefile, NativeRes 等属性
  - `<Image>`: 提取 Name, XPos, YPos, Width, Height 并调用 `EditorDocument::addRegion()`
- **elementEnd()**: 标签结束时的清理工作

### 错误处理

- XML 解析错误会抛出 `CEGUI::Exception`
- 在 `OnOpenDocument()` 中捕获并记录到 Logger
- 用户会看到错误对话框

---

## 保存文件流程 (Save File Workflow)

```mermaid
sequenceDiagram
    participant User
    participant EditorDocument
    participant CEGUIImageset
    participant CEGUIImagesetManager
    participant FileSystem

    User->>EditorDocument: File > Save
    EditorDocument->>EditorDocument: generateRealImageset()
    EditorDocument->>CEGUIImagesetManager: createFromImageFile(name, file)
    CEGUIImagesetManager-->>EditorDocument: Imageset* imageset
    
    loop 遍历 m_mapSetRectangles
        EditorDocument->>CEGUIImageset: defineImage(name, rect, offset)
    end
    
    EditorDocument->>FileSystem: open(filename)
    EditorDocument->>CEGUIImagesetManager: writeImagesetToStream(name, stream)
    CEGUIImagesetManager->>FileSystem: 写入 XML
    EditorDocument->>CEGUIImagesetManager: destroy(*imageset)
    EditorDocument->>EditorDocument: Modify(false)
    EditorDocument-->>User: 保存成功
```

### 为什么需要临时 Imageset？

CEGUI 的 `ImagesetManager::writeImagesetToStream()` 方法需要一个真实的 `Imageset` 对象。  
`EditorDocument` 只存储原始数据（字符串和矩形），因此需要：
1. 从原始数据构造临时 Imageset
2. 让 CEGUI 序列化这个 Imageset 到 XML
3. 立即销毁临时 Imageset

---

## 添加 Region 流程 (Add Region Workflow)

```mermaid
sequenceDiagram
    participant User
    participant PropertiesPanel
    participant EditorDocument
    participant EditorView
    participant EditorGLCanvas

    User->>PropertiesPanel: 点击 "Add Row"
    PropertiesPanel->>PropertiesPanel: getUniqueImageName()
    Note over PropertiesPanel: 生成 "Image_001" 类似名称
    PropertiesPanel->>EditorDocument: addRegion(name, wxRect(0,0,50,50))
    EditorDocument->>EditorDocument: m_mapSetRectangles[name] = rect
    EditorDocument->>EditorView: onAddRegion(name, rect)
    EditorView->>EditorGLCanvas: addRegion(name, rect)
    EditorGLCanvas->>EditorGLCanvas: 创建 ElasticBox CEGUI 部件
    EditorGLCanvas->>EditorGLCanvas: 绑定事件处理器
    EditorView->>PropertiesPanel: addRegion(name, rect)
    PropertiesPanel->>PropertiesPanel: 在 Grid 中添加新行
    EditorDocument->>EditorDocument: Modify(true)
    EditorGLCanvas-->>User: 渲染新 Region
```

### ElasticBox 事件绑定

当创建 ElasticBox 时，Canvas 会订阅以下事件：
- `EventRegionModified`: 调用 `handleRegionModified()` 同步尺寸到 Document
- `EventRegionActivated`: 调用 `handleRegionActivated()` 更新选中状态
- `EventMouseEnter`: 改变光标形状（如调整大小光标）

---

## 编辑 Region 的双向同步 (Bidirectional Region Editing)

### 场景 1: 用户在 Canvas 中拖动 Region

```mermaid
sequenceDiagram
    participant User
    participant ElasticBox
    participant EditorGLCanvas
    participant EditorDocument
    participant PropertiesPanel

    User->>ElasticBox: 拖动边框
    ElasticBox->>ElasticBox: 更新内部位置/尺寸
    ElasticBox->>EditorGLCanvas: fireEvent(EventRegionModified)
    EditorGLCanvas->>EditorGLCanvas: handleRegionModified()
    Note over EditorGLCanvas: 从 ElasticBox 读取新尺寸
    EditorGLCanvas->>EditorDocument: setRegionArea(name, newRect, evt_src=true)
    Note over EditorDocument: evt_src=true 防止循环
    EditorDocument->>EditorDocument: m_mapSetRectangles[name] = newRect
    EditorDocument->>EditorView: onSetRegionArea(name, newRect, true)
    EditorView->>PropertiesPanel: setRegionArea(name, newRect)
    Note over PropertiesPanel: 只更新 Grid 显示
    PropertiesPanel-->>User: Grid 数值自动更新
```

### 场景 2: 用户在 PropertiesPanel 中修改数值

```mermaid
sequenceDiagram
    participant User
    participant PropertiesPanel
    participant EditorDocument
    participant EditorView
    participant EditorGLCanvas

    User->>PropertiesPanel: 修改 Grid 中的 Width
    PropertiesPanel->>PropertiesPanel: onGridCellChange()
    PropertiesPanel->>PropertiesPanel: getRegionArea(name)
    Note over PropertiesPanel: 从 Grid 读取新值
    PropertiesPanel->>EditorDocument: setRegionArea(name, newRect, evt_src=false)
    EditorDocument->>EditorDocument: m_mapSetRectangles[name] = newRect
    EditorDocument->>EditorView: onSetRegionArea(name, newRect, false)
    EditorView->>EditorGLCanvas: setRegionArea(name, newRect)
    Note over EditorGLCanvas: evt_src=false，允许更新 ElasticBox
    EditorGLCanvas->>EditorGLCanvas: 更新 ElasticBox 位置
    EditorGLCanvas-->>User: 渲染更新后的 Region
```

### 防止无限递归的机制

`evt_src` 参数的作用：
- **true**: 事件源是 Canvas，不要再更新 ElasticBox（避免循环）
- **false**: 事件源是 PropertiesPanel，需要更新 ElasticBox

---

## 事件处理架构 (Event Handling Architecture)

### wxWidgets 事件流

```
用户操作
    ↓
wxEvent (wxMouseEvent, wxKeyEvent, etc.)
    ↓
EditorGLCanvas::OnXXX() 或 PropertiesPanel::onXXX()
    ↓
EditorDocument::setXXX() 或 addRegion()
    ↓
wxDocument::Modify(true) + UpdateAllViews()
    ↓
EditorView::OnUpdate()
    ↓
EditorGLCanvas 和 PropertiesPanel 的更新方法
    ↓
UI 刷新
```

### CEGUI 事件流

```
用户操作 (在 ElasticBox 上)
    ↓
CEGUI::Event (EventRegionModified, EventMouseEnter, etc.)
    ↓
EditorGLCanvas::handleRegionXXX()
    ↓
EditorDocument::setRegionArea()
    ↓
(同 wxWidgets 事件流)
```

### 事件订阅代码示例

```cpp
// 在 EditorGLCanvas::createCEGUIWindows() 中
elasticBox->subscribeEvent(
    ElasticBox::EventRegionModified,
    CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionModified, this)
);
```

---

## 渲染循环 (Rendering Loop)

### 触发渲染的时机

1. **窗口重绘**: `OnPaint(wxPaintEvent&)`
2. **数据变化**: `EditorView::OnUpdate()` 调用 `Canvas->Render()`
3. **交互操作**: 鼠标/键盘事件后调用 `Render()`

### 渲染步骤

```cpp
void EditorGLCanvas::Render() {
    // 1. 设置 OpenGL 上下文
    SetCurrent();
    
    // 2. 清空缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 3. 渲染背景图片
    renderImageset();
    
    // 4. CEGUI 渲染所有窗口部件（包括 ElasticBox）
    CEGUI::System::getSingleton().renderGUI();
    
    // 5. 交换前后缓冲区
    SwapBuffers();
}
```

### 双缓冲机制

使用 OpenGL 的双缓冲避免闪烁：
- 后台缓冲区绘制
- 前台缓冲区显示
- `SwapBuffers()` 交换两者

