@echo off

echo Compiling Triangle.cpp...
cl.exe /c /EHsc .\triangle.cpp

if %ERRORLEVEL% NEQ 0 (
    echo Compiling failed...
    GOTO completed
)

echo Linking Triangle.obj...
link.exe triangle.obj opengl32.lib user32.lib kernel32.lib gdi32.lib

if %ERRORLEVEL% NEQ 0 (
    echo Linking failed...
    GOTO completed
)

echo Deleting Triangle.obj...
del triangle.obj 

echo Running Triangle.exe...
triangle.exe

:completed
