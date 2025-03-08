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

3. Build the project using [CMake](https://cmake.org/)

```shell
# Configure the build
cmake -S . -B build

# Build project binaries
cmake --build build
```

A build configuration (`Debug`, `Release` etc.) can also be [specified](https://gist.github.com/Oakamoore/685838c1b4a4c64a008f5461ac9323b5).

#### Disabling Testing

To prevent tests from being built alongside the program, append `-D ENABLE_TESTING=0` to the above build configuration command.

## Usage

Once the project is built, navigate to the newly created `keywords/build/` directory, locate the executable, then run the game using:

```shell
./keywords
```

#### Disabling Audio

In-game audio is **enabled** by default. To disable it across an instance of the game, append `no-audio` to the above command. 

## Testing

[![Catch2](https://img.shields.io/badge/Catch2-3.6.0-orange)](https://github.com/catchorg/Catch2/tree/devel)

Once the project is built (with testing enabled in the build configuration), navigate to `keywords/build/tests/`, locate the testing executable, then run the tests using:

```shell
./keywords-tests
```

## Planned Features and Contributions

There are plans to add support for different languages, and game modes. 

This project is currently **not** open to contributions, though you are of course free to fork it and extend it in any way you deem fit. If you encounter any bugs, feel free to create an [issue](https://github.com/Oakamoore/keywords/issues).
