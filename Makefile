CXX = gcc
FLAGS = -lstdc++ -lm
CXXFLAGS =  -std=c++17
WX_CONFIG = /usr/bin/wx-config
WXFLAGS   = $(shell $(WX_CONFIG) --cxxflags)
WXLIBS    = $(shell $(WX_CONFIG) --libs)
LDFLAGS   =  
EXEEXT    =  
TARGET    = SchedulerSim

MP_INCLUDE    = -I./dependencies
MP_STATIC_LIB = ./dependencies/libwxmathplot_static.a  
all: $(TARGET)

$(TARGET): main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o schedulers/RedRobin.o
	$(CXX) $^ $(MP_STATIC_LIB) $(LDFLAGS) $(FLAGS) -o $@ $(WXLIBS) -s

win64:
	$(MAKE) clean
	$(MAKE) CXX=x86_64-w64-mingw32-g++ \
		CXXFLAGS="-std=c++17" \
		WXFLAGS="-I./dependencies/wxWidgets-win64/include \
		         -I./dependencies/wxWidgets-win64/lib/wx/include/msw-unicode-3.2 \
		         -I./dependencies/wxWidgets-win64/lib/wx/include/x86_64-w64-mingw32-msw-unicode-3.2 \
		         -D__WXMSW__ -DwxDEBUG_LEVEL=0" \
		WXLIBS="-L./dependencies/wxWidgets-win64/lib \
		        -lwx_mswu_core-3.2-x86_64-w64-mingw32 \
		        -lwx_baseu-3.2-x86_64-w64-mingw32 \
		        -lwx_baseu_net-3.2-x86_64-w64-mingw32 \
		        -lwx_baseu_xml-3.2-x86_64-w64-mingw32 \
		        -lwx_mswu_adv-3.2-x86_64-w64-mingw32 \
		        -lwx_mswu_aui-3.2-x86_64-w64-mingw32 \
		        -lwx_mswu_gl-3.2-x86_64-w64-mingw32 \
		        -lwx_mswu_html-3.2-x86_64-w64-mingw32 \
		        -lwx_mswu_stc-3.2-x86_64-w64-mingw32 \
		        -lwxscintilla-3.2-x86_64-w64-mingw32 \
		        -lole32 -luuid -lcomdlg32 -loleaut32 -lwinmm -lgdi32 -lcomctl32 \
		        -lshell32 -lshlwapi -lversion -lws2_32 -limm32 -lwinspool -ladvapi32 -luser32 \
		        -mwindows" \
		MP_INCLUDE="-I./dependencies" \
		MP_STATIC_LIB="./dependencies/libwxmathplot_static_win64.a" \
		EXEEXT=.exe \
		TARGET=SchedulerSim.exe
	mkdir dist
	mv SchedulerSim.exe dist/
	cp dependencies/wxWidgets-win64/bin/*.dll dist/
	cp /usr/lib/gcc/x86_64-w64-mingw32/*/libgcc_s_seh-1.dll dist/ || true
	cp /usr/lib/gcc/x86_64-w64-mingw32/*/libstdc++-6.dll dist/ || true
	cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll dist/ || true


main.o: main.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c main.cpp $(WXFLAGS)

processes.o: processes.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c processes.cpp $(WXFLAGS)

schedulers/FIFO.o: schedulers/FIFO.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/FIFO.cpp -o schedulers/FIFO.o $(WXFLAGS)

schedulers/SJF.o: schedulers/SJF.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/SJF.cpp -o schedulers/SJF.o $(WXFLAGS)

schedulers/IO.o: schedulers/IO.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/IO.cpp -o schedulers/IO.o $(WXFLAGS)

schedulers/RR.o: schedulers/RR.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/RR.cpp -o schedulers/RR.o $(WXFLAGS)

schedulers/SWQ.o: schedulers/SWQ.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/SWQ.cpp -o schedulers/SWQ.o $(WXFLAGS)

schedulers/RedRobin.o: schedulers/RedRobin.cpp
	$(CXX) -x c++ $(CXXFLAGS) $(MP_INCLUDE) -c schedulers/RedRobin.cpp -o schedulers/RedRobin.o $(WXFLAGS)

clean:
	rm -rf dist
	rm -f $(TARGET) *.o schedulers/*.o SchedulerSim.exe
