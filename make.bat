call .\clean.bat

@echo off
@REM ENTER YOUR GBDK DIRECTORY IN @set GBDK = ...
@set GBDK=..\..\..\gbdk
%GBDK%\bin\lcc.exe -autobank -Wl-j -Wm-yS -Wb-ext=.rel -Wb-v -Wl-yt0x1B -Wl-yoA -Wl-ya4 -o platformer_physics.gb^
 src\*.c res\tiles\*.c
 


