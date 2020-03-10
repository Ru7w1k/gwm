# test opengl app

Prereqs:
- Visual Studio 2019

Steps:
1. Find attached 'testapp.rar' file, and extract it to some location. (e.g. D:/OpenGL/TestApp)
2. Open x86 Native Tools command prompt for installed visual studio 2019. 
   This command prompt is different that regular cmd as it is configured with some additional paths.
   Use either a or b to open:
        a. Search 'x86 Native' in cortana search bar and select first option
        b. Open Start menu (Windows Key) 
            +-> All Programs 
                +-> Visual Studio 2019 (folder) 
                    +-> x86 Native Tools Command Prompt
3. Using above command prompt, navigate to extracted folder
        `cd /d D:\OpenGL\TestApp`
4. Run the build script
        `build.bat`
5. A window should appear with colored triangle, which will respond to:
          F: toggle fullscreen
        Esc: exit application
6. If you are able to see the colored triangle, your machine is ready for OpenGL development!
