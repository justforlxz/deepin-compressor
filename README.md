## deepin-compressor
Archive Manager is a fast and lightweight application for creating and extracting archives.

归档管理器是一款提供对文件解压、压缩常用功能的软件工具。

#### Core
The GUI is based on [DTK](https://github.com/linuxdeepin/dtkwidget), Qt (Supported >= 5.12).

Build Depends:
debhelper (>= 11), libgsettings-qt-dev, libarchive-dev, libsecret-1-dev, libpoppler-cpp-dev, libudisks2-qt5-dev, libdisomaster-dev, libkf5codecs-dev, libzip-dev, qttools5-dev-tools, deepin-gettext-tools, qtbase5-dev, qt5-default, libdtkwidget-dev,libqt5svg5-dev, libqt5x11extras5-dev, libkf5archive-dev

Execute Depends:
debhelper (>= 11), libarchive13 (>= 3.2.1), libc6 (>= 2.14), libdisomaster, libdtkcore5 (>= 5.0.13), libdtkgui5 (>= 5.0.13), libdtkwidget5, libgcc1 (>= 1:3.0), libgl1, libglib2.0-0 (>= 2.16.0), libgsettings-qt1 (>= 0.1+14.04.20140408), libkf5codecs5 (>= 4.96.0), libpoppler-cpp0v5 (>= 0.46.0), libpulse-mainloop-glib0 (>= 0.99.1), libpulse0 (>= 0.99.1), libqt5concurrent5 (>= 5.0.2), libqt5core5a (>= 5.11.0~rc1), libqt5dbus5 (>= 5.0.2), libqt5gui5 (>= 5.7.0), libqt5svg5 (>= 5.6.0~beta), libqt5widgets5 (>= 5.11.0~rc1), libqt5x11extras5 (>= 5.6.0), libqt5xml5 (>= 5.0.2), libsecret-1-0 (>= 0.7), libstdc++6 (>= 5), libzip4 (>= 1.3.0), p7zip-full, deepin-shortcut-viewer, unar, rar, unrar

### Third Party Code
[KDE ark](https://github.com/kde/ark)

#### Installation
sudo apt-get install deepin-compressor

#### Build
- mkdir build
- cd build
- qmake ..
- make

Caution:
sudo cp BUILD_PATH/plugin/*/*.so /usr/lib/deepin-compressor/plugins
sudo cp BUILD_PATH/plugin/*/*/*.so /usr/lib/deepin-compressor/plugins
