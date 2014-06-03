# pragma once

namespace controller 
{

  class InputEventHandler
  {
    public:

		// TODO 4.1: declare pure-virtual handle function and define struct keyboard_event DONE?

        enum {
            SHIFT_ACTIVE = 1 << 0,
            CTRL_ACTIVE  = 1 << 1,
            ALT_ACTIVE   = 1 << 2
        };

      struct keyboard_event
      {
		  char key;
		  int modifier_mask;
		  double mouse_pos[2];
      };

	  virtual bool handle(keyboard_event const&) = 0;

  }; // InputEventHandler

} // controller::
