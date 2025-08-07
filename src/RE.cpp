#include "RE.hpp"
#include <cstring>

RuntimeError :: RuntimeError(std :: string s1) : s(s1) {}
std :: string RuntimeError :: message() const { return s; }