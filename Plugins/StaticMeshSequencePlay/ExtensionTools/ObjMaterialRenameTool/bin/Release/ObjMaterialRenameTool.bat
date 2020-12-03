@echo off
setlocal

set /p path=폴더 전체 경로를 입력하세요:
set /p name=새로 적용할 Material 이름을 입력하세요:
set /p numOf=그룹당 Material 파일 갯수를 입력하세요:
echo.
echo.
echo.
rem START /MIN
"%cd%\ObjMaterialRenameTool.exe" %path% %name% %numOf%
rem EXIT
