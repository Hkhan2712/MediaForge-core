#include "cpp_core/image_processor.hpp"
#include <vips/vips.h>
#include "cpp_core/errors.hpp"
#include <string>
#include <cmath>

namespace cpp_core {
namespace transforms {

bool Watermark(VImage& image, const WatermarkOptions& options, VipsError& err) {
    if (!image) {
        err = make_error(ErrorCode::INVALID_ARGUMENT, "Watermark: No image loaded");
        return false;
    }

    int width  = vips_image_get_width(image.get());
    int height = vips_image_get_height(image.get());

    // --- Tạo text image ---
    VipsImage* text_image = nullptr;
    if (vips_text(&text_image, options.text.c_str(),
                  "font", options.font.c_str(),
                  "dpi", 300,
                  "width", width,
                  "height", height,
                  "align", VIPS_ALIGN_CENTRE,
                  nullptr)) {
        err = last_vips_error("Watermark: vips_text failed");
        return false;
    }

    // --- Convert text image sang RGBA ---
    VipsImage* rgba = nullptr;
    if (vips_bandjoin_const1(text_image, &rgba, 255, nullptr)) {
        g_object_unref(text_image);
        err = last_vips_error("Watermark: vips_bandjoin_const1 failed");
        return false;
    }
    g_object_unref(text_image);

    // --- Nhân alpha với opacity ---
    VipsImage* alpha = nullptr;
    double factor[1] = { options.opacity };
    if (vips_linear(rgba, &alpha, factor, nullptr, 1, nullptr)) {
        g_object_unref(rgba);
        err = last_vips_error("Watermark: vips_linear failed");
        return false;
    }
    g_object_unref(rgba);

    // --- Embed vào ảnh gốc theo gravity ---
    int left = 0, top = 0;
    if (options.gravity == Gravity::CENTER) {
        left = (width - vips_image_get_width(alpha)) / 2;
        top  = (height - vips_image_get_height(alpha)) / 2;
    } else if (options.gravity == Gravity::SOUTHEAST) {
        left = width - vips_image_get_width(alpha) - options.margin;
        top  = height - vips_image_get_height(alpha) - options.margin;
    } else if (options.gravity == Gravity::NORTHEAST) {
        left = width - vips_image_get_width(alpha) - options.margin;
        top  = options.margin;
    } else if (options.gravity == Gravity::SOUTHWEST) {
        left = options.margin;
        top  = height - vips_image_get_height(alpha) - options.margin;
    } else if (options.gravity == Gravity::NORTHWEST) {
        left = options.margin;
        top  = options.margin;
    }

    VipsImage* out = nullptr;
    if (vips_insert(image.get(), alpha, &out, left, top, nullptr)) {
        g_object_unref(alpha);
        err = last_vips_error("Watermark: vips_insert failed");
        return false;
    }
    g_object_unref(alpha);

    image.reset(out);
    return true;
}

} // namespace transforms
} // namespace cpp_core