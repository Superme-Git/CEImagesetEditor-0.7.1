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
#ifndef _ELASTIC_BOX_WIDGET_H_
#define _ELASTIC_BOX_WIDGET_H_

// parent class
#include <CEGUIWindow.h>
// for factory creation
#include <CEGUIWindowFactory.h>
// we reuse the SizingLocation enumeration from FrameWindow
#include <elements/CEGUIFrameWindow.h>
// properties for this class
#include "ElasticBoxProperties.h"

/*!
\brief
    边界框小部件的类。

    elastic box（弹性框）小部件是 CEImagesetEditor 的自定义小部件；它提供了一个
    可以拖动调整大小和拖动移动的小部件。小部件的目的是提供
    一种可视化定义某个区域的方法。

    ElasticBox 在某种程度上基于 CEGUI 核心库中的 FrameWindow。
*/
class ElasticBox : public CEGUI::Window {
public:
  //! 全局事件的命名空间
  static const CEGUI::String EventNamespace;
  //! 窗口工厂名称
  static const CEGUI::String WidgetTypeName;
  //! 当光标要更改为正常时触发的事件
  static const CEGUI::String EventSetNormalCursor;
  //! 当光标要更改为移动时触发的事件
  static const CEGUI::String EventSetMoveCursor;
  //! 当光标要更改为东西方向（水平）调整大小时触发的事件
  static const CEGUI::String EventSetEastWestCursor;
  //! 当光标要更改为南北方向（垂直）调整大小时触发的事件
  static const CEGUI::String EventSetNorthSouthCursor;
  //! 当光标要更改为东北/西南方向调整大小时触发的事件
  static const CEGUI::String EventSetNorthEastSouthWestCursor;
  //! 当光标要更改为西北/东南方向调整大小时触发的事件
  static const CEGUI::String EventSetNorthWestSouthEastCursor;
  //! 调整大小边框的默认大小（以像素为单位）
  static const float DefaultSizingBorderSize;

  /*!
  \brief
      ElasticBox 对象的构造函数。
  */
  ElasticBox(const CEGUI::String &name, const CEGUI::String &type);

  /*!
  \brief
      ElasticBox 对象的析构函数。
  */
  virtual ~ElasticBox(void);

  /*!
  \brief
      返回此窗口是否可（由用户）调整大小。

  \return
      - true 如果窗口可调整大小
      - false 如果窗口不可调整大小
  */
  bool isSizable(void) const;

  /*!
  \brief
      返回窗口是否可（由用户）移动。

  \return
      - true 如果窗口可移动
      - false 如果窗口位置固定
  */
  bool isMovable(void) const;

  /*!
  \brief
      返回调整大小边框的厚度（以像素为单位）。

  \return
      描述调整大小边框厚度的 float 值（以像素为单位）。
  */
  float getSizingBorderThickness(void) const;

  /*!
  \brief
      返回当前的缩放“捕捉”值。

      缩放捕捉值用于将调整大小和移动操作限制为缩放捕捉值的
      像素倍数（例如，在操作缩放图像时很有用）。默认为 1。

  \return
      描述当前缩放捕捉值的 float 值。
  */
  float getScaleSnap(void) const;

  /*!
  \brief
      启用或禁用此窗口的（用户）调整大小。

  \param setting
      - true 启用调整大小
      - false 禁用调整大小

  \return
      无
  */
  void setSizable(bool setting);

  /*!
  \brief
      启用或禁用此窗口的（用户）移动。

  \param setting
      - true 启用窗口移动
      - false 禁用窗口移动

  \return
      无。
  */
  void setMovable(bool setting);

  /*!
  \brief
      设置此窗口的调整大小边框的大小（以像素为单位）。

  \param pixels
      指定调整大小边框厚度的 float 值（以像素为单位）。

  \return
      无。
  */
  void setSizingBorderThickness(float pixels);

  /*!
  \brief
      设置当前的缩放“捕捉”值。

      缩放捕捉值用于将调整大小和移动操作限制为缩放捕捉值的
      像素倍数（例如，在操作缩放图像时很有用）。默认为 1。

  \param value
      描述要使用的缩放捕捉值的 float 值。

  \return
      无
  */
  void setScaleSnap(float value);

  // 基类实现和覆盖
  virtual void initialiseComponents(void);

protected:
  /*!
  \brief
      将窗口的左边缘移动 'delta'。窗口的其余部分不移动，
      因此这会改变窗口的大小。

  \param delta
      指定移动窗口边缘的量和方向的 float 值。正值使窗口变小。
  */
  void moveLeftEdge(float delta);

  /*!
  \brief
      将窗口的右边缘移动 'delta'。窗口的其余部分不移动，
      因此这会改变窗口的大小。

  \param delta
      指定移动窗口边缘的量和方向的 float 值。正值使窗口变大。
  */
  void moveRightEdge(float delta);

  /*!
  \brief
      将窗口的上边缘移动 'delta'。窗口的其余部分不移动，
      因此这会改变窗口的大小。

  \param delta
      指定移动窗口边缘的量和方向的 float 值。正值使窗口变小。
  */
  void moveTopEdge(float delta);

  /*!
  \brief
      将窗口的下边缘移动 'delta'。窗口的其余部分不移动，
      因此这会改变窗口的大小。

  \param delta
      指定移动窗口边缘的量和方向的 float 值。正值使窗口变大。
  */
  void moveBottomEdge(float delta);

  /*!
  \brief
      检查本地像素坐标点 'pt'，并根据该点落在调整大小边框上的位置
      返回 CEGUI::FrameWindow::SizingLocation 枚举值之一。

  \param pt
      Point 对象，描述要检查的相对于窗口的像素偏移量。

  \return
      CEGUI::FrameWindow::SizingLocation 枚举值之一，
      描述 \a pt 对应的调整大小边框部分（如果有）。
  */
  CEGUI::FrameWindow::SizingLocation
  getSizingBorderAtPoint(const CEGUI::Point &pt) const;

  /*!
  \brief
      如果给定的 CEGUI::FrameWindow::SizingLocation 在左边缘，则返回 true。

  \param loc
      要检查的 CEGUI::FrameWindow::SizingLocation 值。

  \return
      - true 如果 \a loc 在左边缘。
      - false 如果 \a loc 不在左边缘。
  */
  bool isLeftSizingLocation(CEGUI::FrameWindow::SizingLocation loc) const;

  /*!
  \brief
      如果给定的 CEGUI::FrameWindow::SizingLocation 在右边缘，则返回 true。

  \param loc
      要检查的 CEGUI::FrameWindow::SizingLocation 值。

  \return
      - true 如果 \a loc 在右边缘。
      - false 如果 \a loc 不在右边缘。
  */
  bool isRightSizingLocation(CEGUI::FrameWindow::SizingLocation loc) const;

  /*!
  \brief
      如果给定的 CEGUI::FrameWindow::SizingLocation 在上边缘，则返回 true。

  \param loc
      要检查的 CEGUI::FrameWindow::SizingLocation 值。

  \return
      - true 如果 \a loc 在上边缘。
      - false 如果 \a loc 不在上边缘。
  */
  bool isTopSizingLocation(CEGUI::FrameWindow::SizingLocation loc) const;

  /*!
  \brief
      如果给定的 CEGUI::FrameWindow::SizingLocation 在下边缘，则返回 true。

  \param loc
      要检查的 CEGUI::FrameWindow::SizingLocation 值。

  \return
      - true 如果 \a loc 在下边缘。
      - false 如果 \a loc 不在下边缘。
  */
  bool isBottomSizingLocation(CEGUI::FrameWindow::SizingLocation loc) const;

  /*!
  \brief
      返回一个 Rect，它以窗口相对像素坐标描述此窗口的调整大小
      区域的外边缘。
  */
  CEGUI::Rect getSizingRect(void) const;

  /*!
  \brief
      处理调整大小操作的更新逻辑。

  \param mousePosition
      窗口本地像素坐标中的最新鼠标位置。
  */
  void doSizingUpdate(const CEGUI::Point &mousePosition);

  /*!
  \brief
      处理移动操作的更新逻辑。

  \param mousePosition
      窗口本地像素坐标中的最新鼠标位置。
  */
  void doMovingUpdate(const CEGUI::Point &mousePosition);

  /*!
  \brief
      调用适当的处理程序来处理光标更改要求。

      此处提供的基类处理程序仅触发事件。
  */
  void callCursorChangeHandler(const CEGUI::Point &mousePosition);

  /*!
  \brief
      用于计算 x 轴上的捕捉增量的辅助函数。
  */
  float calculateSnappedXDelta(float delta);

  /*!
  \brief
      用于计算 y 轴上的捕捉增量的辅助函数。
  */
  float calculateSnappedYDelta(float delta);

  /*!
  \brief
      返回此窗口是否在继承层次结构的某个点
      继承自给定的类名。

  \param class_name
      要检查的类名。

  \return
      - true 如果此窗口继承自 \a class_name。
      - false 如果此窗口未继承自 \a class_name。
  */
  virtual bool testClassName_impl(const CEGUI::String &class_name) const;

  /*!
  \brief
      当光标要更改回正常状态时调用的处理程序。
  */
  virtual void onSetNormalCursor(CEGUI::WindowEventArgs &e);

  /*!
  \brief
      当光标要更改为移动时调用的处理程序。
  */
  virtual void onSetMoveCursor(CEGUI::WindowEventArgs &e);

  /*!
  \brief
      当光标要更改为东西方向（水平）调整大小时调用的处理程序。
  */
  virtual void onSetEastWestCursor(CEGUI::WindowEventArgs &e);

  /*!
  \brief
      当光标要更改为南北方向（垂直）调整大小时调用的处理程序。
  */
  virtual void onSetNorthSouthCursor(CEGUI::WindowEventArgs &e);

  /*!
  \brief
      当光标要更改为东北/西南方向调整大小时调用的处理程序。
  */
  virtual void onSetNorthEastSouthWestCursor(CEGUI::WindowEventArgs &e);

  /*!
  \brief
      当光标要更改为西北/东南方向调整大小时调用的处理程序。
  */
  virtual void onSetNorthWestSouthEastCursor(CEGUI::WindowEventArgs &e);

  // 覆盖事件处理
  virtual void onMouseMove(CEGUI::MouseEventArgs &e);
  virtual void onMouseButtonDown(CEGUI::MouseEventArgs &e);
  virtual void onMouseButtonUp(CEGUI::MouseEventArgs &e);
  virtual void onCaptureLost(CEGUI::WindowEventArgs &e);

  // 实现数据
  //! 如果窗口可由用户调整大小，则为 true。
  bool d_sizable;
  //! 如果窗口可由用户移动，则为 true。
  bool d_movable;
  //! 如果窗口当前正在被调整大小，则为 true。
  bool d_beingSized;
  //! 如果窗口当前正在被移动，则为 true。
  bool d_beingMoved;
  //! 此窗口周围调整大小边框的厚度
  float d_borderThickness;
  //! 当前缩放捕捉值
  float d_scaleSnap;
  //! 跟踪当前拖动的 x 像素 - 用于缩放捕捉实现。
  float d_xSnapPixelsMoved;
  //! 跟踪当前拖动的 y 像素 - 用于缩放捕捉实现。
  float d_ySnapPixelsMoved;
  //! 窗口被拖动的点。
  CEGUI::Point d_dragPoint;
  /** 跟踪我们知道的上一个调整大小位置。这是必须的，因为在高缩放级别下
   * - 由于“捕捉” -
   * 我们可能会将鼠标移出窗口边缘，此时它将不会显示为调整大小位置，
   * 从而导致响应非常粗糙。
   */
  CEGUI::FrameWindow::SizingLocation d_lastSizingLocation;

private:
  //! 访问可调整大小状态的属性
  static ElasticBoxProperties::Sizable d_sizableProperty;
  //! 访问可移动状态的属性
  static ElasticBoxProperties::Movable d_movableProperty;
  //! 访问调整大小边框厚度设置的属性
  static ElasticBoxProperties::SizingBorderThickness d_borderThicknessProperty;

  //! 将属性添加到窗口！
  void addElasticBoxProperties(void);
};

/*!
\brief
    ElasticBox 对象的 WindowFactory 类。
*/
class ElasticBoxFactory : public CEGUI::WindowFactory {
public:
  ElasticBoxFactory() : CEGUI::WindowFactory(ElasticBox::WidgetTypeName) {}
  CEGUI::Window *createWindow(const CEGUI::String &name) {
    return new ElasticBox(d_type, name);
  }
  void destroyWindow(CEGUI::Window *window) { delete window; }
};

ElasticBoxFactory &getElasticBoxFactory();

#endif // _ELASTIC_BOX_WIDGET_H_
