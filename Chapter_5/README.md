# 5: File I/O: Further Details
### **Exercise 5-2**
Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?
### **Exercise 5-3**
This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary. Write a program that takes up to three command line arguments:

$ **atomic_append *filename num-bytes [x]***

This program should open the specified filename (creating it if necessary) and append *num-bytes* to the file by using *write()* to write a byte at a time. By default, the program should open the file with the O_APPEND flag, but if a third command-line argument (*x*) is supplied, then the O_APPEND flag should be omitted, and instead the program should perform an *lseek(fd, 0, SEEK_END)* call before each *write()*. Run two instances of this program at the same time without the *x* argument to write 1 million bytes to the same file:

$ **atomic_append f1 1000000 & atomic_append f1 1000000**

Repeat the same steps, writing to a different file, but this time specifying the *x* argument:

$ **atomic_append f2 1000000 x & atomic_append f2 1000000 x**

List the sizes of the files f1 and f2 using *ls -l* and explain the difference.
### **Exercise 5-4**
Implement *dup()* and *dup2()* using *fcntl()* and, where necessary, *close()*. (You may ignore the fact that *dup2()* and *fcntl()* return different *errno* values for some error cases.) For *dup2()*, remember to handle the special case where *oldfd* equals *newfd*. In this case, you should check whether *oldfd* is valid, which can be done by, for example, checking if *fcntl(oldfd, F_GETFL)* succeeds. If *oldfd* is not valid, then the function should return -1 with *errno* set to EBADF.
### **Exercise 5-5**
Write a program to verify that duplicated file descriptors share a file offset value and open file status flags.
### **Exercise 5-7**
Implement *readv()* and *writev()* using *read()*, *write()*, and suitable functions from the *malloc* package (Section 7.1.2).
