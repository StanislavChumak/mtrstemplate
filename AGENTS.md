# AGENTS.md — mtrstemplate

## Build

### Linux
```bash
mkdir build && cd build && cmake .. && make
```

### Windows (MinGW-w64 + Ninja)
Requires: MSYS2/MinGW-w64 toolchain (gcc/g++), Ninja, CMake, `git` on `PATH`.
```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe `
  -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe
cmake --build build
```
- Output: `build/bin/mtrstemplate.exe` (+ `build/bin/res/` with generated packs/scenes and script `.dll`s)
- Run it from `build/bin` (it resolves `res/` relative to the executable).

`run_mtrsc` forwards `CMAKE_GENERATOR`/`CMAKE_MAKE_PROGRAM` and the compilers to the
nested CMake configure that `mtrsc` runs for the scripts — do not use a multi-config
generator (Visual Studio) for the top-level build unless you pass MSVC paths.

- Generates `build/compile_commands.json` for LSP (single-config generators only)

## Architecture
- `CMakeLists.txt` — describes the game to the engine (`MATTRESS_BUILD_APP=ON`, window/resource settings)
- `engine/` — MattressEngine submodule. Builds the app executable (`mtrstemplate`) from `engine/src/Main.cpp` and the static lib `mattress`: ECS (components, systems, resources)
- `asset_compiler/` — mtrsc submodule (asset compiler from JSON to binary)
- `res/` — runtime assets and game logic, one folder per scene/pack:
  - `main.json` — root pack/scene referencing `ScriptFile`s
  - `<group>/<name>.json` — pack + scene definitions (textures, atlases, fonts, sounds, scripts, entities)
  - `<group>/<name>_logic.cpp` — gameplay scripts compiled to shared libs by mtrsc at build time

## Configuration (ENGINE_* macros)
Runtime configuration is compile-time only — there is no config file or `Config` struct.
The game sets `ENGINE_*` CACHE variables in the root `CMakeLists.txt` **before**
`add_subdirectory(engine)`; the engine declares defaults and forwards them as PUBLIC
compile definitions on `mattress`, so scripts see the same values. Notable knobs:
- `ENGINE_WINDOW_SIZE_X/Y` (points) + `ENGINE_START_POINT_SIZE` (pixel scale)
- `ENGINE_ICON_PATHS` — CMake list of icon files relative to the executable
  (e.g. `"a.png;b.png"`, converted to the `"a.png","b.png"` macro); `ENGINE_ICON_COUNT`
  must match the list length
- `ENGINE_FIXED_HORIZONTAL` / `ENGINE_FIXED_VERTICAL` — exactly one side must be fixed;
  enabling both non-fixed trips `#error` in `Camera.cpp`
- `ENGINE_NATIVE_OPT` — see portability notes below

## Script System (key quirk)
Game logic lives in `res/**/*_logic.cpp`, compiled to `.dll`/`.so` by mtrsc:
```cpp
#define SCRIPT
#include "script/engine_api.hpp"
// export on_load, on_unload, update with extern "C" EXPORT
```
- The engine hands each script a `mtrs::EngineAPI*` (function pointers); scripts do **not** link against the executable or `mtrsutils`.
- Access components via `mtrs::get_single_comp<Comp>()` and `mtrs::get_comp<Comp>(entity)`
- Entity lookups by hash: `mtrs::get_entity(scene, HASH64(name))`
- Engine-side helpers (`hash`, `utf8_to_utf32`, `message`, …) are exposed as `EngineAPI` function pointers; the `script/*_api.hpp` headers provide thin script-side wrappers. Add a helper to `EngineAPI` (and assign it in `ScriptFile::load`) instead of linking a helper library into the modules.
- On Windows a module must resolve every symbol at link time, so anything a script calls directly must either come from the API or be `inline`/header-only.

## Resource Pipeline
- `mtrsc` processes `res/` → `build/bin/res/` via custom target `run_mtrsc` (runs every build).
- `run_mtrsc` copies `asset_compiler/scripts_build/` to `build/mtrsc/scripts_build/`, then mtrsc
  generates `scripts_build/generated_scripts.cmake` there and configures/builds that copy itself
  (nested CMake invocation; the root CMake forwards generator and compilers to it).
- Resource paths are assembled/hashed as flat forward-slash names (`res/ui/camera/camera.mtscn`); keep this form on all platforms.

## Distribution (Windows)
The shipped product is `build/bin/mtrstemplate.exe` plus the whole `build/bin/res/`
folder (~21 MB) — nothing else. All binaries are self-contained:
- The executable links the GCC runtime statically (`-static -static-libgcc -static-libstdc++`,
  see `engine/CMakeLists.txt`).
- The script modules do the same (`asset_compiler/scripts_build/CMakeLists.txt`), because the
  game loads them at runtime and they must not require `libstdc++-6.dll` /
  `libgcc_s_seh-1.dll` on the target machine.

Import tables of every binary must only list system DLLs (`KERNEL32`/`USER32`/`GDI32`/`SHELL32`/`msvcrt`).
Verify with `objdump -p <binary> | grep "DLL Name"`.

Known portability caveats for other machines:
- Release builds are tuned generically (`-O3 -DNDEBUG -mtune=generic`) so the binaries run on
  any CPU. `-DENGINE_NATIVE_OPT=ON` adds `-march=native` for local builds; do **not** ship
  binaries built with it — they can hit "illegal instruction" on older CPUs.
- The renderer requires an OpenGL 3.3 core driver.

## Dependencies (git submodules)
- `engine` → https://github.com/StanislavChumak/MattressEngine (branch `develop`)
- `asset_compiler` → https://github.com/StanislavChumak/mtrsc (branch `develop`)

## Conventions
- C++17
- ECS components must implement `static const char* get_type_name()`
- No tests, linters, CI, or formatter configured
- Single branch: `master`