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
#ifndef _IMAGESET_HANDLER_H_
#define _IMAGESET_HANDLER_H_

#include "CEGUIXMLHandler.h"

class EditorDocument;
/*************************************************************************
实现类
*************************************************************************/
/*!
\brief
用于使用 SAX2 解析 Imageset XML 文件的处理程序类
*/
class ImagesetHandler : public CEGUI::XMLHandler {
public:
  /*************************************************************************
  构造与析构
  *************************************************************************/
  /*!
  \brief
  Imageset::xmlHandler 对象的构造函数

  \param document
  指向创建此 xmlHandler 对象的 EditorDocument 对象的指针
  */
  ImagesetHandler(EditorDocument *document) : m_document(document) {}

  /*!
  \brief
  Imageset::xmlHandler 对象的析构函数
  */
  virtual ~ImagesetHandler(void) {}

  /*************************************************************************
  SAX2 处理程序覆盖
  *************************************************************************/
  /*!
  \brief
  文档处理（只关心元素，架构验证格式）
  */
  virtual void elementStart(const CEGUI::String &element,
                            const CEGUI::XMLAttributes &attributes);
  virtual void elementEnd(const CEGUI::String &element);

  /*************************************************************************
  我们的实现使用的函数
  *************************************************************************/

private:
  /*************************************************************************
  实现常量
  *************************************************************************/
  static const CEGUI::String ImagesetElement; //!< Imageset 元素的标签名称。
  static const CEGUI::String ImageElement;    //!< Image 元素的标签名称。
  static const char ImagesetNameAttribute[];  //!< 存储 Imageset 名称的属性名称
  static const char
      ImagesetImageFileAttribute[]; //!< 存储图像文件文件名的属性名称。
  static const char ImagesetResourceGroupAttribute
      []; //!< 存储加载图像文件时使用的资源组标识符的属性名称。
  static const char
      ImagesetNativeHorzResAttribute[]; //!< 存储 Imageset
                                        //!< 的“原始”水平分辨率的可选属性。
  static const char
      ImagesetNativeVertResAttribute[]; //!< 存储 Imageset
                                        //!< 的“原始”垂直分辨率的可选属性。
  static const char
      ImagesetAutoScaledAttribute[]; //!< 指定 Imageset
                                     //!< 是否应自动缩放的可选属性。
  static const char ImageNameAttribute[]; //!< 存储新 Image 名称的属性名称。
  static const char
      ImageXPosAttribute[]; //!< 存储新 Image 的 x 位置的属性名称。
  static const char
      ImageYPosAttribute[]; //!< 存储新 Image 的 y 位置的属性名称。
  static const char ImageWidthAttribute[];  //!< 存储新 Image 宽度的属性名称。
  static const char ImageHeightAttribute[]; //!< 存储新 Image 高度的属性名称。
  static const char
      ImageXOffsetAttribute[]; //!< 存储新 Image 的 x 渲染偏移量的属性名称。
  static const char
      ImageYOffsetAttribute[]; //!< 存储新 Image 的 y 渲染偏移量的属性名称。

  /*************************************************************************
  实现数据
  *************************************************************************/
  EditorDocument
      *m_document; //!< 保存指向创建处理程序对象的 EditorDocument 的指针
};

#endif // end of guard _IMAGESET_HANDLER_H_
