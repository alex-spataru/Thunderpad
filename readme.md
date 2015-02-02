## Thunderpad

[![Build Status](https://travis-ci.org/alex-97/Thunderpad.svg)](https://travis-ci.org/alex-97/Thunderpad)
[![OpenHub](https://www.openhub.net/p/thunderpad/widgets/project_thin_badge.gif)](http://openhub.net/p/thunderpad)
 
Thunderpad is a simple and lightweight text editor designed for general-purpose usage and casual programming. 
 
Thunderpad is released with the GNU 3.0 license, you can read a copy of it [here](http://www.gnu.org/copyleft/gpl.html).
 
## Setup/Compiling
 
 **Note: The development state of Thunderpad is still not complete, do not expect anything great for the moment**
 
### Windows
 
 1. Install the Qt SDK from [http://qt-project.org/downloads](http://qt-project.org/downloads).
 2. [Download](https://github.com/alex-97/thunderpad/archive/master.zip) the source code and extract it with your favorite ZIP utility.
 3. Open the thunderpad.pro file with Qt Creator.
 4. Configure the project by selecting Windows as your target system.
 5. Select the "release" build option.
 5. Build and run Thunderpad by pressing CTRL + R.
 
### Mac OS X
 
 1. Download Xcode from the App Store.
 2. Open Xcode and accept the license agreement.
 3. After you accept the license agreement, you can safely quit Xcode (CMD + Q).
 4. Install the Qt SDK from [http://qt-project.org/downloads](http://qt-project.org/downloads).
 5. [Download](https://github.com/alex-97/thunderpad/archive/master.zip) the source code and extract it with your favorite ZIP utility.
 6. Open the thunderpad.pro file with Qt Creator.
 7. Configure the project by selecting Mac OS X as your target system.
 8. Select the "release" build option.
 9. Build and run Thudnerpad by pressing CMD + R.
 10. To install, navigate to the directory where you extracted the source code and locate a folder similar to "build-thunderpad-Desktop_Qt_4_8_clang_64bit-Release" and copy the Thunderpad.app to your Applications folder.
 
### Linux
 
 1. Run the following command to install the required packages for compiling (Debian-based distros only)
 
        sudo apt-get install build-essential libqt4-core libqt4-dev libqt4-gui qt4-dev-tools libssl-dev libqscintilla2-dev libqt4-webkit
 
 2. [Download](https://github.com/alex-97/thunderpad/archive/master.zip) the source code of Thunderpad. 
 
 3. Then, go to your downloads folder and unzip the current commit.
 
        cd ~/Downloads
        unzip thunderpad-master.zip -d thunderpad-master
        cd thunderpad-master
 
 4. Make a directory to build the software
 
        mkdir build
        cd build
 
 5. Compile the application (with Qt4 libraries)l
 
        qmake-qt4 ../thunderpad.pro CONFIG+=release
        make -j4
 
 6. To run the application:
 
        ./thunderpad
 
 7. To install Thunderpad, run the following:
 
        sudo make install

#### Troubleshooting for Linux

**The application is compiled successfully, but nothing happens when executed**

*Solutions*:

- Make sure that you installed all the packages above, then try again.
- Download the newest stable QScintilla release from [here](http://www.riverbankcomputing.com/software/qscintilla/download) and compile and install it manually. It seems that some distributions provide an outdated version of QScintilla in their repositories.
 
 That's all! To run Thunderpad, you can type "thunderpad" (without quotes) in Terminal. You can also run Thunderpad from your application menu (Gnome) or Dash (Unity).
