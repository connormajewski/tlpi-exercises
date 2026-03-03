# 26: Monitoring Child Processes
### **Exercise 26-1**
Write a program to verify that when a child's parent terminates, a call to *getppid()* returns 1 (the process ID of *init*).
### **Exercise 26-2**
Suppose that we have three processes related as grandparent, parent, and child, and that the grandparent doesn't immediately perform a *wait()* after the parent exits, so that the parent becomes a zombie. When do you expect the grandchild to be adopted by *init* (so that *getppid()* in the grandchild returns 1): after the parent terminates or after the grandparent does a *wait()*? Write a program to verify your answer.
