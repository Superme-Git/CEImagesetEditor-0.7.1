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
#include "ElasticBoxProperties.h"

// 使用 PropertyHelper转换字符串
#include <CEGUIPropertyHelper.h>
// 这些属性操作的类
#include "ElasticBox.h"

// ElasticBoxProperties 命名空间部分开始
namespace ElasticBoxProperties {

CEGUI::String Sizable::get(const CEGUI::PropertyReceiver *receiver) const {
  return CEGUI::PropertyHelper::boolToString(
      static_cast<const ElasticBox *>(receiver)->isSizable());
}

void Sizable::set(CEGUI::PropertyReceiver *receiver,
                  const CEGUI::String &value) {
  static_cast<ElasticBox *>(receiver)->setSizable(
      CEGUI::PropertyHelper::stringToBool(value));
}

CEGUI::String Movable::get(const CEGUI::PropertyReceiver *receiver) const {
  return CEGUI::PropertyHelper::boolToString(
      static_cast<const ElasticBox *>(receiver)->isMovable());
}

void Movable::set(CEGUI::PropertyReceiver *receiver,
                  const CEGUI::String &value) {
  static_cast<ElasticBox *>(receiver)->setMovable(
      CEGUI::PropertyHelper::stringToBool(value));
}

CEGUI::String
SizingBorderThickness::get(const CEGUI::PropertyReceiver *receiver) const {
  return CEGUI::PropertyHelper::floatToString(
      static_cast<const ElasticBox *>(receiver)->getSizingBorderThickness());
}

void SizingBorderThickness::set(CEGUI::PropertyReceiver *receiver,
                                const CEGUI::String &value) {
  static_cast<ElasticBox *>(receiver)->setSizingBorderThickness(
      CEGUI::PropertyHelper::stringToFloat(value));
}

} // namespace ElasticBoxProperties
