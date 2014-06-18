@echo off  
  
wmic environment where "name='path' and username='<system>'" set VariableValue="%path%;%~dp0bin\dll"

regsvr32   Activex控件\FRManager.ocx
regsvr32   Activex控件\SRecoManager.ocx

配置文件.exe