#pragma once

enum button_type {
  key, mouse
};

/** Either a mouse or keyboard press */
struct button {
  int code;
  button_type type: 1;
  bool down: 1;
  /** True if this has been changed THIS frame */
  bool just_changed: 1;
  button(int code, button_type type)
    : code(code), type(type), down(false) {}
  button()
    : code(0), type(button_type::key), down(false) {}

  bool just_down() const { return down && just_changed; }
  bool just_up() const { return !down && just_changed; }
};

struct control_state {
  button move;
  control_state() {};
};

/**
   Manages input and maps that to controls. This is a singleton - use the
   create_instance() and get() static functions.
 */
class input_manager {
public:
  control_state cstate = control_state();

private:
  /** An array of buttons we can iterate over */
  button* buttons[1] { &cstate.move };

  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    input_manager* iman = (input_manager*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS) {
      for (const auto b : iman->buttons) {
        if (b->type == button_type::key && b->code == key && !b->down) {
          b->down = true;
          b->just_changed = true;
        }
      }
    } else if (action == GLFW_RELEASE) {
      for (const auto b : iman->buttons) {
        if (b->type == button_type::key && b->code == key && !b->down) {
          b->down = false;
          b->just_changed = false;
        }
      }
    }
  };

  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    input_manager* iman = (input_manager*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS) {
      for (const auto b : iman->buttons) {
        if (b->type == button_type::mouse && b->code == button && !b->down) {
          b->down = true;
          b->just_changed = true;
        }
      }
    } else if (action == GLFW_RELEASE) {
      for (const auto b : iman->buttons) {
        if (b->type == button_type::mouse && b->code == button && b->down) {
          b->down = false;
          b->just_changed = false;
        }
      }
    }
  };

  static void cursor_pos_callback(GLFWwindow* window, double x, double y) {
    input_manager* iman = (input_manager*)glfwGetWindowUserPointer(window);
    iman->mouse_x = (int)x;
    iman->mouse_y = (int)y;
  }

  input_manager(GLFWwindow* window) {
    // Create the control state
    cstate.move = button { GLFW_MOUSE_BUTTON_RIGHT, button_type::mouse };
    std::cout << cstate.move.code << std::endl;
    std::cout << buttons[0]->code << std::endl;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
  };

public:

  /** We only need this for the default value for input_manager_instance... c++
      is such a mess */
  input_manager() {};

  /** Create the new static instance */
  static input_manager& create_instance(GLFWwindow* window);
  static input_manager& get();

  /** Top left is 0,0 */
  int mouse_x, mouse_y;

  /** Polls the GLFW events (call this every frame) */
  void poll() {
    for (const auto b : buttons) {
      b->just_changed = false;
    }
    glfwPollEvents();
  }
};

/* SINGLETON MANAGEMENT */

/* Declare input manager static instances (We can't store this in the
   input_manager class because C++ forbids initialisation of non-const static
   members.......)

   Do NOT move this type (i.e. copy to another place), because it contains
   pointers into itself.
*/
input_manager input_manager_instance;
/** Have we created an input manager instance yet?
*/
bool input_manager_created = false;


/** Create the new static instance */
input_manager& input_manager::create_instance(GLFWwindow* window) {
  // Construct in-place (We need this because input manager contains an array of
  // pointers into itself) */
  new (&input_manager_instance) input_manager(window);
  input_manager_created = true;
  return input_manager_instance;
};

input_manager& input_manager::get() {
  assert(input_manager_created);
  return input_manager_instance;
}
