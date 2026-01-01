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
#ifndef _WX_PATH_CELL_EDITOR_H_
#define _WX_PATH_CELL_EDITOR_H_

#include <wx/grid.h>

// 前向引用
class wxButton;
class wxTextCtrl;
class wxBoxSizer;

/** 这是一个自定义 wxGridCellEditor，用于 wxWidgets wxGrid 组件。
 * 当我们编辑时，它允许我们在网格单元格中显示一个小的“浏览按钮”——按下浏览按钮
 * 将调用目录选择通用对话框——在该对话框中选择某些内容后，路径文本
 * 将被复制到单元格中。除此之外，该单元格的操作与普通文本单元格相同。
 */
class wxPathCellEditor : public wxEvtHandler, public wxGridCellEditor {
public:
  wxPathCellEditor();

  virtual void Create(wxWindow *parent, wxWindowID id,
                      wxEvtHandler *evtHandler);
  virtual void SetSize(const wxRect &rect);

  virtual void PaintBackground(const wxRect &rectCell, wxGridCellAttr *attr);

  virtual bool IsAcceptedKey(wxKeyEvent &event);
  virtual void BeginEdit(int row, int col, wxGrid *grid);
  virtual bool EndEdit(int row, int col, wxGrid *grid);

  virtual void Reset();
  virtual void StartingKey(wxKeyEvent &event);
  virtual void HandleReturn(wxKeyEvent &event);

  // 参数字符串格式为 "max_width"
  virtual void SetParameters(const wxString &params);

  virtual wxGridCellEditor *Clone() const { return new wxGridCellTextEditor; }

  virtual wxString GetValue() const;

  void OnBrowseButtonClick(wxCommandEvent &event);

protected:
  wxTextCtrl *Text() const { return m_text; }

  void DoBeginEdit(const wxString &startValue);
  void DoReset(const wxString &startValue);

private:
  //! 允许的最大字符数
  size_t m_maxChars;
  wxString m_startValue;

  wxButton *m_browse;
  wxTextCtrl *m_text;
  wxBoxSizer *m_sizer;

  DECLARE_EVENT_TABLE()

  DECLARE_NO_COPY_CLASS(wxPathCellEditor)
};

#endif // _WX_PATH_CELL_EDITOR_H_
