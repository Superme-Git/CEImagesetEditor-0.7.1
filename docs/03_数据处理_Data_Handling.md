# 数据处理 (Data Handling)

## XML 解析 (XML Parsing)
项目使用 `ImagesetHandler` 类来解析 standard CEGUI Imageset XML 文件。

### `ImagesetHandler`
继承自 `CEGUI::XMLHandler` (SAX2 解析器)。

- **工作原理**:
  1. `EditorDocument` 在打开文件时创建 `ImagesetHandler` 实例。
  2. 解析器遍历 XML 节点。
  3. `elementStart` 方法捕获 `Imageset` 和 `Image` 标签。
  4. 提取属性（Name, XPos, YPos, Width, Height）并调用 `EditorDocument` 的方法填充数据。

### 数据流 (Data Flow)
1. **读取**: File -> XML Parser -> ImagesetHandler -> EditorDocument (Memory)
2. **写入**: EditorDocument (Memory) -> XML Generator -> File

### 关键 Tag 处理
- **Imageset**: 提取 Name, Imagefile, ResourceGroup, NativeRes 等属性。
- **Image**: 提取 Name, Rect (X, Y, W, H) 并添加到 Document 中。
