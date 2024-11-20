{
  "targets": [
    {
      "target_name": "node_clonefile",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": ["src/darwin/main.cc"],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }
  ]
}
