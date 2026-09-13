# Third-party dependencies

Run `scripts/setup_dependencies.ps1` from PowerShell to populate this directory.
Downloaded files are intentionally excluded from Git.

Pinned dependencies:

- Vulkan SDK 1.4.328.1 (copy-only local installation)
- GLFW 3.4
- Assimp 5.4.3
- GLM 1.0.1
- tinyobjloader 2.0.0-rc13
- stb commit `2c980bb59875b0d32144a71867fbdebb2f77cd20`

GLAD and tinygltf were present as unused paths in the original CMake file and
are not required by the current source tree. glTF models are loaded by Assimp.
