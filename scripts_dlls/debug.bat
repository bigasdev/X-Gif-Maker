@echo off

mingw32-make -j8 debug DEBUG=true
.\\.build\\fortress.exe
exit