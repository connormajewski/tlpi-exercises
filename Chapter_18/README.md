# 18: Directories and Links
### **Exercise 18-3**
Implement *realpath()*.
### **Exercise 18-5**
Implement a function that performs the equivalent of *getcwd()*. A useful tip for solving this problem is that you can find the name of the current working directory by using *opendir()* and *readdir()* to walk through each of the entries in the parent directory (..) to find an entry with the same i-node and device number as the current working directory (i.e., respectively, the *st_ino* and *st_dev* fields in the *stat* structure returned by *stat()* and *lstat()*). Thus, it is possible to construct the directory path by walking up the directory tree (*chdir("..")*) one step at a time and performing such scans. The walk can be finished when the parent directory is the same as the current working directory. (recall that /.. is the same as /). The caller should be left in the same directory in which it started, regardless of whether your *getcwd()* function succeeds or fails (*open()* plus *fchdir()* are handy for this purpose).
### **Exercise 18-7**
Write a program that uses *nftw()* to traverse a directory tree and finishes by printing out counts and percentages of the various types (regular, directory, sumbolic link, and so on) of files in the tree.
### **Exercise 18-8**
Implement *nftw()*. (This will require the use of *opendir()*, *readdir()*, *closedir()*, and *stat()* system calls, among others.)
