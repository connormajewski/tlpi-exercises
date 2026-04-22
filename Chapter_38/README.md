# 38: Writing Secure Privileged Programs
### **Exercise 38-2**
Write a set-user-ID-*root* program similar to the *sudo(8)* program. This program should take command-line options and arguments as follows: **$ ./douser [-u user] ** ***program-file arg1 arg2 ...***
The *douser* program executes *program-file*, with the given arguments, as though it was run by *user*. (If the *-u* option is omitted, then *user* should default to *root*) Before executing *progam-file*, *douser* should request the password for *user*, authenticate it against the standard password file (see Listing 8-2, on page 164), and then set all of the prces user and group IDs to the correct values for that user.
