set command=astyle.exe --style=allman --indent=spaces=4 --align-pointer=type --align-reference=type --indent-cases --indent-namespaces --pad-oper --remove-brackets --mode=c -r -n
%command% "../include/*.h"
%command% "../src/*.h"
%command% "../src/*.cpp"
