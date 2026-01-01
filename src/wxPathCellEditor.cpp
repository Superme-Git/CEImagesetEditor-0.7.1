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

// 这段代码中与文本编辑相关的部分基本上取自
// wxWidgets 内置的单元格文本编辑器：wxGridCellTextEditor。

#include "wxPathCellEditor.h"

#include <wx/filename.h>
#include <wx/log.h>
#include <wx/wx.h>

#if defined(__WXMOTIF__)
#define WXUNUSED_MOTIF(identifier) WXUNUSED(identifier)
#else
#define WXUNUSED_MOTIF(identifier) identifier
#endif

#if defined(__WXGTK__)
#define WXUNUSED_GTK(identifier) WXUNUSED(identifier)
#else
#define WXUNUSED_GTK(identifier) identifier
#endif

#define ID_BROWSE_BUTTON 10000
#define ID_DIRECTORY_TEXT 10001

//--------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(wxPathCellEditor, wxEvtHandler)
EVT_BUTTON(ID_BROWSE_BUTTON, wxPathCellEditor::OnBrowseButtonClick)
END_EVENT_TABLE()

//--------------------------------------------------------------------------------
wxPathCellEditor::wxPathCellEditor()
    : m_maxChars(0), m_browse(0), m_text(0), m_sizer(0) {}

//--------------------------------------------------------------------------------
void wxPathCellEditor::Create(wxWindow *parent, wxWindowID id,
                              wxEvtHandler *evtHandler) {
  m_control = new wxControl(parent, id, wxDefaultPosition, wxDefaultSize, 0);
  m_browse = new wxButton(m_control, ID_BROWSE_BUTTON, wxT("..."),
                          wxDefaultPosition, wxSize(25, -1), 0);
  m_text = new wxTextCtrl(m_control, ID_DIRECTORY_TEXT, wxEmptyString,
                          wxDefaultPosition, wxDefaultSize
#if defined(__WXMSW__)
                          ,
                          wxTE_PROCESS_TAB | wxTE_AUTO_SCROLL | wxNO_BORDER
#endif
  );

  // 如果设置了参数，则设置 textctrl 中允许的最大长度
  if (m_maxChars != 0)
    m_text->SetMaxLength(m_maxChars);

  m_sizer = new wxBoxSizer(wxHORIZONTAL);
  m_sizer->Add(m_text, 1, wxGROW | wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  m_sizer->Add(m_browse, 0, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL | wxALL,
               0);

  m_control->SetSizer(m_sizer);
  // 确保我们听到事件
  m_control->PushEventHandler(this);

  wxGridCellEditor::Create(parent, id, evtHandler);
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::PaintBackground(const wxRect &WXUNUSED(rectCell),
                                       wxGridCellAttr *WXUNUSED(attr)) {
  // 由于我们填充了整个客户区，
  // 因此此处不执行任何操作以尽量减少闪烁
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::SetSize(const wxRect &rectOrig) {
  wxRect rect(rectOrig);

  // 使编辑控件足够大以容纳内部边距
  //
  // TODO：如果文本控件大小调整得到改进（尤其是对于 UNIX），请删除此内容
  //
#if defined(__WXGTK__)

  if (rect.x != 0) {
    rect.x += 1;
    rect.y += 1;
    rect.width -= 1;
    rect.height -= 1;
  }
#elif defined(__WXMSW__)
  if (rect.x == 0)
    rect.x += 2;
  else
    rect.x += 3;

  if (rect.y == 0)
    rect.y += 2;
  else
    rect.y += 3;

  rect.width -= 2;
  rect.height -= 2;
#else

  int extra_x = (rect.x > 2) ? 2 : 1;
  int extra_y = (rect.y > 2) ? 2 : 1;

#if defined(__WXMOTIF__)

  extra_x *= 2;
  extra_y *= 2;
#endif

  rect.SetLeft(wxMax(0, rect.x - extra_x));
  rect.SetTop(wxMax(0, rect.y - extra_y));
  rect.SetRight(rect.GetRight() + 2 * extra_x);
  rect.SetBottom(rect.GetBottom() + 2 * extra_y);
#endif

  wxGridCellEditor::SetSize(rect);
  m_control->Layout();
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::BeginEdit(int row, int col, wxGrid *grid) {
  wxASSERT_MSG(m_control, wxT("The wxGridCellEditor must be created first!"));

  m_startValue = grid->GetTable()->GetValue(row, col);

  DoBeginEdit(m_startValue);
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::DoBeginEdit(const wxString &startValue) {
  Text()->SetValue(startValue);
  Text()->SetInsertionPointEnd();
  Text()->SetSelection(-1, -1);
  Text()->SetFocus();
}

//--------------------------------------------------------------------------------
bool wxPathCellEditor::EndEdit(int row, int col, wxGrid *grid) {
  wxASSERT_MSG(m_control, wxT("The wxGridCellEditor must be created first!"));

  bool changed = false;
  wxString value = Text()->GetValue();
  if (value != m_startValue)
    changed = true;

  if (changed)
    grid->GetTable()->SetValue(row, col, value);

  m_startValue = wxEmptyString;

  // 设置隐藏控件的文本没有意义
  // Text()->SetValue(m_startValue);

  return changed;
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::Reset() {
  wxASSERT_MSG(m_control, wxT("The wxGridCellEditor must be created first!"));

  DoReset(m_startValue);
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::DoReset(const wxString &startValue) {
  Text()->SetValue(startValue);
  Text()->SetInsertionPointEnd();
}

//--------------------------------------------------------------------------------
bool wxPathCellEditor::IsAcceptedKey(wxKeyEvent &event) {
  return wxGridCellEditor::IsAcceptedKey(event);
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::StartingKey(wxKeyEvent &event) {
  // 由于现在这在 EVT_CHAR 事件中发生，因此 EmulateKeyPress 不再是
  // 将字符输入文本控件的合适方法。我们自己来做。
  // 我们要知道，如果我们到了这一步，我们就有了一个有效的字符，
  // 所以不需要做太多的测试。

  wxTextCtrl *tc = Text();
  wxChar ch;
  long pos;

#if wxUSE_UNICODE

  ch = event.GetUnicodeKey();
  if (ch <= 127)
    ch = (wxChar)event.GetKeyCode();
#else

  ch = (wxChar)event.GetKeyCode();
#endif

  switch (ch) {
  case WXK_DELETE:
    // 删除光标处的字符
    pos = tc->GetInsertionPoint();
    if (pos < tc->GetLastPosition())
      tc->Remove(pos, pos + 1);
    break;

  case WXK_BACK:
    // 删除光标前的字符
    pos = tc->GetInsertionPoint();
    if (pos > 0)
      tc->Remove(pos - 1, pos);
    break;

  default:
    tc->WriteText(ch);
    break;
  }
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::HandleReturn(
    wxKeyEvent &WXUNUSED_GTK(WXUNUSED_MOTIF(event))) {
#if defined(__WXMOTIF__) || defined(__WXGTK__)
  // wxMotif 需要一点额外的帮助...
  size_t pos = (size_t)(Text()->GetInsertionPoint());
  wxString s(Text()->GetValue());
  s = s.Left(pos) + wxT("\n") + s.Mid(pos);
  Text()->SetValue(s);
  Text()->SetInsertionPoint(pos);
#else
  // 其他端口可以处理 Return 按键
  //
  event.Skip();
#endif
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::SetParameters(const wxString &params) {
  if (!params) {
    // 重置为默认值
    m_maxChars = 0;
  } else {
    long tmp;
    if (params.ToLong(&tmp)) {
      m_maxChars = (size_t)tmp;
    } else {
      wxLogDebug(_T( "Invalid wxPathCellEditor parameter string '%s' ignored" ),
                 params.c_str());
    }
  }
}

//--------------------------------------------------------------------------------
wxString wxPathCellEditor::GetValue() const { return Text()->GetValue(); }

//--------------------------------------------------------------------------------
void wxPathCellEditor::OnBrowseButtonClick(wxCommandEvent &event) {
  wxFileName currpath(m_text->GetValue());
  wxString dirpath = wxDirSelector(wxT("Select Directory"), currpath.GetPath());

  // 如果选择了某些内容
  if (!dirpath.empty()) {
    // 如果需要，追加斜杠
    int idx = dirpath.size() - 1;
    if (dirpath[idx] != '/')
      dirpath.append(1, '/');

    // 设置新文本
    m_text->SetValue(dirpath);
    // 将插入点放在末尾
    m_text->SetInsertionPointEnd();
    // 将焦点设置到父级
    m_control->SetFocus();
  }

  event.Skip();
}
