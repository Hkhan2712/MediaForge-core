#pragma once

#include <string>
#include <vector>
#include "vimage.hpp"
#include "options.hpp"
#include "errors.hpp"

namespace cpp_core {
class ImageProcessor {
    VImage image_;
    VipsError last_err_;

public:
    // High-level transforms
    bool Resize(const ResizeOptions& options);
    bool Crop(const CropOptions& options);
    bool Rotate90();
    bool Colour(const ColourOptions& options);
    bool Sharpen(double radius = 0.0);
    bool Watermark(const WatermarkOptions& options);

    // ---------------- I/O helpers ----------------
    void SetImage(VImage&& img) { image_ = std::move(img); }
    VImage& GetImage() { return image_; }
    const VImage& GetImage() const { return image_; }

    // Error info
    const VipsError& LastError() const { return last_err_; }
};
} // namespace cpp_core
