# 6: Processes
### **Exercise 6-2**
Write a program to see what happens if we try to *longjmp()* to a function that has already returned.
### **Exercise 6-3**
Implement *setenv()* and *unsetenv()* using *getenv()*, *putenv()*, and where necessary, code that directly modifies *environ*. Your version of *unsetenv()* should check to see whether there are multiple definitios of an environment variable, and remove them all (which is what the *glibc* version of *unsetenv()* does).
