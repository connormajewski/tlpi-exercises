# 23: Timers and Sleeping
### **Exercise 23-1**
Although *alarm()* is implemented as a system call within the Linux kernel, this is redundant. Implement *alarm()* using *setitimer()*.
### **Exercise 23-3**
Write a program to show that if the *evp* argument to *timer_create()* is specified as NULL, then this is equivalent to specifying *evp* as a pointer to a *sigevent* structure with *sigev_notify* set to **SIGEV_SIGNAL**, *sigev_signo* set to **SIGALARM**, and *si_value.sival_int* set to the timer ID.
