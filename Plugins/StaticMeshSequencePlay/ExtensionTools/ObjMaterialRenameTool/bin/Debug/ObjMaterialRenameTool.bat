@echo off
setlocal

set /p path=���� ��ü ��θ� �Է��ϼ���:
set /p name=���� ������ Material �̸��� �Է��ϼ���:
set /p numOf=�׷�� Material ���� ������ �Է��ϼ���:
echo.
echo.
echo.
rem START /MIN
"%cd%\ObjMaterialRenameTool.exe" %path% %name% %numOf%
rem EXIT
