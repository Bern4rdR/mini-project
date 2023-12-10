# mini-project
A potentiometer game on the ChipKIT Uno32 board

## Table of Contents

- Description
- Tutorial
- Installation
- Usage
- Contributing
- License

## Description

This is a simple maze runner game using ray tracing to display from 2D to 3D and a potentiometer to rotate the player POV.

## Tutorial

There are four buttons on the ChipKIT. However this project only uses/needs one button, button 4 (BTN4). BTN4 is responsible for moving forward in the players direction.

The potientiometer can be used to rotate the players field of view, i.e what the player can see which is also what is displayed on the screen. The potientiometer uses the relative rotation since it can only rotate about 270 degress (3/2 PI radians) while the player is supposed to be able to rotate 360 degress (2 PI). So if it rotates a third of its rotation to the left then the players field of view will also be rotated a third of its rotation to the left.

## Installation

1. Clone this repository.
2. Open a terminal with the path for the cloned repository.
3. Enter to the mcb32 terminal

```bash
. /opt/mcb32tools/environment 
```

4. Install the repository to the ChipKit.

```bash
make
```
```bash
make install
```

5. Follow the instructions on the screen and enjoy.

For more detailed installations instructions regarding cloning of the repository, we recommend [Github Docs](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository).

For further installation instructions regarding the mcb32 toolchain, we recommend the [mcb32tools installation guide](https://github.com/is1200-example-projects/mcb32tools), the ChipKit manual as well as the provided guides, FAQ's and other help provided by the KTH course IS1500.

## Usage

- BTN4: go forwards.
- Potentiometer: rotate to rotate player FOV in inputed rotational direction.

## Contributing

This is a school project which means no contributions are allowed.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

- Inspiration for this project came from the game Wolfenstein 3D
- Some of the functions used to interact with the ChipKIT were taken from the labs in the course IS1500 HT23
  - they were written by Axel Isaksson, F Lundevall and Ture Teknolog
