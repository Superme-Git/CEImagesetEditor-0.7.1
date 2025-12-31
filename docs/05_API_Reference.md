# API 鍙傝€?(API Reference)
> 鐢熸垚鏃堕棿: 2025-12-31 11:13:04

## 鏂囦欢: CEGUIHelper.h
### Class: `CEGUIHelper`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |

---
## 鏂囦欢: CEImagesetEditor.h
### Class: `CEImagesetEditor`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `bool` | **OnInit** | `` |
| `int` | **OnExit** | `` |
| `void` | **OnAbout** | `wxCommandEvent& event` |

---
## 鏂囦欢: DialogAbout.h
### Class: `DialogAbout`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `public:` | **DialogAbout** | `wxWindow* parent` |
| `void` | **OnClickOk** | `wxCommandEvent& event` |

---
## 鏂囦欢: DialogResourceGroups.h
### Class: `DialogResourceGroups`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `public:` | **WX_DECLARE_STRING_HASH_MAP** | `wxString, ResourceGroupsMap` |
| `bool` | **Create** | `wxWindow* parent, wxWindowID id = SYMBOL_DIALOGRESOURCEGROUPS_IDNAME, const wxString& caption = SYMBOL_DIALOGRESOURCEGROUPS_TITLE, const wxPoint& pos = SYMBOL_DIALOGRESOURCEGROUPS_POSITION, const wxSize& size = SYMBOL_DIALOGRESOURCEGROUPS_SIZE, long style = SYMBOL_DIALOGRESOURCEGROUPS_STYLE` |
| `void` | **CreateControls** | `` |
| `void` | **OnEditorShown** | `wxGridEvent& event` |
| `void` | **OnCellEdit** | `wxGridEvent& event` |
| `void` | **OnAddgroupButtonClick** | `wxCommandEvent& event` |
| `void` | **OnRemovegroupButtonClick** | `wxCommandEvent& event` |
| `void` | **OnSetDefaultGroupButtonClick** | `wxCommandEvent& event` |
| `void` | **OnOkClick** | `wxCommandEvent& event` |
| `void` | **OnCancelClick** | `wxCommandEvent& event` |
| `wxBitmap` | **GetBitmapResource** | `const wxString& name` |
| `wxIcon` | **GetIconResource** | `const wxString& name` |
| `void` | **invoke** | `` |
| `bool` | **isResourceGroupDefined** | `const wxString& groupName` |
| `bool` | **renameResourceGroup** | `const wxString& currentName, const wxString& newName` |
| `bool` | **addGroupEntry** | `const wxString& groupName, const wxString& directory` |
| `bool` | **deleteGroupEntry** | `const wxString& groupName` |
| `bool` | **setGroupDirectory** | `const wxString& groupName, const wxString& directory` |
| `wxString` | **getGroupDirectory** | `const wxString& groupName` |
| `bool` | **setDefaultGroup** | `const wxString& groupName` |
| `ResourceGroupsMap::const_iterator` | **getGroupIteratorStart** | `` |
| `ResourceGroupsMap::const_iterator` | **getGroupIteratorEnd** | `` |
| `wxString` | **getUniqueGroupName** | `` |
| `int` | **getInternalGroupIndex** | `const wxString& groupName` |
| `void` | **clearGroupsGrid** | `` |
| `void` | **populateGrid** | `` |

---
## 鏂囦欢: EditorDocument.h
### Class: `EditorDocument`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `bool` | **OnOpenDocument** | `const wxString& filename` |
| `bool` | **OnNewDocument** | `` |
| `bool` | **OnSaveDocument** | `const wxString& filename` |
| `bool` | **OnCloseDocument** | `` |
| `void` | **Modify** | `bool modify` |
| `void` | **reset** | `` |
| `void` | **setImageFilename** | `const wxString& newfilename` |
| `void` | **setImagesetName** | `const wxString& newname` |
| `void` | **setNativeResolution** | `const wxPoint& newnativeres` |
| `void` | **setAutoScaled** | `const bool newAutoScaled` |
| `void` | **addRegion** | `const wxString& name, const wxRect& dim` |
| `void` | **deleteRegion** | `const wxString& name` |
| `void` | **deleteAllRegions** | `` |
| `void` | **setRegionArea** | `const wxString& name, const wxRect& dim, const bool evt_src = false` |
| `void` | **renameRegion** | `const wxString& currentname, const wxString& newname` |
| `wxRect` | **getRegionArea** | `const wxString& name` |
| `bool` | **isViewValid** | `` |
| `EditorView*` | **getView** | `` |
| `CEGUI::Imageset*` | **generateRealImageset** | `` |
| `wxString` | **getDefaultResourceGroupRelativePath** | `const wxString& filename` |

---
## 鏂囦欢: EditorFrame.h
### Class: `EditorFrame`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `void` | **SetDocument** | `EditorDocument* document` |
| `EditorGLCanvas*` | **AttachCanvas** | `EditorView* view` |
| `void` | **AttachMenubar** | `` |
| `void` | **AttachToolbar** | `` |
| `void` | **createContent** | `` |
| `bool` | **Close** | `bool force = FALSE` |
| `void` | **OnPaint** | `wxPaintEvent& event` |
| `void` | **OnChangeBackgroundColour** | `wxCommandEvent& event` |
| `void` | **OnEditResourceGroups** | `wxCommandEvent& event` |

---
## 鏂囦欢: EditorGLCanvas.h
### Class: `EditorGLCanvas`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `void` | **changeSize** | `int newWidth, int newHeight` |
| `void` | **setBackgroundColour** | `const wxColour& colour` |
| `void` | **setImage** | `const wxString& name` |
| `void` | **selectRegion** | `const wxString& name` |
| `void` | **renameRegion** | `const wxString& currentname, const wxString& newname` |
| `void` | **addRegion** | `const wxString& name, const wxRect& dim` |
| `void` | **setRegionArea** | `const wxString& name, const wxRect& dim` |
| `void` | **deleteRegion** | `const wxString& name` |
| `void` | **deleteAllRegions** | `` |
| `void` | **computeZoomFactor** | `int MouseWheelDirection` |
| `bool` | **Reset** | `` |
| `void` | **Render** | `` |
| `bool` | **isDocumentValid** | `` |
| `void` | **renderImageset** | `` |
| `void` | **initialiseCEGUI** | `` |
| `void` | **cleanupCEGUI** | `` |
| `bool` | **resetCEGUI** | `` |
| `void` | **createCEGUIWindows** | `` |
| `void` | **updateScrollbars** | `` |
| `void` | **updateImageScrollPosition** | `` |
| `bool` | **handleRegionNorthSouthCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionEastWestCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionNorthEastSouthWestCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionNorthWestSouthEastCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionMoveCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionNormalCursor** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionMouseEnter** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionMouseLeave** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionActivated** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionDeactivated** | `const CEGUI::EventArgs& e` |
| `bool` | **handleRegionModified** | `const CEGUI::EventArgs& e` |
| `void` | **OnResize** | `wxSizeEvent& event` |
| `void` | **OnPaint** | `wxPaintEvent& event` |
| `void` | **OnErase** | `wxEraseEvent& event` |
| `void` | **OnMouseMotion** | `wxMouseEvent& event` |
| `void` | **OnLeftDown** | `wxMouseEvent& event` |
| `void` | **OnLeftUp** | `wxMouseEvent& event` |
| `void` | **OnRightDown** | `wxMouseEvent& event` |
| `void` | **OnRightUp** | `wxMouseEvent& event` |
| `void` | **OnMiddleDown** | `wxMouseEvent& event` |
| `void` | **OnMiddleUp** | `wxMouseEvent& event` |
| `void` | **OnMouseWheel** | `wxMouseEvent& event` |
| `void` | **OnKeyDown** | `wxKeyEvent& event` |
| `void` | **OnKeyUp** | `wxKeyEvent& event` |
| `void` | **OnScrollWin** | `wxScrollWinEvent& event` |

---
## 鏂囦欢: EditorView.h
### Class: `EditorView`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `void` | **onChangeFilename** | `` |
| `void` | **onChangeImageFile** | `const wxString& newfilename` |
| `void` | **onChangeImagesetName** | `const wxString& newname` |
| `void` | **onChangeNativeResolution** | `const wxPoint& newnativeres` |
| `void` | **onChangeAutoScaled** | `const bool newAutoScaled` |
| `void` | **onAddRegion** | `const wxString& name, const wxRect& dim` |
| `void` | **onDelRegion** | `const wxString& name` |
| `void` | **onDelAllRegion** | `` |
| `void` | **onSetRegionArea** | `const wxString& name, const wxRect& dim, const bool evt_src = false` |
| `void` | **onRenameRegion** | `const wxString& currentname, const wxString& newname` |
| `void` | **updateTitle** | `` |
| `void` | **updateMouseStatusBar** | `const float posx, const float posy` |
| `void` | **updateDocStatusBar** | `const float zoom, const float width, const float height` |
| `void` | **OnDraw** | `wxDC* dc` |
| `bool` | **OnCreate** | `wxDocument* doc, long flags` |
| `bool` | **OnClose** | `bool deleteWindow = true` |
| `void` | **OnUpdate** | `wxView* sender, wxObject* hint` |

---
## 鏂囦欢: PropertiesPanel.h
### Class: `PropertiesPanel`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `void` | **setView** | `EditorView* aView` |
| `void` | **createContent** | `` |
| `void` | **reset** | `` |
| `void` | **onGridCellChange** | `wxGridEvent& event` |
| `void` | **onGridCellEdit** | `wxGridEvent& event` |
| `void` | **onGridCellSelect** | `wxGridEvent& event` |
| `wxString` | **getImagesetName** | `` |
| `void` | **setImagesetName** | `const wxString& name` |
| `wxString` | **getImageFilename** | `` |
| `void` | **setImageFilename** | `const wxString& name` |
| `wxPoint` | **getNativeResolution** | `` |
| `void` | **setNativeResolution** | `const wxPoint& NativeResolution` |
| `bool` | **getAutoScaled** | `` |
| `void` | **setAutoScaled** | `const bool AutoScaled` |
| `void` | **addRegion** | `const wxString& name, const wxRect& dim, int rowindex = -1` |
| `int` | **deleteRegion** | `const wxString& name` |
| `void` | **deleteAllRegions** | `` |
| `int` | **selectRegion** | `const wxString& name` |
| `wxString` | **getSelectedRegion** | `` |
| `void` | **setRegionArea** | `const wxString& name, const wxRect& dim` |
| `wxRect` | **getRegionArea** | `const wxString& name` |
| `void` | **renameRegion** | `const wxString& currentname, const wxString& newname` |
| `bool` | **isDocumentValid** | `` |
| `EditorDocument*` | **getDocument** | `` |
| `wxString` | **getUniqueImageName** | `` |
| `void` | **createPropertiesGrid** | `wxWindow* parent` |
| `int` | **getInternalRegionIndex** | `const wxString& name` |
| `wxString` | **getDefaultResourceGroupRelativePath** | `const wxString& filename` |

---
## 鏂囦欢: wxPathCellEditor.h
### Class: `wxPathCellEditor`

| 杩斿洖绫诲瀷 | 鏂规硶鍚?| 鍙傛暟 |
| :--- | :--- | :--- |
| `public:` | **wxPathCellEditor** | `` |
| `void` | **Create** | `wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler` |
| `void` | **SetSize** | `const wxRect& rect` |
| `void` | **PaintBackground** | `const wxRect& rectCell, wxGridCellAttr *attr` |
| `bool` | **IsAcceptedKey** | `wxKeyEvent& event` |
| `void` | **BeginEdit** | `int row, int col, wxGrid* grid` |
| `bool` | **EndEdit** | `int row, int col, wxGrid* grid` |
| `void` | **Reset** | `` |
| `void` | **StartingKey** | `wxKeyEvent& event` |
| `void` | **HandleReturn** | `wxKeyEvent& event` |
| `void` | **SetParameters** | `const wxString& params` |
| `wxString` | **GetValue** | `` |
| `void` | **OnBrowseButtonClick** | `wxCommandEvent& event` |
| `void` | **DoBeginEdit** | `const wxString& startValue` |
| `void` | **DoReset** | `const wxString& startValue` |

---

