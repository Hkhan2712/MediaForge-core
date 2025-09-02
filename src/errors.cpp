#include "cpp_core/errors.hpp"
#include <vips/vips.h>
#include <vips/error.h>
#include <string>

namespace cpp_core {

static ErrorCode map_vips_error(const std::string& msg) {
    if (msg.find("no such file") != std::string::npos ||
        msg.find("No such file") != std::string::npos) {
        return ErrorCode::FILE_NOT_FOUND;
    }
    if (msg.find("Permission denied") != std::string::npos) {
        return ErrorCode::PERMISSION_DENIED;
    }
    if (msg.find("memory") != std::string::npos ||
        msg.find("out of memory") != std::string::npos) {
        return ErrorCode::OUT_OF_MEMORY;
    }
    if (msg.find("invalid") != std::string::npos ||
        msg.find("Invalid") != std::string::npos) {
        return ErrorCode::INVALID_ARGUMENT;
    }
    if (msg.find("unsupported") != std::string::npos ||
        msg.find("Unsupported") != std::string::npos) {
        return ErrorCode::UNSUPPORTED_FORMAT;
    }
    if (msg.find("timeout") != std::string::npos) {
        return ErrorCode::TIMEOUT;
    }
    if (msg.find("cancelled") != std::string::npos) {
        return ErrorCode::CANCELLED;
    }
    return ErrorCode::PROCESSING_ERROR;
}

// định nghĩa thật, KHÔNG INLINE
VipsError last_vips_error(const char* context) {
    const char* buf = vips_error_buffer();
    std::string raw_msg = buf ? buf : "Unknown libvips error";
    ErrorCode code = map_vips_error(raw_msg);

    std::string full_msg = context ? std::string(context) + ": " + raw_msg : raw_msg;

    vips_error_clear();
    return VipsError{code, full_msg};
}

VipsError make_error(ErrorCode code, const std::string& msg) {
    return VipsError{code, msg};
}

} // namespace cpp_core