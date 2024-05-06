@echo off
setlocal EnableDelayedExpansion
set PATH=..\windows;..\windows\lib;%PATH%
set PROJ_LIB=..\windows\lib

for %%o in (inputs_TX\outlets*.shp) do (
	set o=%%~no
	set w=!o:outlets=wsheds!
	meshed -c inputs_TX\fdr.tif %%o cat outputs_TX\!w!.tif
)
