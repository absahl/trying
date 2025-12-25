!include "FileFunc.nsh"
; !include "LogicLib.nsh"

OutFile "LogTimeInstaller.exe"

Section
    ; Get local time
    ${GetTime} "" "L" $0 $1 $2 $3 $4 $5 $6
    ; Format and log it
    DetailPrint "Current time: $2-$1-$0 $4:$5:$6"
SectionEnd
