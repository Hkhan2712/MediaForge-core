#pragma once
#include <string>
#include <vips/vips.h>

namespace cpp_core {

class VImage {
    VipsImage* image_ = nullptr;

public:
    VImage();
    explicit VImage(VipsImage* img);
    VImage(const VImage&) = delete;
    VImage& operator=(const VImage&) = delete;
    VImage(VImage&& other) noexcept;
    VImage& operator=(VImage&& other) noexcept;
    ~VImage();

    void reset(VipsImage* img = nullptr);

    VipsImage* get() const;
    VipsImage* release();
    explicit operator bool() const;

    // ---- Helper API ----
    int width() const;
    int height() const;
    int bands() const;
    VipsBandFormat format() const;
    VipsInterpretation interpretation() const;
    std::string filename() const;
};

} // namespace cpp_core