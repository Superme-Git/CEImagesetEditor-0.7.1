# CEImagesetEditor 技术文档

> 文档生成时间: 2025-12-31

## 目录
1. [项目概览](01_项目概览_Project_Overview.md)
2. [核心模块](02_核心核心模块_Core_Modules.md)
3. [数据处理](03_数据处理_Data_Handling.md)
4. [用户界面架构](04_用户界面架构_UI_Architecture.md)
5. [API 参考](05_API_Reference.md)

## 自动化维护
- **生成 API 文档**: 运行根目录下的 `update_docs.bat`。
- **CI/CD 集成**:
  - 在 Build Pipeline 中添加步骤调用 `scripts/doc_gen.ps1`。
  - 文档生成的产物可发布到 Wiki 或 Pages。
