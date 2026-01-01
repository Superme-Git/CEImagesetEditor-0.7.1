///////////////////////////////////////////////////////////////////////////////
//  有关项目详情和作者，请参阅 README 和 AUTHORS 文件
//
//  本文件是自由软件；您可以根据自由软件基金会发布的
//  GNU 通用公共许可证条款重新分发和/或修改它；
//  版本 2 或（由您选择）任何更高版本。
//
//  分发此文件是希望它有用，
//  但没有任何担保；甚至没有对适销性或
//  特定用途适用性的暗示担保。有关更多详细信息，请参阅
//  GNU 通用公共许可证。
//
//  您应该已收到随本程序一起分发的 GNU 通用公共许可证副本；
//  如果没有，请写信给 Free Software Foundation, Inc.,
//  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA。
//
//  要在线查看许可证，请访问：http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROPERTIES_PANEL_H_
#define _PROPERTIES_PANEL_H_

// 标准 wxWidget 头文件
#include "wx/wx.h"

#include "EditorDocument.h"

#include <wx/dynarray.h>
#include <wx/grid.h>

class EditorView;

class PropertiesPanel : public wxPanel {
public:
  //! 构造函数
  PropertiesPanel(wxWindow *parent, const wxPoint &pos, const wxSize &size);

  //! 设置我们所属的视图。
  void setView(EditorView *aView);

  //! 创建填充 PropertiesPanel 的组件。
  void createContent();

  //! 将 PropertiesPanel 重置为其初始状态。
  void reset();

  //
  // 以下均为 GUI 处理程序成员；它们对面板 GUI 中的更改做出反应，
  // 并相应地更新编辑器文档。
  //
  // 请注意，这些成员不应直接更新与视图相关的任何内容，
  // 这将通过文档对更新的响应发生。
  //
  //! 按下图像文件浏览按钮时的处理程序
  void onBrowseImageFile(wxCommandEvent &WXUNUSED(event));
  //! 通过键入更改图像集名称时的处理程序
  void onChangeImagesetName(wxCommandEvent &WXUNUSED(event));
  //! 通过键入更改图像文件名时的处理程序。
  void onChangeImageFilename(wxCommandEvent &WXUNUSED(event));
  //! 原始分辨率选择更改时的处理程序
  void onChangeNativeResolution(wxCommandEvent &WXUNUSED(event));
  //! 更改自动缩放设置时的处理程序。
  void onChangeAutoScaled(wxCommandEvent &WXUNUSED(event));
  //! 将行添加到图像定义网格时调用的处理程序。
  void onGridAddRow(wxCommandEvent &WXUNUSED(event));
  //! 从图像定义网格中删除行时调用的处理程序。
  void onGridDelRow(wxCommandEvent &WXUNUSED(event));
  //! 更改网格单元格内容时调用的处理程序。
  void onGridCellChange(wxGridEvent &event);
  //! 当网格单元格即将被编辑时调用的处理程序。
  void onGridCellEdit(wxGridEvent &event);
  //! 选择网格单元格（行）时调用的处理程序。
  void onGridCellSelect(wxGridEvent &event);

  //
  // 以下是访问器和操作器成员，用于获取或更新 GUI 面板组件中的信息。
  //
  // 通常，操作器由 EditorView 调用以响应 EditorDocument 上的更改。
  // EditorDocument 实际上不应该直接调用这些成员 - 一切都应该通过
  // EditorView 完成；这保持了良好的文档/视图抽象，并避免了一切过于紧密耦合。
  //
  //! 返回面板中显示的图像集名称。
  wxString getImagesetName() const;

  //! 设置要在面板中显示的图像集名称。
  void setImagesetName(const wxString &name);

  //! 返回面板中显示的源图像文件名。
  wxString getImageFilename() const;

  //! 设置要在面板中显示的源图像文件名。
  void setImageFilename(const wxString &name);

  //! 返回描述面板中选择的原始分辨率的 wxPoint。
  wxPoint getNativeResolution() const;

  //! 设置要在面板中选择的原始分辨率。
  void setNativeResolution(const wxPoint &NativeResolution);

  //! 返回是否选中了自动缩放复选框。
  bool getAutoScaled() const;

  //! 设置是否选中自动缩放复选框。
  void setAutoScaled(const bool AutoScaled);

  //! 向网格添加新的图像区域定义
  void addRegion(const wxString &name, const wxRect &dim, int rowindex = -1);

  //! 从网格中移除图像区域定义。
  int deleteRegion(const wxString &name);

  //! 从网格中移除所有图像区域定义。
  void deleteAllRegions();

  //! 在网格中选择一个图像区域
  int selectRegion(const wxString &name);

  //! 返回任何当前选择的图像区域的名称。
  wxString getSelectedRegion() const;

  //! 设置图像区域的区域字段。
  void setRegionArea(const wxString &name, const wxRect &dim);

  //! 返回当前为图像区域定义的区域。
  wxRect getRegionArea(const wxString &name) const;

  //! 重命名图像区域定义。
  void renameRegion(const wxString &currentname, const wxString &newname);

private:
  //! 返回我们是否可以访问有效的 EditorDocument 对象。
  bool isDocumentValid() const;

  //! 返回当前的 EditorDocument 对象。
  EditorDocument *getDocument() const;

  //! 返回用于新图像区域的唯一名称。
  wxString getUniqueImageName();

  static const char s_charParseNativeResolution;

  void createPropertiesGrid(wxWindow *parent);

  //! 返回网格中命名区域的索引
  int getInternalRegionIndex(const wxString &name) const;

  //! 返回相对于默认资源组目录的给定路径 'filename'
  wxString getDefaultResourceGroupRelativePath(const wxString &filename) const;

  //
  // 数据字段
  //
  //! 我们附加到的视图。
  EditorView *m_view;
  //! 保存图像集名称的文本控件
  wxTextCtrl *m_imagesetNameTextCtrl;
  //! 保存源图像文件名的文本控件。
  wxTextCtrl *m_imageFilenameTextCtrl;
  //! 调用文件对话框以选择图像文件的按钮。
  wxButton *m_imageFilenameBrowseButton;
  //! 提供一组原始分辨率选项的组合框
  wxComboBox *m_nativeResolutionComboBox;
  //! 控制自动缩放选项的复选框。
  wxCheckBox *m_autoScaleCheckbox;
  //! 保存图像区域定义的网格
  wxGrid *m_propertiesGrid;
  //! 用于生成唯一图像定义名称的数字 GUID。
  size_t m_imageGUID;

  wxString m_oldRowName;

  DECLARE_EVENT_TABLE()
};

#endif // _PROPERTIES_PANEL_H_
