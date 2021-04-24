@ECHO OFF
chcp 65001
cd /d %~dp0/input

SET tmp=
SET filename=
SET inputpath=../../input/
SET formatWAV=.wav
SET formatPNG=.png
SET formatCSV=.csv

:ProcessSelect
echo +-------------------------------------------------------+
echo  何を起動しますか？ 
echo　1:文字列をバーコードに 
echo　2:wavファイルをバーコードに 
echo　3:バーコードを小倉織化 
echo +-------------------------------------------------------+
SET /P auth=
IF %auth%==1 (GOTO Text2Wav) ELSE IF %auth%==2 (GOTO Wav2Png) ELSE IF %auth%==3 (GOTO Barcode2KokuraOri) ELSE (GOTO ProcessSelect)

:Text2Wav
ECHO +-------------------------------------------------------+
ECHO  バーコード化する文字を入力してください。
ECHO +-------------------------------------------------------+
SET /P filename=

cd /d %~dp0/Text2Wav/Text2Wav/bin/Debug
call Text2Wav.exe %filename%

:Wav2Png
cd /d %~dp0/Wav2PNG_20201025/Wav2PNG
IF %auth%==2 (echo バーコード化するwavファイル名を入力して下さい。) 
IF %auth%==2 (SET /P filename=)
start /wait Wav2PNG_20201025.exe filename=%inputpath%%filename%

timeout 10

ECHO +-------------------------------------------------------+
ECHO  Bracode2KokuraOriを起動しますか.。[y/n]
ECHO +-------------------------------------------------------+
SET /P tmp=
IF %tmp%==n (GOTO FIN)

:Barcode2KokuraOri
chcp 932
cd /d %~dp0/Talking KokuraOri/Release
call Barcode2KokuraOri.exe %filename%

:FIN
cd /d %~dp0/input
IF %tmp%==n (move %filename%%formatPNG% ../output)
cd /d %~dp0/DPIChanger/DPIChanger/bin/Release
call DPIChanger.exe %filename%
cd /d %~dp0/input
del %filename%%formatPNG% %filename%%formatCSV%

PAUSE
EXIT