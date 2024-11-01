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

## Requirements

Before running the game, ensure that you have installed all the required libraries and compiled the project. Once the compilation is successful, follow the instructions below to launch the server and client.

## How to Launch the Game

1. **Open two terminals** and navigate to the same directory (repository) where the executables are located.

2. **Run the server** in the first terminal using the following command:

   `./r-type_server 8080`

   This will start the server on port `8080`.

3. **Run the client** in the second terminal using this command:

   `./r-type_client 127.0.0.1 8080`

   This command connects the client to the server running on `127.0.0.1` (localhost) at port `8080`.

4. The game will launch, but you will need to return to the **client terminal**. A prompt will appear: 

   `$> `

5. **Login** to the game by entering the following command:

   `login <username> <password>`

   Replace `<username>` and `<password>` with your credentials.

6. **Create a lobby** using the command:

   `create_lobby`

7. **Start the game** by typing:

   `start`

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue for any bug reports, feature requests, or general improvements.
