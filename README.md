# mb
MotherBoard Firmware


## PlatformIO
### Installing PlatformIO
PlatformIO can be installed as a plugin for many IDEs. You can find a complete list [here](https://docs.platformio.org/en/latest/ide.html#desktop-ide).
My recommendation is to use Microsoft's Visual Studio Code (VSCode), you can find installation guides [here](https://docs.platformio.org/en/latest/ide/vscode.html).

### Open the project in VSCode
Once you have cloned this project to a local directory, you can open it on VSCode in `File` :arrow_forward: `Add Folder to Workspace...`.
Then select the `mb` folder of the repository and click open, make sure it is the root folder and that it has the `platformio.ino` inside.
After that, the project should be loaded in VSCode and ready to configure and build.

### Build and upload the project
Connect the board to the computer and click on the `Upload` button (:arrow_right:) from the PlatformIO toolbar, </br> or go to `Terminal` :arrow_forward: `Run Task` :arrow_forward: `Upload`.

All the dependencies will be configured and built automatically.

> **Note** that if you are a Linux user and it is your first time using platformio, you will have to install the udev rules to grant permissions to platformio to upload the program to the board. You can follow the instructions [here](https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules)!