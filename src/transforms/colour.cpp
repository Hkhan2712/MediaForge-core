#include "cpp_core/image_processor.hpp"
#include <vips/vips.h>
#include <vips/error.h>

namespace cpp_core {
namespace transforms {

bool Colour(VImage& image, const ColourOptions& options, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Colour: No image loaded");
        return false;
    }

    VipsInterpretation target_interp;
    switch (options.to) {
        case ColorSpace::SRGB:
            target_interp = VIPS_INTERPRETATION_sRGB;
            break;
        case ColorSpace::CMYK:
            target_interp = VIPS_INTERPRETATION_CMYK;
            break;
        case ColorSpace::GREY:
            target_interp = VIPS_INTERPRETATION_B_W;
            break;
        case ColorSpace::LAB:
            target_interp = VIPS_INTERPRETATION_LAB;
            break;
        default:
            err = make_error(ErrorCode::UNSUPPORTED_FORMAT, "Colour: Unsupported color space");
            return false;
    }

    VipsImage* out = nullptr;
    int ret = vips_colourspace(image.get(), &out, target_interp, nullptr);
    if (ret != 0) {
        err = last_vips_error("Colour");
        return false;
    }

    image.reset(out);
    return true;
}

} // namespace transforms
} // namespace cpp_core