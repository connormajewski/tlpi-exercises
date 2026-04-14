# 35: Process Priorities and Scheduling
### **Exercise 35-1**
Implement the *nice(1)* command.
### **Exercise 35-2**
Write a set-user-ID-*root* program that is the realtime scheduling analog of *nice(1)*. The command-line interface of this program should be as follows:
	**# ./rtsched**  ***policy priority command arg...***
In the above command, *policy* is *r* for SCHED_RR or *f* for SCHED_FIFO. This program should drop its privileged ID before execing the command, for the reasons described in Sections 9.7.1 and 38.3. 
### **Exercise 35-3**
Write a program that places itself under the SCHED_FIFO scheduling policy and then creates a child process. Both processes should execute a function that causes the process to consume a maximum of 3 seconds of CPU time. (This can be done by using a loop in which the *times()* system call is repeatedly called to determine the amount of CPU time so far consumed.) After each quarter of a second of consumed CPU time, the function should print a message that displays the process ID and the amount of CPU time so far consumed. After each second of consumed CPU time, the function should call *sched_yield()* to yield the CPU to the other process. (Alternatively, the processes could raise each other's scheduling priority using *sched_setparam()*.) The program's output should demonstrate that the two processes alternatively execute for 1 second of CPU time. (Note carefully the advice given in Section 35.3.2 about preventing a runaway realtime process from hogging the CPU.)

