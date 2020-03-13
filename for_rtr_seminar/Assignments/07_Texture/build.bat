rc.exe res.rc

cl.exe /c /EHsc .\07_Texture.cpp

link.exe 07_Texture.obj res.res opengl32.lib user32.lib kernel32.lib gdi32.lib

del 07_Texture.obj res.res

07_Texture.exe
