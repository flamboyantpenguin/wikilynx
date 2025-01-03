# wikiLYNX

A simple browser app made using Qt C++ to play wikipedia speedruns the right way. The project was initially made for the wikiLYNX event conducted by [@jonesrison](https://jonesrison.github.io) at Rajagiri School of Engineering and Technology, Ernakulam, Kerala.

![WelcomeDialog](https://projects.dawn.org.in/wikilynx/.cfg/assets/WelcomeDialog.png)

![GameWindow](https://projects.dawn.org.in/wikilynx/.cfg/assets/GameWindow.png)

## Introduction

### Wikipedia Speed-runs

Wikipedia, one of the world's largest online encyclopedia is a universe of hyperlinks. While researching, deviating from an article to an article completely different from our initial plans is not an uncommon experience. This experience slowly developed into the game of Wikipedia speed-runs. The player's task is to navigate through a wikipedia page through hyperlinks to reach another article perhaps within a short time or limited path.
This game is commonly known to folks among social media as Wikipedia speed-run. Wikipedia itself calls it [Wikiracing](https://wikipedia.org/wiki/Wikiracing).

### wikiLYNX Game Browser

Now a game must have rules! Though the game is not really documented by any Internation Organisation, we have formulated a set of rules we believe is the fundametal aspect of playing the game the right way. Do note that some of these rules can still be disabled in this app. wikiLYNX Game Browser implements these rules and gives you a game field to play the game with your friends.

The app is a simple C++ app based on Qt WebEngine. If one happens to be someone interested in the game, one is invited to play the game and contribute to the development of this project.

### How to Play

Each level in our game systems consists of a some checkpoints. A checkpoint is basically an article on Wikipedia. Your initial article will be the first checkpoint. You are tasked with navigating to the next checkpoint (next article) through the hyperlinks on the page, The game ends once you reach the final checkpoint. Checkpoints are calculated incrementally, meaning you have to visit them the right order to win (0 -> 1 -> 2) that is, one can't skip checkpoints. The next checkpoint URL will be displayed on the status bar. You can see the current level stats on the View Checkpoints dialog. `Time Taken` denotes the time taken to complete the game and `Clicks` denote the number of times you clicked on hyperlinks. You can set limits on `Time Taken` and `Clicks`. If a player is unable to reach the last checkpoint, the `Time Taken` field denotes the time taken to reach the last checkpoint.

The name you enter in the text field in the welcome dialog is used to register in the leaderboard. The leaderboard can have only 12 names for each level. If you leave the name line blank before starting a game session, your name will be registered as `Blondie`. You will know what this means if you have watch a certain Western Cowboy film released in 1966.

By default, you are not allowed to access websites outside wikipedia.org. Doing so will restart the current checkpoint. You're also not allowed to change game window, this will end the game abruptly. Keyboard is also disabled during the game session. These policies can be turned off in settings.

To learn more about the Gameplay, check the [wiki section on Gameplay](https://github.com/flamboyantpenguin/wikilynx/wiki/Game-%7C-Gameplay).

## Terms, Licensing and Privacy Policy

Our app is completely free and open source. This means you can use our code as per the terms of MIT License, explained in the `LICENSE` file in this repository. We have also used open source libraries to develop the game.

You as a user can contribute to this project. Read development section for more info on contributution.

wikiLYNX is a game browser application. Since the game of Wikipedia speed-runs is browsing from one article to the other, this application is basically a browser with features and options attached to enhance gameplay. This means that almost all standard rules and practises of browsing applies to using the browser within this app as well.
wikiLYNX makes use of QtWebEngine to implement browsers. QtWebEngine is wrapped around Chromium Embedded Framework (CEF), which means most of the things applicable for Chromium browsers applies here as well.

As of wikiLYNX 1.5.5, the application *does not collect* any type of data for statistics or for any other purposes. The app interacts with network only to make requests to fetch information (update check and downloading custom levels) and for browsing as mentioned below.

This does not prevent websites you visit via the in-app browsers from collecting some info based on their site practises. These info include your IP Address, Browser User agent, hardware info etc. For more info, read [PRIVACY.md](https://github.com/flamboyantpenguin/wikilynx/blob/master/docs/PRIVACY.md).

Read [Terms section in Wiki](https://github.com/flamboyantpenguin/wikilynx/wiki/Development) for more info.

## Installation Instructions

The app is available for Windows and Linux.

| Platform                                                                                                                 | Auto Update Support | System Architecture  | OS Version            |
| -------------------------------------------------------------------------------------------------------------------------|------------- | -------------------- | --------------------- |
| [<img src="https://github.com/user-attachments/assets/410801d9-79a6-4a69-b783-976f0592ecdb" width="48">](https://github.com/flamboyantpenguin/wikilynx/wiki/Installation#windows)                   |      ☑️      |  amd64               | Windows 10 and above  |
| [<img src="https://github.com/user-attachments/assets/041eb89f-7c98-4e18-b3c9-ac448558a9dd" width="48">](https://github.com/flamboyantpenguin/wikilynx/wiki/Installation#linux)                   |      ☑️      |  amd64, i386, arm64, armhf | Debian, RPM, Arch, AppImage, Flatpak, Snap    |

You can find the installation files in [GitHub Releases](https://github.com/flamboyantpenguin/wikilynx/releases/latest). To learn more about installation, please refer the [wiki](https://github.com/flamboyantpenguin/wikilynx/wiki/Installation).

## Updates

To update wikiLYNX, you can use the Maintenance Tool in Windows or update packages via your respective package managers for Linux. You can access Maintenance Tool from wikiLYNX start menu folder. AppImage users can download the latest binary from the GitHub release page. 

wikiLYNX does not provide support for older versions. So it's recommended that you update the app to the latest version as soon as possible to avoid possible securirty vulnerabilites or bugs and enjoy new features. 

## Development

wikiLYNX is made using Qt WebEngine. As of now the source is compatible for Windows and Linux (should be compatible for macOS as well). You can contribute to the project by adding a PR to this repo. Do follow basic contribution guidelines. If you want to contribute to the project but is unsure where to get started, read this [section](https://github.com/flamboyantpenguin/wikilynx/wiki/Development) or contact me at [penguin@dawn.org.in](mailto:penguin@dawn.org.in).

### Getting Started

To compile this project, you need to have the following components installed in your system.

- Qt6/Qt5
  - Qt WebEngine
  - Qt Positioning
  - Qt WebChannel
  - Qt Multimedia
  - Qt Desktop (MSVC)
  - cmake
- git
- gcc/msvc (project not tested in clang; should work)

After this clone this repository and start contributing. You can contact me via email or raise an issue on GitHub if you
encounter errors during build. For more info please refer the [wiki](https://github.com/flamboyantpenguin/wikilynx/wiki/Development).

### Contribute

Use any IDE like Qt Creator to modify the files. Do note the following before you submit a Pull Request.

- Respect everyone
- Follow exisiting project structure
- Follow contribution guidelines
- Check changes before you commit them
- Avoid adding secrets, build files or user configurations to commits

The source files are in wikiLYNX directory.

- docs container license, deployment documentation, linux desktop implementation assets and the Privacy Policy
- wikiLYNX/header contains the necessary C++ header files
- wikiLYNX/ui contains the Qt UI files
- wikiLYNX/assets contains the game assets
- wikiLYNX/debian contains debian packaging info

## Feedback

You can submit your feedback about the project in the discussions page or in this [Google Form](https://forms.gle/SScZKbFLFBffdVay8).

## About

```Txt
wikiLYNX 1.5.5

Last Updated: 03-01-2025 <Happy New Year!>
Made by DAWN/ペンギン
```

![DAWN](https://github.com/user-attachments/assets/e634151e-0178-4168-aca4-9664237d7ace)
