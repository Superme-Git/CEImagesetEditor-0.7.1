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
#ifndef _DIALOG_RESOURCE_GROUPS_H_
#define _DIALOG_RESOURCE_GROUPS_H_

#include <wx/grid.h>
#include <wx/hashmap.h>
#include <wx/wx.h>


//--------------------------------------------------------------------------------
#define ID_RESOURCE_GROUPS_DIALOG 10000
#define SYMBOL_DIALOGRESOURCEGROUPS_STYLE wxCAPTION | wxRESIZE_BORDER
#define SYMBOL_DIALOGRESOURCEGROUPS_TITLE wxT("Resource Groups")
#define SYMBOL_DIALOGRESOURCEGROUPS_IDNAME ID_RESOURCE_GROUPS_DIALOG
#define SYMBOL_DIALOGRESOURCEGROUPS_SIZE wxSize(400, 300)
#define SYMBOL_DIALOGRESOURCEGROUPS_POSITION wxDefaultPosition
#define ID_GROUPS_GRID 10001
#define ID_ADDGROUP_BUTTON 10002
#define ID_REMOVEGROUP_BUTTON 10003
#define ID_DEFAULTGROUP_TEXTCTRL 10004
#define ID_DEFAULTGROUP_BUTTON 1005

//--------------------------------------------------------------------------------
class DialogResourceGroups : public wxDialog {
  DECLARE_EVENT_TABLE()

public:
  WX_DECLARE_STRING_HASH_MAP(wxString, ResourceGroupsMap);

  //! 构造函数
  DialogResourceGroups(
      wxWindow *parent, wxWindowID id = SYMBOL_DIALOGRESOURCEGROUPS_IDNAME,
      const wxString &caption = SYMBOL_DIALOGRESOURCEGROUPS_TITLE,
      const wxPoint &pos = SYMBOL_DIALOGRESOURCEGROUPS_POSITION,
      const wxSize &size = SYMBOL_DIALOGRESOURCEGROUPS_SIZE,
      long style = SYMBOL_DIALOGRESOURCEGROUPS_STYLE);

  //! 创建
  bool Create(wxWindow *parent,
              wxWindowID id = SYMBOL_DIALOGRESOURCEGROUPS_IDNAME,
              const wxString &caption = SYMBOL_DIALOGRESOURCEGROUPS_TITLE,
              const wxPoint &pos = SYMBOL_DIALOGRESOURCEGROUPS_POSITION,
              const wxSize &size = SYMBOL_DIALOGRESOURCEGROUPS_SIZE,
              long style = SYMBOL_DIALOGRESOURCEGROUPS_STYLE);

  //! 创建控件和布局器
  void CreateControls();

  //! ID_GROUPS_GRID 的 wxEVT_GRID_EDITOR_SHOWN 事件处理程序
  void OnEditorShown(wxGridEvent &event);

  //! ID_GROUPS_GRID 的 wxEVT_GRID_CELL_EDIT 事件处理程序
  void OnCellEdit(wxGridEvent &event);

  //! ID_ADDGROUP_BUTTON 的 wxEVT_COMMAND_BUTTON_CLICKED 事件处理程序
  void OnAddgroupButtonClick(wxCommandEvent &event);

  //! ID_REMOVEGROUP_BUTTON 的 wxEVT_COMMAND_BUTTON_CLICKED 事件处理程序
  void OnRemovegroupButtonClick(wxCommandEvent &event);

  //! ID_DEFAULTGROUP_BUTTON 的 wxEVT_COMMAND_BUTTON_CLICKED 事件处理程序
  void OnSetDefaultGroupButtonClick(wxCommandEvent &event);

  //! wxID_OK 的 wxEVT_COMMAND_BUTTON_CLICKED 事件处理程序
  void OnOkClick(wxCommandEvent &event);

  //! wxID_CANCEL 的 wxEVT_COMMAND_BUTTON_CLICKED 事件处理程序
  void OnCancelClick(wxCommandEvent &event);

  //! 获取位图资源
  wxBitmap GetBitmapResource(const wxString &name);

  //! 获取图标资源
  wxIcon GetIconResource(const wxString &name);

  //! 我们应该显示工具提示吗？
  static bool ShowToolTips();

  /** 调用对话框，应该调用此方法来显示对话框，而不是其他可能看起来也有效的方法。
   */
  void invoke();

  bool isResourceGroupDefined(const wxString &groupName) const;
  bool renameResourceGroup(const wxString &currentName,
                           const wxString &newName);
  bool addGroupEntry(const wxString &groupName, const wxString &directory);
  bool deleteGroupEntry(const wxString &groupName);
  bool setGroupDirectory(const wxString &groupName, const wxString &directory);
  wxString getGroupDirectory(const wxString &groupName) const;

  bool setDefaultGroup(const wxString &groupName);
  const wxString &getDefaultGroup() const;

  ResourceGroupsMap::const_iterator getGroupIteratorStart() const;
  ResourceGroupsMap::const_iterator getGroupIteratorEnd() const;

protected:
  //! 生成新的组名
  wxString getUniqueGroupName();
  //! 获取网格中组名的索引，如果没有则为 -1。
  int getInternalGroupIndex(const wxString &groupName) const;
  //! 清除网格（但不清除内部组映射）
  void clearGroupsGrid();
  //! 根据内部组映射填充网格
  void populateGrid();

  //! 已定义的组的映射
  ResourceGroupsMap m_resourceGroups;
  //! 当前选定的默认资源组
  wxString m_defaultGroupName;
  //! 编辑前已定义的组的映射
  ResourceGroupsMap m_backupResourceGroups;
  //! 编辑前已定义的默认资源组
  wxString m_backupDefaultGroupName;
  //! 旧组名（用于跟踪组的编辑和重命名）
  wxString m_oldGroupName;
  //! 用于生成唯一组名的值（添加新组时）
  int m_groupGUID;
  //! 显示所选默认组的只读文本控件
  wxTextCtrl *m_defaultGroupTextCtrl;
  //! 选择默认组的按钮
  wxButton *m_setDefaultButton;
  //! 已定义组及其目录的网格。
  wxGrid *m_groupsGrid;
  //! 添加新组的按钮
  wxButton *m_addGroupButton;
  //! 删除所选组的按钮
  wxButton *m_removeGroupButton;
  //! 主对话框确定按钮
  wxButton *m_okButton;
  //! 主对话框取消按钮
  wxButton *m_cancelButton;
};

#endif // _DIALOG_RESOURCE_GROUPS_H_
