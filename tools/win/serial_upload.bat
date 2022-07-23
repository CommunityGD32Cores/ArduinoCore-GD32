rem @echo off
rem: Note %~dp0 get path of this batch file
rem: Need to change drive if My Documents is on a drive other than C:
set scriptPath=%~dp0
set driverLetter=%scriptPath:~0,2%
set scriptPath=%scriptPath:/=\%

rem %driverLetter%
rem cd %~dp0

rem: ------------- use GD's own uploader 
rem: ---- Need to remove the COM bit from the comm port as the GD prog just wants the number
set commport=%1
set commportnum=%commport:COM=%

rem: the two line below are needed to fix path issues with incorrect slashes before the bin file name
set strBinFile=%4
set strBinFile=%strBinFile:/=\%

%scriptPath%\GD32_ISP_CLI.exe -c --pn %commportnum% --br 57600 -e --all -d --a 8000000 --fn %strBinFile% --v -r --a 8000000
