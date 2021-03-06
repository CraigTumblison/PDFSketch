// Copyright...

#ifndef PDFKSETCH_TEXT_AREA_H__
#define PDFKSETCH_TEXT_AREA_H__

#include "graphic.h"

namespace pdfsketch {

// Text responsibilities:

// text_ - the text to show (saved)

// Graphic when not editing: paint text to screen
// Prerendered: just draw prerendered PDF
//   graphic_

// Non-prerendered: use fonts to draw text at precomputed locations
//   left_edges_
//   This will get more complex as we do more than just have new lines

// Graphic when resizing: reflow text
// Prerendered: rerender the graphic. Generates graphic_
// Non-prerendered: generates left edges

// Graphic when editing: handle keyboard/mouse input to make edits,
// redraw portions (while reflowing).

// class TextDraw {
//   graphic_ Prerender(string text);
//   Update(offset);
//   line_state_;
// };

// class TextEdit {
//   HandleKeyboard/Mouse();
//   Draw();
//   cursor_location_, virtual_cursor_location_;
//   selection_;
// };

class TextArea : public Graphic {
 public:
  TextArea()
      : selection_start_(0),
        selection_size_(0) {}
  TextArea(const pdfsketchproto::Graphic& msg)
      : Graphic(msg),
        text_(msg.text_area().text()),
        selection_start_(text_.size()),
        selection_size_(0) {}
  virtual void Serialize(pdfsketchproto::Graphic* out) const;
  virtual void Place(int page, const Point& location, bool constrain);
  virtual void PlaceUpdate(const Point& location, bool constrain);
  virtual bool PlaceComplete();

  virtual bool Editable() const { return true; }
  virtual void BeginEditing();
  virtual void EndEditing();
  virtual void OnKeyText(const KeyboardInputEvent& event);
  virtual void OnKeyDown(const KeyboardInputEvent& event);
  virtual void OnKeyUp(const KeyboardInputEvent& event);

  // virtual void BeginResize(const Point& location, int knob, bool constrain);
  // virtual void UpdateResize(const Point& location, bool constrain);
  // virtual void EndResize();

  virtual void Draw(cairo_t* cr, bool selected);

 protected:
  virtual int Knobs() const { return kKnobMiddleLeft | kKnobMiddleRight; }

 private:
  void UpdateLeftEdges(cairo_t* cr);
  std::string GetLine(size_t start_idx, size_t* out_advance);
  std::string DebugLeftEdges();
  size_t GetRowIndex(size_t index);

  void EraseSelection();

  // These two should have the same length:
  std::string text_;
  std::vector<double> left_edges_;

  size_t selection_start_;
  size_t selection_size_;
};

}  // namespace pdfsketch

#endif  // PDFKSETCH_TEXT_AREA_H__
