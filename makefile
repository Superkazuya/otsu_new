CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

otsu : $(wildcard *.cpp)
	$(CXX) $^ $(CFLAGS) $(LIBS) -o $@ -O3 

