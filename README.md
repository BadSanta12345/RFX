# RFX
Refractor 2 game engine extension. (ATM. only for Battlefield 2)

Custom C++ code is compiled to a .dll file which is injected to the game .exe (BF2.exe) by modifying the IAT (import address table) of the .exe. 
The injection could be done in many ways but this is the only method I came up with that doesn’t require a special injector executable. I had to decide whether I wanted to supply an injector or a modified BF2.exe with the mod. I went with modifying the BF2.exe.
In the code there is a dummy function which is exported so that it can be added to the BF2.exes’ IAT.I did the IAT modification with a free program called CFF Explorer. 

The code is written in C++. It relies on some features of the relatively new C++11/14 standard so you will need a modern compiler. The project file is for Microsoft Visual Studio 2015 IDE. The IDE is completely free so I see no reason to use any other compiler. Additionally BF2.exe was also originally made with Visual Studio so it is safest to go with it to make sure the memory layout matches that of BF2.exe.

The code interfaces with the BF2.exe by hooking some WinAPI and DirectX function calls of the game and via unofficial BF2 class headers. From a hackers point of view BF2 has a very useful class called ClassManager. It contains a lookup table of many of the internal components of the game engine such as Renderer, PlayerManager, MainConsole etc. Basically if you can somehow get a pointer to the ClassManager you can from there get pointers to any of those components.  In the code pointer to the ClassManager is calculated with a known static offset. This offset only works for the vanilla BF2.exe version 1.5. Doesn’t work with debugger or version 1.41. For those new offsets would need to be figured and the class headers might not match.

Most of the class headers are sourced from open source BF2 cheat hacks and some are reverse engineered by me. The headers are not complete and some parts are just simply wrong. Supplementing and fixing the header files is a never ending process.
