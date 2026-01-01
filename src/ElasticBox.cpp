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
#include "ElasticBox.h"
#include <CEGUICoordConverter.h>
#include <cmath>

using namespace CEGUI;

// 全局事件的命名空间
const CEGUI::String ElasticBox::EventNamespace("ElasticBox");
// 窗口工厂名称
const CEGUI::String ElasticBox::WidgetTypeName("CEImagesetEditor/ElasticBox");
// 调整大小边框的默认大小（以像素为单位）
const float ElasticBox::DefaultSizingBorderSize = 3.0f;
// 当光标要更改为正常时触发的事件
const CEGUI::String ElasticBox::EventSetNormalCursor("SetNormalCursor");
// 当光标要更改为移动时触发的事件
const CEGUI::String ElasticBox::EventSetMoveCursor("SetMoveCursor");
// 当光标要更改为东西方向（水平）调整大小时触发的事件
const CEGUI::String ElasticBox::EventSetEastWestCursor("SetEastWestCursor");
// 当光标要更改为南北方向（垂直）调整大小时触发的事件
const CEGUI::String ElasticBox::EventSetNorthSouthCursor("SetNorthSouthCursor");
// 当光标要更改为东北/西南方向调整大小时触发的事件
const CEGUI::String
    ElasticBox::EventSetNorthEastSouthWestCursor("SetNorthEastSouthWestCursor");
// 当光标要更改为西北/东南方向调整大小时触发的事件
const CEGUI::String
    ElasticBox::EventSetNorthWestSouthEastCursor("SetNorthWestSouthEastCursor");
// 访问可调整大小状态的属性
ElasticBoxProperties::Sizable ElasticBox::d_sizableProperty;
// 访问可移动状态的属性
ElasticBoxProperties::Movable ElasticBox::d_movableProperty;
// 访问调整大小边框厚度设置的属性
ElasticBoxProperties::SizingBorderThickness
    ElasticBox::d_borderThicknessProperty;

//-----------------------------------------------------------------------
ElasticBox::ElasticBox(const CEGUI::String &name, const CEGUI::String &type)
    : Window(name, type), d_sizable(true), d_movable(true), d_beingSized(false),
      d_beingMoved(false), d_borderThickness(DefaultSizingBorderSize),
      d_scaleSnap(1), d_xSnapPixelsMoved(0), d_ySnapPixelsMoved(0),
      d_lastSizingLocation(FrameWindow::SizingNone) {
  addElasticBoxProperties();
}

//-----------------------------------------------------------------------
ElasticBox::~ElasticBox(void) {}

//-----------------------------------------------------------------------
bool ElasticBox::isSizable(void) const { return d_sizable; }

//-----------------------------------------------------------------------
bool ElasticBox::isMovable(void) const { return d_movable; }

//-----------------------------------------------------------------------
float ElasticBox::getSizingBorderThickness(void) const {
  return d_borderThickness;
}

//-----------------------------------------------------------------------
float ElasticBox::getScaleSnap(void) const { return d_scaleSnap; }

//-----------------------------------------------------------------------
void ElasticBox::setSizable(bool setting) {
  d_sizable = setting;

  if (d_beingSized)
    releaseInput();
}

//-----------------------------------------------------------------------
void ElasticBox::setMovable(bool setting) {
  d_movable = setting;

  if (d_beingMoved)
    releaseInput();
}

//-----------------------------------------------------------------------
void ElasticBox::setSizingBorderThickness(float pixels) {
  d_borderThickness = pixels;
}

//-----------------------------------------------------------------------
void ElasticBox::initialiseComponents(void) {
  // 目前这里无需执行任何操作；我们没有组件。
}

//-----------------------------------------------------------------------
void ElasticBox::setScaleSnap(float value) {
  d_scaleSnap = value;
  d_xSnapPixelsMoved = 0.0f;
  d_ySnapPixelsMoved = 0.0f;
}

//-----------------------------------------------------------------------
void ElasticBox::moveLeftEdge(float delta) {
  // TODO：我们目前不处理 HA_LEFT 以外的对齐方式

  URect area(d_area);
  area.d_min.d_x.d_offset += delta;
  setArea(area);
}

//-----------------------------------------------------------------------
void ElasticBox::moveRightEdge(float delta) {
  // TODO：我们目前不处理 HA_LEFT 以外的对齐方式

  float orgWidth = d_pixelSize.d_width;

  URect area(d_area);
  area.d_max.d_x.d_offset += delta;
  setArea(area);

  // 移动拖动点，以便鼠标保持附着在窗口边缘
  d_dragPoint.d_x += d_pixelSize.d_width - orgWidth;
}

//-----------------------------------------------------------------------
void ElasticBox::moveTopEdge(float delta) {
  // TODO：我们目前不处理 HA_LEFT 以外的对齐方式

  URect area(d_area);
  area.d_min.d_y.d_offset += delta;
  setArea(area);
}

//-----------------------------------------------------------------------
void ElasticBox::moveBottomEdge(float delta) {
  // TODO：我们目前不处理 HA_LEFT 以外的对齐方式

  float orgHeight = d_pixelSize.d_height;

  URect area(d_area);
  area.d_max.d_y.d_offset += delta;
  setArea(area);

  // 移动拖动点，以便鼠标保持附着在窗口边缘
  d_dragPoint.d_y += d_pixelSize.d_height - orgHeight;
}

//-----------------------------------------------------------------------
CEGUI::FrameWindow::SizingLocation
ElasticBox::getSizingBorderAtPoint(const CEGUI::Point &pt) const {
  CEGUI::Rect frame(getSizingRect());

  // 我们只能在启用框架且开启调整大小时调整大小
  if (isSizable()) {
    // 点必须在外边缘内
    if (frame.isPointInRect(pt)) {
      // 调整矩形以获取内边缘
      frame.d_left += d_borderThickness;
      frame.d_top += d_borderThickness;
      frame.d_right -= d_borderThickness;
      frame.d_bottom -= d_borderThickness;

      // 检测我们在哪条边缘上
      bool top = (pt.d_y < frame.d_top);
      bool bottom = (pt.d_y >= frame.d_bottom);
      bool left = (pt.d_x < frame.d_left);
      bool right = (pt.d_x >= frame.d_right);

      // 返回适当的 'SizingLocation' 值
      if (top && left)
        return FrameWindow::SizingTopLeft;
      else if (top && right)
        return FrameWindow::SizingTopRight;
      else if (bottom && left)
        return FrameWindow::SizingBottomLeft;
      else if (bottom && right)
        return FrameWindow::SizingBottomRight;
      else if (top)
        return FrameWindow::SizingTop;
      else if (bottom)
        return FrameWindow::SizingBottom;
      else if (left)
        return FrameWindow::SizingLeft;
      else if (right)
        return FrameWindow::SizingRight;
    }
  }

  // 默认值：无。
  return FrameWindow::SizingNone;
}
//-----------------------------------------------------------------------
bool ElasticBox::isLeftSizingLocation(
    CEGUI::FrameWindow::SizingLocation loc) const {
  return ((loc == CEGUI::FrameWindow::SizingLeft) ||
          (loc == CEGUI::FrameWindow::SizingTopLeft) ||
          (loc == CEGUI::FrameWindow::SizingBottomLeft));
}

//-----------------------------------------------------------------------
bool ElasticBox::isRightSizingLocation(
    CEGUI::FrameWindow::SizingLocation loc) const {
  return ((loc == CEGUI::FrameWindow::SizingRight) ||
          (loc == CEGUI::FrameWindow::SizingTopRight) ||
          (loc == CEGUI::FrameWindow::SizingBottomRight));
}

//-----------------------------------------------------------------------
bool ElasticBox::isTopSizingLocation(
    CEGUI::FrameWindow::SizingLocation loc) const {
  return ((loc == CEGUI::FrameWindow::SizingTop) ||
          (loc == CEGUI::FrameWindow::SizingTopLeft) ||
          (loc == CEGUI::FrameWindow::SizingTopRight));
}

//-----------------------------------------------------------------------
bool ElasticBox::isBottomSizingLocation(
    CEGUI::FrameWindow::SizingLocation loc) const {
  return ((loc == CEGUI::FrameWindow::SizingBottom) ||
          (loc == CEGUI::FrameWindow::SizingBottomLeft) ||
          (loc == CEGUI::FrameWindow::SizingBottomRight));
}

//-----------------------------------------------------------------------
CEGUI::Rect ElasticBox::getSizingRect(void) const {
  return CEGUI::Rect(0, 0, d_pixelSize.d_width, d_pixelSize.d_height);
}

//-----------------------------------------------------------------------
void ElasticBox::doSizingUpdate(const CEGUI::Point &mousePosition) {
  // 计算用于调整大小的增量
  float delta_x = calculateSnappedXDelta(mousePosition.d_x - d_dragPoint.d_x);
  float delta_y = calculateSnappedYDelta(mousePosition.d_y - d_dragPoint.d_y);

  // 调整左边缘或右边缘的大小
  if (isLeftSizingLocation(d_lastSizingLocation))
    moveLeftEdge(delta_x);
  else if (isRightSizingLocation(d_lastSizingLocation))
    moveRightEdge(delta_x);

  // 调整上边缘或下边缘的大小
  if (isTopSizingLocation(d_lastSizingLocation))
    moveTopEdge(delta_y);
  else if (isBottomSizingLocation(d_lastSizingLocation))
    moveBottomEdge(delta_y);
}

//-----------------------------------------------------------------------
void ElasticBox::doMovingUpdate(const CEGUI::Point &mousePosition) {
  // 计算窗口移动的量（以像素为单位）
  float delta_x = calculateSnappedXDelta(mousePosition.d_x - d_dragPoint.d_x);
  float delta_y = calculateSnappedYDelta(mousePosition.d_y - d_dragPoint.d_y);

  // 计算偏移量
  UVector2 offset(cegui_absdim(delta_x), cegui_absdim(delta_y));

  URect area(d_area);
  area.offset(offset);

  // 更新位置
  setArea(area);
}

//-----------------------------------------------------------------------
void ElasticBox::callCursorChangeHandler(const CEGUI::Point &mousePosition) {
  WindowEventArgs args(this);

  if (isSizable()) {
    switch (getSizingBorderAtPoint(mousePosition)) {
    case FrameWindow::SizingTop:
    case FrameWindow::SizingBottom:
      onSetNorthSouthCursor(args);
      return;

    case FrameWindow::SizingLeft:
    case FrameWindow::SizingRight:
      onSetEastWestCursor(args);
      return;

    case FrameWindow::SizingTopLeft:
    case FrameWindow::SizingBottomRight:
      onSetNorthWestSouthEastCursor(args);
      return;

    case FrameWindow::SizingTopRight:
    case FrameWindow::SizingBottomLeft:
      onSetNorthEastSouthWestCursor(args);
      return;
    }
  }

  if (isMovable())
    onSetMoveCursor(args);
  else
    onSetNormalCursor(args);
}

//-----------------------------------------------------------------------
float ElasticBox::calculateSnappedXDelta(float delta) {
  // 更新部分移动的拖动点
  d_dragPoint.d_x += delta;
  // 跟踪目前为止进行的部分移动的程度。
  d_xSnapPixelsMoved += delta;

  // 在此缩放设置下，我们是否移动了相当于是 1 个完整像素的距离？
  if (std::fabs(d_xSnapPixelsMoved) > d_scaleSnap) {
    // 如果我们在缩放级别 1，这就是移动的完整像素数
    delta =
        (static_cast<int>(d_xSnapPixelsMoved) / static_cast<int>(d_scaleSnap)) *
        d_scaleSnap;
    // 现在我们实际移动窗口，重置拖动点
    d_dragPoint.d_x -= delta;
    // 保留移动的任何部分像素
    d_xSnapPixelsMoved -= delta;
  } else {
    delta = 0.0f;
  }

  return delta;
}

//-----------------------------------------------------------------------
float ElasticBox::calculateSnappedYDelta(float delta) {
  // 更新部分移动的拖动点
  d_dragPoint.d_y += delta;
  // 跟踪目前为止进行的部分移动的程度。
  d_ySnapPixelsMoved += delta;

  // 在此缩放设置下，我们是否移动了相当于是 1 个完整像素的距离？
  if (std::fabs(d_ySnapPixelsMoved) > d_scaleSnap) {
    // 如果我们在缩放级别 1，这就是移动的完整像素数
    delta =
        (static_cast<int>(d_ySnapPixelsMoved) / static_cast<int>(d_scaleSnap)) *
        d_scaleSnap;
    // 现在我们实际移动窗口，重置拖动点
    d_dragPoint.d_y -= delta;
    // 保留移动的任何部分像素
    d_ySnapPixelsMoved -= delta;
  } else {
    delta = 0.0f;
  }

  return delta;
}

//-----------------------------------------------------------------------
bool ElasticBox::testClassName_impl(const CEGUI::String &class_name) const {
  if (class_name == "ElasticBox")
    return true;
  else
    return Window::testClassName_impl(class_name);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNormalCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetNormalCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetMoveCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetMoveCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetEastWestCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetEastWestCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthSouthCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetNorthSouthCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthEastSouthWestCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetNorthEastSouthWestCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthWestSouthEastCursor(CEGUI::WindowEventArgs &e) {
  fireEvent(EventSetNorthWestSouthEastCursor, e, EventNamespace);
}

//-----------------------------------------------------------------------
void ElasticBox::onMouseMove(CEGUI::MouseEventArgs &e) {
  // 执行默认处理并触发事件。
  Window::onMouseMove(e);

  // 如果我们不是包含鼠标的窗口，则不执行任何操作
  if (System::getSingleton().getWindowContainingMouse() != this)
    return;

  CEGUI::Point localMousePos(CoordConverter::screenToWindow(*this, e.position));

  if (d_beingSized)
    doSizingUpdate(localMousePos);
  else if (d_beingMoved)
    doMovingUpdate(localMousePos);
  else
    callCursorChangeHandler(localMousePos);

  // 将事件标记为已处理
  e.handled = true;
}

//-----------------------------------------------------------------------
void ElasticBox::onMouseButtonDown(CEGUI::MouseEventArgs &e) {
  // 调用默认处理程序以触发事件
  Window::onMouseButtonDown(e);

  // 我们只对左键感兴趣
  if (e.button == LeftButton) {
    // 获取鼠标位置作为此窗口的本地坐标。
    CEGUI::Point localPos(CoordConverter::screenToWindow(*this, e.position));

    // 获取鼠标的调整大小边框位置
    d_lastSizingLocation = getSizingBorderAtPoint(localPos);

    if (isSizable() && (d_lastSizingLocation != FrameWindow::SizingNone)) {
      // 确保现在所有输入都传给我们
      if (captureInput()) {
        // 设置 'dragging'（拖动）状态变量
        d_beingSized = true;
        d_dragPoint = localPos;
        e.handled = true;
      }
    } else if (isMovable() &&
               (d_lastSizingLocation == FrameWindow::SizingNone)) {
      // 确保现在所有输入都传给我们
      if (captureInput()) {
        // 设置 'dragging'（拖动）状态变量
        d_beingMoved = true;
        d_dragPoint = localPos;
        e.handled = true;
      }
    }
  }
}
//-----------------------------------------------------------------------
void ElasticBox::onMouseButtonUp(CEGUI::MouseEventArgs &e) {
  // 调用默认处理程序以触发事件
  Window::onMouseButtonUp(e);

  if (e.button == LeftButton && isCapturedByThis()) {
    // 释放我们对输入数据的捕获
    // （最终这会导致拖动状态重置）
    releaseInput();
    e.handled = true;
  }
}

//-----------------------------------------------------------------------
void ElasticBox::onCaptureLost(CEGUI::WindowEventArgs &e) {
  Window::onCaptureLost(e);

  // 重置拖动状态
  d_beingSized = false;
  d_beingMoved = false;
  d_xSnapPixelsMoved = 0;
  d_ySnapPixelsMoved = 0;

  e.handled = true;
}

//-----------------------------------------------------------------------
void ElasticBox::addElasticBoxProperties(void) {
  addProperty(&d_sizableProperty);
  addProperty(&d_movableProperty);
  addProperty(&d_borderThicknessProperty);
}

ElasticBoxFactory &getElasticBoxFactory() {
  static ElasticBoxFactory s_factory;
  return s_factory;
}
