#include "cpp_core/image_processor.hpp"
#include <vips/vips.h>
#include <vips/error.h>
#include "cpp_core/utils/enum_utils.hpp"
#include "cpp_core/transforms.hpp"
#include "cpp_core/errors.hpp"

namespace cpp_core {

// ------------------- Transformations -------------------
bool ImageProcessor::Resize(const ResizeOptions& options) {
    if (!image_) return false;
    return transforms::Resize(image_, options, last_err_);
}

bool ImageProcessor::Crop(const CropOptions& options) {
    if (!image_) return false;
    return transforms::Crop(image_, options, last_err_);
}

bool ImageProcessor::Rotate90() {
    if (!image_) return false;

    return transforms::Rotate90(image_, last_err_);
}

bool ImageProcessor::Colour(const ColourOptions& options) {
    if (!image_) return false;

    return transforms::Colour(image_, options, last_err_);
}

bool ImageProcessor::Sharpen(double radius) {
    if (!image_) return false;

    return transforms::Sharpen(image_, radius, last_err_);
}

bool ImageProcessor::Watermark(const WatermarkOptions& options) {
    if (!image_) return false;

    return transforms::Watermark(image_, options, last_err_);
}

} // namespace cpp_core