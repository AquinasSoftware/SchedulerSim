FLAGS = -lstdc++

SchedulerSim: main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o
	gcc main.o processes.o schedulers/FIFO.o schedulers/SJF.o schedulers/IO.o schedulers/RR.o schedulers/SWQ.o $(FLAGS) -o SchedulerSim
	@chmod +x SchedulerSim

main.o: main.cpp
	gcc -c main.cpp $(FLAGS)

processes.o: processes.cpp
	gcc -c processes.cpp $(FLAGS)

schedulers/FIFO.o: schedulers/FIFO.cpp
	gcc -c schedulers/FIFO.cpp -o schedulers/FIFO.o $(FLAGS)

schedulers/SJF.o: schedulers/SJF.cpp
	gcc -c schedulers/SJF.cpp -o schedulers/SJF.o $(FLAGS)

schedulers/IO.o: schedulers/IO.cpp
	gcc -c schedulers/IO.cpp -o schedulers/IO.o $(FLAGS)

schedulers/RR.o: schedulers/RR.cpp
	gcc -c schedulers/RR.cpp -o schedulers/RR.o $(FLAGS)

schedulers/SWQ.o: schedulers/SWQ.cpp
	gcc -c schedulers/SWQ.cpp -o schedulers/SWQ.o $(FLAGS)
	
clean:
	rm SchedulerSim *.o schedulers/*.o

clean-win:
	del SchedulerSim.exe 
	del .\*.o   
	del schedulers\*.o   