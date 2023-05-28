#include "cs19_hsv_color.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cassert>

namespace cs19 {
    HsvColor::HsvColor(float hue, float saturation, float value) {
        if (hue < 0 || hue > 360 || saturation < 0 || saturation > 1 || value < 0 || value > 1) {
            throw std::domain_error("Parameter out of range");
        }
        _hue = hue;
        _saturation = saturation;
        _value = value;
        C = value * saturation;
        X = C * (1 - fabs(fmod(hue / 60.0, 2) - 1));
        m = value - C;
        if (_hue >= 0 && _hue < 60) {
            r = C;
            g = X;
            b = 0;
        } else if (_hue >= 60 && _hue < 120) {
            r = X;
            g = C;
            b = 0;
        } else if (_hue >= 120 && _hue < 180) {
            r = 0;
            g = C;
            b = X;
        } else if (_hue >= 180 && _hue < 240) {
            r = 0;
            g = X;
            b = C;
        } else if (_hue >= 240 && _hue < 300) {
            r = X;
            g = 0;
            b = C;
        } else {
            r = C;
            g = 0;
            b = X;
        }
    }

    HsvColor HsvColor::operator~() const {
        float complement_hue = fmod(_hue + 180, 360);
        return HsvColor(complement_hue, _saturation, _value);
    }

    HsvColor HsvColor::operator|(const HsvColor &that) const {
        float inter_hue = (_hue + that._hue) / 2;
        float inter_saturation = (_saturation + that._saturation) / 2;
        float inter_value = (_value + that._value) / 2;
        return HsvColor(inter_hue, inter_saturation, inter_value);
    }

    HsvColor HsvColor::grayscale() const {
        return HsvColor(_hue, 0, _value);
    }

    std::string HsvColor::to_hex_string() const {
        int red = this->red();
        std::stringstream red_stream;
        if (red >= 0 && red <= 15) {
            red_stream << "0" << std::hex << red;
        } else {
            red_stream << std::hex << red;
        }
        std::string red_hex = red_stream.str();
        int green = this->green();
        std::stringstream green_stream;
        if (green >= 0 && green <= 15) {
            green_stream << "0" << std::hex << green;
        } else {
            green_stream << std::hex << green;
        }
        std::string green_hex = green_stream.str();
        int blue = this->blue();
        std::stringstream blue_stream;
        if (blue >= 0 && blue <= 15) {
            blue_stream << "0" << std::hex << blue;
        } else {
            blue_stream << std::hex << blue;
        }
        std::string blue_hex = blue_stream.str();
        std::stringstream return_stream;
        return_stream << "#" << red_hex << green_hex << blue_hex;
        std::string return_string = return_stream.str();
        return return_string;
    }

    std::string HsvColor::to_rgb_string() const {
        std::string red = std::to_string(this->red());
        std::string green = std::to_string(this->green());
        std::string blue = std::to_string(this->blue());
        std::stringstream return_stream;
        return_stream << "rgb(" << red << "," << green << "," << blue << ")";
        std::string return_string = return_stream.str();
        return return_string;
    }

    float HsvColor::hue() const {
        return _hue;
    }

    float HsvColor::saturation() const {
        return _saturation * 100;
    }

    float HsvColor::value() const {
        return _value * 100;
    }

    int HsvColor::red() const {
        return floor((r + m) * 255 + 0.5);
    }

    int HsvColor::green() const {
        return floor((g + m) * 255 + 0.5);
    }

    int HsvColor::blue() const {
        return floor((b + m) * 255 + 0.5);
    }
}  // namespace cs19