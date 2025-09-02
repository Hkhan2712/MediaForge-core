// src/core.cpp
#include <cpp_core/core.hpp>

#include <vips/vips.h>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <string>

namespace cpp_core {
    static std::atomic<int> g_vips_refcount{0};
    static std::mutex g_vips_mutex;

    VipsEnv::VipsEnv(const char* prog_name, 
                     int concurrency,
                    std::size_t cache_max_mem,
                     int cache_max_ops,
                     int cache_max_files) {
        std::lock_guard<std::mutex> lock(g_vips_mutex);

        if (g_vips_refcount.fetch_add(1) == 0) {
            if (VIPS_INIT(prog_name)) {
                const char* buf = vips_error_buffer();
                std::string msg = std::string("vips_init failded: ") + (buf ? buf : "unknown error");
                vips_error_clear();
                // down refcount since init failed
                g_vips_refcount.store(0);
                throw std::runtime_error(msg);
            } 
            if (concurrency > 0) {
                vips_concurrency_set(concurrency);
            }
            if (cache_max_mem > 0) {
                vips_cache_set_max_mem(static_cast<size_t>(cache_max_mem));
            }
            if (cache_max_ops > 0) {
                vips_cache_set_max(cache_max_ops);
            }
            if (cache_max_files > 0) {
                vips_cache_set_max_files(cache_max_files);
            }   
        } else {
            if (concurrency > 0) {
                vips_concurrency_set(concurrency);
            }
            if (cache_max_mem > 0) {
                vips_cache_set_max_mem(static_cast<size_t>(cache_max_mem));
            }
            if (cache_max_ops > 0) {
                vips_cache_set_max(cache_max_ops);
            }
            if (cache_max_files > 0) {
                vips_cache_set_max_files(cache_max_files); 
            }
        }
    }

    VipsEnv::~VipsEnv() {
        std::lock_guard<std::mutex> lock(g_vips_mutex);

        int prev = g_vips_refcount.fetch_sub(1);
        if (prev == 1) {
            vips_shutdown();
        }
    }
}