#run: main.cpp
#	g++ -O2 -std=c++11 -o run main.cpp


# To include headers for python, for matplotlibcpp
run: main.cpp epidemic.cpp epidemic.h
	g++ -g -o run -std=c++11 -I/usr/include/python2.7 -lpython2.7  main.cpp epidemic.cpp
