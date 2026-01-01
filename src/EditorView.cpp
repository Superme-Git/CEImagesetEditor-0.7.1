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

// 标准 wxWidget 头文件
#include "wx/wx.h"
// 用于在 CEGUI 和 wxWidgets 类型之间转换的实用类
#include "CEGUIHelper.h"
#include "CEImagesetEditor.h"
#include "EditorFrame.h"
#include "EditorGLCanvas.h"
#include "EditorView.h"
#include "PropertiesPanel.h"
#include "wx/docview.h"

IMPLEMENT_DYNAMIC_CLASS(EditorView, wxView)

//-----------------------------------------------------------------------
EditorView::EditorView() : m_frame(0) {}

//-----------------------------------------------------------------------
bool EditorView::OnCreate(wxDocument *document, long WXUNUSED(flags)) {
  // 单窗口模式
  m_frame = GetMainFrame();
  m_glcanvasImageset = m_frame->getCanvas();
  m_glcanvasImageset->setView(this);
  m_glcanvasImageset->Reset();
  m_glcanvasImageset->Enable();
  m_propsPanel = m_frame->getPropsPanel();
  m_propsPanel->Enable();
  m_propsPanel->setView(this);

  // 将适当的框架与此视图关联。
  SetFrame(m_frame);

  // 告知框架有关文档的信息
  m_frame->SetDocument(static_cast<EditorDocument *>(document));

  // 确保文档管理器知道这是当前视图。
  Activate(true);

  return true;
}

//-----------------------------------------------------------------------
bool EditorView::OnClose(bool WXUNUSED(deleteWindow)) {
  if (!GetDocument()->Close())
    return false;

  m_glcanvasImageset->ClearBackground();
  m_glcanvasImageset->setView(0);
  m_glcanvasImageset->Disable();
  m_glcanvasImageset = 0;

  m_propsPanel->reset();
  m_propsPanel->Disable();

  SetFrame(0);

  // 告知框架有关文档的信息
  m_frame->SetDocument(0);
  m_frame = 0;

  Activate(false);

  return true;
}

//-----------------------------------------------------------------------
void EditorView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint)) {
  if (m_frame && m_frame->IsShown()) {
    // wxLogDebug(wxT("Needs repaint."));
    m_glcanvasImageset->Refresh(false);
    m_glcanvasImageset->Render();
  }
}

//-----------------------------------------------------------------------
void EditorView::OnDraw(wxDC *dc) {}

//-----------------------------------------------------------------------
void EditorView::onChangeFilename() { updateTitle(); }

//-----------------------------------------------------------------------
void EditorView::onChangeImageFile(const wxString &newfilename) {
  m_propsPanel->setImageFilename(newfilename);
  m_glcanvasImageset->setImage(newfilename);
}

//-----------------------------------------------------------------------
void EditorView::onChangeImagesetName(const wxString &newname) {
  m_propsPanel->setImagesetName(newname);
}

//-----------------------------------------------------------------------
void EditorView::onChangeNativeResolution(const wxPoint &newnativeres) {
  m_propsPanel->setNativeResolution(newnativeres);
}

//-----------------------------------------------------------------------
void EditorView::onChangeAutoScaled(const bool newAutoScaled) {
  m_propsPanel->setAutoScaled(newAutoScaled);
}

//-----------------------------------------------------------------------
void EditorView::onAddRegion(const wxString &name, const wxRect &dim) {
  m_propsPanel->addRegion(name, dim);
  m_glcanvasImageset->addRegion(name, dim);
}

//-----------------------------------------------------------------------
void EditorView::onDelRegion(const wxString &name) {
  m_propsPanel->deleteRegion(name);
  m_glcanvasImageset->deleteRegion(name);
}

//-----------------------------------------------------------------------
void EditorView::onDelAllRegion() {
  m_propsPanel->deleteAllRegions();
  m_glcanvasImageset->deleteAllRegions();
}

//-----------------------------------------------------------------------
void EditorView::onSetRegionArea(const wxString &name, const wxRect &dim,
                                 const bool evt_src) {
  m_propsPanel->setRegionArea(name, dim);

  if (!evt_src)
    m_glcanvasImageset->setRegionArea(name, dim);
}

//-----------------------------------------------------------------------
void EditorView::onRenameRegion(const wxString &currentname,
                                const wxString &newname) {
  m_glcanvasImageset->renameRegion(currentname, newname);
  m_propsPanel->renameRegion(currentname, newname);
}

//-----------------------------------------------------------------------
void EditorView::updateTitle() {
  wxString title(wxTheApp->GetAppName());

  if (GetDocument()) {
    title << wxT("- [") << GetDocument()->GetFilename() << wxT("]");
  }
  if (m_frame) {
    m_frame->SetTitle(title);
  }
}

//-----------------------------------------------------------------------
void EditorView::updateMouseStatusBar(const float posx, const float posy) {
  if (!m_frame)
    return;

  wxChar statusInfo[100];
  wxSnprintf(statusInfo, sizeof(statusInfo) / sizeof(wxChar),
             wxT("pos[%d, %d]"), static_cast<int>(posx),
             static_cast<int>(posy));

  m_frame->GetStatusBar()->SetStatusText(statusInfo, 0);
}

//-----------------------------------------------------------------------
void EditorView::updateDocStatusBar(const float zoom, const float width,
                                    const float height) {
  if (!m_frame)
    return;

  wxChar statusInfo[100];

  wxSnprintf(statusInfo, sizeof(statusInfo) / sizeof(wxChar),
             wxT("size[%d, %d] zoom[%d%%]"), static_cast<int>(width),
             static_cast<int>(height), static_cast<int>(zoom * 100.0f));

  m_frame->GetStatusBar()->SetStatusText(statusInfo, 1);
}
