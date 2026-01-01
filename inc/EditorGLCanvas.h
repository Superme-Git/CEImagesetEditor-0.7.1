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

#ifndef _EDITOR_CANVAS_H_
#define _EDITOR_CANVAS_H_

// 包含 wxWidgets 头文件
#include <wx/wx.h>
// CEGUI 头文件
#include <CEGUISystem.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>

#include "EditorDocument.h"
#include "wx/glcanvas.h"

class EditorView;

/** 提供 OpenGL 输出。此画布是我们对“文档”的“视图”的实际可见部分。
 * 渲染时必须初始化 CEGUI 系统，这会经过验证。
 */

class EditorGLCanvas : public wxGLCanvas {
public:
  /** 构造函数。*/
  EditorGLCanvas(EditorView *view, wxWindow *parent,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize);

  /** 析构函数。*/
  ~EditorGLCanvas();

  /** 更新 OpenGL 视口设置。*/
  void changeSize(int newWidth, int newHeight);

  void setBackgroundColour(const wxColour &colour);
  void setImage(const wxString &name);

  // 区域操作
  void selectRegion(const wxString &name);
  void renameRegion(const wxString &currentname, const wxString &newname);
  void addRegion(const wxString &name, const wxRect &dim);
  void setRegionArea(const wxString &name, const wxRect &dim);
  void deleteRegion(const wxString &name);
  void deleteAllRegions();

  void computeZoomFactor(int MouseWheelDirection);

  void setView(EditorView *aView) { m_view = aView; }

  /** 强制重置 cegui。*/
  bool Reset();

  /** 强制重绘。*/
  void Render();

  /** 返回文档是否仍然有效。*/
  bool isDocumentValid() const;

private:
  /** 我们依附的视图。*/
  EditorView *m_view;

  /** 指向 cegui 单例的指针。*/
  CEGUI::System *m_GUISystem;
  /** 指向 cegui 渲染器的指针。*/
  CEGUI::OpenGLRenderer *m_GUIRenderer;
  /** wxWidgets 3.0+ 的 OpenGL 上下文 */
  wxGLContext *m_context;

  // CEGUI 小部件
  CEGUI::Window *m_imagesetStaticImage;

  /** 当按键移动选区时应使用大步长时为 True。*/
  bool m_useBigSteps;

  //! 用于跟踪拖动动作。
  wxPoint m_dragPoint;

  /** 当前文档缩放。float 类型，因为它是相对于原始大小（等于 1）的相对值。*/
  float m_zoomFactor;

  /** 当前画布宽度。经常使用，所以进行了缓存。*/
  float m_realWidth;

  /** 当前画布高度。经常使用，所以进行了缓存。*/
  float m_realHeight;

  /** 上次已知的 mouseX 位置。*/
  long m_lastMouseX;

  /** 上次已知的 mouseY 位置。*/
  long m_lastMouseY;

  // 这里我们要有滚动状态变量；这是必须的，因为 wxWidgets 在不同平台上
  // 对于 wxWindow 滚动条有不同的行为。（wxGTK 自动处理它们而无需干预，wxMSW
  // 则没有）。
  //! 当前水平滚动位置
  int m_scrollPosX;
  //! '文档'的水平大小（缩放后的图像宽度）
  int m_scrollDocX;
  //! 水平'页面'大小（画布窗口的宽度）
  int m_scrollPageX;
  //! 当前垂直滚动位置
  int m_scrollPosY;
  //! '文档'的垂直大小（缩放后的图像高度）
  int m_scrollDocY;
  //! 垂直'页面'大小（画布窗口的高度）
  int m_scrollPageY;

  /** 渲染当前图像集。*/
  void renderImageset();

  /** 初始化 CEGUI 核心 + 渲染器。当生成新视图时我们不清理框架，
   * 所以这是放置 CEGUI 功能的好地方。*/
  void initialiseCEGUI();

  /** 关闭 CEGUI 系统，释放所有基于 CEGUI 的资源。*/
  void cleanupCEGUI();

  bool resetCEGUI();
  void createCEGUIWindows();

  //! 更新滚动条
  void updateScrollbars();
  //! 根据滚动条值更新图像的滚动位置。
  void updateImageScrollPosition();

  // 我们用于触发光标更改的处理程序。
  bool handleRegionNorthSouthCursor(const CEGUI::EventArgs &e);
  bool handleRegionEastWestCursor(const CEGUI::EventArgs &e);
  bool handleRegionNorthEastSouthWestCursor(const CEGUI::EventArgs &e);
  bool handleRegionNorthWestSouthEastCursor(const CEGUI::EventArgs &e);
  bool handleRegionMoveCursor(const CEGUI::EventArgs &e);
  bool handleRegionNormalCursor(const CEGUI::EventArgs &e);

  //! 用于自动激活图像区域的处理程序
  bool handleRegionMouseEnter(const CEGUI::EventArgs &e);
  //! 用于自动停用图像区域的处理程序
  bool handleRegionMouseLeave(const CEGUI::EventArgs &e);
  //! 用于在区域激活时做出反应的处理程序
  bool handleRegionActivated(const CEGUI::EventArgs &e);
  //! 用于在区域停用时做出反应的处理程序
  bool handleRegionDeactivated(const CEGUI::EventArgs &e);
  //! 用于在区域更改时更新属性面板的处理程序
  bool handleRegionModified(const CEGUI::EventArgs &e);

  //
  // 下面是 wxWidgets 部分
  //
  /** 处理 wxSizeEvent。将新分辨率传播到 gui 渲染器。*/
  void OnResize(wxSizeEvent &event);

  /** 事件处理程序：通知我们画布需要重绘。我们渲染当前图像集。*/
  void OnPaint(wxPaintEvent &event);

  /** 事件处理程序：通知我们画布需要刷新背景。
   * 如果有背景的话，这是绘制当前背景的好时机。*/
  void OnErase(wxEraseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnMouseMotion(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnLeftDown(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnLeftUp(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnRightDown(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnRightUp(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnMiddleDown(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnMiddleUp(wxMouseEvent &event);

  /** 事件处理程序：我们在这里处理鼠标输入。*/
  void OnMouseWheel(wxMouseEvent &event);

  /** 事件处理程序：处理按键按下事件。我们用它来移动当前选区。*/
  void OnKeyDown(wxKeyEvent &event);

  /** 事件处理程序：停止移动当前选区。*/
  void OnKeyUp(wxKeyEvent &event);

  /** 事件处理程序：滚动条更改 */
  void OnScrollWin(wxScrollWinEvent &event);

  DECLARE_EVENT_TABLE()
};

#endif // _EDITOR_CANVAS_H_
