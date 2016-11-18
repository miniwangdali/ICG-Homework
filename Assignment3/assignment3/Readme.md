# Instruction
This is the solution of assignment3. The solution is writen and built with Visual Studio 2015 Community Edition in Windows 10 Pro 64-bit version 14393.447 (1607). So I do not have the instruction for Linux and Mac OS.

## Demand and Accomplishment
- [x] Create a simple scene with multiple positional lights.
- [x] Use a normal map and a specular map on at least one object.
- [x] Use either the object loader or the geometry generator to create objects in your scene.

## How to Run?
In this folder, you can see a folder called Release. Go into this folder and run the "assignment3.exe". Then it will show on your screen.

![Image of Playing](https://github.com/miniwangdali/ICG-Homework/blob/master/Assignment3/assignment3/assignment3.gif)

## How to Compile?
I use Visual Studio to build this solution. So I did not write any makefile. So if you could use a Visual Studio 2015, it's easy for you to open this solution and build it by using the IDE. I put the dependency of OpenGL under the directory of VC of VisualStudio, as below:
- .h/.cpp IN %VSFileDir/VC/include
- .lib IN %VSFileDir/VC/lib
- .dll IN same folder with the executable files

## Additional Detail
- Viewpoint can be manipulated by mouse. It is implemented by using arcball to control the ```eyeMatrix```.
- Light colors are ```#EE0000``` and ```#66CCFF```.
- Light positions are **(20, 20, 0)** and **(-20, 20, 0)**
