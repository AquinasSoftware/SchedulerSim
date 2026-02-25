CXX = gcc  # Default for Linux (use g++ if you prefer)
FLAGS = -lstdc++  # C++ std lib (but better handled by g++)
CXXFLAGS =  # Empty default; overridden for win64
LDFLAGS =   # Empty default; overridden for win64
EXEEXT =    # Empty for Linux
TARGET = SchedulerSim

all: $(TARGET)$(EXEEXT)

$(TARGET)$(EXEEXT): main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o
	$(CXX) main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o $(FLAGS) $(LDFLAGS) -o $(TARGET)$(EXEEXT)
	@chmod +x $(TARGET)$(EXEEXT)

win64:
	$(MAKE) clean
	$(MAKE) CXX=x86_64-w64-mingw32-g++ \
		CXXFLAGS="`wx-config --host=x86_64-w64-mingw32 --cxxflags`" \
		LDFLAGS="`wx-config --host=x86_64-w64-mingw32 --libs --static`" \
		EXEEXT=.exe \
		TARGET=SchedulerSim

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp $(FLAGS)

processes.o: processes.cpp
	$(CXX) $(CXXFLAGS) -c processes.cpp $(FLAGS)

schedulers/FIFO.o: schedulers/FIFO.cpp
	$(CXX) $(CXXFLAGS) -c schedulers/FIFO.cpp -o schedulers/FIFO.o $(FLAGS)

schedulers/SJF.o: schedulers/SJF.cpp
	$(CXX) $(CXXFLAGS) -c schedulers/SJF.cpp -o schedulers/SJF.o $(FLAGS)

schedulers/IO.o: schedulers/IO.cpp
	$(CXX) $(CXXFLAGS) -c schedulers/IO.cpp -o schedulers/IO.o $(FLAGS)

schedulers/RR.o: schedulers/RR.cpp
	$(CXX) $(CXXFLAGS) -c schedulers/RR.cpp -o schedulers/RR.o $(FLAGS)

schedulers/SWQ.o: schedulers/SWQ.cpp
	$(CXX) $(CXXFLAGS) -c schedulers/SWQ.cpp -o schedulers/SWQ.o $(FLAGS)
	
clean:
	rm -f $(TARGET) *.o schedulers/*.o

clean-win: clean
	rm -f $(TARGET).exe