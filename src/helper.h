#pragma once

#include <sstream>

#define prt(x) Godot::print(String((std::stringstream() << x).str().c_str()))
