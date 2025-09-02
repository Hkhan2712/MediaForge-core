#include "cpp_core/vimage.hpp"

namespace cpp_core {

VImage::VImage() = default;

VImage::VImage(VipsImage* img) : image_(img) {}

VImage::~VImage() {
    reset();
}

void VImage::reset(VipsImage* img) {
    if (image_) {
        g_object_unref(image_);
    }
    image_ = img;
}

VImage::VImage(VImage&& other) noexcept : image_(other.image_) {
    other.image_ = nullptr;
}

VImage& VImage::operator=(VImage&& other) noexcept {
    if (this != &other) {
        reset();
        image_ = other.image_;
        other.image_ = nullptr;
    }
    return *this;
}

VipsImage* VImage::get() const {
    return image_;
}

VipsImage* VImage::release() {
    VipsImage* t = image_;
    image_ = nullptr;
    return t;
}

VImage::operator bool() const {
    return image_ != nullptr;
}

// ---- Helper API ----
int VImage::width() const {
    return image_ ? image_->Xsize : 0;
}

int VImage::height() const {
    return image_ ? image_->Ysize : 0;
}

int VImage::bands() const {
    return image_ ? image_->Bands : 0;
}

VipsBandFormat VImage::format() const {
    return image_ ? image_->BandFmt : VIPS_FORMAT_NOTSET;
}

VipsInterpretation VImage::interpretation() const {
    return image_ ? image_->Type : VIPS_INTERPRETATION_ERROR;
}

std::string VImage::filename() const {
    return (image_ && image_->filename) ? std::string(image_->filename) : "";
}

} // namespace cpp_core