@echo off
set PATH=..\windows;..\windows\lib;%PATH%
set PROJ_LIB=..\windows\lib


meshed inputs_TX\fdr.tif inputs_TX\outlets1000.shp cat outputs_TX\wsheds1000.tif
meshed -l inputs_TX\fdr.tif inputs_TX\outlets1000.shp cat outputs_TX\wsheds1000-lessmem.tif
