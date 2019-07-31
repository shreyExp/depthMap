pro: main.cpp getParams.h helper.h
	g++ -o pro main.cpp `pkg-config --cflags --libs opencv plplot-c++ jsoncpp`

