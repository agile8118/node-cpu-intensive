{
  "targets": [
    {
      "target_name": "heavy-lifter",
      "sources": [ "./addon/src/binding.cc", "./addon/src/prime-generator.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./addon/deps/GMP"
      ],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "14.0",
        "OTHER_LDFLAGS": [
          "-L./addon/deps/GMP/.libs",
          "-lgmp",
          "-lgmpxx"
        ]
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 },
        "VCLinkerTool": {
          "AdditionalLibraryDirectories": [
            "addon\\deps\\GMP\\.libs"
          ],
          "AdditionalDependencies": ["gmp.lib", "gmpxx.lib"]
        }
      },
      "conditions": [
        ["OS=='linux' or OS=='freebsd'", {
          "libraries": [
            "-L./addon/deps/GMP/.libs",
            "-lgmp",
            "-lgmpxx"
          ]
        }]
      ],
      "product_dir": "../build"
    }
  ]
}