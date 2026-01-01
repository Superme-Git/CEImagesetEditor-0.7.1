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
#ifndef _CEGUI_HELPER_H_
#define _CEGUI_HELPER_H_

#include <CEGUIString.h>
#include <CEGUIcolour.h>

/** 用于 CEGUI 类型和 wxWidgets 类型之间转换的实用程序类。
 */
class CEGUIHelper {
public:
  /** 将 wxWidgets 字符串转换为 CEGUI 字符串，支持 Unicode。*/
  static CEGUI::String ToCEGUIString(const wxString &str) {
    return CEGUI::String(ToCEUTF8(wxConvLibc.cWX2MB(str)));
  }

  /** 将 ansi 字符串转换为 CEGUI 字符串。*/
  static const CEGUI::utf8 *ToCEUTF8(const char *cstr) {
    return reinterpret_cast<const CEGUI::utf8 *>(cstr);
  }

  /** 将 CEGUI 字符串转换为 wxWidgets 字符串，支持 Unicode。*/
  static wxString ToWXString(const CEGUI::String &str) {
    return wxString(wxConvLibc.cMB2WX(str.c_str()));
  }

  /** 将 wxWidgets 字符串转换为 float。*/
  static float WXStringToFloat(const wxString &str) {
    double dVal = 0;
    str.ToDouble(&dVal);
    return static_cast<float>(dVal);
  }

  /** 将 wxWidgets 字符串转换为 int。*/
  static int WXStringToInt(const wxString &str) {
    long val = 0;
    str.ToLong(&val);
    return static_cast<int>(val);
  }

  /** 将 wxWidgets 颜色转换为 cegui 颜色。*/
  static CEGUI::colour WXColourToCEGUIColour(const wxColour &colour) {
    CEGUI::colour col(colour.Red() / 255.0f, colour.Green() / 255.0f,
                      colour.Blue() / 255.0f);
    return col;
  }
};

#endif // _CEGUI_HELPER_H_
