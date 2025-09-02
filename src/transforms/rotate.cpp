#include "cpp_core/image_processor.hpp"
#include <vips/vips.h>

namespace cpp_core {
namespace transforms {

bool Rotate90(VImage& image, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Rotate90: No image loaded");
        return false;
    }

    VipsImage* out = nullptr;

    // vips_rot90 quay 90 độ theo chiều kim đồng hồ
    int ret = vips_rot90(image.get(), &out, nullptr);
    if (ret != 0) {
        err = last_vips_error("Rotate90");
        return false;
    }

    image.reset(out);
    return true;
}

// Nếu muốn rotate theo góc tùy chọn
bool Rotate(VImage& image, int angle, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Rotate: No image loaded");
        return false;
    }

    VipsImage* out = nullptr;
    int ret = 0;

    switch (angle % 360) {
        case 0:
            // không cần quay
            return true;
        case 90:
            ret = vips_rot90(image.get(), &out, nullptr);
            break;
        case 180:
            ret = vips_rot180(image.get(), &out, nullptr);
            break;
        case 270:
            ret = vips_rot270(image.get(), &out, nullptr);
            break;
        default:
            err = make_error(ErrorCode::INVALID_ARGUMENT, "Rotate: Unsupported angle (must be 0,90,180,270)");
            return false;
    }

    if (ret != 0) {
        err = last_vips_error("Rotate");
        return false;
    }

    image.reset(out);
    return true;
}

} // namespace transforms
} // namespace cpp_core