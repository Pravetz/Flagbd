# Flagbd
C++/TGUI/SFML program for creating flags.

# Compiling(LINUX ONLY): 
0. Be sure, that you have installed SFML and TGUI libraries. (Flagbd was written using SFML 2.5.1 and TGUI 0.9.3 so it's recommended to install these versions)
1. Open terminal in directory of the project
2. Paste the following commands:
 ```
 chmod u+x compiler.sh
 ```
 ```
 ./compiler.sh
 ```
3. In order to run compiled program type:
 ```
 chmod u+x main
 ```
 
 # RUNNING:
 ```
 ./main
 ```
 # Further information and tips: 
 Flagbuilder(or Flagbd) can display up to 512 elements and has 31 heraldry elements(so far) by default.
 Number of elements to display and heraldry to load can be changed in flagbd.cfg file, which is created automatically on the first start.
 The structure of the config file is simple: 
 ```
 1. FLAGBD VERSION
 2. WINDOW WIDTH
 3.	WINDOW HEIGHT
 4.	GUI THEME
 5. NUMBER OF HERALDRY
 6. MAXIMUM NUMBER OF ELEMENTS FLAGBD CAN DISPLAY
 7+: TEXTURE PATHS(assets/elements/...) path count is equal to the number of heraldry
 ```
 How to create text elements in Flagbd:
 1. Put the font in assets/elements/ directory (Fonts should be in the working directory, otherwise Flagbd most likely wont create the font face)
 2. Create text element and write a path to the font in program, then right-click the font path to load it


 Element pictures are taken from Flaggen(https://github.com/Pravetz/sfml-flaggen)
 
