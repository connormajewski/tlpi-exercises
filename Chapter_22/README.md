# 22: Signals: Advanced Features
### **Exercise 22-1**
Section 22.2 noted thiat if a stopped process that has established a handler for and blocked **SIGCONT** is later resumed as a consequence of receiving a **SIGCONT**, then the handler is invoked only when **SIGCONT** is unblocked. Write a program to verify this. Recall that a process can be stopped by typing the terminal *suspend* character (usually *Ctrl-Z*) and can be sent a **SIGCONT** signal using the command *kill -CONT* (or implicitally, using the shell *fg* command).
### **Exercise 22-2**
If both a realtime and a standard signal are pending for a process, SUSv3 leaves it unspecified which is delivered first. Write a program that shows what Linux does in this case. (Have the program set up a handler for all signals, block signals for a a period of time so that you can send various signals to it, and then unblock all signals.)
### **Exercise 22-4**
Implement the System V functions *sigset()*, *sighold()*, *sigrelse*, *sigignore()*, and *sigpause()* using the POSIX signal API.

