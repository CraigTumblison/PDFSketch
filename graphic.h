// Copyright...

#ifndef PDFSKETCH_GRAPHIC_H__
#define PDFSKETCH_GRAPHIC_H__

#include <cairo.h>

#include "view.h"

namespace pdfsketch {

class Color {
 public:
  Color()
      : red_(0.0), green_(0.0), blue_(0.0), alpha_(1.0) {}  // opaque black
  Color(double red, double green, double blue, double alpha)
      : red_(red), green_(green), blue_(blue), alpha_(alpha) {}
  void CairoSetSourceRGBA(cairo_t* cr) const {
    cairo_set_source_rgba(cr, red_, green_, blue_, alpha_);
  }
  double red_, green_, blue_, alpha_;
};

const int kKnobNone        = 0;
const int kKnobUpperLeft   = 1 << 0;
const int kKnobUpperMiddle = 1 << 1;
const int kKnobUpperRight  = 1 << 2;
const int kKnobMiddleLeft  = 1 << 3;
const int kKnobMiddleRight = 1 << 4;
const int kKnobLowerLeft   = 1 << 5;
const int kKnobLowerMiddle = 1 << 6;
const int kKnobLowerRight  = 1 << 7;
const int kAllKnobs = 0xff;

class GraphicDelegate {
 public:
  virtual void SetNeedsDisplayInPageRect(int page, const Rect& rect) = 0;
  virtual Point ConvertPointFromGraphic(int page, const Point& point) = 0;
  virtual Point ConvertPointToGraphic(int page, const Point& point) = 0;
  virtual double GetZoom() = 0;
};

class Graphic {
 public:
  Graphic()
      : frame_(30.0, 30.0, 10.0, 20.0),
        natural_size_(1.0, 1.0),
        page_(1),
        fill_color_(0.0, 1.0, 0.0, 0.3),
        line_width_(1.0),
        upper_sibling_(NULL),
        lower_sibling_(NULL),
        resizing_knob_(kKnobNone),
        delegate_(NULL) {}

  void SetDelegate(GraphicDelegate* delegate) {
    delegate_ = delegate;
  }

  // For these, constrain means shift key is held (so aspect ratio should
  // be constrained)

  // Placement
  virtual void Place(int page, const Point& location, bool constrain);
  virtual void PlaceUpdate(const Point& location, bool constrain);
  // Returns true if this graphic should be deleted.
  // By default, graphics with 0 size are deleted.
  virtual bool PlaceComplete();

  virtual void BeginResize(const Point& location, int knob, bool constrain);
  virtual void UpdateResize(const Point& location, bool constrain);
  virtual void EndResize();

  virtual void Draw(cairo_t* cr) {}
  void DrawKnobs(cairo_t* cr);
  int Page() const { return page_; }

  // returns the knob hit, or kKnobNone if none.
  int PointInKnob(const Point& location) const;

  // Drawing frames are for the regions that need to be redrawn
  Rect DrawingFrame() const;
  Rect DrawingFrameWithKnobs() const;

  Rect KnobFrame(int knob) const;
  Rect DrawingKnobFrame(int knob) const;

  void SetNeedsDisplay(bool withKnobs) const;
  Rect frame_;  // location in page
  Size natural_size_;
  int page_;
  Color fill_color_;
  Color stroke_color_;
  double line_width_;

  // if should be drawn horizontall/vertically flipped
  bool h_flip_:1;
  bool v_flip_:1;

  Graphic* upper_sibling_;
  Graphic* lower_sibling_;

 protected:
  virtual int Knobs() const { return kAllKnobs; }

 private:
  int resizing_knob_;  // kKnobNone if no resize in progress
  GraphicDelegate* delegate_;
};

}  // namespace pdfsketch

#endif  // PDFSKETCH_GRAPHIC_H__
