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

#include "ElasticBoxWindowRenderer.h"
#include <falagard/CEGUIFalWidgetLookFeel.h>

const CEGUI::String
    ElasticBoxWindowRenderer::TypeName("CEImagesetEditor/ElasticBoxRenderer");

ElasticBoxWindowRenderer::ElasticBoxWindowRenderer(const CEGUI::String &type)
    : CEGUI::WindowRenderer(type) {}

void ElasticBoxWindowRenderer::render() {
  // 获取分配外观的 WidgetLookFeel。
  const CEGUI::WidgetLookFeel &wlf = getLookNFeel();
  // 渲染当前状态的图像
  wlf.getStateImagery(d_window->isActive() ? "Active" : "Inactive")
      .render(*d_window);
}

CEGUI::WindowRenderer *ElasticBoxWRFactory::create(void) {
  return new ElasticBoxWindowRenderer(ElasticBoxWindowRenderer::TypeName);
}

void ElasticBoxWRFactory::destroy(CEGUI::WindowRenderer *wr) { delete wr; }

ElasticBoxWRFactory &getElasticBoxWRFactory() {
  static ElasticBoxWRFactory s_factory;
  return s_factory;
}
