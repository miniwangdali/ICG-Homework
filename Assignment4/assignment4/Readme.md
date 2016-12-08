# Instruction
This is the solution of assignment3. The solution is writen and built with Visual Studio 2015 Community Edition in Windows 10 Pro 64-bit version 14393.447 (1607). So I do not have the instruction for Linux and Mac OS.

## Demand and Accomplishment
- [x] Add a post-processing screen shader to your previous homework.
- [x] It must combine at least two different post processing effects.

## How to Run?
In this folder, you can see a folder called Release. Go into this folder and run the "assignment4.exe". Then it will show on your screen.

![Image of Playing](https://github.com/miniwangdali/ICG-Homework/blob/master/Assignment4/assignment4/assignment4.gif)

## How to Compile?
I use Visual Studio to build this solution. So I did not write any makefile. So if you could use a Visual Studio 2015, it's easy for you to open this solution and build it by using the IDE. I put the dependency of OpenGL under the directory of VC of VisualStudio, as below:
- .h/.cpp IN %VSFileDir/VC/include
- .lib IN %VSFileDir/VC/lib
- .dll IN same folder with the executable files

## Additional Detail
- the post processing effects are:
    - FXAA
    - Guassian Blur
- Light colors are ```#EE0000``` and ```#66CCFF```.
- Light positions are **(20, 20, 0)** and **(-20, 20, 0)**
