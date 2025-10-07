# Question
Lab exp.  no. 5
Implement a simple shell that acts as a command line interface similar to /bin/bash to OS. Your shell should have the following functionalities
1. Create a single process i.e. a shell that may be considered as a parent process.
2. The parent process accepts one linux command at a time e.g. ls -l
3. The parent process creates a child process and passes the linux command as an input parameter to the child process.
4. The child process invokes the system call exec(), executes and then terminates. Till then the parent process wait.
5. The parent process never dies even though it's child terminates but instead waits for another command as input from the user. The same set of steps above from 3 to 4 is repeated.
6. The parent only dies when you press Ctrl X
7. Also, if you have entered an incorrect linux command, it should flag a message to the user that it is an invalid command.