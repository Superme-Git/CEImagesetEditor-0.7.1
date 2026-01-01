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
#ifndef _ELASTIC_BOX_WINDOW_RENDERER_H_
#define _ELASTIC_BOX_WINDOW_RENDERER_H_

// 父类
#include <CEGUIWindowRenderer.h>

/*!
\brief
    ElasticBox 窗口的窗口渲染器。

    LookNFeel 应提供以下内容：

    States:
        - Active       - 激活状态的渲染。
        - Inactive     - 非激活状态的渲染。
*/
class ElasticBoxWindowRenderer : public CEGUI::WindowRenderer {
public:
  //! WindowRenderer 的类型名称
  static const CEGUI::String TypeName;

  ElasticBoxWindowRenderer(const CEGUI::String &type);

  void render();
};

/*!
\brief
    ElasticBox 窗口渲染器类型的工厂
*/
class ElasticBoxWRFactory : public CEGUI::WindowRendererFactory {
public:
  ElasticBoxWRFactory(void)
      : CEGUI::WindowRendererFactory(ElasticBoxWindowRenderer::TypeName) {}
  CEGUI::WindowRenderer *create(void);
  void destroy(CEGUI::WindowRenderer *wr);
};

ElasticBoxWRFactory &getElasticBoxWRFactory();

#endif // _ELASTIC_BOX_WINDOW_RENDERER_H_
