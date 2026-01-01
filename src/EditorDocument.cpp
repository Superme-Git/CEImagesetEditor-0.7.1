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
#include "wx/filename.h"
#include "wx/wx.h"
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIImageset.h>
#include <CEGUISystem.h>
#include <CEGUIXMLParser.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>

#include "CEImagesetEditor.h"
// 更具体的 wxWidgets 头文件
#include "CEGUIHelper.h"
#include "EditorDocument.h"
#include "EditorView.h"
#include "ImagesetHandler.h"
#include "wx/docview.h"

using namespace CEGUI;

IMPLEMENT_DYNAMIC_CLASS(EditorDocument, wxDocument)

// 在 Imageset::xmlHandler 中声明
const wxPoint EditorDocument::c_defaultNativeResolution(wxPoint(640, 480));

//-----------------------------------------------------------------------
EditorDocument::EditorDocument() { reset(); }

//-----------------------------------------------------------------------
bool EditorDocument::OnOpenDocument(const wxString &filename) {
  if (filename.empty() || !wxDocument::OnOpenDocument(filename))
    return false;

  wxString relFilename(getDefaultResourceGroupRelativePath(filename));

  // 创建处理程序对象
  ImagesetHandler handler(this);

  // 这可能需要一些时间，进入“忙碌”状态
  wxWindowDisabler wd;
  wxBusyCursor bc;

  // 执行解析（使用处理程序创建实际数据）
  try {
    System::getSingleton().getXMLParser()->parseXMLFile(
        handler, CEGUIHelper::ToCEGUIString(relFilename), "Imageset.xsd", "");
  } catch (...) {
    Logger::getSingleton().logEvent(
        "EditorDocument::onOpenDocument - loading of Imageset from file '" +
            CEGUIHelper::ToCEGUIString(filename) + "' failed.",
        Errors);
    throw;
  }

  // 既然我们要覆盖此方法，我们就必须自己通知视图！
  UpdateAllViews();

  SetFilename(filename, true);
  SetTitle(filename);

  Modify(false);

  return true;
}

//-----------------------------------------------------------------------
bool EditorDocument::OnSaveDocument(const wxString &filename) {
  try {
    Imageset *newImageset = generateRealImageset();

    std::ofstream fileSave;

    fileSave.open(static_cast<const char *>(wxConvLibc.cWX2MB(filename)),
                  std::ios::out);
    if (!fileSave.is_open()) {
      wxLogError(
          wxT("Error: could not open file for saving. Is it read-only?"));
      return false;
    }

    ImagesetManager::getSingleton().writeImagesetToStream(
        CEGUIHelper::ToCEGUIString(m_imagesetName), fileSave);

    fileSave.close();

    // 清理图像集
    ImagesetManager::getSingleton().destroy(*newImageset);

    // 告诉文档它再次变得“干净”了
    Modify(false);
  } catch (CEGUI::Exception &e) {
    wxLogError(wxT("EditorDocument::OnSaveDocument - Error: %s"),
               CEGUIHelper::ToWXString(e.getMessage()).c_str());
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------
bool EditorDocument::OnCloseDocument() {
  reset();
  return true;
}

//-----------------------------------------------------------------------
bool EditorDocument::OnNewDocument() {
  wxDocument::OnNewDocument();

  reset();

  // 既然我们要覆盖此方法，我们就必须自己通知视图！
  UpdateAllViews();

  return true;
}

//-----------------------------------------------------------------------
void EditorDocument::reset() {
  deleteAllRegions();
  setImagesetName(wxT(""));
  setImageFilename(wxT(""));
  setNativeResolution(c_defaultNativeResolution);
  setAutoScaled(false);
  Modify(false);
}

//-----------------------------------------------------------------------
void EditorDocument::setImageFilename(const wxString &newfilename) {
  if (m_imageFilename != newfilename) {
    m_imageFilename = newfilename;

    if (isViewValid())
      getView()->onChangeImageFile(newfilename);

    Modify(true);
  }
}

//-----------------------------------------------------------------------
void EditorDocument::setImagesetName(const wxString &newname) {
  if (m_imagesetName != newname) {
    m_imagesetName = newname;

    if (isViewValid())
      getView()->onChangeImagesetName(newname);

    Modify(true);
  }
}

//-----------------------------------------------------------------------
void EditorDocument::setNativeResolution(const wxPoint &newnativeres) {
  if (m_nativeResolution != newnativeres) {
    m_nativeResolution = newnativeres;

    if (isViewValid())
      getView()->onChangeNativeResolution(newnativeres);

    Modify(true);
  }
}

//-----------------------------------------------------------------------
void EditorDocument::setAutoScaled(const bool newAutoScaled) {
  if (m_autoScaled != newAutoScaled) {
    m_autoScaled = newAutoScaled;

    if (isViewValid())
      getView()->onChangeAutoScaled(newAutoScaled);

    Modify(true);
  }
}

//-----------------------------------------------------------------------
void EditorDocument::addRegion(const wxString &name, const wxRect &dim) {
  m_mapSetRectangles[name] = dim;

  if (isViewValid())
    getView()->onAddRegion(name, dim);

  Modify(true);
}

//-----------------------------------------------------------------------
void EditorDocument::deleteRegion(const wxString &name) {
  m_mapSetRectangles.erase(name);

  if (isViewValid())
    getView()->onDelRegion(name);

  Modify(true);
}

//-----------------------------------------------------------------------
void EditorDocument::deleteAllRegions() {
  m_mapSetRectangles.clear();

  if (isViewValid())
    getView()->onDelAllRegion();

  Modify(true);
}

//-----------------------------------------------------------------------
void EditorDocument::setRegionArea(const wxString &name, const wxRect &dim,
                                   const bool evt_src) {
  // 如果区域相同，则不执行任何操作（避免无限递归的可能性）
  if (getRegionArea(name) == dim)
    return;

  m_mapSetRectangles[name] = dim;

  if (isViewValid())
    getView()->onSetRegionArea(name, dim, evt_src);

  Modify(true);
}

//-----------------------------------------------------------------------
wxRect EditorDocument::getRegionArea(const wxString &name) const {
  wxRect area;

  mapNamedRegion::const_iterator iter = m_mapSetRectangles.find(name);

  if (iter != m_mapSetRectangles.end())
    area = iter->second;

  return area;
}

//-----------------------------------------------------------------------
void EditorDocument::renameRegion(const wxString &currentname,
                                  const wxString &newname) {
  // 检查新名称是否在 CEGUI 中不存在
  if (WindowManager::getSingleton().isWindowPresent(
          CEGUIHelper::ToCEGUIString(newname))) {
    wxLogError(wxT("EditorDocument::renameRegion - Error: A region with that "
                   "name already exists."));
  }
  // 新名称没问题，继续重命名
  else {
    wxRect dim = m_mapSetRectangles[currentname];
    m_mapSetRectangles.erase(currentname);
    m_mapSetRectangles[newname] = dim;

    if (isViewValid())
      getView()->onRenameRegion(currentname, newname);

    Modify(true);
  }
}

//-----------------------------------------------------------------------
void EditorDocument::Modify(bool modify) {
  wxDocument::Modify(modify);
  UpdateAllViews();
}

//-----------------------------------------------------------------------
CEGUI::Imageset *EditorDocument::generateRealImageset() {
  const String imagesetName(CEGUIHelper::ToCEGUIString(m_imagesetName));
  const String imageFilename(CEGUIHelper::ToCEGUIString(m_imageFilename));

  ImagesetManager &isMgr = ImagesetManager::getSingleton();

  if (isMgr.isDefined(imagesetName))
    // TODO: Maybe ask user whether to continue here?
    isMgr.destroy(imagesetName);

  // 创建图像集
  Imageset &imageset(isMgr.createFromImageFile(imagesetName, imageFilename));

  // 自动缩放选项
  imageset.setAutoScalingEnabled(m_autoScaled);
  imageset.setNativeResolution(
      CEGUI::Size(m_nativeResolution.x, m_nativeResolution.y));

  // 迭代类中的所有元素
  mapNamedRegion::iterator it;
  for (it = m_mapSetRectangles.begin(); it != m_mapSetRectangles.end(); ++it) {
    const String imageName = CEGUIHelper::ToCEGUIString(wxString(it->first));

    if (!imageset.isImageDefined(imageName)) {
      const wxRect r(it->second);
      const CEGUI::Rect newDim(
          ((r.GetLeft() > 0) ? r.GetLeft() : 0),
          ((r.GetTop() > 0) ? r.GetTop() : 0),
          ((r.GetRight() + 1 > 0) ? r.GetRight() + 1 : 0),
          ((r.GetBottom() + 1 > 0) ? r.GetBottom() + 1 : 0));
      const CEGUI::Point p(0.0f, 0.0f);
      imageset.defineImage(imageName, newDim, p);
    }
  }

  if (imageset.isImageDefined("full_image"))
    imageset.undefineImage("full_image");

  return &imageset;
}

//-----------------------------------------------------------------------
bool EditorDocument::isViewValid() const { return GetFirstView() != 0; }

//-----------------------------------------------------------------------
EditorView *EditorDocument::getView() const {
  return static_cast<EditorView *>(GetFirstView());
}

//-----------------------------------------------------------------------
wxString EditorDocument::getDefaultResourceGroupRelativePath(
    const wxString &filename) const {
  wxString resourcePath(wxT("./"));
  wxFileName filespec(filename);

  CEGUI::DefaultResourceProvider *rp =
      static_cast<CEGUI::DefaultResourceProvider *>(
          CEGUI::System::getSingleton().getResourceProvider());

  CEGUI::String defResGrp = rp->getDefaultResourceGroup();

  if (!defResGrp.empty())
    resourcePath =
        CEGUIHelper::ToWXString(rp->getResourceGroupDirectory(defResGrp));

  filespec.MakeRelativeTo(resourcePath);
  return filespec.GetFullPath();
}
