set gui=C:\Prime\AAA_Stenograph\NextGen\src_linux\gui
set inc=%gui%\extlibs\rislib\installs\include
set lib=%gui%\extlibs\rislib\installs\arm-build

set dev_rislib=C:\Prime\DevelopLib\Dev_RisLib
set build=C:\Users\chris\Desktop\out\build\prime-arm8



rmdir /s /q %gui%\extlibs
mkdir %inc%
mkdir %lib%

copy %dev_rislib%\RisLib\*.h %inc% 
copy %build%\RisLib\*.a %lib% 
pause


