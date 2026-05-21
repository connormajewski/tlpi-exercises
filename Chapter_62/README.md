# 62: Terminals
### **Exercise 62-1**
Implement *isatty()*. (You may find it useful to read the descriptio nof *tcgetattr()* in Section 62.2)
### **Exercise 62-2**
Implement *ttyname()*.
### **Exercise 62-3**
Implement the *getpass()* function described in Section 8.5. (The *getpass()* function can obtain a file descriptor for the controlling terminal by opening /dev/tty.)
### **Exercise 62-4**
Write a program that displays information indicating whether the terminal referred to by standard input is in canonical or concanonical mode. If in noncanonical mode, then display the values of TIME and MIN.
