# LLVM Opaque Pointers Transformer

A C++ library for transforming LLVM IR with typed pointers to use opaque pointers, compatible with LLVM 16+.

## Features

- Transform LLVM IR from typed pointers to opaque pointers
- Comprehensive error handling and validation
- Support for both text IR and bitcode input
- Multi-configuration build support (Debug/Release)
- Full test coverage

## Building

### Prerequisites

- CMake 3.10+
- LLVM 16 (both Debug and Release builds, targets X86;AMDGCN;NVPTX, clang disabled)
- Visual Studio 2022 (for Windows)

### Quick Start

1. Clone the repository:
```bash
git clone https://github.com/jpola-amd/llvm-opaque-ptrs-transformer.git
cd llvm-opaque-ptrs-transformer
```

2. Configure and build:
```bash
# Configure for debug
cmake --preset windows-msvc-debug

# Build debug
cmake --build --preset windows-debug

# Build release
cmake --build --preset windows-release
```

3. Run tests:
```bash
cd build/windows-msvc-debug
ctest
```

### Configuration

Update the paths in `CMakePresets.json` to match your environment:
- `LLVM_ROOT_DIR_DEBUG`: Path to debug LLVM cmake files
- `LLVM_ROOT_DIR_RELEASE`: Path to release LLVM cmake files  
## Usage

```cpp
#include "transformer.h"

using namespace llvm_transformer;

std::string_view ir_code = R"(
define i32 @test(i32* %arg) {
  %val = load i32, i32* %arg
  ret i32 %val
}
)";

auto result = transform_llvm_ir_to_opaque_pointers(ir_code);
if (result) {
    std::cout << "Transformed IR:\n" << *result << std::endl;
} else {
    std::cerr << "Error: " << result.getErrorHandler().getFormattedAll();
}
```

## License
