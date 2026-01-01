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

#ifndef _CEIMAGESET_EDITOR_H_
#define _CEIMAGESET_EDITOR_H_

#include "EditorFrame.h"

class wxDocManager;

/** 应用程序类。程序从这里开始。
 */
class CEImagesetEditor : public wxApp {
public:
  /** 构造函数 - 初始化/准备成员。*/
  CEImagesetEditor();

  /** 由框架调用。将其视为应用程序的 Main 函数。
   * @return 成功 (true) 或失败 (false)。
   */
  bool OnInit();

  /** 应用程序退出时由框架调用。
   * 此时保存当前文档等请求已经完成。
   */
  int OnExit();

  /** 我们自己处理关于对话框。
   */
  void OnAbout(wxCommandEvent &event);

protected:
  wxDocManager *m_docManager;

private:
  DECLARE_EVENT_TABLE()
};

DECLARE_APP(CEImagesetEditor)

extern EditorFrame *GetMainFrame();

#endif // _CEIMAGESET_EDITOR_H_
