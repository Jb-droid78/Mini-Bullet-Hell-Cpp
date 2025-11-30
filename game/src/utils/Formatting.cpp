#include "../include/MyInclude/utils/Formatting.hpp"

#include <sstream>
#include <iomanip>
#include <string>

std::string ut::floatFormatting(float value) {
    std::stringstream str;
    str << std::fixed << std::setprecision(1) << value;
    return str.str();
}