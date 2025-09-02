#include <vips/vips.h>
#include "cpp_core/image_processor.hpp"
#include "cpp_core/transforms.hpp"
#include "cpp_core/errors.hpp"

namespace cpp_core {
namespace transforms {

bool Resize(VImage& image, const ResizeOptions& options, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::PROCESSING_ERROR, "Resize: No image loaded");
        return false;
    }

    if (options.width <= 0 && options.height <= 0) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Resize: Invalid target size: both width & height <= 0");
        return false;
    }

    const int in_w = vips_image_get_width(image.get());
    const int in_h = vips_image_get_height(image.get());

    const double scale_x = (options.width  > 0) ? 
        static_cast<double>(options.width)  / in_w : 0.0;
    const double scale_y = (options.height > 0) ? 
        static_cast<double>(options.height) / in_h : 0.0;

    double scale = 1.0;
    VipsImage* out = nullptr;

    switch (options.fit) {
        case FitMode::COVER:
            scale = std::max(scale_x > 0 ? scale_x : scale_y,
                             scale_y > 0 ? scale_y : scale_x);
            break;
        case FitMode::CONTAIN:
            scale = std::min(scale_x > 0 ? scale_x : scale_y,
                             scale_y > 0 ? scale_y : scale_x);
            break;
        case FitMode::FILL:
            if (vips_resize(image.get(), &out, scale_x > 0 ? scale_x : scale_y,
                            "vscale", scale_y > 0 ? scale_y : scale_x,
                            nullptr)) {
                err = make_error(ErrorCode::PROCESSING_ERROR, std::string("Resize: vips_resize failed: ") + vips_error_buffer());
                return false;
            }
            break;
        case FitMode::INSIDE:
            scale = std::min(scale_x > 0 ? scale_x : scale_y,
                             scale_y > 0 ? scale_y : scale_x);
            if (options.without_enlargement && scale > 1.0) {
                return true;
            }
            break;
        case FitMode::OUTSIDE:
            scale = std::max(scale_x > 0 ? scale_x : scale_y,
                             scale_y > 0 ? scale_y : scale_x);
            break;
        default:
            scale = std::min(scale_x, scale_y);
    }

    if (!out) {
        if (scale <= 0) {
            err = make_error(ErrorCode::PROCESSING_ERROR, "Resize: Invalid computed scale factor: " + std::to_string(scale));
            return false;
        }

        if (vips_resize(image.get(), &out, scale, nullptr)) {
            err = make_error(ErrorCode::PROCESSING_ERROR, std::string("Resize: vips_resize failed: ") + vips_error_buffer());
            return false;
        }

        if (options.fit == FitMode::COVER && options.width > 0 && options.height > 0) {
            VipsImage* cropped = nullptr;
            const int left = std::max(0, (vips_image_get_width(out) - options.width) / 2);
            const int top  = std::max(0, (vips_image_get_height(out) - options.height) / 2);

            if (vips_crop(out, &cropped, left, top, options.width, options.height, nullptr)) {
                g_object_unref(out);
                err = make_error(ErrorCode::PROCESSING_ERROR, std::string("Resize: vips_crop failed: ") + vips_error_buffer());
                return false;
            }
            g_object_unref(out);
            out = cropped;
        }
    }

    if (options.sharpen) {
        VipsImage* sharpened = nullptr;
        if (vips_sharpen(out, &sharpened,
                         "sigma", options.sharpen_sigma,
                         "x1", options.sharpen_x1,
                         nullptr)) {
            g_object_unref(out);
            err = make_error(ErrorCode::PROCESSING_ERROR, std::string("Resize: vips_sharpen failed: ") + vips_error_buffer());
            return false;
        }
        g_object_unref(out);
        out = sharpened;
    }

    image.reset(out);
    return true;
}

} // namespace transforms
} // namespace cpp_core