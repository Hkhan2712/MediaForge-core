#include <cstdio>
#include "cpp_core/image_io.hpp"
#include "cpp_core/vimage.hpp"
#include "cpp_core/errors.hpp"
#include <vips/vips8>

namespace cpp_core {

// ---------------- Load ----------------
VImage ImageIO::LoadFromFile(const std::string& path, VipsError* err) {
    VipsImage* img = vips_image_new_from_file(path.c_str(), nullptr);
    if (!img) {
        if (err) *err = last_vips_error("LoadFromFile");
        return VImage();
    }
    return VImage(img); // move constructor
}

VImage ImageIO::LoadFromBuffer(const void* data, size_t size, VipsError* err) {
    VipsImage* img = vips_image_new_from_buffer(data, size, nullptr, nullptr);
    if (!img) {
        if (err) *err = last_vips_error("LoadFromBuffer");
        return VImage();
    }
    return VImage(img); // move constructor
}

// ---------------- Save ----------------
std::vector<u_int8_t> ImageIO::SaveToBuffer(const VImage& img, const SaveOptions& options, VipsError* err) {
    std::vector<u_int8_t> buffer;
    if (!img) return buffer;

    void* out_data = nullptr;
    size_t out_size = 0;
    int ret = -1;

    switch (options.format) {
        case ImageFormat::JPEG:
            ret = vips_jpegsave_buffer(img.get(), &out_data, &out_size,
                                       "Q", options.quality,
                                       "strip", options.strip,
                                       "interlace", options.progressive,
                                       "optimize_coding", TRUE,
                                       nullptr);
            break;
        case ImageFormat::PNG:
            ret = vips_pngsave_buffer(img.get(), &out_data, &out_size,
                                      "compression", options.effort,
                                      "strip", options.strip,
                                      "lossless", options.lossless,
                                      nullptr);
            break;
        case ImageFormat::WEBP:
            ret = vips_webpsave_buffer(img.get(), &out_data, &out_size,
                                       "Q", options.quality,
                                       "strip", options.strip,
                                       "lossless", options.lossless,
                                       "interlace", options.progressive,
                                       nullptr);
            break;
        case ImageFormat::HEIF:
            ret = vips_heifsave_buffer(img.get(), &out_data, &out_size,
                                       "Q", options.quality,
                                       "strip", options.strip,
                                       "compression", options.heif_compression.c_str(),
                                       nullptr);
            break;
        case ImageFormat::AVIF:
            ret = vips_heifsave_buffer(img.get(), &out_data, &out_size,
                                       "Q", options.quality,
                                       "lossless", options.lossless,
                                       "compression", options.heif_compression.c_str(),
                                       nullptr);
            break;
        default:
            if (err) *err = make_error(ErrorCode::UNSUPPORTED_FORMAT, "Format not supported");
            return {};
    }

    if (ret != 0) {
        if (err) *err = last_vips_error("SaveToBuffer");
        return {};
    }

    buffer.assign((u_int8_t*)out_data, (u_int8_t*)out_data + out_size);
    g_free(out_data);
    return buffer;
}

bool ImageIO::SaveToFile(const VImage& img, const std::string& path, const SaveOptions& options, VipsError* err) {
    if (!img) return false;

    int ret = -1;
    switch (options.format) {
        case ImageFormat::JPEG:
            ret = vips_jpegsave(img.get(), path.c_str(),
                                "Q", options.quality,
                                "strip", options.strip,
                                "interlace", options.progressive,
                                "optimize_coding", TRUE,
                                nullptr);
            break;
        case ImageFormat::PNG:
            ret = vips_pngsave(img.get(), path.c_str(),
                               "compression", options.effort,
                               "strip", options.strip,
                               "lossless", options.lossless,
                               nullptr);
            break;
        case ImageFormat::WEBP:
            ret = vips_webpsave(img.get(), path.c_str(),
                                "Q", options.quality,
                                "strip", options.strip,
                                "lossless", options.lossless,
                                "interlace", options.progressive,
                                nullptr);
            break;
        case ImageFormat::HEIF:
            ret = vips_heifsave(img.get(), path.c_str(),
                                "Q", options.quality,
                                "strip", options.strip,
                                "compression", options.heif_compression.c_str(),
                                nullptr);
            break;
        case ImageFormat::AVIF:
            ret = vips_heifsave(img.get(), path.c_str(),
                                "Q", options.quality,
                                "lossless", options.lossless,
                                "compression", options.heif_compression.c_str(),
                                nullptr);
            break;
        default:
            if (err) *err = make_error(ErrorCode::UNSUPPORTED_FORMAT, "Format not supported");
            return false;
    }

    if (ret != 0) {
        if (err) *err = last_vips_error("SaveToFile");
        return false;
    }

    return true;
}

} // namespace cpp_core