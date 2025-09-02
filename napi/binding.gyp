{
  "targets": [
    {
      "target_name": "image_processor",
      "sources": [
      "image_processor_binding.cpp",
      "../src/core.cpp",
      "../src/image_processor.cpp",
      "../src/image_io.cpp",
      "../src/vimage.cpp",
      "../src/transforms.cpp",
    ],
      	"include_dirs": [
			"<!(node -p \"require('path').join(require.resolve('node-addon-api/package.json'), '../')\")",
			"../include",
			"/usr/include",
			"/usr/include/glib-2.0",
			"/usr/lib/x86_64-linux-gnu/glib-2.0/include",
			"/usr/include/libvips"
		],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "libraries": ["-lvips", "-lgobject-2.0", "-lglib-2.0"]
    }
  ]
}