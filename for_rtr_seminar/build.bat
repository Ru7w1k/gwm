
REM copy glew32.dll into C:\system\SysWOW64 folder

cl.exe /c /EHsc /I"C:\glew\include" .\triangle.cpp
link.exe /LIBPATH:"C:\glew\lib\Release\Win32" triangle.obj opengl32.lib user32.lib kernel32.lib gdi32.lib
