#pragma once

#include <vips/vips.h>
#include <string>
#include "cpp_core/enums.hpp"

namespace cpp_core {
namespace utils {

// ImageFormat -> string
inline const char* to_string(ImageFormat fmt) {
    switch (fmt) {
        case ImageFormat::JPEG:  return "JPEG";
        case ImageFormat::PNG:   return "PNG";
        case ImageFormat::WEBP:  return "WEBP";
        case ImageFormat::TIFF:  return "TIFF";
        case ImageFormat::GIF:   return "GIF";
        case ImageFormat::BMP:   return "BMP";
        case ImageFormat::HEIF:  return "HEIF";
        case ImageFormat::AVIF:  return "AVIF";
        default: return "UNKNOWN";
    }
}

// FitMode -> string
inline const char* to_string(FitMode mode) {
    switch (mode) {
        case FitMode::FILL:    return "FILL";
        case FitMode::CONTAIN: return "CONTAIN";
        case FitMode::COVER:   return "COVER";
        case FitMode::INSIDE:  return "INSIDE";
        case FitMode::OUTSIDE: return "OUTSIDE";
        default: return "UNKNOWN";
    }
}

// Gravity -> string
inline const char* to_string(Gravity g) {
    switch (g) {
        case Gravity::CENTER:     return "CENTER";
        case Gravity::NORTH:      return "NORTH";
        case Gravity::EAST:       return "EAST";
        case Gravity::SOUTH:      return "SOUTH";
        case Gravity::WEST:       return "WEST";
        case Gravity::NORTHEAST:  return "NORTHEAST";
        case Gravity::SOUTHEAST:  return "SOUTHEAST";
        case Gravity::SOUTHWEST:  return "SOUTHWEST";
        case Gravity::NORTHWEST:  return "NORTHWEST";
        default: return "UNKNOWN";
    }
}

// ColorSpace -> string
inline const char* to_string(ColorSpace cs) {
    switch (cs) {
        case ColorSpace::SRGB:         return "sRGB";
        case ColorSpace::ADOBE_RGB:    return "AdobeRGB";
        case ColorSpace::PROPHOTO_RGB: return "ProPhotoRGB";
        case ColorSpace::CMYK:         return "CMYK";
        case ColorSpace::LAB:          return "Lab";
        case ColorSpace::XYZ:          return "XYZ";
        case ColorSpace::GREY:         return "Grey";
        default: return "UNKNOWN";
    }
}

// ErrorCode -> string
inline const char* to_string(ErrorCode code) {
    switch (code) {
        case ErrorCode::SUCCESS:            return "SUCCESS";
        case ErrorCode::FILE_NOT_FOUND:     return "FILE_NOT_FOUND";
        case ErrorCode::PERMISSION_DENIED:  return "PERMISSION_DENIED";
        case ErrorCode::OUT_OF_MEMORY:      return "OUT_OF_MEMORY";
        case ErrorCode::INVALID_ARGUMENT:   return "INVALID_ARGUMENT";
        case ErrorCode::UNSUPPORTED_FORMAT: return "UNSUPPORTED_FORMAT";
        case ErrorCode::PROCESSING_ERROR:   return "PROCESSING_ERROR";
        case ErrorCode::TIMEOUT:            return "TIMEOUT";
        case ErrorCode::CANCELLED:          return "CANCELLED";
        default:                            return "UNKNOWN_ERROR";
    }
}

// Convenience overload: return std::string
template <typename EnumT>
inline std::string to_string_s(EnumT e) {
    return std::string(to_string(e));
}

inline VipsCompassDirection to_vips(Gravity g) {
    switch (g) {
        case Gravity::CENTER:     return VIPS_COMPASS_DIRECTION_CENTRE;
        case Gravity::NORTH:      return VIPS_COMPASS_DIRECTION_NORTH;
        case Gravity::EAST:       return VIPS_COMPASS_DIRECTION_EAST;
        case Gravity::SOUTH:      return VIPS_COMPASS_DIRECTION_SOUTH;
        case Gravity::WEST:       return VIPS_COMPASS_DIRECTION_WEST;
        case Gravity::NORTHEAST:  return VIPS_COMPASS_DIRECTION_NORTH_EAST;
        case Gravity::SOUTHEAST:  return VIPS_COMPASS_DIRECTION_SOUTH_EAST;
        case Gravity::SOUTHWEST:  return VIPS_COMPASS_DIRECTION_SOUTH_WEST;
        case Gravity::NORTHWEST:  return VIPS_COMPASS_DIRECTION_NORTH_WEST;
    }
    return VIPS_COMPASS_DIRECTION_CENTRE; // fallback
}

} // namespace utils
} // namespace cpp_core