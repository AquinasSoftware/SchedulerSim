CXX = gcc  # Default for Linux (use g++ if you prefer)
FLAGS = -lstdc++  # C++ std lib (but better handled by g++)
CXXFLAGS =  # Empty default; overridden for win64
WX_CONFIG = /usr/bin/wx-config
WXFLAGS = $(shell $(WX_CONFIG) --cxxflags)
WXLIBS = $(shell $(WX_CONFIG) --libs)
LDFLAGS =   # Empty default; overridden for win64
EXEEXT =    # Empty for Linux
TARGET = SchedulerSim

all: $(TARGET)$(EXEEXT)

$(TARGET)$(EXEEXT): main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o schedulers/RedRobin.o
	$(CXX) main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o schedulers/RedRobin.o \
	$(LDFLAGS) $(FLAGS) -o $(TARGET)$(EXEEXT) $(WXLIBS)

win64:
	$(MAKE) clean
	$(MAKE) CXX=x86_64-w64-mingw32-g++ \
		CXXFLAGS="`wx-config --host=x86_64-w64-mingw32 --cxxflags`" \
		WX_CONFIG="wx-config --host=x86_64-w64-mingw32" \
		LDFLAGS="`wx-config --host=x86_64-w64-mingw32 --libs --static`" \
		EXEEXT=.exe \
		TARGET=SchedulerSim

main.o: main.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c main.cpp $(WXFLAGS)

processes.o: processes.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c processes.cpp $(WXFLAGS)

schedulers/FIFO.o: schedulers/FIFO.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/FIFO.cpp -o schedulers/FIFO.o $(WXFLAGS)

schedulers/SJF.o: schedulers/SJF.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/SJF.cpp -o schedulers/SJF.o $(WXFLAGS)

schedulers/IO.o: schedulers/IO.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/IO.cpp -o schedulers/IO.o $(WXFLAGS)

schedulers/RR.o: schedulers/RR.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/RR.cpp -o schedulers/RR.o $(WXFLAGS)

schedulers/SWQ.o: schedulers/SWQ.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/SWQ.cpp -o schedulers/SWQ.o $(WXFLAGS)

schedulers/RedRobin.o: schedulers/RedRobin.cpp
	$(CXX) -x c++ $(CXXFLAGS) -c schedulers/RedRobin.cpp -o schedulers/RedRobin.o $(WXFLAGS)
	
clean:
	rm -f $(TARGET) *.o schedulers/*.o

clean-win: clean
	rm -f $(TARGET).exe