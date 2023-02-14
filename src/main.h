#pragma once
#include "helper.h"

#include <Godot.hpp>
#include <Node.hpp>

namespace godot {
class Main: public Node {
    GODOT_CLASS(Main, Node)

public:
    static void _register_methods();

    Main() {}
    ~Main() {}

    void _init();
    void _ready();
    void _process(float delta);
};

} // namespace godot
