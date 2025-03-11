<p align="center">
  <img src="https://github.com/user-attachments/assets/65a2fba8-793b-4db0-83d1-9e87e610558e"/>
</p>

<p align="center">
	<a href="https://github.com/Oakamoore/keywords/blob/main/LICENSE">
		<img src="https://img.shields.io/badge/License-MIT-green">
	</a>
	<a href="https://github.com/ArthurSonzogni/FTXUI">
		<img src="https://img.shields.io/badge/FTXUI-5.0.0-orange">
	</a>
	<a href="https://github.com/mackron/miniaudio">
		<img src="https://img.shields.io/badge/miniaudio-0.11.21-orange">
	</a>
</p>

## Overview 

A terminal based typing game, inspired by [wspeed](https://bisqwit.iki.fi/wspeed/) and [typespeed](https://typespeed.sourceforge.net/). It uses [FTXUI](https://github.com/ArthurSonzogni/FTXUI) to handle rendering in the terminal, and [miniaudio](https://github.com/mackron/miniaudio) to play audio files - which were generated using [AutoTracker](https://www.vitling.xyz/toys/autotracker/).

<p align="center">
	<video src="https://github.com/user-attachments/assets/647c2d93-bdfd-44a4-a480-3f5b4e36f20d" width=1100/>
</p>

<p align="center">
	<i><font size=2>Unmute to hear game audio</font></i>
</p>

> A terminal emulator that supports a 256 colour palette and ANSI escape codes is required for this program to function as expected

Choose between **three** different difficulties, that scale with each word that is typed, and aim for highscores to enter your name in the local leaderboard.

<p align="center">
	<a href="https://oakamoore.itch.io/keywords">
		<img src="https://github.com/user-attachments/assets/ddd31af4-2e28-4d5f-b8ef-8064df624d69" width=200>
	</a>
</p>

## Installation

> A C++20 compatible compiler is required for this program to function as expected

1. Clone this project 

```shell
git clone https://github.com/Oakamoore/keywords.git
```

2. Step into the repository

```shell
cd keywords
```

#### Standard Build (Native)

- Build the project using [CMake](https://cmake.org/)

```shell
# Configure the build
cmake -S . -B build

# Build project binaries
cmake --build build
```


#### WebAssembly Build

- Build the project using [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)

```shell
# Configure the build (in a separate directory)
emcmake cmake -S . -B bulid.em

# Build project binaries
cmake --build build.em
```

Emscripten does **not** support [Visual Studio's CMake Generator](https://cmake.org/cmake/help/latest/generator/Visual%20Studio%2017%202022.html), it is recommended to use either the [MinGW Makefiles](https://cmake.org/cmake/help/latest/generator/MinGW%20Makefiles.html) or [Ninja](https://cmake.org/cmake/help/latest/generator/Ninja.html) generators.

To explicitly specify a generator use CMake's `-G` option followed by said generator's name, during the build configuration stage, i.e. `-G "MinGW Makefiles"`.

A build configuration type (`Debug`, `Release` etc.) can also be [specified](https://gist.github.com/Oakamoore/685838c1b4a4c64a008f5461ac9323b5), in **both** the native and WebAssembly builds.

## Usage

#### Standard Build (Native)

Once the project is built, navigate to the `keywords/build/` directory, locate the executable, then run the game using:

```shell
# Run the executable
./keywords
```

#### WebAssembly Build

Once the project is build, navigate to the `keywords/build.em/` directory, then run the following:

```shell
# Start a local server
emrun keywords.html
```

#### Disabling Audio

In-game audio is **enabled** by default, though can **only** be disabled in native builds. To do so, append `no-audio` to the above native build command. 

## Testing

[![Catch2](https://img.shields.io/badge/Catch2-3.6.0-orange)](https://github.com/catchorg/Catch2/tree/devel)

By default, tests are disabled in native builds (*and unavailable in WebAssembly builds*). To build them alongside the program append `-D ENABLE_TESTING=1` to the above build configuration command.

Once the project is built navigate to `keywords/build/tests/`, locate the testing executable, then run the tests using:

```shell
./keywords-tests
```

## Planned Features and Contributions

There are plans to add support for different languages, and game modes. 

This project is currently **not** open to contributions, though you are of course free to fork it and extend it in any way you deem fit. If you encounter any bugs, feel free to create an [issue](https://github.com/Oakamoore/keywords/issues).
