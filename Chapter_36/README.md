# 36: Process Resources
### **Exercise 36-1**
Write a program that shows that the *getrusage()* RUSAGE_CHILDREN flag retrieves information about only the children for which a *wait()* call has been performed. (Have the program create a child process that consumes some CPU time, and then have the parent call *getrusage()* before and after calling *wait()*.)
### **Exercise 36-2**
Write a program that executes a command and then displays its resource usage. This is analogous to what the *time(1)* command doe. Thus, we would use this program as follows:
	$ **./rusage** ***command arg ...*** 
### **Exercise 36-3**
Write a program to determine what happens if a process's consumption of various resources already exceeds the soft limit specified in a call to *setrlimit()*.
