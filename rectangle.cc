// Copyright...

#include "rectangle.h"

namespace pdfsketch {

Rectangle::~Rectangle() {
  printf("~Rectangle()\n");
}

void Rectangle::Serialize(pdfsketchproto::Graphic* out) const {
  printf("%s:%d\n", __FILE__, __LINE__);
  Graphic::Serialize(out);
  printf("%s:%d\n", __FILE__, __LINE__);
  out->set_type(pdfsketchproto::Graphic::RECTANGLE);
  printf("%s:%d\n", __FILE__, __LINE__);
}

void Rectangle::Draw(cairo_t* cr, bool selected) {
  frame_.CairoRectangle(cr);
  fill_color_.CairoSetSourceRGBA(cr);
  cairo_fill_preserve(cr);
  stroke_color_.CairoSetSourceRGBA(cr);
  cairo_set_line_width(cr, line_width_);
  cairo_stroke(cr);
}

}  // namespace pdfsketch
