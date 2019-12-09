# BiKayaOS -> Phase 0

## Goal of the phase 0
The target of this project is to read a string from the user input (via terminal) and then to put that string to the printer device. In order to do this we had at first to implement a function to read the characters from the terminal and save them, at the same time
handling possible error. 

## Implementation
In order to do so we have written two function in the **termutils.c** file respectively *term_getchar()* that follows this 4 steps:

1) Controls that the terminal is open "correctly"
2) Send the receive command in the tp->recv_command "channel"
3) Wait for the terminal to execute the command and retrieve the data fetched
4) Check for possible error before returning the value retrieved and if an error is present it returns an error code

In case of error the method print out a error message and then stops the execution
We have also implemented the *term_gets()* that takes as input both a char array and the array length, is a simple for loop that cycles **at max** till the second-last index (the last is always reserved for the '\0' terminator). Note that the method stops before the max iterations for two main reason:

1) The user has pressed enter (ASCII \n) meaning it doesn't want to write anymore
2) An error code status has been returned by term_getchar() function