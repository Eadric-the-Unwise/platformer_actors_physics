call .\clean.bat

@echo off
@REM ENTER YOUR GBDK DIRECTORY IN @set GBDK = ...
@REM -Wl-j .noi bgb debugging -Wm-yS
@set GBDK=..\..\..\gbdk
%GBDK%\bin\lcc.exe -autobank -Wf--debug -Wl-y -Wb-ext=.rel -Wb-v -Wl-yt0x1B -Wl-yoA -Wl-ya4 -o platformer_physics.gb^
 src\*.c res\tiles\*.c
 


