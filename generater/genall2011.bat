@echo off
if not exist project mkdir project
if not exist project\\src mkdir project\\src
if not exist project\\include mkdir project\\include
if not exist project\\mfc mkdir project\\mfc
if not exist project\\profile mkdir project\\profile
if not exist project\\mfc\\res mkdir project\\mfc\\res
for %%1 in (*.xml) do call generate2011 %%~n1
for %%1 in (*.xml) do copy %%1 project\\profile
pause
