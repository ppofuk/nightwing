#include "session.h"

namespace nightwing {


Session* Session::instance_ = 0;

Session* Session::Instance() {
  if (instance_ == 0) {
    instance_ = new Session();
    instance_->has_errors_ = false;
  }

  return instance_;
}


void Session::Release() {
  if (instance_) {
    delete instance_->dummy_root_;
    delete instance_;
  }

  instance_ = 0;
}


void Session::SetupMouse() {
  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS
                  | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC,
                  XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                  1, /* left mouse button */
                  MOUSEMODKEY);

  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS
                  | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
                  root_, XCB_NONE,
                  2, /* middle mouse button */
                  MOUSEMODKEY);

  xcb_grab_button(dpy_, 0, root_,
                  XCB_EVENT_MASK_BUTTON_PRESS
                  | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC,
                  XCB_GRAB_MODE_ASYNC, root_, XCB_NONE,
                  3, /* right mouse button */
                  MOUSEMODKEY);
}


// Moved all thing from constructor to Init method. 
// Member class instances of Session get theirs constructor 
// called after Session constuctor. 
Session::Session() {
  // IT'S NOT SAFE HERE! MOVE TO Init()!
}

void Session::Init() {
  dpy_ = xcb_connect(NULL, &screen_number);
  if (xcb_connection_has_error(dpy_)) {
    has_errors_ = true;
    return;
  }

  screen_iter = xcb_setup_roots_iterator(xcb_get_setup(dpy_));
  for (int i = 0; i < screen_number; ++i){ 
    xcb_screen_next(&screen_iter); 
  }
  screen_ = screen_iter.data;

  if (!screen_) {
      ERROR("Can't get current screen. Exiting");
      xcb_disconnect(dpy_);
      return;
  }
  root_ = screen_->root;

  DEBUG("Session size is %dx%d\n\t\tRoot window: %d",
        screen_->width_in_pixels,
        screen_->height_in_pixels,
        screen_->root);
  
  screen_rect_.set_width(screen_->width_in_pixels);
  screen_rect_.set_height(screen_->height_in_pixels);

  DEBUG("Session root data %ud", screen_->root);

  /* Check for RANDR extension and configure */
  // TODO:

  /* Loop over all clients and setup */
  SetupScreens();

  /* Setup key bindings */
  SetupKeys();

  /* Grab mouse buttons */
  SetupMouse();

  /* Subscribe to events */
  mask_ = XCB_CW_EVENT_MASK;

  values_[0] = XCB_EVENT_MASK_STRUCTURE_NOTIFY
               | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
               | XCB_EVENT_MASK_KEY_RELEASE // values[1]?
               | XCB_EVENT_MASK_BUTTON_PRESS
               | XCB_EVENT_MASK_EXPOSURE
               | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
               | XCB_EVENT_MASK_POINTER_MOTION
               | XCB_EVENT_MASK_EXPOSURE; 


  cookie_ = xcb_change_window_attributes_checked(dpy_, 
                                                 root_,
                                                 mask_,
                                                 values_);
  error_ = xcb_request_check(dpy_, cookie_);
  
  dummy_root_ = new Window(root_); 
  dummy_root_->set_rect(screen_rect_); 

  InitVisualType(); 
  window_handler_.set_dpy(dpy_);
  window_handler_.set_screen(screen_); 
  drawing_handler_.set_dpy(dpy_); 
  drawing_handler_.set_visual_type(visual_type_);

  xcb_flush(dpy_);
}

void Session::InitVisualType() {
  visual_type_ = NULL;
  xcb_depth_iterator_t depth_iter;

  depth_iter = xcb_screen_allowed_depths_iterator (screen_);
  for (; depth_iter.rem; xcb_depth_next (&depth_iter)) {
    xcb_visualtype_iterator_t visual_iter;

    visual_iter = xcb_depth_visuals_iterator (depth_iter.data);
    for (; visual_iter.rem; xcb_visualtype_next (&visual_iter)) {
      if (screen_->root_visual == visual_iter.data->visual_id) {
        visual_type_ = visual_iter.data;
        break;
      }
    }
  }              
}


/*
 * @brief Setup all shortcut keys
 */
void Session::SetupKeys() {
  // This all SetupX() thing aren't way to go. 
  // You'll kill the modularity (always restart after key bind change!?)
  // TODO:
}

/*
 * @brief Walk through all existing windows and set them up
 */
void Session::SetupScreens() {
  int len;
  xcb_window_t* children;
  xcb_query_tree_reply_t *reply;

  reply = xcb_query_tree_reply(dpy_,
                               xcb_query_tree(dpy_, root_), 
                               0);
  if (NULL == reply) {
    ERROR(" SetupScreens(), reply is NULL");
    return;
  }

  len = xcb_query_tree_children_length(reply);
  children = xcb_query_tree_children(reply);
  DEBUG("Number of children windows: %d", len);
  /* Setup all windows on this root */
  // TODO:
}

// private destructor (no wai sherlock! =)) 
Session::~Session() {
}

void Session::MainLoop() {
  while (event_ = xcb_wait_for_event(get_dpy())) {
    switch (event_->response_type & ~0x80) {   
      case XCB_DESTROY_NOTIFY:
        DEBUG("DESTROY_NOTIFY event");
        OnDestroyNotify(); 
        break; 

      case XCB_CONFIGURE_REQUEST:
        DEBUG("CONFIGURE_REQUEST event");
        OnConfigureRequest();
        break;
        
      case XCB_MAP_REQUEST:
        DEBUG("MAP_REQUEST event");
        OnMapRequest();
        break;
          
      case XCB_UNMAP_NOTIFY:
        DEBUG("UNMAP_REQUEST event"); 
        OnUnmapNotify(); 
        break; 
        
      case XCB_ENTER_NOTIFY:
        DEBUG("ENTER_NOTIFY event"); 
        OnEnterNotify(); 
        break;
        
      case XCB_MOTION_NOTIFY:
        // Please, don't DEBUG this out, it's annoying to read one 
        // millions lines of motion notify robo-rap sentences. 
        OnMotionNotify(); 
        break; 
        
      case XCB_EXPOSE:
        DEBUG("XCB_EXPOSE event");
        OnExpose();
        break; 
        
      default:
        // Ignoring uknown event type
        break; 
    }
  }
}

void Session::OnMotionNotify() {
  xcb_motion_notify_event_t* event = 
      (xcb_motion_notify_event_t *) event_; 
}


void Session::OnEnterNotify() {
  xcb_enter_notify_event_t* event = 
      (xcb_enter_notify_event_t *) event_; 
}

void Session::OnMapRequest() {
  xcb_map_request_event_t* event = 
      (xcb_map_request_event_t *)event; 
  
  RegisterWindow(event->window);
}

void Session::OnConfigureRequest() {
  xcb_configure_request_event_t* event = 
      (xcb_configure_request_event_t *) event_; 
  
  RegisterWindow(event->window);
}

void Session::OnDestroyNotify() {
  xcb_destroy_notify_event_t* event = 
      (xcb_destroy_notify_event_t *) event_; 
  
  if (window_handler_.Exists(event->window)) {
    Window* window = window_handler_.Remove(event->window);
    Decorator* decorator = 
        static_cast<Decorator*>(window->get_decorator()); 
    
    if (decorator) {
      window_handler_.Remove(decorator->get_id()); 
      window_handler_.Destroy(decorator); 
      delete decorator; 
    }
    delete window; 
  }
}

void Session::OnUnmapNotify() {
  xcb_unmap_notify_event_t* event = 
      (xcb_unmap_notify_event_t *) event_; 
  
  if (window_handler_.Exists(event->window)) {
    Window* window = window_handler_.Get(event->window);
    if (window->get_decorator()) {
      window->get_decorator()->Unmap(); 
      window_handler_.ApplyVisiablity(window->get_decorator());
    }
  }
}

void Session::OnExpose() {
  xcb_expose_event_t* event = 
      (xcb_expose_event_t *) event_; 
  
  if (!window_handler_.Exists(event->window)) 
    return; 
  
  Window* window = window_handler_.Get(event->window);
  if (window) {
    if (window->get_type() == kDecorator) {
      Decorator* decorator = static_cast<Decorator*>(window);
      decorator->OnExpose(); 

    } else if (window->get_type() == kSpecial) {
      // Our special redraw need :D
    }
  }
}

void Session::RegisterWindow(xcb_window_t id) {
  if (!window_handler_.Exists(id)) {
    Window* window = new Window(id);
    window->set_type(kNormal); 

    xcb_configure_window(dpy_, 
                         id, 
                         XCB_CW_EVENT_MASK,
                         values_);
    
    window->set_rect(screen_rect_);
    window->set_border_width(0);
    CreateDecorator(window); 
    window_handler_.Apply(window); 
    window_handler_.SendConfigureNotify(window); 
  } else {
    Window* window = window_handler_.Get(id);
    if (window->get_type() == kNormal) {
      if (window->get_decorator() == NULL) {
        CreateDecorator(window);
      } else {
        Decorator* decorator = 
            static_cast<Decorator*>(window->get_decorator()); 
        decorator->Map(); 
        decorator->ApplyRect(); 
        window_handler_.Apply(window->get_decorator());
      }
    }

    window_handler_.Apply(window); 
    window_handler_.SendConfigureNotify(window); 
  }
}

void Session::CreateDecorator(Window* window) {
  // TODO: check for fails. 
  xcb_window_t id = xcb_generate_id(dpy_); 
  Decorator* decorator = new Decorator(id);
  
  decorator->set_type(kDecorator); 
  decorator->set_target(window); 
  window->set_decorator(decorator);
  decorator->ApplyRects(window->get_rect());
  
  drawing_handler_.InitSurface(decorator->get_surface(), decorator);
  if (decorator->get_surface().get_cr()) {
    window_handler_.Add(decorator); 
    window_handler_.Raise(decorator);
    window_handler_.Raise(window); 
    decorator->OnExpose(); 
  } else {
    ERROR("Can't init cairo surface on decorator!\n"); 
  }
}

} // namespace nightwing
