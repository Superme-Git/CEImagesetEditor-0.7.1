# CEImagesetEditor 技术文档

> 文档生成时间: 2025-12-31  
> 文档版本: 2.0

## 📑 目录

### 基础文档
1. [项目概览 (Project Overview)](01_项目概览_Project_Overview.md)
2. [核心模块 (Core Modules)](02_核心核心模块_Core_Modules.md)
3. [数据处理 (Data Handling)](03_数据处理_Data_Handling.md)
4. [用户界面架构 (UI Architecture)](04_用户界面架构_UI_Architecture.md)
5. [API 参考 (API Reference)](05_API_Reference.md)

### 深入文档
6. [详细类参考 (Detailed Class Reference)](06_详细类参考_Detailed_Class_Reference.md)
7. [工作流程与事件处理 (Workflows and Events)](07_工作流程与事件处理_Workflows_and_Events.md)
8. [依赖管理与构建系统 (Dependencies and Build)](08_依赖管理与构建系统_Dependencies_and_Build.md)
9. [高级主题与扩展 (Advanced Topics)](09_高级主题与扩展_Advanced_Topics.md)

## 自动化维护
- **生成 API 文档**: 运行根目录下的 `update_docs.bat`。
- **CI/CD 集成**:
  - 在 Build Pipeline 中添加步骤调用 `scripts/doc_gen.ps1`。
  - 文档生成的产物可发布到 Wiki 或 Pages。
