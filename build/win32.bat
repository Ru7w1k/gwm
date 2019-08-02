echo "Compiling for win32"
cl.exe /c /EHsc /LD /I"C:\glew\include" "C:\user\administrator\win32.cpp"

echo "Linking for win32"
link.exe /OUT:win32gl.dll /DLL /LIBPATH:"C:\glew\lib\Release\Win32" win32.obj opengl32.lib user32.lib kernel32.lib gdi32.lib
link.exe /LIBPATH:"C:\glew\lib\Release\Win32" win32.obj opengl32.lib user32.lib kernel32.lib gdi32.lib

echo "Moving win32 files"
