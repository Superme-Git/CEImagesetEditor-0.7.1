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

#ifndef _EDITOR_VIEW_H_
#define _EDITOR_VIEW_H_

// 标准 wxWidget 头文件
#include "wx/wx.h"
// 更具体的 wxWidgets 头文件
#include "wx/docview.h"
// #include "wx/cmdproc.h"

class EditorFrame;
class EditorGLCanvas;
class PropertiesPanel;

/** "文档"上的"视图"。注意，视图不是在应用程序启动时创建的，
 * 而是在用户选择“文件->新建”或“文件->打开”时创建的。
 * 另请注意，对话框不是真正的视图。
 */
class EditorView : public wxView {
  // 文档/视图管理器所需
  DECLARE_DYNAMIC_CLASS(EditorView)

public:
  /** 构造函数。*/
  EditorView();

  /** 当文档名称更改（例如保存或新建）时由文档调用。*/
  void onChangeFilename();

  // 对文档的可能操作
  void onChangeImageFile(const wxString &newfilename);

  /** 当图像集名称更改时由文档调用。*/
  void onChangeImagesetName(const wxString &newname);

  /** 当原始分辨率更改时由文档调用。*/
  void onChangeNativeResolution(const wxPoint &newnativeres);

  /** 当自动缩放更改时由文档调用。*/
  void onChangeAutoScaled(const bool newAutoScaled);

  /** 当添加区域时由文档调用。*/
  void onAddRegion(const wxString &name, const wxRect &dim);

  /** 当删除区域时由文档调用。*/
  void onDelRegion(const wxString &name);

  /** 当所有区域被删除时由文档调用。*/
  void onDelAllRegion();

  /** 当区域移动和/或缩放时由文档调用。*/
  void onSetRegionArea(const wxString &name, const wxRect &dim,
                       const bool evt_src = false);

  /** 当重命名区域时由文档调用。*/
  void onRenameRegion(const wxString &currentname, const wxString &newname);

  // 框架部分更新部分
  /** 根据当前打开的文档更新主框架的标题栏。*/
  void updateTitle();

  /** 更新状态栏的鼠标部分。*/
  void updateMouseStatusBar(const float posx, const float posy);

  /** 更新状态栏的视图部分。*/
  void updateDocStatusBar(const float zoom, const float width,
                          const float height);

  EditorGLCanvas *getCanvas() { return m_glcanvasImageset; }

  PropertiesPanel *getPropsPanel() { return m_propsPanel; }

private:
  //! 我们附加到的 EditorFrame。
  EditorFrame *m_frame;
  //! 构成此 EditorView 一部分的 PropertiesPanel
  PropertiesPanel *m_propsPanel;
  //! 我们进行基于 OpenGL 和 CEGUI 渲染的 EditorGLCanvas。
  EditorGLCanvas *m_glcanvasImageset;

  /** 已重写，因为基类是 Window。我们不在这里使用它。*/
  void OnDraw(wxDC *dc);

  /** 通知我们已创建一个新文档，通过“文件->新建”或“文件->打开”。*/
  bool OnCreate(wxDocument *doc, long flags);

  /** 通知用户关闭当前文档。通过显式关闭，或通过“文件->新建”或“文件->打开”。
   */
  bool OnClose(bool deleteWindow = true);

  /** 文档中 UpdateAllViews 调用的通知。
   */
  void OnUpdate(wxView *sender, wxObject *hint);
};

#endif // _EDITOR_VIEW_H_
