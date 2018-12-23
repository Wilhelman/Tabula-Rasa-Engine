
## **Project overview** 

Tabula Rasa Engine is a 3D game engine developed/coded from scratch with OpenGL in C++. The engine contains all the basic [sub-systems](https://wilhelman.github.io/Tabula-Rasa-Engine/#main-core-subsystems) of a modern 3D engine and we have also added support to handle [skeletal animations](https://wilhelman.github.io/Tabula-Rasa-Engine/#skeletal-animation-subsystem). Although this project has been done with educational purposes it will be used (along other engines from our class) to produce a game in the Project 3 subject in the first half of 2019. 

This project has been done by two students of CITM - UPC (Terrasa, Spain) for the subject Game Engines.

> ### *Download the last version of the engine [HERE]()*

***

## **Our Team**

![team_photo](https://user-images.githubusercontent.com/25589509/50381505-0bd05680-0689-11e9-9a8e-0e0573ebbbce.jpg)


_García Subirana, Guillermo's responsability and GitHub account_

* TODO: add resposabilities here...
* [GitHub account](https://github.com/Wilhelman)

_Masó Garcia, Víctor's responsability and GitHub account_

* TODO: add resposabilities here...
* [GitHub account](https://github.com/nintervik)

***

## **Main core subsystems**

Our game engine code is structured in modules. The main module (called trApp.cpp) manages all the other modules calling in a loop its respective awake, preupdate, update, postupdte, cleanup that they share thorugh a base class trModule. Down below a basic scheme of this structure (only some example modules are showed here):

![dibujo sin titulo](https://user-images.githubusercontent.com/25589509/50381846-84d3ac00-0691-11e9-8390-3f2fc765b614.png)

On the following sections we will explain the main core sub-systems of this engine.


### 3D Renderer

...

### Geometry loading

...

### Textures

...

### Gameobject structure

...

### Binary files format

...

### 3D Camera and frustum culling

...

### Quadtree

...

### Data serialization

...

### Mouse picking

...

### Time managment & game mode

...

### Resource Management

...


***

## **Skeletal animation subsystem**

// TODO: explanation here...

***

## **Engine Demo**

TODO: add video here

***

## **Installation instructions**

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy!

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game._

***

## **Notes on performance**

There are two main cases where the overall perfomance might decrease significantly:

* If the z-buffer visualization is activated.
* When an animated model is loaded on scene **(especially if its playing its animation)** the engine perfomance might drop a lot. That's because the skinning process of deforming the mesh to match the bones positions of the corresponding skeleton needs to do a lot of matrix related operations multiple times per frame. As we don't use shaders in this engine all these operations are sended to the CPU (instead of the GPU) which is not prepared to handle matrix operations as well as the GPU does. 

***

## **Controls**

~~~~~~~~~~~~~~~
Controls

- Select object: Left click

Gizmos

- W/E/R: Change gizmo to Translate, Rotation and Scale

Assets controls

- Load scene: Double left clic

Camera

- Free look around: Right Click Mouse Button
- Orbit around object: LALT + Left Click Mouse Button
- Zoom in/out Mouse wheel
- Pan: Middle mouse button
- Duplicate movement speeed: Hold Shift key
- Focus camera on object: F key

UI 

- Open config window: 4
- Open console: 1 
- Open Inspector: I key
- Show/Hide UI: LALT + G key
- Quit application: ESC / Alt+F4

Click again on Window->(any option) to close that window

~~~~~~~~~~~~~~~

***

## **Tools used**

* IDE: Microsoft Visual Studio 2017
* Language: C++
* Containers: STL
* Input and audio: SDL 2.0.8
* Graphics: OpenGL
* Math: MathGeoLib 1.5
* Random Number Generator: PCG 0.9 (Minimal C Edition)
* GUI: Deat ImGui 1.65
* 3D Model / Animation importer: Assimp 1.4.0
* Image loader: Developer's Image Library (DevIL) 1.8.0
* Data persistance: JSON parser - Parson 2017
* Profiler: Brofiler 1.1.2
* Memory manager: mmgr
* File system: PHYSFS 3.0.1
* Code repository: GitHub
* Others: Adobe Photoshop CS6 / MS Paint / Aseprite

***

## **Credits attributions**

* All the animated models along with its corresponding textures have been downloaded from [Mixamo](https://www.mixamo.com/#/)

***

## **License**

~~~~~~~~~~~~~~~

MIT License

Copyright (c) 2017 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

~~~~~~~~~~~~~~~
