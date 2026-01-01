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
#ifndef _ELASTIC_BOX_PROPERTIES_H_
#define _ELASTIC_BOX_PROPERTIES_H_

// 属性的基类
#include <CEGUIProperty.h>

// ElasticBoxProperties 命名空间部分开始
/*!
\brief
    包含构成 ElasticBox 基类属性接口的所有类的命名空间。
*/
namespace ElasticBoxProperties {
/*!
\brief
    访问 ElasticBox 可调整大小状态的属性

    \par 用法：
        - Name: Sizable
        - Format: "[text]".

    \par 其中 [Text] 是：
        - "True" 表示 ElasticBox 可由用户调整大小。
        - "False" 表示 ElasticBox 不可由用户调整大小。
*/
class Sizable : public CEGUI::Property {
public:
  Sizable()
      : Property("Sizable",
                 "Property to get/set the sizable state of the ElasticBox.  "
                 "Value is either \"True\" or \"False\".",
                 "True") {}

  CEGUI::String get(const CEGUI::PropertyReceiver *receiver) const;
  void set(CEGUI::PropertyReceiver *receiver, const CEGUI::String &value);
};

/*!
\brief
    访问 ElasticBox 可移动状态的属性

    \par 用法：
        - Name: Movable
        - Format: "[text]".

    \par 其中 [Text] 是：
        - "True" 表示 ElasticBox 可由用户移动。
        - "False" 表示 ElasticBox 不可由用户移动。
*/
class Movable : public CEGUI::Property {
public:
  Movable()
      : Property("Movable",
                 "Property to get/set the movable state of the ElasticBox.  "
                 "Value is either \"True\" or \"False\".",
                 "True") {}

  CEGUI::String get(const CEGUI::PropertyReceiver *receiver) const;
  void set(CEGUI::PropertyReceiver *receiver, const CEGUI::String &value);
};

/*!
\brief
    访问 ElasticBox 上调整大小边框厚度设置的属性。

    \par 用法：
        - Name: SizingBorderThickness
        - Format: "[float]".

    \par 其中：
        - [float] 是调整大小边框的厚度（以像素为单位）。
*/
class SizingBorderThickness : public CEGUI::Property {
public:
  SizingBorderThickness()
      : Property("SizingBorderThickness",
                 "Property to get/set the thickness of the sizing border on "
                 "the ElasticBox.  Value is a float specifying the border "
                 "thickness in pixels.",
                 "3") {}

  CEGUI::String get(const CEGUI::PropertyReceiver *receiver) const;
  void set(CEGUI::PropertyReceiver *receiver, const CEGUI::String &value);
};

} // namespace ElasticBoxProperties

#endif // _ELASTIC_BOX_PROPERTIES_H_
