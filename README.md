# bright
A simple program to adjust brightness on linux computers using intel_backlight.

The program adjusts the brightness in percentage steps, either default (5%) or chosen value (0-50%), manipulating the `sys/class/backlight/intel_backlight/brightness` file directly.

The program is my final project in the Harvard course CS50x, and must be considered a basic program with basic functionality. I chose this problem for my final project to implement a brightness control on my Arch-distro running the Awesome WM, and I have created keyboard shortcuts in the rc.lua-file (the config-file of Awesome.)

## Drawbacks
I do not consider this program complete, even though it satisfies my current need on my computer. Here are some of the short commings of this program that I have recognized:
- The paths for the brightness and max_brightness files are hard coded.
- The program reads out the max_brightness at every run (Should be implemented during compiling/make)
- The program has not been tested on any other system, and I do not know if it will work properly.
- The default increments (5%) can not be changed.

## Usage
The following is the user-message when running `bright -h`:
```
Usage: bright [-i | -d] [-h] [-v <value>]  
options:
  -i	increase backlight
  -d	decrease backlight
  -v	gives the adjustment step a new value in percent (0-50)
  -h	show this message

example: bright -iv 20 - increase brightness by a value of 20 percent.
if -v is not passed, default increment is 5 percent.
```

## Setup
I have used Autotools to create the make and configure files, and I have created a tarball for easy installing. First you need to create a file in the `/etc/udev/rules.d/` catalog with a file called `backlight.rules` containing the following rules to grant access to the brightness-file for users in the video group:
```
SUBSYSTEM=="backlight", ACTION=="add", \
  RUN+="/bin/chgrp video /sys/class/backlight/intel_backlight/brightness", \
  RUN+="/bin/chmod g+w /sys/class/backlight/intel_backlight/brightness"
ACTION=="add", SUBSYSTEM=="backlight", KERNEL=="intel_backlight", GROUP="video", MODE="0664"
```
And do not forget to add the relevant users to the video group if they are not already members. 

There are currently two ways of installing:
1. Downloading the tarball:
  - Download the tarball to your computer
  - Extract the contens in a suited place
  - Run the following code inside the extracted folder to install:
    ```
    ./configure
    make
    sudo make install
    ```
2. Download the git-repository (excluding the tarball) and use autotools yourself:
  - If you chose this option I assume you know what you are doing and do not need instructions.

## License
This program is licensed under the GNU GPL v3, for more information see [LICENSE](/LICENSE).
Copyright (C) 2021 Jonas Jørgensen

Since this primarily is a final project in a web course, I do not want contributions on the code for now. If you have any comments or suggestions feel free to email me on jonas.jrgensen@gmail.com.
