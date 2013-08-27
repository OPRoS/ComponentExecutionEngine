@echo off
set oldpath=%PATH%
set PATH=%PATH%;%~dp0
IF '%1__'=='__' goto helparg
IF NOT '%2__'=='__'  goto secondarg
call jd -param filename '^"%CD%\%1^"' -param usage 'none' "%~dp0GenerateProfiles2011.xsl" "%~dp0GenerateProfiles2011.xsl"

goto end

:secondarg
call jd -param filename '^"%CD%\%1^"' -param usage '%2' "%~dp0GenerateProfiles2011.xsl"  "%~dp0GenerateProfiles2011.xsl" 

goto end
:helparg
@echo generate {profile name}
@echo   ex} generate helpercomponent
:end
set PATH=%oldpath%
