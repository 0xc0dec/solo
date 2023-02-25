set command=astyle.exe --style=java --indent=spaces=4 --align-pointer=name --align-reference=name --indent-cases --indent-switches --indent-namespaces --pad-oper --remove-brackets --mode=c -r -n
%command% "../src/*.h"
%command% "../src/*.cpp"
