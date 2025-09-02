#include <vector>
#include <string>
#include "errors.hpp"
#include "vimage.hpp"
#include "options.hpp"

namespace cpp_core {

class ImageIO {
    public: 
    // Load image from file or memory buffer
    static VImage LoadFromFile(const std::string& path, VipsError* err = nullptr);
    static VImage LoadFromBuffer(const void* data, size_t size, VipsError* err = nullptr);
    // Save image to file or memory buffer
    static bool SaveToFile(const VImage& image, const std::string& path, const SaveOptions& options, VipsError* err = nullptr);
    static std::vector<u_int8_t> SaveToBuffer(const VImage& image, const SaveOptions& options, VipsError* err = nullptr);
};

} // namespace cpp_core