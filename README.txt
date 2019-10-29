segoe UI 8/12/15 BOLD

#####  Color Palette by Color Scheme Designer
#####  Palette URL: http://colorschemedesigner.com/#3q31TmR--w0w0
#####  Color Space: RGB;

//Bugs:
a) Check if files are indeed closed
b) Fit objects sometimes does not update


// For compiler to run:
The host computer should have a compiler already installed and enviroment settings should be initialized properly.
Seems to be a trouble to have it now. Check this page for more:
http://root.cern.ch/phpBB3/viewtopic.php?f=5&t=6905
http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=16181

Almost all the libs of root are to be placed in the bin directory for proper operation of compiler
One should run IROOT from the vs2010 or Windows sdk command prompt for operation of compiler without problems
vs2010 express edition command prompt default is in 32 bit (x86) mode>: Just cd to IROOT and run IROOTNEW.exe
Win sdk 7.1 command prompt is by default in 64 bit debug mode therefore, type cd Bin and then SetEnv.cmd /x86
which will change the mode to 32 bit release. Then cd to IROOT and run IROOTNEW.exe

Be careful when building IROOT: There are multiple Qt Creators using different compilers and therefore different
output folder names. Copy the appropriate exe file for packaging
