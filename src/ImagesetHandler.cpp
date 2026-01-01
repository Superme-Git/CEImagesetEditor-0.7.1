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
#include "ImagesetHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUISystem.h"
#include "CEGUIXMLAttributes.h"

#include "CEGUIHelper.h"
#include "EditorDocument.h"

// CEGUI 命名空间部分开始
using namespace CEGUI;

/*************************************************************************
Imageset（及其子类）的常量数据定义
*************************************************************************/
// 在 Imageset::xmlHandler 中声明
const CEGUI::String ImagesetHandler::ImagesetElement("Imageset");
const CEGUI::String ImagesetHandler::ImageElement("Image");
const char ImagesetHandler::ImagesetImageFileAttribute[] = "Imagefile";
const char ImagesetHandler::ImagesetResourceGroupAttribute[] = "ResourceGroup";
const char ImagesetHandler::ImagesetNameAttribute[] = "Name";
const char ImagesetHandler::ImagesetNativeHorzResAttribute[] = "NativeHorzRes";
const char ImagesetHandler::ImagesetNativeVertResAttribute[] = "NativeVertRes";
const char ImagesetHandler::ImagesetAutoScaledAttribute[] = "AutoScaled";
const char ImagesetHandler::ImageNameAttribute[] = "Name";
const char ImagesetHandler::ImageXPosAttribute[] = "XPos";
const char ImagesetHandler::ImageYPosAttribute[] = "YPos";
const char ImagesetHandler::ImageWidthAttribute[] = "Width";
const char ImagesetHandler::ImageHeightAttribute[] = "Height";
const char ImagesetHandler::ImageXOffsetAttribute[] = "XOffset";
const char ImagesetHandler::ImageYOffsetAttribute[] = "YOffset";

/*************************************************************************
SAX2 处理程序方法
*************************************************************************/
void ImagesetHandler::elementStart(const String &element,
                                   const XMLAttributes &attributes) {
  // 处理 Image 元素（提取所有元素属性并使用数据定义 Imageset 的 Image）
  if (element == ImageElement) {
    wxString name;

    if (attributes.exists(ImageNameAttribute))
      name = CEGUIHelper::ToWXString(
          attributes.getValueAsString(ImageNameAttribute));

    wxRect rect;

    if (attributes.exists(ImageXPosAttribute))
      rect.SetX(attributes.getValueAsInteger(ImageXPosAttribute));
    if (attributes.exists(ImageYPosAttribute))
      rect.SetY(attributes.getValueAsInteger(ImageYPosAttribute));
    if (attributes.exists(ImageWidthAttribute))
      rect.SetWidth(attributes.getValueAsInteger(ImageWidthAttribute));
    if (attributes.exists(ImageHeightAttribute))
      rect.SetHeight(attributes.getValueAsInteger(ImageHeightAttribute));

    wxPoint offset;

    if (attributes.exists(ImageXOffsetAttribute))
      offset.x = attributes.getValueAsInteger(ImageXOffsetAttribute, 0);
    if (attributes.exists(ImageYOffsetAttribute))
      offset.y = attributes.getValueAsInteger(ImageYOffsetAttribute, 0);

    m_document->addRegion(name, rect);
  }
  // 处理根 Imageset 元素
  else if (element == ImagesetElement) {
    if (attributes.exists(ImagesetNameAttribute)) {
      wxString name(CEGUIHelper::ToWXString(
          attributes.getValueAsString(ImagesetNameAttribute)));
      m_document->setImagesetName(name);
    }

    if (attributes.exists(ImagesetImageFileAttribute)) {
      wxString filename(CEGUIHelper::ToWXString(
          attributes.getValueAsString(ImagesetImageFileAttribute)));
      m_document->setImageFilename(filename);
    }

    //
    // 加载自动缩放配置
    //
    int hres = 640, vres = 480;

    if (attributes.exists(ImagesetNativeHorzResAttribute)) {
      // 获取原始水平分辨率
      hres = attributes.getValueAsInteger(ImagesetNativeHorzResAttribute, 640);
    }
    if (attributes.exists(ImagesetNativeVertResAttribute)) {
      // 获取原始垂直分辨率
      vres = attributes.getValueAsInteger(ImagesetNativeVertResAttribute, 480);
    }
    m_document->setNativeResolution(wxPoint(hres, vres));

    bool autoscal = false;
    if (attributes.exists(ImagesetAutoScaledAttribute)) {
      // 根据设置启用/禁用此 Imageset 的自动缩放
      autoscal = attributes.getValueAsBool(ImagesetAutoScaledAttribute, false);
    }
    m_document->setAutoScaled(autoscal);
  }
}

void ImagesetHandler::elementEnd(const String &element) {
  if (element == ImagesetElement) {
    // Logger::getSingleton().logEvent("Finished creation of Imageset '" +
    // d_imageset->d_name + "' via XML file.", Informative);
    m_document->Modify(true);
  }
}
