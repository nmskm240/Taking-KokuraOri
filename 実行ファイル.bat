@ECHO OFF
cd /d %~dp0

SET auth=
SET filename=
SET inputpath=../../input/
SET formatWAV=.wav
SET formatPNG=.png
SET formatCSV=.csv

cd input

:FILE_SELECT
ECHO +-------------------------------------------------------+
ECHO  inputフォルダに存在するファイル名を入力してください。
ECHO +-------------------------------------------------------+
SET /P filename=
 
IF "%filename%"=="" (GOTO FILE_SELECT)
IF EXIST %filename%%formatWAV% (GOTO Wav2PNG) ELSE (GOTO FILE_SELECT)

:Wav2PNG
cd /d %~dp0
cd Wav2PNG_20201025/Wav2PNG
start /wait Wav2PNG_20201025.exe filename=%inputpath%%filename%

timeout 10

ECHO +-------------------------------------------------------+
ECHO  Bracode2KokuraOriを起動しますか.。[y/n]
ECHO +-------------------------------------------------------+
SET /P auth=
IF %auth%==y (GOTO Barcode2KokuraOri) ELSE (GOTO END)

:Barcode2KokuraOri
cd /d %~dp0
cd Talking KokuraOri/Debug
call Barcode2KokuraOri.exe %filename%
GOTO FIN

:END
cd /d %~dp0
cd input
move %filename%%formatPNG% ../output/
cd /d %~dp0
cd Talking KokuraOri/DPIChanger/bin/Debug
call DPIChanger.exe %filename%
GOTO FIN

:FIN
cd /d %~dp0
cd input
del %filename%%formatPNG% %filename%%formatCSV%

PAUSE
EXIT