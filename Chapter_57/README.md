# 57: Sockets: UNIX Domain
### **Exercise 57-1**
In Section 57.3, we noted that UNIX domain datagram sockets are reliable. Write programs to show that if a sender transmits datagrams to a UNIX domain datagram socket faster than the receiver reads them, then the sender is eventually blocked, and remains blocked until the receiver reads some of the pending datagrams.
### **Exercise 57-3**
Reimplement the sequence-number server and client of Section 44.8 using UNIX domain stream sockets.
### **Exercise 57-4**
Suppose we create two UNIX domain datagram sockets bound to the paths */somepath/a* and */somepath/b*, and that we connect the socket */somepath/a* to */somepath/b*. What happens if we create a third datagram socket and try to send (*sendto()*) a datagramvia that socket to */somepath/a*? Write a program to determine the answer. If you have access to other UNIX systems, test the program on those systems to see if the answer differs.
