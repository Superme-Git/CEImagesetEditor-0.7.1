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

#ifndef _EDITOR_DOCUMENT_H_
#define _EDITOR_DOCUMENT_H_

// 标准 wxWidget 头文件
#include "wx/wx.h"
// 更具体的 wxWidgets 头文件
#include "wx/docview.h"
#include "wx/hashmap.h"

#include <CEGUIImageset.h>

class EditorView;

/** 应用程序的文档实现。
 */
class EditorDocument : public wxDocument {
  DECLARE_DYNAMIC_CLASS(EditorDocument)

  // 属性
public:
  /** 构造函数。
   */
  EditorDocument();

  /** 打开给定的图像集文件进行编辑。
   */
  bool OnOpenDocument(const wxString &filename);

  /** 开始一个新的图像集。
   */
  bool OnNewDocument();

  /** 将当前图像集保存到其文件路径。
   */
  bool OnSaveDocument(const wxString &filename);

  /** 当前图像集已关闭。如果有更改，系统已提示用户保存。
   */
  bool OnCloseDocument();

  /** 将文档设置为“脏”并更新附加的视图。*/
  void Modify(bool modify);

  /** 重置文档以进行编辑，例如在更改分辨率后重新启动 Ogre。*/
  void reset();

  // 对文档的可能操作
  void setImageFilename(const wxString &newfilename);
  void setImagesetName(const wxString &newname);
  void setNativeResolution(const wxPoint &newnativeres);
  void setAutoScaled(const bool newAutoScaled);
  void addRegion(const wxString &name, const wxRect &dim);
  void deleteRegion(const wxString &name);
  void deleteAllRegions();
  void setRegionArea(const wxString &name, const wxRect &dim,
                     const bool evt_src = false);
  void renameRegion(const wxString &currentname, const wxString &newname);

  //! 返回区域的当前面积
  wxRect getRegionArea(const wxString &name) const;

private:
  //! 返回我们是否分配了有效视图
  bool isViewValid() const;

  //! 返回指向我们视图的指针（如果有）
  EditorView *getView() const;

  //! 根据定义的区域生成真实 CEGUI 图像集的助手。*/
  CEGUI::Imageset *generateRealImageset();

  //! 返回相对于默认资源组目录的给定路径 'filename'
  wxString getDefaultResourceGroupRelativePath(const wxString &filename) const;

  //! NativeResolution 值的默认值。
  static const wxPoint c_defaultNativeResolution;

  //! 图像集的名称
  wxString m_imagesetName;

  //! 源图像的文件名。
  wxString m_imageFilename;

  // 文档数据
  wxPoint m_nativeResolution;

  // 文档数据
  bool m_autoScaled;

  // 文档数据
  WX_DECLARE_STRING_HASH_MAP(wxRect, mapNamedRegion);

  mapNamedRegion m_mapSetRectangles;
};

#endif // _EDITOR_DOCUMENT_H_
