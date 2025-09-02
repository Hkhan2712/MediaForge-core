#include "cpp_core/image_processor.hpp"
#include "cpp_core/errors.hpp"
#include <vips/vips.h>
#include <vips/error.h>

namespace cpp_core {
namespace transforms {

bool Sharpen(VImage& image, double radius, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::PROCESSING_ERROR, "Sharpen: No image loaded");
        return false;
    }

    if (radius <= 0.0) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Sharpen: Radius must be > 0");
        return false;
    }

    VipsImage* sharpened = nullptr;

    // vips_sharpen mặc định: sigma = radius
    if (vips_sharpen(image.get(), &sharpened,
                     "sigma", radius,
                     nullptr)) {
        err = make_error(ErrorCode::PROCESSING_ERROR,
                         std::string("Sharpen: vips_sharpen failed: ") + vips_error_buffer());
        return false;
    }

    image.reset(sharpened);
    return true;
}

} // namespace transforms
} // namespace cpp_core