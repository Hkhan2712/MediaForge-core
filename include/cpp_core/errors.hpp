#pragma once
#include <string>
#include "enums.hpp"

namespace cpp_core {

struct VipsError {
    ErrorCode code;
    std::string message;

    VipsError(ErrorCode c = ErrorCode::SUCCESS, std::string msg = "")
        : code(c), message(std::move(msg)) {}

    bool ok() const { return code == ErrorCode::SUCCESS; }
};

// chỉ khai báo
VipsError last_vips_error(const char* context = nullptr);
VipsError make_error(ErrorCode code, const std::string& msg);

} // namespace cpp_core
