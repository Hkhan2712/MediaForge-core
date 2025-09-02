#include <vips/vips.h>
#include "cpp_core/image_processor.hpp"
#include "cpp_core/errors.hpp"

namespace cpp_core {
namespace transforms {

bool Crop(VImage& image, const CropOptions& options, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::PROCESSING_ERROR, "Crop: No image loaded");
        return false;
    }

    int img_w = vips_image_get_width(image.get());
    int img_h = vips_image_get_height(image.get());

    // Validate crop rectangle
    if (options.width <= 0 || options.height <= 0 ||
        options.left < 0 || options.top < 0 ||
        options.left + options.width > img_w ||
        options.top + options.height > img_h) {
        err = make_error(ErrorCode::INVALID_ARGUMENT,
                         "Crop: Invalid crop rectangle");
        return false;
    }

    VipsImage* cropped = nullptr;
    if (vips_crop(image.get(), &cropped,
                  options.left, options.top,
                  options.width, options.height,
                  nullptr)) {
        err = make_error(ErrorCode::PROCESSING_ERROR,
                         std::string("Crop: vips_crop failed: ") + vips_error_buffer());
        return false;
    }

    image.reset(cropped);
    return true;
}

} // namespace transforms
} // namespace cpp_core
