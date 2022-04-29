# Round Robin
A C++ program that simulates the Round Robin scheduling algorithm in which each process is assigned a fixed time slot in a cyclic way. This scheduling method is easy to implement and prevents starvation as all processes get a fair share of CPU!

The program reads a list of processes and their burst times until end of transmission, EOF(^d). The alarm syscall is used to schedule an interrupt every 3 seconds and sleep is used to simulate the process running. 

```
A 9
B 10
C 3
D 5
E 7
^d
```

# Running the program
Download ```roundRobin.cpp``` and compile the code in terminal or command prompt using this command: ```g++ roundRobin.cpp -o roundRobin```. Simply enter  ```roundRobin```, the executable file, to run the program. 

![](https://i.imgur.com/zaIKwa8.gif)


