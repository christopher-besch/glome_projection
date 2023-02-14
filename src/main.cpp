#include "main.h"

#include <Input.hpp>

using namespace godot;

void Main::_register_methods()
{
    register_method("_ready", &godot::Main::_ready);
    register_method("_process", &godot::Main::_process);
}

void Main::_init()
{
}

void Main::_ready()
{
}

void Main::_process(float delta)
{
    Input* input = Input::get_singleton();
    if(input->is_action_just_pressed("quit"))
        quick_exit(0);
}
