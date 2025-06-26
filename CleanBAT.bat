REM 清理项目垃圾文件 
del *.log  *.ipch *.aps *.user /s
del *.exe *.dll /s

del *.sdf /s
del *.ilk *.pdb *.exp  *.tlog    *.lastbuildstate /s
del *.obj *.pch *.idb/s

REM 额外 rd用于删除整个文件夹
del *.APS /s
rd Debug /Q /s
rd .vs /Q /s
pause 
