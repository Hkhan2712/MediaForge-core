#pragma once
#include <vips/vips.h>
#include "cpp_core/image_processor.hpp"
#include "cpp_core/errors.hpp"

namespace cpp_core {
namespace transforms {

// API declarations only
bool Resize(VImage& image, const ResizeOptions& options, VipsError& err);
bool Crop(VImage& image, const CropOptions& options, VipsError& err);
bool Rotate90(VImage& image, VipsError& err);
bool Colour(VImage& image, const ColourOptions& options, VipsError& err);
bool Sharpen(VImage& image, double radius, VipsError& err);
bool Watermark(VImage& image, const WatermarkOptions& options, VipsError& err);

} // namespace transforms
} // namespace cpp_core
