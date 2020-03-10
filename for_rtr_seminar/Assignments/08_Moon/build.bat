rc.exe res.rc

cl.exe /c /EHsc .\08_Moon.cpp

link.exe 08_Moon.obj res.res opengl32.lib user32.lib kernel32.lib gdi32.lib

del 08_Moon.obj res.res

08_Moon.exe
