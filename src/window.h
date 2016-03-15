#ifndef NIGHTWING_WINDOW_H_
#define NIGHTWING_WINDOW_H_

#include <xcb/xcb.h>
#include <string.h>
#include "rect.h"
#include "build.h"

namespace nightwing {

class WindowHandler;
class Session;

enum WindowTypes {
  kNormal,
  kDecorator,
  kSpecial
};

// This a base class for meta informations and properties on Window.
// It's designed to get easy access and manipulation of properties.
// Each window is identified by its |id_|.
//
// Basic geometrical properties (x, y, width, height) are contained
// in |rect_|. See rect.h for more informatio about this structure.
//
// Applying properties on X can be done with WindowHandler class.
// Ref to window-handler.h.
class Window {
 public:
  friend class WindowHandler;
  friend class Session;

  Window(xcb_window_t id);

  int get_border_width() const {
    return border_width_;
  }

  void set_border_width(int border_width) {
    border_width_ = border_width;
  }

  Rect get_rect() {
    return rect_;
  }

  void set_rect(const Rect rect) {
    rect_ = rect;
  }

  xcb_window_t get_id() const {
    return id_;
  }

  Window* get_decorator() {
    return decorator_;
  }

  void set_decorator(Window* decorator) {
    decorator_ = decorator;
  }

  Window* get_parent() {
    return   parent_;
  }

  void set_parent(Window* parent) {
    parent_ = parent;
  }

  bool is_visiable() {
    return visiable_;
  }

  void Map() {
    visiable_ = true;
  }

  void Unmap() {
    visiable_ = false;
  }

  WindowTypes get_type() const {
    return type_;
  }

  void set_type(const WindowTypes type) {
    type_ = type;
  }

  operator xcb_window_t() const {
    return id_;
  }

  const char* get_title() const {
    return title_;
  }

  void set_title(const char* title) {
    // TODO(ppofuk): Safer method for string copy.
    if(strlen(title) >= NIGHTWING_TITLE_MAX_SIZE) {
      strncpy(title_, title, NIGHTWING_TITLE_MAX_SIZE - 1);
    } else {
      strcpy(title_, title);
    }
    DEBUG("window.set_title(\"%s\")", title_);
  }

  void set_title(const char* title, uint32_t size) {
    if (size >= NIGHTWING_TITLE_MAX_SIZE)
      size = NIGHTWING_TITLE_MAX_SIZE - 1;

    memcpy(title_, title, static_cast<size_t>(size));
    title_[size] = '\0';
  }

 protected:
  xcb_window_t id_;
  int border_width_;
  Rect rect_;

  Window* decorator_;
  Window* parent_;

  bool visiable_;

  WindowTypes type_;

  char title_[NIGHTWING_TITLE_MAX_SIZE];

 private:
};

} // namespace nightwing
#endif // NIGHTWING_WINDOW_H_
