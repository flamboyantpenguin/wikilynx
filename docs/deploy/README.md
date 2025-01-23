# Deployment | Linux

## Introduction

wikiLYNX was initially aimed for Windows devices. As a Linux user myself, I wanted to release wikiLYNX for Linux. Initially I went for deploying packages the conventional yet still important ways of preparing native distribution libraries like `deb` and `rpm` files. It is relatively easier to deploy packages on `rpm` systems and as of now we support Fedora 40 and above. I found writing `debian` spec files notoriouly hard and it's still imperfect (As always, contributions are welcome). wikiLYNX is also available for Arch Linux via the Arch User Repository. Since I have spent a lot of time on this, I felt the importance of publishing some build info to help everyone build packages on their own and refer for their own projects if required.

Containers are a great way to deploy packages as it won't disturb the host system. Multi-architecture support (I personally use qemu) makes it even easier to deploy packages for all available platforms. You can find the `Containerfile` I prepared for deployment in this directory. You're free to contribute better alternatives to methods I have used or refer these for your own projects.

## Debian

For Debian systems, the spec files in the source directory of the project is used to prepare the package. The package must be ideally built in an older version of `glibc` to ensure platform-compatability. Initially my attempts to port enable support for Qt5 didn't end well (as Debian 11, the ideal build version does not support Qt6). After lot of tries and errors, I'm finally able to build wikiLYNX using Qt5. Some minor changes have to be done regarding slots, I change them during build.

## Fedora

Fedora uses `rpmbuild` command to build the package. The deployment is based on the SPEC file. You can refer it for more info.

## AppImage

AppImages are a great way to deploy as it bundles everything, making it easy to execute on any system. It collects all dependencies of a binary and packages it into a single file that works like an image. When you run the image, the files are extracted to tmp and executed. Though I spent a lot of time fixing things, I still believe it was worth it. AppImage bundling was done thanks to the [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy) project along with [linuxdeploy-plugin-qt](https://github.com/linuxdeploy/linuxdeploy-plugin-qt) and [linux-deploy-plugin-gstreamer](https://github.com/linuxdeploy/linuxdeploy-plugin-gstreamer). The `AppDir` is bundled using [appimagetool](https://github.com/AppImage/appimagetool). To learn how it works, check the deploy script in AppImage directory.

## Flatpak

wikiLYNX is finally available on flathub.org. You can checkout the [manifest repo](https://github.com/flathub/in.org.dawn.wikilynx) to learn more about flatpak deployment.
