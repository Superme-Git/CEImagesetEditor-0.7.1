// Doxygen lines:
//

/*! \mainpage 欢迎使用 CEImagesetEditor，这是 CEGUI 库的图像集编辑器。
 * CEGUI 库可以在这里找到：http://www.cegui.org.uk/
 *
 * 本产品使用 wxWidgets 编写：http://www.wxwidgets.org/
 *
 * 原始代码版权所有 (C) 2006 Martin Fleurent。
 * 当前版权所有 (C) 2006 The CEGUI team。
 */

///////////////////////////////////////////////////////////////////////////////
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
#include "CEImagesetEditor.h"
#include "CEGUIHelper.h"
#include "DialogAbout.h"
#include "EditorDocument.h"
#include "EditorFrame.h"
#include "EditorView.h"
#include "wx/image.h"

using namespace CEGUI;

//-----------------------------------------------------------------------
EditorFrame *GlobalFrame = 0;

IMPLEMENT_APP(CEImagesetEditor)

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE(CEImagesetEditor, wxApp)
EVT_MENU(wxID_ABOUT, CEImagesetEditor::OnAbout)
END_EVENT_TABLE()

//-----------------------------------------------------------------------
CEImagesetEditor::CEImagesetEditor() : m_docManager(0) {}

//-----------------------------------------------------------------------
bool CEImagesetEditor::OnInit(void) {
  // 记录一些平台信息
  wxLogDebug(
      wxT("Default Gui font: %s, %d"),
      wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFaceName().c_str(),
      wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize());

  // 创建文档管理器
  m_docManager = new wxDocManager;

  wxImage::AddHandler(new wxPNGHandler);
  // 创建关联文档和视图的模板
  new wxDocTemplate(m_docManager, wxT("imageset files"),
                    wxT("*.xml;*.imageset"), wxT(""), wxT("xml"),
                    wxT("Editor Document"), wxT("Editor View"),
                    CLASSINFO(EditorDocument), CLASSINFO(EditorView));

  // 如果我们只有一个窗口，我们一次只能编辑一个文档。
  m_docManager->SetMaxDocsOpen(1);

  try {

    // 创建单文档主框架，使用默认位置和大小
    GlobalFrame = new EditorFrame(m_docManager, 0, wxID_ANY,
                                  wxT("CEImagesetEditor"), wxDefaultPosition,
                                  wxSize(800, 600), wxDEFAULT_FRAME_STYLE);

    // 准备渲染画布
    // 我们需要显示框架，否则 CEGUI opengl 渲染器将无法初始化，
    // 因为那时候 opengl 视口无效。
    GlobalFrame->Centre(wxBOTH);
    GlobalFrame->Show(true);
    // 这需要在 try 块中！
    GlobalFrame->AttachCanvas(0);
    SetTopWindow(GlobalFrame);

    return true;
  } catch (CEGUI::Exception &guiException) {
    // 如果出错，通知用户
    wxLogError(wxT("ImagesetEditor::OnInit - Error: %s"),
               CEGUIHelper::ToWXString(guiException.getMessage()).c_str());
    return false;
  }
}

//-----------------------------------------------------------------------
int CEImagesetEditor::OnExit() {
  // 清理
  delete m_docManager;
  // delete COptions::GetInstancePtr();
  return 0;
}

//-----------------------------------------------------------------------
void CEImagesetEditor::OnAbout(wxCommandEvent &event) {
  new DialogAbout(GlobalFrame);
}

//-----------------------------------------------------------------------
EditorFrame *GetMainFrame() { return GlobalFrame; }
