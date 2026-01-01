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

// 对于支持预编译的编译器，包含 "wx.h"。
#include "wx/wxprec.h"

#include "wx/filename.h"
#include "wx/image.h"

#include "EditorGLCanvas.h"
#include "EditorView.h"
#include "PropertiesPanel.h"
// 用于在 CEGUI 和 wxWidgets 类型之间转换的实用类
#include "CEGUIHelper.h"

// CEGUI 包含
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIWindowManager.h>

// 工具栏图标
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/save.xpm"

// 我们在面板上使用的组件的 ID 代码
const int ID_TEXT = 10000;
const int ID_IMAGESETNAME_TEXTCTRL = 10001;
const int ID_IMAGE_FILENAME_TEXTCTRL = 10002;
const int ID_IMAGE_FILENAME_BROWSE_BUTTON = 10003;
const int ID_NATIVE_RESOLUTION_COMBO = 10004;
const int ID_AUTOSCALE_CHECKBOX = 10005;
const int ID_GRID = 10006;
const int ID_SCROLLED = 10007;
const int ID_GRID_ADD_ROW = 10008;
const int ID_GRID_DEL_ROW = 10009;

// wxWidgets 事件绑定表
BEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
EVT_BUTTON(ID_IMAGE_FILENAME_BROWSE_BUTTON, PropertiesPanel::onBrowseImageFile)
EVT_TEXT(ID_IMAGESETNAME_TEXTCTRL, PropertiesPanel::onChangeImagesetName)
EVT_TEXT_ENTER(ID_IMAGE_FILENAME_TEXTCTRL,
               PropertiesPanel::onChangeImageFilename)
EVT_COMBOBOX(ID_NATIVE_RESOLUTION_COMBO,
             PropertiesPanel::onChangeNativeResolution)
EVT_CHECKBOX(ID_AUTOSCALE_CHECKBOX, PropertiesPanel::onChangeAutoScaled)
EVT_TOOL(ID_GRID_ADD_ROW, PropertiesPanel::onGridAddRow)
EVT_TOOL(ID_GRID_DEL_ROW, PropertiesPanel::onGridDelRow)
EVT_GRID_SELECT_CELL(PropertiesPanel::onGridCellSelect)
EVT_GRID_EDITOR_SHOWN(PropertiesPanel::onGridCellEdit)
EVT_GRID_CELL_CHANGE(PropertiesPanel::onGridCellChange)
END_EVENT_TABLE()

const char PropertiesPanel::s_charParseNativeResolution('*');

//-----------------------------------------------------------------------
PropertiesPanel::PropertiesPanel(wxWindow *parent, const wxPoint &pos,
                                 const wxSize &size)
    : wxPanel(parent, wxID_ANY, pos, size), m_view(0), m_propertiesGrid(0),
      m_imageGUID(0) {}

//-----------------------------------------------------------------------
void PropertiesPanel::setView(EditorView *aView) { m_view = aView; }

//-----------------------------------------------------------------------
void PropertiesPanel::createContent() {
  // 管理属性面板的布局
  wxBoxSizer *sizerProps = new wxBoxSizer(wxVERTICAL);
  SetSizer(sizerProps);

  // 通用属性部分
  wxStaticText *lblProps =
      new wxStaticText(this, ID_TEXT, wxT("Properties"), wxDefaultPosition,
                       wxDefaultSize, wxALIGN_CENTRE);
  lblProps->SetBackgroundColour(*wxGREEN);
  sizerProps->Add(lblProps, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 0);

  // 开始
  // ----------------------------------------------------------------------------
  // 图像集名称选择器
  wxStaticText *lblImagesetName =
      new wxStaticText(this, ID_TEXT, wxT("Imageset name :"), wxDefaultPosition,
                       wxDefaultSize, 0);
  sizerProps->Add(lblImagesetName, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxTOP,
                  5);

  m_imagesetNameTextCtrl =
      new wxTextCtrl(this, ID_IMAGESETNAME_TEXTCTRL, wxT(""), wxDefaultPosition,
                     wxDefaultSize, wxTE_PROCESS_ENTER);
  sizerProps->Add(m_imagesetNameTextCtrl, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
  // 结束
  // ------------------------------------------------------------------------------

  // 开始
  // ----------------------------------------------------------------------------
  // 图像文件选择器
  wxStaticText *lblImageFile = new wxStaticText(
      this, ID_TEXT, wxT("Image file :"), wxDefaultPosition, wxDefaultSize, 0);
  sizerProps->Add(lblImageFile, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxTOP,
                  5);

  wxBoxSizer *sizerImageFile = new wxBoxSizer(wxHORIZONTAL);

  m_imageFilenameTextCtrl =
      new wxTextCtrl(this, ID_IMAGE_FILENAME_TEXTCTRL, wxT(""),
                     wxDefaultPosition, wxSize(175, -1), wxTE_PROCESS_ENTER);
  sizerImageFile->Add(m_imageFilenameTextCtrl, 1,
                      wxGROW | wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

  m_imageFilenameBrowseButton =
      new wxButton(this, ID_IMAGE_FILENAME_BROWSE_BUTTON, wxT("..."),
                   wxDefaultPosition, wxSize(20, -1), 0);
  sizerImageFile->Add(m_imageFilenameBrowseButton, 0,
                      wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

  // 将图像文件选择器添加到属性面板
  sizerProps->Add(sizerImageFile, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
  // 结束
  // ------------------------------------------------------------------------------

  // 开始
  // ----------------------------------------------------------------------------
  // 原始分辨率选择器
  wxGridSizer *sizerNativeResolution = new wxGridSizer(2, 2, 0, 5);

  wxStaticText *lblNativeResolution =
      new wxStaticText(this, ID_TEXT, wxT("Native resolution :"),
                       wxDefaultPosition, wxDefaultSize, 0);
  sizerNativeResolution->Add(
      lblNativeResolution, 0,
      wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxTOP, 0);

  sizerNativeResolution->AddSpacer(0);

  // 我们提供的原始分辨率选项
  wxString strs14[] = {wxT("640*480"), wxT("800*600"), wxT("1024*768"),
                       wxT("1280*1024"), wxT("1600*1200")};

  m_nativeResolutionComboBox = new wxComboBox(
      this, ID_NATIVE_RESOLUTION_COMBO, wxT(""), wxDefaultPosition,
      wxDefaultSize, 5, strs14, wxCB_DROPDOWN);
  sizerNativeResolution->Add(m_nativeResolutionComboBox, 1,
                             wxGROW | wxALIGN_CENTER | wxLEFT | wxRIGHT, 0);

  // 选择图像集是否自动缩放
  m_autoScaleCheckbox =
      new wxCheckBox(this, ID_AUTOSCALE_CHECKBOX, wxT("Auto scaled"),
                     wxDefaultPosition, wxDefaultSize, 0);
  sizerNativeResolution->Add(m_autoScaleCheckbox, 0,
                             wxALIGN_CENTER_VERTICAL | wxALL, 0);

  // 将原始分辨率选择器添加到属性面板
  sizerProps->Add(sizerNativeResolution, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
  // 结束
  // ------------------------------------------------------------------------------

  // 设置属性部分
  wxStaticText *lblImageSet =
      new wxStaticText(this, ID_TEXT, wxT("Image definitions"),
                       wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTRE);
  lblImageSet->SetBackgroundColour(*wxGREEN);
  sizerProps->Add(lblImageSet, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 0);

  // 创建仅图标工具栏
  wxToolBar *toolBar =
      new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTB_HORIZONTAL); // |wxNO_BORDER
  //
  toolBar->AddTool(ID_GRID_ADD_ROW, wxBitmap(new_xpm));
  toolBar->AddTool(ID_GRID_DEL_ROW, wxBitmap(cut_xpm));
  sizerProps->Add(toolBar, 0,
                  wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxTOP,
                  5);
  // 别忘了这个！
  toolBar->Realize();

  // 设置属性网格
  createPropertiesGrid(this);

  // 将属性网格添加到属性面板
  sizerProps->Add(m_propertiesGrid, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL,
                  5);
  // 结束
  // ----------------------------------------------------------------------------

  // 设置初始状态
  reset();
}

//-----------------------------------------------------------------------
void PropertiesPanel::reset() {
  setImagesetName(wxT(""));
  setImageFilename(wxT(""));
  setNativeResolution(wxPoint(640, 480));
  setAutoScaled(false);

  deleteAllRegions();
  m_imageGUID = 0;
}

//-----------------------------------------------------------------------
void PropertiesPanel::setImagesetName(const wxString &name) {
  m_imagesetNameTextCtrl->SetValue(name);
}

//-----------------------------------------------------------------------
wxString PropertiesPanel::getImagesetName() const {
  return m_imagesetNameTextCtrl->GetValue();
}

//-----------------------------------------------------------------------
void PropertiesPanel::setImageFilename(const wxString &name) {
  m_imageFilenameTextCtrl->SetValue(name);
}

//-----------------------------------------------------------------------
wxString PropertiesPanel::getImageFilename() const {
  return m_imageFilenameTextCtrl->GetValue();
}

//-----------------------------------------------------------------------
void PropertiesPanel::setNativeResolution(const wxPoint &NativeResolution) {
  wxChar statusInfo[100];
  wxSnprintf(statusInfo, sizeof(statusInfo) / sizeof(wxChar), wxT("%d%c%d"),
             NativeResolution.x, s_charParseNativeResolution,
             NativeResolution.y);
  m_nativeResolutionComboBox->SetValue(statusInfo);
}

//-----------------------------------------------------------------------
wxPoint PropertiesPanel::getNativeResolution() const {
  wxString values = m_nativeResolutionComboBox->GetValue();
  wxString xNative(values.BeforeFirst(s_charParseNativeResolution));
  wxString yNative(values.AfterFirst(s_charParseNativeResolution));

  return wxPoint(CEGUIHelper::WXStringToInt(xNative),
                 CEGUIHelper::WXStringToInt(yNative));
}

//-----------------------------------------------------------------------
bool PropertiesPanel::getAutoScaled() const {
  return m_autoScaleCheckbox->GetValue();
}

//-----------------------------------------------------------------------
void PropertiesPanel::setAutoScaled(const bool AutoScaled) {
  m_autoScaleCheckbox->SetValue(AutoScaled);
}

//-----------------------------------------------------------------------
int PropertiesPanel::selectRegion(const wxString &name) {
  int regionIdx = getInternalRegionIndex(name);

  if (regionIdx != -1) {
    m_propertiesGrid->SelectRow(regionIdx);
    // 可以作为网格的一个选项（跟踪选择）
    m_propertiesGrid->MakeCellVisible(regionIdx, 0);
  }

  return regionIdx;
}

//-----------------------------------------------------------------------
void PropertiesPanel::addRegion(const wxString &name, const wxRect &dim,
                                int rowindex) {
  if (rowindex == -1) {
    if (m_propertiesGrid->IsSelection()) {
      wxArrayInt sel = m_propertiesGrid->GetSelectedRows();
      if (sel.Count() > 0) {
        rowindex = m_propertiesGrid->GetSelectedRows().Item(0);
      } else {
        rowindex = m_propertiesGrid->GetNumberRows();
      }
    } else {
      rowindex = m_propertiesGrid->GetNumberRows();
    }
  }

  m_propertiesGrid->InsertRows(rowindex, 1);

  m_propertiesGrid->SetCellAlignment(rowindex, 0, wxALIGN_LEFT, wxALIGN_CENTRE);
  m_propertiesGrid->SetCellValue(rowindex, 0, name);

  wxChar value[100];
  wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetX());
  m_propertiesGrid->SetCellValue(rowindex, 1, value);

  wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetY());
  m_propertiesGrid->SetCellValue(rowindex, 2, value);

  wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetWidth());
  m_propertiesGrid->SetCellValue(rowindex, 3, value);

  wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetHeight());
  m_propertiesGrid->SetCellValue(rowindex, 4, value);

  m_propertiesGrid->AutoSizeColumn(0, false);

  m_propertiesGrid->ForceRefresh();
}

//-----------------------------------------------------------------------
void PropertiesPanel::setRegionArea(const wxString &name, const wxRect &dim) {
  int row = getInternalRegionIndex(name);
  if (row != -1) {
    wxChar value[100];

    if (dim.GetX() != m_propertiesGrid->GetCellValue(row, 1)) {
      wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetX());
      m_propertiesGrid->SetCellValue(row, 1, value);
    }

    if (dim.GetY() != m_propertiesGrid->GetCellValue(row, 2)) {
      wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"), dim.GetY());
      m_propertiesGrid->SetCellValue(row, 2, value);
    }

    if (dim.GetWidth() != m_propertiesGrid->GetCellValue(row, 3)) {
      wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"),
                 dim.GetWidth());
      m_propertiesGrid->SetCellValue(row, 3, value);
    }

    if (dim.GetHeight() != m_propertiesGrid->GetCellValue(row, 4)) {
      wxSnprintf(value, sizeof(value) / sizeof(wxChar), wxT("%d"),
                 dim.GetHeight());
      m_propertiesGrid->SetCellValue(row, 4, value);
    }
  }
}

//-----------------------------------------------------------------------
wxRect PropertiesPanel::getRegionArea(const wxString &name) const {
  wxRect dim;
  int regionIdx = getInternalRegionIndex(name);

  if (regionIdx != -1) {
    dim.x = CEGUIHelper::WXStringToInt(
        m_propertiesGrid->GetCellValue(regionIdx, 1));
    dim.y = CEGUIHelper::WXStringToInt(
        m_propertiesGrid->GetCellValue(regionIdx, 2));
    dim.width = CEGUIHelper::WXStringToInt(
        m_propertiesGrid->GetCellValue(regionIdx, 3));
    dim.height = CEGUIHelper::WXStringToInt(
        m_propertiesGrid->GetCellValue(regionIdx, 4));
  }

  return dim;
}

//-----------------------------------------------------------------------
void PropertiesPanel::renameRegion(const wxString &currentname,
                                   const wxString &newname) {
  int regionIdx = getInternalRegionIndex(currentname);

  if ((regionIdx != -1) && (currentname != newname)) {
    // 确保我们不允许重命名为已经存在的内容
    if (CEGUI::WindowManager::getSingleton().isWindowPresent(
            CEGUIHelper::ToCEGUIString(newname))) {
      wxLogError(
          wxT("RenameRegion - Error: A region with that name already exists."));
      // 确保我们在网格中保留旧名称
      m_propertiesGrid->SetCellValue(regionIdx, 0, currentname);
    } else {
      m_propertiesGrid->SetCellValue(regionIdx, 0, newname);
    }
  }
}

//-----------------------------------------------------------------------
int PropertiesPanel::deleteRegion(const wxString &name) {
  int regionIdx = getInternalRegionIndex(name);

  if (regionIdx != -1)
    m_propertiesGrid->DeleteRows(regionIdx, 1);

  return regionIdx;
}

//-----------------------------------------------------------------------
void PropertiesPanel::deleteAllRegions() {
  if (m_propertiesGrid->GetNumberRows() > 0) {
    m_propertiesGrid->ClearGrid();
    m_propertiesGrid->DeleteRows(0, m_propertiesGrid->GetNumberRows());
  }
}

//-----------------------------------------------------------------------
wxString PropertiesPanel::getSelectedRegion() const {
  wxString name;

  wxArrayInt selections = m_propertiesGrid->GetSelectedRows();

  if (!selections.IsEmpty())
    name = m_propertiesGrid->GetCellValue(selections.Item(0), 0);

  return name;
}

//-----------------------------------------------------------------------
void PropertiesPanel::createPropertiesGrid(wxWindow *parent) {
  m_propertiesGrid =
      new wxGrid(parent, ID_GRID, wxDefaultPosition, wxSize(190, 160),
                 wxWANTS_CHARS | wxSUNKEN_BORDER);

  m_propertiesGrid->CreateGrid(0, 5, wxGrid::wxGridSelectCells);
  m_propertiesGrid->EnableDragRowSize(false);

  // SetColLabelSize(20);
  m_propertiesGrid->SetColLabelSize(20);
  m_propertiesGrid->SetRowLabelSize(20);
  m_propertiesGrid->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);

  // hack, a enlever
  m_propertiesGrid->SetColLabelValue(0, wxT("Name"));
  m_propertiesGrid->SetColSize(0, -1);

  m_propertiesGrid->SetColLabelValue(1, wxT("x"));
  m_propertiesGrid->SetColSize(1, 30);

  m_propertiesGrid->SetColLabelValue(2, wxT("y"));
  m_propertiesGrid->SetColSize(2, 30);

  m_propertiesGrid->SetColLabelValue(3, wxT("w"));
  m_propertiesGrid->SetColSize(3, 30);

  m_propertiesGrid->SetColLabelValue(4, wxT("h"));
  m_propertiesGrid->SetColSize(4, 30);

  m_propertiesGrid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
  m_propertiesGrid->ForceRefresh();
}

//-----------------------------------------------------------------------
int PropertiesPanel::getInternalRegionIndex(const wxString &name) const {
  int rowsize = m_propertiesGrid->GetRows();

  for (int i = 0; i < rowsize; i++) {
    const wxString rowname = m_propertiesGrid->GetCellValue(i, 0);
    if (rowname.Cmp(name) == 0)
      return i;
  }

  return -1;
}

//-----------------------------------------------------------------------
bool PropertiesPanel::isDocumentValid() const {
  return (m_view && m_view->GetDocument());
}

//-----------------------------------------------------------------------
EditorDocument *PropertiesPanel::getDocument() const {
  return m_view ? static_cast<EditorDocument *>(m_view->GetDocument()) : 0;
}

//-----------------------------------------------------------------------
wxString PropertiesPanel::getUniqueImageName() {
  wxString name(wxT("NewImage"));
  wxString basename(name);
  wxString count;

  do {
    m_imageGUID++;
    count.sprintf(wxT("%d"), m_imageGUID);
    name = basename + count;
  } while (CEGUI::WindowManager::getSingleton().isWindowPresent(
      CEGUIHelper::ToCEGUIString(name)));

  return name;
}

//-----------------------------------------------------------------------
void PropertiesPanel::onBrowseImageFile(wxCommandEvent &WXUNUSED(event)) {
  wxFileName currFilespec(getImageFilename());

  // Show file selector.
  wxString filename = wxFileSelector(
      wxT("Open Image"), currFilespec.GetPath(), currFilespec.GetFullName(),
      wxT("Image"), wxT("tga files (*.tga)|*.tga|all files (*.*)|*.*"));

  // if something was selected
  if (!filename.empty()) {
    filename = getDefaultResourceGroupRelativePath(filename);
    // update the filename in the dialog
    setImageFilename(filename);
    // trigger handler to force update of document
    wxCommandEvent dummy;
    onChangeImageFilename(dummy);
  }
}

//-----------------------------------------------------------------------
void PropertiesPanel::onChangeImagesetName(wxCommandEvent &WXUNUSED(event)) {
  if (isDocumentValid())
    getDocument()->setImagesetName(getImagesetName());
}

//-----------------------------------------------------------------------
void PropertiesPanel::onChangeImageFilename(wxCommandEvent &WXUNUSED(event)) {
  if (isDocumentValid())
    getDocument()->setImageFilename(getImageFilename());
}

//-----------------------------------------------------------------------
void PropertiesPanel::onChangeNativeResolution(
    wxCommandEvent &WXUNUSED(event)) {
  if (isDocumentValid())
    getDocument()->setNativeResolution(getNativeResolution());
}

//-----------------------------------------------------------------------
void PropertiesPanel::onChangeAutoScaled(wxCommandEvent &WXUNUSED(event)) {
  if (isDocumentValid())
    getDocument()->setAutoScaled(getAutoScaled());
}

//-----------------------------------------------------------------------
void PropertiesPanel::onGridAddRow(wxCommandEvent &WXUNUSED(event)) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    // 图像区域的默认区域
    wxRect dim(50, 50, 10, 10);
    // 添加新区域
    getDocument()->addRegion(getUniqueImageName(), dim);
  }
}

//-----------------------------------------------------------------------
void PropertiesPanel::onGridDelRow(wxCommandEvent &WXUNUSED(event)) {
  if (m_propertiesGrid->IsSelection()) {
    // 检查这一点，因为我们可能会在文档关闭期间收到此事件
    if (isDocumentValid()) {
      wxArrayInt sel = m_propertiesGrid->GetSelectedRows();
      if (sel.Count() > 0) {
        wxString name = m_propertiesGrid->GetCellValue(sel.Item(0), 0);
        getDocument()->deleteRegion(name);
      }
    }
  }
}

//-----------------------------------------------------------------------
// 我们必须跟踪单元格编辑以存储旧名称
void PropertiesPanel::onGridCellSelect(wxGridEvent &e) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid() && m_view->getCanvas()) {
    int row = e.GetRow();
    wxString name = m_propertiesGrid->GetCellValue(row, 0);
    m_view->getCanvas()->selectRegion(name);
  }
  e.Skip();
}

//-----------------------------------------------------------------------
// 我们必须跟踪单元格编辑以存储旧名称
void PropertiesPanel::onGridCellEdit(wxGridEvent &e) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    int col = e.GetCol();
    int row = e.GetRow();

    if (col == 0)
      m_oldRowName = m_propertiesGrid->GetCellValue(row, col);
  }
}

//-----------------------------------------------------------------------
void PropertiesPanel::onGridCellChange(wxGridEvent &e) {
  // 检查这一点，因为我们可能会在文档关闭期间收到此事件
  if (isDocumentValid()) {
    EditorDocument *document = getDocument();

    wxString name = m_propertiesGrid->GetCellValue(e.GetRow(), 0);
    wxRect dim = getRegionArea(name);

    if (e.GetCol() == 0) {
      if (m_oldRowName.Cmp(name) != 0) {
        // 确保我们不允许在此处传递现有名称 - 否则我们很难将其改回！
        if (CEGUI::WindowManager::getSingleton().isWindowPresent(
                CEGUIHelper::ToCEGUIString(name))) {
          wxLogError(wxT(
              "RenameRegion - Error: A region with that name already exists."));
          // 确保我们在网格中保留旧名称
          m_propertiesGrid->SetCellValue(e.GetRow(), 0, m_oldRowName);
          // 我们离开这里！
          return;

        }
        // 新名称没问题，继续重命名
        else {
          document->renameRegion(m_oldRowName, name);
          m_oldRowName = name;
        }
      }
    }

    document->setRegionArea(name, dim);
  }
}

//-----------------------------------------------------------------------
wxString PropertiesPanel::getDefaultResourceGroupRelativePath(
    const wxString &filename) const {
  wxString resourcePath(wxT("./"));
  wxFileName filespec(filename);

  CEGUI::DefaultResourceProvider *rp =
      static_cast<CEGUI::DefaultResourceProvider *>(
          CEGUI::System::getSingleton().getResourceProvider());

  CEGUI::String defResGrp = rp->getDefaultResourceGroup();

  if (!defResGrp.empty())
    resourcePath =
        CEGUIHelper::ToWXString(rp->getResourceGroupDirectory(defResGrp));

  if (filename.empty()) {
    return resourcePath;
  } else {
    filespec.MakeRelativeTo(resourcePath);
    return filespec.GetFullPath();
  }
}
