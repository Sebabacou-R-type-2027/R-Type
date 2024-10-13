# R-Type

## Description

This project is a remake of the R-Type game, a shoot'em up game where the player controls a spaceship and has to destroy enemies and obstacles.

This implementation includes new features like networking and multipleyr co-op mode.

## Installation

### Building from source

To build the project from source, you will need to have the following dependencies installed on your system:

- C++17 compiler
- CMake
- Ninja
- vcpkg (see [Installation tutorial](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash) up to step 2.1)
- Extra SFML build dependencies on Linux (see [SFML documentation](https://www.sfml-dev.org/tutorials/2.6/compile-with-cmake.php#installing-dependencies))

First, clone the repository:

```bash
git clone https://github.com/Sebabacou-R-type-2027/R-Type.git
cd R-Type
```

Then, select one of the available CMake presets (excluding `base`):

```bash
cmake --list-presets
Available configure presets:

  "base"
  "default" - Default
  "release" - Release
  "ci"      - CI
```

Finally install the dependencies, configure, build and test the project (in release mode, for example):

```bash
vcpkg install
cmake --preset release
cmake --build --preset release
ctest --preset release
```

## Running the game

After building the project, you can run the game by executing the `r-type_server` executable:

```bash
./r-type_server <port>
./r-type_client <server-ip> <port>
```

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue for any bug reports, feature requests, or general improvements.
