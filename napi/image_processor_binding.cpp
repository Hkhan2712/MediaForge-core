#include <napi.h>
#include "../include/cpp_core/core.hpp"               // VipsEnv
#include "../include/cpp_core/image_processor.hpp"
#include "../include/cpp_core/image_io.hpp"
#include "../include/cpp_core/vimage.hpp"
#include "../include/cpp_core/transforms.hpp"
#include <memory>
#include <vector>
#include <string>

using namespace cpp_core;

// Global VipsEnv instance, init 1 lần khi addon load
std::unique_ptr<VipsEnv> g_vips_env;

Napi::Buffer<uint8_t> ProcessImageWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber())
        throw Napi::TypeError::New(env, "Expected arguments: inputPath(string), width(number), height(number)");

    std::string input = info[0].As<Napi::String>().Utf8Value();
    int width = info[1].As<Napi::Number>().Int32Value();
    int height = info[2].As<Napi::Number>().Int32Value();

    // Load image
    VipsError err;
    VImage img = ImageIO::LoadFromFile(input, &err);
    if (!err.ok())
        throw Napi::Error::New(env, err.message);
        
    // Process image
    ImageProcessor processor;
    processor.SetImage(std::move(img));

    ResizeOptions resize_opts{width, height};
    if (!processor.Resize(resize_opts))
        throw Napi::Error::New(env, "Resize failed");

    // Save to buffer
    SaveOptions save_opts;
    save_opts.format = ImageFormat::JPEG;
    std::vector<uint8_t> buffer = ImageIO::SaveToBuffer(processor.GetImage(), save_opts, &err);
    if (buffer.empty())
        throw Napi::Error::New(env, "SaveToBuffer failed");

    return Napi::Buffer<uint8_t>::Copy(env, buffer.data(), buffer.size());
}

// Module init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Init libvips once
    if (!g_vips_env) {
        g_vips_env = std::make_unique<VipsEnv>(
            "mediaforge",   // prog_name
            4,              // concurrency
            1024*1024*1024, // cache_max_mem ~1GB
            500,            // cache_max_ops
            100             // cache_max_files
        );
    }

    exports.Set("processImage", Napi::Function::New(env, ProcessImageWrapped));
    return exports;
}

NODE_API_MODULE(image_processor, Init)