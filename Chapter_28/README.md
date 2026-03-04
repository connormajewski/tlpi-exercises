# 28: Process Creation and Program Execution in More Detail
### **Exercise 28-1**
Write a program to see how fast the *fork()* and *vfork()* system calls are on your system. Each child process should imediately exit, and the parent should *wait()* on each child before creating the next. Compare the relative differences for these two system calls with those of Table 28-3. The shell built-in command *time* can be used to measure the execution time of a program. 
