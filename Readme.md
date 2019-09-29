# CUHK CSCI3150 2019 SEM1 ASSGN1
**The master branch is the finished product using hardcode technique.**

**The test branch is trying to implement loop technique to finish the assignment in a more natural way.**

**The fix branch is to fix the instability of program in master branch**

Readme provided from lecturer below
-----------------------------------------------------------------------------------------------------------

1. The current zip file contains a simple shell program that can execute one command.  To compile, input:
  make

Then you can run it:

  ./SimpleShell



2. For Assignment One:
(1) Revise simple-execute.c (that is the only file you need to submit) to implement the functions mentioned in Assignment One. In simple-execute.c, you can define your own functions and add header files if needed. However, you must use "dup()", "pipe()", and "execvp()" to execute the commands.

(2) Your program should be compiled by:
   make

(3) At least, your shell program should be able to correctly execute the following commands:

$$$ ls

$$$ ps

$$$ ls -l

$$$ ps -al

$$$ ls -l | wc -l

$$$ ls -l | grep D | wc -l


Submission: You only need to submit simple-execute.c.
