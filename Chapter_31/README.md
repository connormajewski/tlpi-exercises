# 31: Threads: Thread Safety and Per-Thread Storage
### **Exercise 31-1**
Implement a function, *one_time_init(control, unit)* that performs the equivalent of *pthread_once()*. The *control* argument should be a pointer to a statically allocated structure containing a Boolean variable and a mutex. The Boolean variable indicates whether the function *init* has already been called, and the mutex controls access to the variable. To keep the implementation simple, you can ignore possibilities such as *init()* failing or being canceled when first called from a thread (i.e., it is not necessary to devise a scheme whereby, if such an event occurs, the next thread that calls *one_time_init()* reattmpts the call to *init()*).
### **Exercise 31-2**
Use thread-specific data to write thread-safe versions of *dirname()* and *basename()* (Section 18.14).
