cl.exe /c /EHsc .\00_demo.cpp

link.exe 00_demo.obj opengl32.lib user32.lib kernel32.lib gdi32.lib

del 00_demo.obj 

00_demo.exe
