# wikiLYNX

A simple browser app made using Qt C++ to play wikipedia speedruns the right way. The project was initially made for the wikiLYNX event conducted by [@jonesrison](https://jonesrison.github.io) at Rajagiri School of Engineering and Technology, Ernakulam, Kerala. Documentation is being updated.

## Introduction

### Wikipedia Speed-runs

Wikipedia, one of the world's largest online encyclopedia is sort of a universe of hyperlinks. Deviating from a main article we were supposed to research for our assignment/project, to an article completely different from our initial plans is not an uncommon experience. This experience slowly developed into game of Wikipedia speed-runs. The player will be given an initial Wikipedia article from which one should find another article, perhaps completely unrelated to the initial topic, only through hyperlinks in the page.
This game though not probably recognised by any international organisation, is commonly known to folks among social media as Wikipedia speed-run. The first one to reach, will be the winner!

### wikiLYNX Game Browser

Now a game must have rules! Though the game is not really documented by any Internation Organisation, we have formulated a set of rules we believe is the fundametal aspect of playing the game the right way. Some of these rules can still be disabled in this app, however we apply these rules by default. wikiLYNX Game Browser implements these rules and gives you a game field to play the game with your friends. The app is a simple C++ app based on Qt WebEngine.
If one happens to be someone interested in the game, one is invited to play the game and contribute to the development of this project.

### How to Play

Each level in our game systems consists of a some checkpoints. A checkpoint is basically an article on Wikipedia. Your initial article will be the first checkpoint. You are tasked with navigating to the next checkpoint (next article) through the hyperlinks on the page, The game ends once you reach the final checkpoint. Checkpoints are calculated incrementally, meaning you have to visit them the right order to win (0 -> 1 -> 2) that is, one can't skip checkpoints. The next checkpoint URL will be displayed on the status bar. You can see the current level stats on the View Checkpoints dialog. Each level has a predefined timer. The game ends abrubtly when the timer ends. For competing with your friends, you can use the `Time Taken` field to declare the winner. Time Taken denotes the time taken by the player to finish the level from start till the end. If a player is unable to reach the last checkpoint, the `Time Taken` field denotes the time taken to reach the last checkpoint.

The name you enter in the text field in the welcome dialog is used to register in the leaderboard. The leaderboard can have only 12 names for each level. If you do not enter any name, your name will be registered as `Blondie`. You will know what this means if you have watched a certain Western Cowboy film released in 1966.

By default, you are not allowed to access websites outside Wikipedia from reference links. Doing so will restart the current checkpoint. You're also not allowed to change game window, this will end the game abruptly. These policies can be turned off in settings.

Apart from seeing the Leaderboads, you can see more detailed info about the Game in the directory named gData. This is usally seen in the Program Files folder of Windows or in your Home Directory in Linux. This logs directory contain more detailed info about the game. We initially developed this software to conduct Wikipedia speed-runs as a competition in our college. So as a measure de security, the game logs all the URLs accessed in a game. The article accessed at the end of the game is also saved as a PDF. Some other details are stored in a file named `report.txt`. You can find all these files in gData/logs in a folder named `instanceYYYYMMDDHHMMSS`, denoting the starting time of the game session.

We have included some interesting levels by default. You can add your own levels in the `Edit Level` option in settings. You can also export your levels as a JSON file to share it with your friends. Download more levels from [DAWN Public Archives](https://archive.pcland.co.in/DAWN/Projects/wikiLYNX/levels). To get to know the game, get started with the level `flyingduck`.

## Installation Instructions

The app is available for Windows and Linux. One was unable to port the app to Android since Qt Webengine is still not available for Android. Qt Webview is deprecated. To see all available installation files, check [DAWN Public Archives](https://archive.pcland.co.in/DAWN/Projects/wikiLYNX). All project files from DAWN are hosted here.

### Windows

wikiLYNX Browser is available as an installation for Windows 10 and above. Download the online installer from [here](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikiLYNX_win_installer_amd64.exe). You can update wikiLYNX using the MaintenanceTool from Start Menu.

| Platform                                                                                                                 | Availability | System Architecture |
| -------------------------------------------------------------------------------------------------------------------------|------------- | --------------------|
| [Windows 10/11](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikiLYNX_win_installer_amd64.exe) |      ☑️      |  amd64              |
| Windows 8.1                                                                                                              |      ❎      |  NULL               |
| Windows 7                                                                                                                |      ❎      |  NULL               |

Please feel free to let me know if you can help me build wikiLYNX for Windows 7 and 8.1.

### Linux

wikiLYNX is available for Debian, RPM and Arch Linux users. You can download the DEB and RPM files from GitHub releases page or from [DAWN Public Archives](https://archive.pcland.co.in/DAWN/Projects/wikiLYNX). wikiLYNX is available on AUR for Arch Linux users.

| Distribution                                                                                                          | Availability | Recommended Version     | System Architecture |
| --------------------------------------------------------------------------------------------------------------------- |------------- | ------------------------| --------------------|
| [Ubuntu (Debian)](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikilynx_1.2.7-0_amd64.deb)  |      ☑️      | 22.04 LTS or above      |  amd64              |
| [Fedora (RPM)](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikilynx-1.2.7-0.fc40.x86_64.rpm)     |      ☑️      | 40           |  amd64              |
| [Arch Linux (AUR)](https://aur.archlinux.org/packages/wikilynx)                                                       |      ☑️      | Built from source       |  any                |

#### Ubuntu (Debian)

Download deb from [here](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikilynx_1.2.7-0_amd64.deb).

To install, open it with Software in GNOME or Discover in KDE or any other graphical package manager. To install using `apt` in cli, run the command

```bash
sudo apt install ./wikilynx_1.2.7-0_amd64.deb
```

You can also install by adding DAWN APT Repository

```bash
sudo sh -c "curl https://repo.pcland.co.in/.keys/dawn-pgp.public >> /usr/share/keyrings/dawn-pgp.public" 
sudo sh -c "echo deb [arch=amd64 signed-by=/usr/share/keyrings/dawn-pgp.public] https://repo.pcland.co.in/apt stable main > /etc/apt/sources.list.d/dawn.list" 
sudo apt update && sudo apt install wikilynx
```

#### Fedora (RPM)

Download rpm from [here](https://github.com/flamboyantpenguin/wikiLYNX/releases/latest/download/wikilynx-1.2.7-0.fc40.x86_64.rpm).

To install it, open it with a supported package manager or run the command

```bash
sudo dnf install ./wikilynx-1.2.7-0.fc40.x86_64.rpm
```

You can also install by adding DAWN RPM repository

```bash
sudo dnf config-manager --add-repo https://repo.pcland.co.in/rpm/dawn.repo
sudo dnf install wikilynx
```

#### Arch Linux (AUR)

Install using `yay` or any other `AUR` helper.

##### yay

```bash
yay -S wikilynx
```

##### makepkg

```bash
git clone https://aur.archlinux.org/packages/wikilynx
cd wikilynx
makepkg -si
```

#### Build from Source

You can also install the app by building from source using cmake.

Prerequisites: cmake, qt6-webengine-dev

```bash
git clone https://github.com/flamboyantpenguin/wikilynx
cd wikilynx/wikiLYNX
cmake . && make
sudo make install
```

### macOS

Currently, wikiLYNX is not available for macOS. One is invited to raise suggestions on how the app can be packaged for macOS users.

## Updates

To update wikiLYNX, you can use the Maintenance Tool in Windows or update packages via your respective package managers for Linux. You can access Maintenance Tool from wikiLYNX start menu folder.

## Development

wikiLYNX is made using Qt6 WebEngine. As of now the source is compatible for Windows and Linux (should be compatible for macOS as well). You can contribute to the project by adding a PR to this repo. Do follow basic contribution guidelines. You can also open issues here. If you want to contribute to the project but is unsure where to get started, read this section or contact me at [penguin@pcland.co.in](mailto:penguin@pcland.co.in).

### Getting Started

To compile this project, you need to install Qt6 from their online installer or from your respective package managers. This project uses cmake for compilation.

### Building from Source | Windows

#### Install Git

Git for Windows can be installed from [git-scm](https://git-scm.com/downloads/win).

#### Install Qt6

Install Qt6 using their [online installer](https://doc.qt.io/qt-6/qt-online-installation.html). Install the following components.

- Qt WebEngine
- Qt Positioning
- Qt WebChannel
- Qt Desktop (MSVC)
- CMake (You can install cmake manually as well)

I personally prefer Qt Creator for Qt projects since it's easier to edit the ui in Qt Creator.

#### Install MSVC

The app can be compiled using GCC or MSVC. However for Windows, Qt WebEngine won't be supported if you use GCC. If you use MSVC to compile the project along with Windows SDK, the Windows themeing will be applied to the project and it actually looks good :).

To install MSVC, download Visual Studio 2022. You need not install the entire workload for Desktop C++ development. The following components are necessary.

- MSVC
- Windows SDK

#### Install CMake

CMake can be installed via Qt Installer or from [their website](https://cmake.org/download/).

### Building from Source | Linux

#### Installation

You can install Qt6, gcc and cmake from your package manager.

For Ubuntu

```bash
sudo apt install qt6-webengine-dev cmake git
```

For Fedora

```bash
sudo dnf install qt6-qtwebengine-devel cmake git
```

For Arch Linux

```bash
sudo pacman -S qt6-webengine cmake base-devel git
```

### Clone the Repository

To clone the repository, copy the project GitHub URL.

```bash
git clone https://github.com/flamboyantpenguin/wikilynx
```

### Build the project

```bash
cd wikilynx/wikiLYNX
cmake .
cmake --build
```

### Contribute

Use any IDE like Qt Creator to modify the files. Do note the following before you submit a Pull Request.

- Respect everyone
- Follow exisiting project structure
- Follow contribution guidelines

The source files are in wikiLYNX directory.

- wikiLYNX/header contains the necessary C++ header files
- wikiLYNX/ui contains the Qt UI files
- wikiLYNX/assets contains the assets files for the game

## Feedback

You can submit your feedback about the project in the discussions page or in this [Google Form](https://forms.gle/SScZKbFLFBffdVay8).

## About

```Txt
wikiLYNX 1.2.7

Last Updated: 24-10-2024
Made by DAWN/ペンギン
```

![DAWN](https://github.com/user-attachments/assets/e634151e-0178-4168-aca4-9664237d7ace)