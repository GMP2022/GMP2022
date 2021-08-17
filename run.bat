@echo off & setlocal enabledelayedexpansion

::get the administrator rights
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
 
::Datasets Names
DATASETS=(audio)

EXE=".\bin\mips.exe"

DIR="D:\code\mips"

set objLength=2
set obj[0]-dataset=audio
set obj[0]-k_para=50
set obj[0]-c_para=4.1
set obj[1]-dataset=video
set obj[1]-k_para=10
set obj[1]-c_para=6.1

set objIndex=0

:loopStart
if %objIndex% equ %objLength% goto end

set objCurrent-dataset=0
set objCurrent-k_para=0
set objCurrent-c_para=0

for /f "usebackq delims==- tokens=1-3" %%i in (`set obj[%objIndex%]`) do (
    set objCurrent-%%j=%%k

    echo %%i
    echo %%j
    echo %%k
)

echo dataset=%objCurrent-dataset%
echo k_para=%objCurrent-k_para%
echo c_para=%objCurrent-c_para%

set /a objIndex=%objIndex% + 1

goto loopStart

:end

pause
