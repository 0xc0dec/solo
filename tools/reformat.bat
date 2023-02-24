set command=astyle.exe --style=allman --indent=spaces=4 --align-pointer=name --align-reference=name --indent-cases --indent-namespaces --pad-oper --remove-brackets --mode=c -r -n
%command% "../src/*.h"
%command% "../src/*.cpp"
