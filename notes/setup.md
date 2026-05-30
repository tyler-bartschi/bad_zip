# Notes on C++ Setup And Build Commands

Build configuration command:

```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

or

```bash
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release
```


Build command:

```bash
cmake --build build/debug
```

or

```bash
cmake --build build/release
```


Run command:

```bash
./build/debug/my_project
```

or

```bash
./build/release/my_project
```


Test command:

```bash
ctest --test-dir build/debug
```

or

```bash
./build/debug/my_project_tests
```