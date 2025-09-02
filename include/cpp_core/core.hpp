// Bootstrap vips, global config (RAII)
#include <iostream>

namespace cpp_core {
    class VipsEnv {
        public:
        VipsEnv(const char* prog_name = "cpp_core",
                int concurrency = 0,
                std::size_t cache_max_mem = 512ull * 1024 * 1024,
                int cache_max_ops = 1000,
                int cache_max_files = 200);
        ~VipsEnv();
    };
}