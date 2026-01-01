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
#include "DialogAbout.h"

#include "Config.h"

#include <CEGUIVersion.h>

#define BORDER 5

//--------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(DialogAbout, wxDialog)
EVT_BUTTON(ID_OK_BUTTON, DialogAbout::OnClickOk)
END_EVENT_TABLE()

//--------------------------------------------------------------------------------------
DialogAbout::DialogAbout(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, wxT("About CEImagesetEditor"),
               wxDefaultPosition, wxSize(400, 300)) {
  // 构建版本字符串
  wxString ver;
  ver.Printf(wxT("Version %s (built with CEGUI %d.%d.%d)"),
             wxT(PACKAGE_VERSION), CEGUI_VERSION_MAJOR, CEGUI_VERSION_MINOR,
             CEGUI_VERSION_PATCH);

  wxStaticText *title =
      new wxStaticText(this, wxID_ANY, wxT("CEImagesetEditor"));
  wxStaticText *version = new wxStaticText(this, wxID_ANY, ver);
  wxTextCtrl *about = new wxTextCtrl(
      this, wxID_ANY,
      wxT("CEImagesetEditor is the official imageset editor tool of Crazy "
          "Eddie's GUI System.\n\n") wxT(
          "Project website: http://www.cegui.org.uk/\n")
          wxT("Report bugs at http://mantis.cegui.org.uk/\n\n") wxT(
              "Martin Fleurent:\n") wxT("\tOriginal version code.\n\n")
              wxT("Paul D. Turner:\n") wxT("\tProject co-ordination.\n") wxT(
                  "\tInitial code cleanup work.\n")
                  wxT("\tCEGUI 0.5.x compatibility updates.\n") wxT(
                      "\tElasticBox custom widget.\n\n")
                      wxT("Frederico Jeronimo:\n") wxT(
                          "\tMSVC++ projects (replaced premake)\n")
                          wxT("\tWin32 static config updates.\n\n") wxT(
                              "Tomas Lindquist Olsen:\n")
                              wxT("\tAdditional Win32 testing.\n") wxT(
                                  "\tpremake project generation support.\n\n")
                                  wxT("(C) 2005-2010 CEGUI Team.\n")
                                      wxT("Contact: team@cegui.org.uk"),
      wxDefaultPosition, wxDefaultSize,
      wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2);

  wxButton *ok = new wxButton(this, ID_OK_BUTTON, wxT("&OK"));

  // 进一步初始化控件
  wxFont font = title->GetFont();
  font.SetWeight(wxBOLD);
  title->SetFont(font);

  ok->SetDefault();

  // 垂直调整组件大小
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(title, 0, wxEXPAND | wxALL, BORDER);
  sizer->Add(version, 0, wxEXPAND | wxALL, BORDER);
  sizer->Add(about, 1, wxEXPAND | wxALL, BORDER);
  sizer->Add(ok, 0, wxALL, BORDER);

  SetSizer(sizer);

  // 居中对话框
  Centre();

  Show(true);
}

//--------------------------------------------------------------------------------------
void DialogAbout::OnClickOk(wxCommandEvent &event) { Close(); }
