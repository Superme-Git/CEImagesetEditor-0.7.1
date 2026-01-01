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

#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

// 标准 wxWidget 头文件
#include "wx/wx.h"
#include <CEGUI.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>

// 更具体的 wxWidgets 头文件
#include "PropertiesPanel.h"
#include "wx/docview.h"
#include "wx/splitter.h"

// 我们需要对获取的客户区矩形进行微小的补充
#define BORDER_SIZE 4

// 前向声明
class EditorDocument;
class EditorGLCanvas;
class EditorView;
class DialogResourceGroups;

class EditorFrame : public wxDocParentFrame {
  DECLARE_CLASS(EditorFrame)
public:
  /** 构造函数。附加一个兼容 OpenGL 的画布。*/
  EditorFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id,
              const wxString &title, const wxPoint &pos, const wxSize &size,
              const long type);

  /** 析构函数。*/
  ~EditorFrame();

  /** 从附加的视图接收文档，以便通过菜单事件进行访问。
   */
  void SetDocument(EditorDocument *document);

  /** 独立的方法，以便在整个框架初始化后调用它，
   * 因为画布必须可见，而在创建框架时看起来不太好。*/
  EditorGLCanvas *AttachCanvas(EditorView *view);

  EditorGLCanvas *getCanvas() { return m_glcanvasImageset; }

  PropertiesPanel *getPropsPanel() { return m_propsPanel; }

  /** 我们监听菜单栏的编辑事件（包括对齐部分）。*/
  DECLARE_EVENT_TABLE()

private:
  EditorDocument *m_document;
  wxSplitterWindow *m_splitter;
  PropertiesPanel *m_propsPanel;
  EditorGLCanvas *m_glcanvasImageset;
  DialogResourceGroups *m_resGrpsEditor;

  wxColour m_backgroundColour;

  /** 我们存储编辑菜单，以传递给命令处理器。*/
  wxMenu *m_EditMenu;

  /** 将菜单栏附加到框架。*/
  void AttachMenubar();

  /** 将工具栏附加到框架。*/
  void AttachToolbar();

  void createContent();

  /** 重载以便我们也可以关闭对话框。
   */
  bool Close(bool force = FALSE);

  /** 事件处理程序：通知我们框架需要重绘。 */
  void OnPaint(wxPaintEvent &event);

  void OnChangeBackgroundColour(wxCommandEvent &event);
  void OnEditResourceGroups(wxCommandEvent &event);
};

#endif // _EDITOR_FRAME_H_
