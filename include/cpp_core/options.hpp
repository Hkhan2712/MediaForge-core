#include <string>
#include "cpp_core/enums.hpp"

#pragma once

namespace cpp_core {

struct SaveOptions {
    ImageFormat format = ImageFormat::JPEG;
    int quality = 90; // 1-100
    bool progressive = true;
    bool strip = true;
    bool lossless = false;
    int effort = 6;
    std::string heif_compression = "av1";
};

struct ResizeOptions {
    int width = 0;
    int height = 0;
    FitMode fit = FitMode::CONTAIN;
    bool without_enlargement = false;
    bool sharpen = false;
    double sharpen_sigma = 1.0;   
    double sharpen_x1 = 2.0;      
};

struct CropOptions {
    int left = 0, top = 0, width = 0, height = 0;
    bool use_gravity = false;
    Gravity gravity = Gravity::CENTER;
};

struct ColourOptions {
    ColorSpace to = ColorSpace::SRGB;
    bool prefer_icc = true;
    bool autrotate = true;
};

struct WatermarkOptions {
    std::string text;
    std::string font = "Arial";
    int font_size = 24;
    double opacity = 0.25; // 0.0 - 1.0
    Gravity gravity = Gravity::SOUTHEAST;
    int margin = 10; // pixels
};

} // namespace cpp_core