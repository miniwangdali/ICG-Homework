# Instruction
This is the solution of assignment1. The solution is writen and built with Visual Studio 2015 Community Edition in Windows 10 Pro 64-bit version 14393.187 (1607). So I do not have the instruction for Linux and Mac OS.

## Demand and Accomplishment
- [x] Render a simple 3D scene using primitives.
- [x] At least 3 objects must be in a hierarchy (transforming relative to another object).
- [x] Use the simple lighting shader for your objects and set their color using the uColor uniform.

## How to Run?
In this folder, you can see a folder called Release. Go into this folder and run the "assignment2.exe". Then it will show on your screen.

![Image of Playing](https://github.com/miniwangdali/ICG-Homework/blob/master/Assignment2/assignment2/assignment2.gif)

## How to Compile?
I use Visual Studio to build this solution. So I did not write any makefile. So if you could use a Visual Studio 2015, it's easy for you to open this solution and build it by using the IDE. I put the dependency of OpenGL under the directory of VC of VisualStudio, as below:
- .h/.cpp IN %VSFileDir/VC/include
- .lib IN %VSFileDir/VC/lib
- .dll IN same folder with the executable files

## Additional Detail
- I use makeRotation methods in **Matrix4** rather than using **Quat** to implement rotation.
- My three entities is organized as following hierarchy
    - **EntityLv1**: a Cube geometry with a transform of rotation on X axis.
        - **EntityLv2**: a Cube geometry with a transform of rotation on Y asix, scale for 1.5 times on X axis, translation for 1 unit on X axis. 
            - **EntityLv3**: a Cube geometry with a transform of rotation on Z axis, translation for 1 unit on Y axis.
- V1.0 does not implement **lookAt** method.
