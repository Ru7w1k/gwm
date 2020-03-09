
REM copy glew\ folder into C:\ directory

REM copy lib\Release\Win32\glew32.dll from glew directory into C:\system\SysWOW64 directory

cl.exe /c /EHsc /I"C:\glew\include" .\triangle.cpp
link.exe /LIBPATH:"C:\glew\lib\Release\Win32" triangle.obj opengl32.lib user32.lib kernel32.lib gdi32.lib
