#include <iostream>
#include "cpp_core/image_io.hpp"
#include "cpp_core/image_processor.hpp"

using namespace cpp_core;

int main() {
    // ---------------- Load image ----------------
    VipsError err;
    VImage img = ImageIO::LoadFromFile("input.jpg", &err);
    if (!img) {
        std::cerr << "Failed to load image: " << err.message << "\n";
        return 1;
    }
    std::cout << "Loaded image: " << img.filename() 
              << " (" << img.width() << "x" << img.height() << ")\n";

    // ---------------- Process: Resize ----------------
    ImageProcessor processor;
    processor.SetImage(std::move(img));

    cpp_core::ResizeOptions resize_opts{600, 600}; // resize width=800, height=600
    if (!processor.Resize(resize_opts)) {
        std::cerr << "Resize failed\n";
        return 1;
    }

    VImage& result = processor.GetImage(); // Lấy ảnh đã resize

    // ---------------- Save to file ----------------
    cpp_core::SaveOptions save_opts;
    save_opts.format = ImageFormat::JPEG;
    save_opts.quality = 90;
    save_opts.progressive = true;
    save_opts.strip = true;

    if (!ImageIO::SaveToFile(result, "resized_output.jpg", save_opts, &err)) {
        std::cerr << "Failed to save resized image: " << err.message << "\n";
        return 1;
    }

    std::cout << "Saved resized image to resized_output.jpg\n";
    return 0;
}