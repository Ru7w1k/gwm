cl.exe /c /EHsc .\03_MultiViewPort.cpp

link.exe 03_MultiViewPort.obj opengl32.lib user32.lib kernel32.lib gdi32.lib

del 03_MultiViewPort.obj 

03_MultiViewPort.exe
