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
2. Open an terminal with the path for the cloned repository.
3. Enter to the mcb32 terminal

```bash
. /opt/mcb32tools/environment 
```

4. Install the repository to the ChipKit.

```bash
make
make install
```

5. Follow the instructions on the screen and enjoy. 

For further installation instructions 

## Usage

- BTN4: go forwards.
- Potentiometer: rotate to rotate player FOV in inputed rotational direction.

## Contributing

This is a school project which means no contributions are allowed.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

- Inspiration for this project came from the game Wolfenstein 3D
