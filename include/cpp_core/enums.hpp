#ifndef CPP_CORE_ENUMS_HPP
#define CPP_CORE_ENUMS_HPP

namespace cpp_core {

enum class ImageFormat {
    UNKNOWN,
    JPEG,
    PNG,
    WEBP,
    TIFF,
    GIF,
    BMP,
    HEIF,
    AVIF
};

enum class FitMode {
    FILL,
    CONTAIN,
    COVER,
    INSIDE,
    OUTSIDE
};

enum class Gravity {
    CENTER,
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NORTHEAST,
    SOUTHEAST,
    SOUTHWEST,
    NORTHWEST
};

enum class ColorSpace {
    SRGB,
    ADOBE_RGB,
    PROPHOTO_RGB,
    CMYK,
    LAB,
    XYZ,
    GREY
};

enum class ErrorCode {
    SUCCESS = 0,

    // IO / System
    FILE_NOT_FOUND,
    PERMISSION_DENIED,
    OUT_OF_MEMORY,

    // Input / Validation
    INVALID_ARGUMENT,
    UNSUPPORTED_FORMAT,

    // Processing / Pipeline
    PROCESSING_ERROR,
    TIMEOUT,
    CANCELLED,

    // Generic
    UNKNOWN_ERROR
};

} // namespace cpp_core

#endif // CPP_CORE_ENUMS_HPP