FLAGS = -lstdc++

SchedulerSim: main.o processes.o schedulers/FIFO.o
	gcc main.o processes.o schedulers/FIFO.o $(FLAGS) -o SchedulerSim
	@chmod +x SchedulerSim

main.o: main.cpp
	gcc -c main.cpp $(FLAGS)

processes.o: processes.cpp
	gcc -c processes.cpp $(FLAGS)

schedulers/FIFO.o: schedulers/FIFO.cpp
	gcc -c schedulers/FIFO.cpp -o schedulers/FIFO.o $(FLAGS)

clean:
	rm SchedulerSim *.o schedulers/*.o