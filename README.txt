This repository contains code and building instructions for the arduino lasertag project.

Right now there is only the "Standard" project.
In due time I would like to add different game modes with increasing levels of sophistication.
Right now we should focus on getting the basics implemented.

The code should explain itself (it has lots of comments).
Your only problem will be compiling it.
You need:
1. The arduino IDE
2. The IRLIB library https://github.com/cyborg5/IRLib

Download a zip of that library from github, and use the arduino IDE to "install a library". This is a menu option in the IDE. Unfortunately I can't explain exactly how this is supposed to happen, since I use Visual Studio Micro as a layer on top of the Arduino IDE and the interface is surely different.

Only after installing the library and adding it to your project, you will be able to compile the code.

Don't try to copy the code into your project, the include paths for arduino projects are so strange, I really doubt that you're going to get it to work (I didn't). Just use the default way and install it as a library.