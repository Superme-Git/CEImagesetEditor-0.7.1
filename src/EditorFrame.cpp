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

#ifdef __linux__
#include <unistd.h>
#endif

#include <wx/colordlg.h>
#include <wx/dir.h>

#include "DialogResourceGroups.h"
#include "EditorFrame.h"
#include "EditorGLCanvas.h"
// 用于在 CEGUI 和 wxWidgets 类型之间转换的实用类
#include "CEGUIHelper.h"

// 工具栏图标
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/save.xpm"

// 菜单项和顶层 UI 组件的 ID 代码
const int ID_MENU_BACKGROUND_COLOR = 12000;
const int ID_MENU_RESOURCE_GROUPS = 12001;

const int ID_SPLITTER_FRAME = 11000;

using namespace CEGUI;

IMPLEMENT_CLASS(EditorFrame, wxDocParentFrame)

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE(EditorFrame, wxDocParentFrame)
EVT_PAINT(EditorFrame::OnPaint)
EVT_MENU(ID_MENU_BACKGROUND_COLOR, EditorFrame::OnChangeBackgroundColour)
EVT_MENU(ID_MENU_RESOURCE_GROUPS, EditorFrame::OnEditResourceGroups)
EVT_PAINT(EditorFrame::OnPaint)
END_EVENT_TABLE()

//-----------------------------------------------------------------------
EditorFrame::EditorFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id,
                         const wxString &title, const wxPoint &pos,
                         const wxSize &size, const long type)
    : wxDocParentFrame(manager, frame, id, title, pos, size, type),
      m_document(0), m_propsPanel(0), m_glcanvasImageset(0), m_resGrpsEditor(0),
      m_backgroundColour(255, 170, 200) {
  // 添加菜单栏，工具栏和状态栏
  AttachMenubar();
  AttachToolbar();
  CreateStatusBar(2);

  // 创建主对话框
  createContent();

  // 预创建资源组编辑器
  m_resGrpsEditor =
      new DialogResourceGroups(this, 0, wxT("Resource Groups Editor"),
                               wxDefaultPosition, wxSize(512, 480));
}

//-----------------------------------------------------------------------
EditorFrame::~EditorFrame() {}

//-----------------------------------------------------------------------
void EditorFrame::AttachMenubar() {
  wxMenuBar *menu_bar = new wxMenuBar;

  wxMenu *menu_file = new wxMenu;
  menu_file->Append(wxID_NEW, wxT("New\tCtrl-N"),
                    wxT("Create a new document."));
  menu_file->Append(wxID_OPEN, wxT("Load...\tCtrl-L"),
                    wxT("Open an existing document."));
  menu_file->Append(wxID_SAVEAS, wxT("Save...\tCtrl-S"),
                    wxT("Save the active document."));
  menu_file->Append(wxID_CLOSE, wxT("&Close \tCtrl-K"),
                    wxT("Close the active document."));
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT, wxT("Quit\tCtrl-Q"),
                    wxT("Quit the application; prompts to save document."));
  menu_bar->Append(menu_file, wxT("File"));

  wxMenu *menu_options = new wxMenu;
  menu_options->Append(ID_MENU_BACKGROUND_COLOR,
                       wxT("Set Background colour..."),
                       wxT("choice the colour of the background."));
  menu_options->AppendSeparator();
  menu_options->Append(
      ID_MENU_RESOURCE_GROUPS, wxT("Resource Groups..."),
      wxT("View and edit currently defined resource groups and directories."));
  menu_bar->Append(menu_options, wxT("Options"));

  wxMenu *menu_help = new wxMenu;
  menu_help->Append(wxID_HELP, wxT("Help..."), wxT(""));
  menu_help->Append(
      wxID_ABOUT, wxT("About..."),
      wxT("Display program information, version number and copyright."));
  menu_bar->Append(menu_help, wxT("?"));

  SetMenuBar(menu_bar);
}

//-----------------------------------------------------------------------
void EditorFrame::AttachToolbar() {
  // 创建仅图标工具栏
  wxToolBar *toolBar =
      new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTB_HORIZONTAL); // |wxNO_BORDER
  //
  toolBar->AddTool(wxID_NEW, wxBitmap(new_xpm));
  toolBar->AddTool(wxID_OPEN, wxBitmap(open_xpm));
  toolBar->AddTool(wxID_SAVE, wxBitmap(save_xpm));
  toolBar->AddSeparator();
  toolBar->AddTool(wxID_CUT, wxBitmap(cut_xpm));
  toolBar->AddTool(wxID_COPY, wxBitmap(copy_xpm));
  toolBar->AddTool(wxID_PASTE, wxBitmap(paste_xpm));
  toolBar->AddTool(wxID_CLEAR, wxBitmap(delete_xpm));
  toolBar->AddSeparator();
  toolBar->AddTool(wxID_ABOUT, wxBitmap(help_xpm));
  // 别忘了这个！
  toolBar->Realize();
  SetToolBar(toolBar);
}

//-----------------------------------------------------------------------
void EditorFrame::SetDocument(EditorDocument *document) {
  m_document = document;
}

//-----------------------------------------------------------------------
void EditorFrame::createContent() {

  m_splitter = new wxSplitterWindow(this, ID_SPLITTER_FRAME, wxDefaultPosition,
                                    wxDefaultSize, wxSP_NOBORDER);
  m_splitter->SetSashGravity(0.0);

  // 分割窗口子项，左侧部分
  m_propsPanel =
      new PropertiesPanel(m_splitter, wxDefaultPosition, wxSize(300, 100));
  m_propsPanel->createContent();
  m_propsPanel->Disable();
}

//-----------------------------------------------------------------------
EditorGLCanvas *EditorFrame::AttachCanvas(EditorView *view) {
  // 创建 GLCanvas。这种方法可能在将来提供 MDI 支持
  EditorGLCanvas *canvas =
      new EditorGLCanvas(view, m_splitter, wxDefaultPosition, wxDefaultSize);
  // 将画布添加到框架
  // 告知其初始大小
  m_splitter->SplitVertically(m_propsPanel, canvas, 250);
  canvas->ClearBackground();
  canvas->Disable();
  canvas->setBackgroundColour(m_backgroundColour);

  // 复制到成员
  m_glcanvasImageset = canvas;

  return canvas;
}

//-----------------------------------------------------------------------
void EditorFrame::OnChangeBackgroundColour(wxCommandEvent &WXUNUSED(event)) {
  wxColourData colourBackground;
  colourBackground.SetColour(m_backgroundColour);

  // 显示颜色选择器
  wxColourDialog dialog(this, &colourBackground);
  if (dialog.ShowModal() == wxID_OK) {
    m_backgroundColour = dialog.GetColourData().GetColour();

    // 设置画布上的颜色
    m_glcanvasImageset->setBackgroundColour(m_backgroundColour);
  }
}

//-----------------------------------------------------------------------
void EditorFrame::OnEditResourceGroups(wxCommandEvent &event) {
  // DialogResourceGroups 类基本上是一个独立的
  // 资源组迷你编辑器，所以我们所要做的就是
  // 在每次被问到时调用对话框。
  m_resGrpsEditor->invoke();
}

//-----------------------------------------------------------------------
void EditorFrame::OnPaint(wxPaintEvent &event) {
  wxPaintDC paintDC(this);

  /*
   * wxMac 似乎有一个问题，导致 wxGLCanvas 在包含的 wxFrame 调整大小之前
   * 无法正确更新，为了解决这个问题（至少目前如此），我们手动触发一个
   * 无操作的调整大小，以便其他大小效果正确发生。或者换句话说，
   * 这是一个讨厌的 hack！
   */
  if (m_glcanvasImageset)
    // m_glcanvasImageset->SetSize(m_glcanvasImageset->GetSize());
    SetSize(GetSize());
}

//-----------------------------------------------------------------------
