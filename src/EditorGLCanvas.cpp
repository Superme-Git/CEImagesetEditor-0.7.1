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

#include "Config.h"

#include "CEGUIHelper.h"
#include "CEImagesetEditor.h"
#include "EditorGLCanvas.h"
#include "EditorView.h"
#include "ElasticBox.h"
#include "ElasticBoxWindowRenderer.h"
#include "PropertiesPanel.h"

// CEGUI 包含
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIExceptions.h>
#include <CEGUIGlobalEventSet.h>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIPropertyHelper.h>
#include <CEGUIScheme.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowFactoryManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindowRendererManager.h>
#include <RendererModules/OpenGL/CEGUIOpenGLTexture.h>

// wx 包含
#include <wx/stdpaths.h>

#define S_(X) #X
#define STRINGIZE(X) S_(X)

using namespace CEGUI;

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE(EditorGLCanvas, wxGLCanvas)
EVT_SIZE(EditorGLCanvas::OnResize)
EVT_PAINT(EditorGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(EditorGLCanvas::OnErase)
EVT_MOTION(EditorGLCanvas::OnMouseMotion)
EVT_LEFT_DOWN(EditorGLCanvas::OnLeftDown)
EVT_LEFT_UP(EditorGLCanvas::OnLeftUp)
EVT_RIGHT_DOWN(EditorGLCanvas::OnRightDown)
EVT_RIGHT_UP(EditorGLCanvas::OnRightUp)
EVT_MIDDLE_DOWN(EditorGLCanvas::OnMiddleDown)
EVT_MOUSEWHEEL(EditorGLCanvas::OnMouseWheel)
EVT_KEY_DOWN(EditorGLCanvas::OnKeyDown)
EVT_KEY_UP(EditorGLCanvas::OnKeyUp)
EVT_SCROLLWIN(EditorGLCanvas::OnScrollWin)
END_EVENT_TABLE()

//-----------------------------------------------------------------------
EditorGLCanvas::EditorGLCanvas(EditorView *v, wxWindow *parent,
                               const wxPoint &pos, const wxSize &size)
    : wxGLCanvas(parent, wxID_ANY, NULL, pos, size,
                 wxSUNKEN_BORDER | wxVSCROLL | wxHSCROLL),
      m_view(v), m_GUISystem(0), m_GUIRenderer(0), m_context(0),
      m_imagesetStaticImage(0), m_useBigSteps(true), m_zoomFactor(1.0f),
      m_realWidth(0), m_realHeight(0), m_scrollPosX(0), m_scrollDocX(0),
      m_scrollPageX(0), m_scrollPosY(0), m_scrollDocY(0), m_scrollPageY(0) {
  // 创建 OpenGL 上下文（wxWidgets 3.0+ 需要）
  m_context = new wxGLContext(this);

  // 初始化一次性内容
  SetCurrent(*m_context);

  // 我们将使用十字光标
  SetCursor(wxCURSOR_CROSS);

  // 设置 CEGUI 系统
  initialiseCEGUI();
}

//-----------------------------------------------------------------------
EditorGLCanvas::~EditorGLCanvas() {
  // 清理 CEGUI。
  cleanupCEGUI();

  // 删除 OpenGL 上下文
  delete m_context;
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setImage(const wxString &name) {
  // 这可能需要一些时间，进入“忙碌”状态
  wxWindowDisabler wd;
  wxBusyCursor bc;

  // 删除用于主图像文件显示的旧图像集
  if (ImagesetManager::getSingleton().isDefined("__auto_editor_imageset__"))
    ImagesetManager::getSingleton().destroy("__auto_editor_imageset__");

  // 重置所有图像信息
  m_realWidth = 0;
  m_realHeight = 0;
  m_imagesetStaticImage->setProperty("Image", "");

  // 将新图像文件加载为图像集
  if (!name.IsEmpty()) {
    try {
      Imageset &imageset = ImagesetManager::getSingleton().createFromImageFile(
          "__auto_editor_imageset__", CEGUIHelper::ToCEGUIString(name));

      imageset.setAutoScalingEnabled(false);

      // 将图像设置为我们的主 'StaticImage' 小部件（以便显示）
      assert(imageset.isImageDefined("full_image"));
      m_imagesetStaticImage->setProperty(
          "Image", "set:__auto_editor_imageset__ image:full_image");

      // 更新我们持有的关于当前源图像的详细信息
      const Image &img(imageset.getImage("full_image"));
      CEGUI::Size sizeImage = img.getSize();
      m_realWidth = sizeImage.d_width;
      m_realHeight = sizeImage.d_height;

      // 一个很好的 OpenGL 渲染器滥用 hack，用于更改图像纹理的过滤。
      // 伙计...感觉真好。啊！:-D
      GLuint tex = static_cast<CEGUI::OpenGLTexture *>(imageset.getTexture())
                       ->getOpenGLTexture();
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } catch (CEGUI::Exception &e) {
      wxLogError(wxT("EditorGLCanvas::setImage - Error: %s"),
                 CEGUIHelper::ToWXString(e.getMessage()).c_str());
    }
  }

  // 静音全局事件，以便如果由于新图像而调整大小或移动区域，文档不会更改。
  CEGUI::GlobalEventSet::getSingleton().setMutedState(true);
  // 设置主图像大小。
  m_imagesetStaticImage->setSize(
      UVector2(UDim(0, m_realWidth), UDim(0, m_realHeight)));

  // 我们甚至有文档吗？
  if (isDocumentValid()) {
    EditorDocument *doc = (EditorDocument *)m_view->GetDocument();

    // 对于每个区域窗口，将其大小与文档为其保留的大小重新同步。
    for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i) {
      CEGUI::Window *wnd = m_imagesetStaticImage->getChildAtIdx(i);

      if (wnd->testClassName("ElasticBox")) {
        wxString name(CEGUIHelper::ToWXString(wnd->getName()));
        // 从文档中提取正确的大小
        wxRect area(doc->getRegionArea(name));
        // 设置该区域的区域
        setRegionArea(name, area);
      }
    }
  }

  // 再次取消静音全局事件。
  CEGUI::GlobalEventSet::getSingleton().setMutedState(false);

  // 更新缩放
  computeZoomFactor(0);

  Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::changeSize(int newWidth, int newHeight) {
  // 更新 OpenGL 视口区域
  glViewport(0, 0, static_cast<GLsizei>(newWidth),
             static_cast<GLsizei>(newHeight));

  // 通知 CEGUI 渲染器更改
  m_GUISystem->notifyDisplaySizeChanged(
      CEGUI::Size(static_cast<float>(newWidth), static_cast<float>(newHeight)));

  // 无论如何强制完全重绘
  m_GUISystem->signalRedraw();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnResize(wxSizeEvent &event) {
  // 我们需要客户区大小
  int clientWidth, clientHeight;
  GetClientSize(&clientWidth, &clientHeight);

  changeSize(clientWidth, clientHeight);

  updateScrollbars();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::initialiseCEGUI() {
  // 初始化 OpenGL 渲染器和 CEGUI 系统
  m_GUIRenderer = &OpenGLRenderer::create();
  m_GUISystem = &System::create(*m_GUIRenderer);

// 除非我们在调试，否则将日志记录保持在最低限度
#if defined(DEBUG) || defined(_DEBUG)
  Logger::getSingleton().setLoggingLevel(Informative);
#else
  Logger::getSingleton().setLoggingLevel(Errors);
#endif

  // 为我们的自定义 'ElasticBox' 小部件添加对 CEGUI 的支持
  WindowFactoryManager::getSingleton().addFactory(&getElasticBoxFactory());
  WindowRendererManager::getSingleton().addFactory(&getElasticBoxWRFactory());

  DefaultResourceProvider *rp = static_cast<DefaultResourceProvider *>(
      m_GUISystem->getResourceProvider());

  // 构建可以找到我们所需文件的字符串。
  String dataDir(
      CEGUIHelper::ToCEGUIString(wxStandardPaths::Get().GetResourcesDir()));
  if (!dataDir.empty() && (dataDir[dataDir.length() - 1] != '/'))
    dataDir += '/';

// 在 Windows 上，此处的数据文件将在 'data' 子目录中
#if defined(__WIN32__) || defined(_WIN32)
  dataDir += "data/";
#endif

  // 设置数据路径
  rp->setResourceGroupDirectory("editor_data", dataDir);
  rp->setDefaultResourceGroup("editor_data");

  // 加载编辑器中使用的 CEGUI 方案
  SchemeManager::getSingleton().create("CEImagesetEditor.scheme");

  // 在全局事件集上设置一些订阅
  GlobalEventSet &ges = CEGUI::GlobalEventSet::getSingleton();
  ges.subscribeEvent(
      "ElasticBox/SetNorthSouthCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionNorthSouthCursor, this));
  ges.subscribeEvent(
      "ElasticBox/SetEastWestCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionEastWestCursor, this));
  ges.subscribeEvent(
      "ElasticBox/SetNorthEastSouthWestCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionNorthEastSouthWestCursor,
                        this));
  ges.subscribeEvent(
      "ElasticBox/SetNorthWestSouthEastCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionNorthWestSouthEastCursor,
                        this));
  ges.subscribeEvent(
      "ElasticBox/SetMoveCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionMoveCursor, this));
  ges.subscribeEvent(
      "ElasticBox/SetNormalCursor",
      Event::Subscriber(&EditorGLCanvas::handleRegionNormalCursor, this));

  // 我们全局订阅这些事件，因为它允许我们在需要时快速“静音”所有区域的这些事件。
  ges.subscribeEvent(
      "Window/Moved",
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionModified, this));
  ges.subscribeEvent(
      "Window/Sized",
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionModified, this));

  // 重置默认资源组，以便当用户尝试加载某些文件时，我们要去我们的数据目录中搜索。
  rp->setDefaultResourceGroup("");

  createCEGUIWindows();

  // 将 CEGUI 设置为编辑器所需的初始状态
  resetCEGUI();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::cleanupCEGUI() {
  CEGUI::System::destroy();
  CEGUI::OpenGLRenderer::destroy(*m_GUIRenderer);
  m_GUISystem = 0;
  m_GUIRenderer = 0;
}

//-----------------------------------------------------------------------
void EditorGLCanvas::createCEGUIWindows() {
  // 获取窗口管理器，我们将在这里用它来完成一些工作。
  WindowManager &winMgr = WindowManager::getSingleton();

  try {
    m_imagesetStaticImage = winMgr.createWindow("CEImagesetEditor/StaticImage",
                                                "__editor_static_image__");
    m_imagesetStaticImage->setMaxSize(
        UVector2(UDim(0.0f, 20480.0f), UDim(0.0f, 20480.0f)));
    m_imagesetStaticImage->setPosition(
        UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
    m_imagesetStaticImage->setSize(
        UVector2(UDim(0, m_realWidth), UDim(0, m_realHeight)));
    m_GUISystem->setGUISheet(m_imagesetStaticImage);
  } catch (Exception &guiException) {
    wxLogError(wxT("%s"),
               CEGUIHelper::ToWXString(guiException.getMessage()).c_str());
  }
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::resetCEGUI() {
  // 清理区域
  deleteAllRegions();

  // 将图像大小重置为 0
  m_realWidth = 0;
  m_realHeight = 0;
  m_imagesetStaticImage->setSize(
      UVector2(UDim(0, m_realWidth), UDim(0, m_realHeight)));

  return true;
}

//-----------------------------------------------------------------------
void EditorGLCanvas::selectRegion(const wxString &name) {
  if (m_imagesetStaticImage->isChild(CEGUIHelper::ToCEGUIString(name))) {
    CEGUI::Window *selectedWin =
        m_imagesetStaticImage->getChild(CEGUIHelper::ToCEGUIString(name));

    if (!selectedWin->isActive()) {
      selectedWin->activate();
      Render();
    }
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::renameRegion(const wxString &currentname,
                                  const wxString &newname) {
  if (m_imagesetStaticImage->isChild(CEGUIHelper::ToCEGUIString(currentname))) {
    CEGUI::Window *toolbox = m_imagesetStaticImage->getChild(
        CEGUIHelper::ToCEGUIString(currentname));
    toolbox->rename(CEGUIHelper::ToCEGUIString(newname));
    selectRegion(newname);
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::addRegion(const wxString &name, const wxRect &dim) {
  ElasticBox *toolbox =
      static_cast<ElasticBox *>(WindowManager::getSingleton().createWindow(
          "CEImagesetEditor/ImageRegion", CEGUIHelper::ToCEGUIString(name)));

  float posx = 0, posy = 0, width = 0, height = 0;

  if ((m_realWidth != 0) && (m_realHeight != 0)) {
    posx = static_cast<float>(dim.GetX()) * m_zoomFactor;
    posy = static_cast<float>(dim.GetY()) * m_zoomFactor;
    width = static_cast<float>(dim.GetWidth()) * m_zoomFactor;
    height = static_cast<float>(dim.GetHeight()) * m_zoomFactor;
  }
  // 设置大小和位置（所有相对坐标）
  toolbox->setPosition(UVector2(UDim(0.0f, posx), UDim(0.0f, posy)));
  toolbox->setSize(UVector2(UDim(0.0f, width), UDim(0.0f, height)));
  // 设置我们用于这些 ElasticBox 小部件的标准选项
  toolbox->setSizingBorderThickness(3.0f);
  toolbox->setScaleSnap(m_zoomFactor);
  // 将此新框添加到主图像窗口。
  m_imagesetStaticImage->addChildWindow(toolbox);

  // 订阅一些事件...
  toolbox->subscribeEvent(
      ElasticBox::EventActivated,
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionActivated, this));

  toolbox->subscribeEvent(
      ElasticBox::EventDeactivated,
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionDeactivated, this));

  toolbox->subscribeEvent(
      ElasticBox::EventMouseEnters,
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionMouseEnter, this));

  toolbox->subscribeEvent(
      ElasticBox::EventMouseLeaves,
      CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionMouseLeave, this));

  // 最后执行此操作，以便调用事件处理程序
  toolbox->activate();

  // 更新显示。
  Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setRegionArea(const wxString &name, const wxRect &dim) {
  if (m_imagesetStaticImage->isChild(CEGUIHelper::ToCEGUIString(name))) {
    CEGUI::Window *win =
        m_imagesetStaticImage->getChild(CEGUIHelper::ToCEGUIString(name));

    float posx = static_cast<float>(dim.GetX()) * m_zoomFactor;
    float posy = static_cast<float>(dim.GetY()) * m_zoomFactor;
    float width = static_cast<float>(dim.GetWidth()) * m_zoomFactor;
    float height = static_cast<float>(dim.GetHeight()) * m_zoomFactor;

    win->setArea(URect(UDim(0.0f, posx), UDim(0.0f, posy),
                       UDim(0.0f, posx + width), UDim(0.0f, posy + height)));
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::deleteRegion(const wxString &name) {
  if (m_imagesetStaticImage->isChild(CEGUIHelper::ToCEGUIString(name))) {
    m_imagesetStaticImage->getChild(CEGUIHelper::ToCEGUIString(name))
        ->destroy();
    Render();
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::deleteAllRegions() {
  typedef std::vector<CEGUI::Window *> DeleteList;
  DeleteList deletables;

  // 收集所有附加的 ElasticBox：
  for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i) {
    CEGUI::Window *wnd = m_imagesetStaticImage->getChildAtIdx(i);

    if (wnd->testClassName("ElasticBox"))
      deletables.push_back(wnd);
  }

  // 删除所有 ElasticBox
  for (DeleteList::iterator it = deletables.begin(); it != deletables.end();
       ++it)
    // 是的，是的，我知道打破了我自己的规则 :-p
    (*it)->destroy();

  Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMouseMotion(wxMouseEvent &event) {
  this->SetFocus();

  // 看看我们是否想拖动编辑器图像视图
  if (event.Dragging() && event.MiddleIsDown()) {
    // 提取鼠标位置
    wxPoint pt(event.GetPosition());
    // 计算移动
    wxPoint change = m_dragPoint - pt;
    // 更新拖动点
    m_dragPoint = pt;

    // 设置新的滚动条位置
    m_scrollPosX += change.x;
    m_scrollPosY += change.y;
    updateScrollbars();
    // 确保图像位置同步
    updateImageScrollPosition();
  }
  // 未拖动，因此执行正常的 CEGUI 输入注入。
  else {
    // 将事件告知 CEGUI
    System::getSingleton().injectMousePosition(event.GetX(), event.GetY());

    // 检查这一点，因为我们可能会在文档关闭期间收到此事件
    if (isDocumentValid() &&
        (event.GetX() != m_lastMouseX || event.GetY() != m_lastMouseY)) {
      m_lastMouseX = event.GetX();
      m_lastMouseY = event.GetY();

      // 使用图像集上的鼠标位置更新状态栏（计算偏移和缩放）
      float mousePosX = m_imagesetStaticImage->getXPosition().asAbsolute(
          m_imagesetStaticImage->getParentPixelWidth());
      float mousePosY = m_imagesetStaticImage->getYPosition().asAbsolute(
          m_imagesetStaticImage->getParentPixelHeight());
      mousePosX = (m_lastMouseX - mousePosX) / m_zoomFactor;
      mousePosY = (m_lastMouseY - mousePosY) / m_zoomFactor;
      m_view->updateMouseStatusBar(static_cast<int>(mousePosX),
                                   static_cast<int>(mousePosY));

      // 使用缩放和图像集大小更新状态栏
      m_view->updateDocStatusBar(m_zoomFactor, m_realWidth, m_realHeight);
    }
  }

  // 将鼠标移动触发的重绘限制为仅必要的重绘
  if (m_GUISystem->isRedrawRequested())
    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMouseWheel(wxMouseEvent &event) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    int MouseWheelDirection = event.GetWheelRotation();
    computeZoomFactor(MouseWheelDirection);
    m_view->updateDocStatusBar(m_zoomFactor, m_realWidth, m_realHeight);
    Render();
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnLeftDown(wxMouseEvent &event) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    // 将事件告知 CEGUI
    System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    Render();
  }

  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnLeftUp(wxMouseEvent &event) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    // 将事件告知 CEGUI
    System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    Render();
  }

  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnRightDown(wxMouseEvent &event) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    // 将事件告知 CEGUI
    System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
    Render();
  }

  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnRightUp(wxMouseEvent &event) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    // 将事件告知 CEGUI
    System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
    Render();
  }

  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMiddleDown(wxMouseEvent &event) {
  // 初始化拖动开始的点
  m_dragPoint = event.GetPosition();
  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::computeZoomFactor(int MouseWheelDirection) {
  // Logger::getSingleton().logEvent("imagesetSize '" + AppliConfigFile +"'.",
  // Standard);
  if (MouseWheelDirection > 0)
    m_zoomFactor += 0.125;
  else if (MouseWheelDirection < 0)
    m_zoomFactor -= 0.125;

  // 极低下限验证
  if (m_zoomFactor < 0.25)
    m_zoomFactor = 0.25;

  // 极高上限验证
  if (m_zoomFactor > 10.0)
    m_zoomFactor = 10.0;

  float width = m_zoomFactor * m_realWidth;
  float height = m_zoomFactor * m_realHeight;

  // 静音事件以阻止我们产生大量不需要的噪音
  CEGUI::GlobalEventSet::getSingleton().setMutedState(true);
  // 更新主图像的大小
  m_imagesetStaticImage->setSize(
      UVector2(cegui_absdim(width), cegui_absdim(height)));

  // 为所有定义的图像区域设置捕捉比例
  for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i) {
    CEGUI::Window *wnd = m_imagesetStaticImage->getChildAtIdx(i);

    if (wnd->testClassName("ElasticBox")) {
      float old_zoom = static_cast<ElasticBox *>(wnd)->getScaleSnap();
      static_cast<ElasticBox *>(wnd)->setScaleSnap(m_zoomFactor);

      URect area(wnd->getArea());
      // 取消旧的缩放比例
      area.d_min.d_x.d_offset /= old_zoom;
      area.d_min.d_y.d_offset /= old_zoom;
      area.d_max.d_x.d_offset /= old_zoom;
      area.d_max.d_y.d_offset /= old_zoom;
      // 应用新的缩放比例
      area.d_min.d_x.d_offset *= m_zoomFactor;
      area.d_min.d_y.d_offset *= m_zoomFactor;
      area.d_max.d_x.d_offset *= m_zoomFactor;
      area.d_max.d_y.d_offset *= m_zoomFactor;
      // 为新的缩放系数更新窗口矩形
      wnd->setArea(area);
    }
  }

  // 允许再次触发事件。
  CEGUI::GlobalEventSet::getSingleton().setMutedState(false);

  updateScrollbars();
  // 在这里，我们调用我们的肮脏 hack 来解决 wxMac 刷新问题。
  GetMainFrame()->Refresh();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::Reset() { return resetCEGUI(); }

//-----------------------------------------------------------------------
void EditorGLCanvas::Render() {
  SetCurrent();

  // 清除颜色缓冲区
  glClear(GL_COLOR_BUFFER_BIT);

  // CEGUI 渲染
  renderImageset();

  glFlush();
  SwapBuffers();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::isDocumentValid() const {
  return (m_view && m_view->GetDocument());
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnPaint(wxPaintEvent &event) {
  // 我们需要这样做以避免泛滥的绘制事件
  wxPaintDC paintDC(this);

  Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::renderImageset() {
  if (CEGUI::System::getSingletonPtr())
    CEGUI::System::getSingleton().renderGUI();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnErase(wxEraseEvent &event) {}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnKeyDown(wxKeyEvent &event) {}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnKeyUp(wxKeyEvent &event) {
  // Control 键用于按键将选择更改为小步长（当您靠近目标位置时。）
  if (event.ControlDown()) {
    m_useBigSteps = true;
  }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnScrollWin(wxScrollWinEvent &event) {
  int orientation = event.GetOrientation();
  int adjustment, page, range, position;

  if (orientation == wxVERTICAL) {
    page = m_scrollPageY;
    range = m_scrollDocY;
    position = m_scrollPosY;
  } else {
    page = m_scrollPageX;
    range = m_scrollDocX;
    position = m_scrollPosX;
  }

  wxEventType evtype = event.GetEventType();

  if (evtype == wxEVT_SCROLLWIN_TOP)
    adjustment = -position;
  else if (evtype == wxEVT_SCROLLWIN_BOTTOM)
    adjustment = range - page - position;
  else if (evtype == wxEVT_SCROLLWIN_LINEDOWN)
    adjustment = 1;
  else if (evtype == wxEVT_SCROLLWIN_LINEUP)
    adjustment = -1;
  else if (evtype == wxEVT_SCROLLWIN_PAGEDOWN)
    adjustment = page;
  else if (evtype == wxEVT_SCROLLWIN_PAGEUP)
    adjustment = -page;
  else if (evtype == wxEVT_SCROLLWIN_THUMBTRACK)
    adjustment = event.GetPosition() - position;
  else if (evtype == wxEVT_SCROLLWIN_THUMBRELEASE)
    adjustment = event.GetPosition() - position;
  else
    adjustment = 0;

  position += adjustment;

  if (orientation == wxVERTICAL)
    m_scrollPosY = position;
  else
    m_scrollPosX = position;

  updateScrollbars();
  updateImageScrollPosition();
  event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setBackgroundColour(const wxColour &colour) {
  glClearColor(colour.Red() / 255.0f, colour.Green() / 255.0f,
               colour.Blue() / 255.0f, 1.0f);
  Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::updateScrollbars() {
  // 更新此信息的内部跟踪。
  GetClientSize(&m_scrollPageX, &m_scrollPageY);
  m_scrollDocX = static_cast<int>(m_realWidth * m_zoomFactor);
  m_scrollDocY = static_cast<int>(m_realHeight * m_zoomFactor);

  // 更新滚动条
  SetScrollbar(wxVERTICAL, m_scrollPosY, m_scrollPageY, m_scrollDocY);
  SetScrollbar(wxHORIZONTAL, m_scrollPosX, m_scrollPageX, m_scrollDocX);

  // 最后，更新我们的本地副本（省去我们自己的约束）
  m_scrollPosX = GetScrollPos(wxHORIZONTAL);
  m_scrollPosY = GetScrollPos(wxVERTICAL);
}

//-----------------------------------------------------------------------
void EditorGLCanvas::updateImageScrollPosition() {
  m_imagesetStaticImage->setPosition(
      UVector2(UDim(0, 0 - m_scrollPosX), UDim(0, 0 - m_scrollPosY)));

  Render();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionActivated(const CEGUI::EventArgs &e) {
  const ActivationEventArgs &args = static_cast<const ActivationEventArgs &>(e);
  ElasticBox *win = static_cast<ElasticBox *>(args.window);

  if (win) {
    win->setAlpha(0.75f);

    // 检查这一点，因为我们可能会在文档关闭期间收到此事件
    if (isDocumentValid()) {
      wxString name = CEGUIHelper::ToWXString(win->getName());
      m_view->getPropsPanel()->selectRegion(name);
    }
  }

  // 事件已处理
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionDeactivated(const CEGUI::EventArgs &e) {
  const ActivationEventArgs &args = static_cast<const ActivationEventArgs &>(e);
  ElasticBox *win = static_cast<ElasticBox *>(args.window);

  if (win) {
    win->setAlpha(0.3f);
  }

  // 事件已处理
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMouseEnter(const EventArgs &e) {
  const WindowEventArgs &wea = static_cast<const WindowEventArgs &>(e);
  ElasticBox *win = static_cast<ElasticBox *>(wea.window);

  if (win)
    win->activate();

  // 事件已处理
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMouseLeave(const EventArgs &e) {
  const WindowEventArgs &wea = static_cast<const WindowEventArgs &>(e);
  ElasticBox *win = static_cast<ElasticBox *>(wea.window);

  if (win)
    win->deactivate();

  // 这样做，我们才不会卡在“调整大小”类型的光标上
  SetCursor(wxCURSOR_CROSS);

  // 事件已处理
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionModified(const EventArgs &e) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    CEGUI::Window *wnd = static_cast<const WindowEventArgs &>(e).window;

    if (wnd && wnd->testClassName("ElasticBox")) {
      wxString name = CEGUIHelper::ToWXString(wnd->getName());
      float posx = wnd->getXPosition().d_offset / m_zoomFactor;
      float posy = wnd->getYPosition().d_offset / m_zoomFactor;
      float width = wnd->getWidth().d_offset / m_zoomFactor;
      float height = wnd->getHeight().d_offset / m_zoomFactor;

      EditorDocument *document =
          static_cast<EditorDocument *>(m_view->GetDocument());
      document->setRegionArea(
          name,
          wxRect(static_cast<int>(posx), static_cast<int>(posy),
                 static_cast<int>(width), static_cast<int>(height)),
          true);

      // 事件已处理
      return true;
    }
  }

  // 事件未处理
  return false;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthSouthCursor(const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_SIZENS);
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionEastWestCursor(const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_SIZEWE);
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthEastSouthWestCursor(
    const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_SIZENESW);
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthWestSouthEastCursor(
    const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_SIZENWSE);
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMoveCursor(const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_CROSS);
  return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNormalCursor(const CEGUI::EventArgs &e) {
  SetCursor(wxCURSOR_CROSS);
  return true;
}
