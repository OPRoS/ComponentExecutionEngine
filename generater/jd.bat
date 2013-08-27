@echo off
set LIBPATH=%~dp0
set CP=%LIBPATH%bsf.jar;%LIBPATH%xerces.jar;%LIBPATH%jd.jar
java -cp ^"%CP%^" jd.xml.xslt.Stylesheet %1 %2 %3 %4 %5 %6 %7 %8 %9
