# BiKayaOS
**A variation of the Kaya OS that can run on both uARM and uMPS, this is the project for the OS course during the academic year 2019/2020**

This project is written by [Enea Guidi](https://github.com/its-hmny), [Marco Tomasone](https://github.com/MarcoTomasone), [Luca Genova](https://github.com/Lucajett99) and [Simone Boldrini](https://github.com/minosse99). The goal of the project is to write a kernel wich can run both on uMPS and uARM architechture, the project is divided in 3 main phases:

## **Compilation**
In order to use effectively this project you both need the [uARM](http://mellotanica.github.io/uARM/) and [uMPS](https://github.com/tjonjic/umps) simulator and all their dependencies (cross compilers, linkers, etc) you can find all the information needed at the links above. To build the project just go to the project main directory and type:

```console
usr@computer:~/BiKayaOS$ make uarm
usr@computer:~/BiKayaOS$ make umps
```
to build respectively the uarm version or the umps version, if everything has been set correctly the compilation should go fine. After that remember to set the kernel.core.uarm and the kernel.stab.uarm  (respectively .umps) as Core and Symbol Table file in the respective simulators, then boot up the machines

## **Phase 0**

## Goal
The target of this project is to read a string from the user input (via terminal) and then to put that string to the printer device. In order to do this we had at first to implement a function to read characters from the terminal and save them, at the same time
handling possible error from the terminal device. 

## Implementation
In order to do so we have written two function in the **termutils.c** file respectively *term_getchar()* and *term_gets()* that executed the following algorithm:

1) Controls that the terminal is open "correctly"
2) Send the receive command in the tp->recv_command "channel"
3) Wait for the terminal to execute the command and retrieves the data fetched
4) Check for possible error before returning the value retrieved and if an error is present it returns an error code
5) Executes until the endline char ("\n") is returned

In case of error the method print out a message and then stops the execution indeterminately
*term_gets()* takes two parameters the char array and its length and cycles until the endline is retrieved or until the max index is reached.

When the input is read the string is passed to the *print()* method in the **devutils.c** file. This is a simple infinite loop cycle that loops till the \0 is reached and at every iteration calls the *print_char()* method that as for the term_getchar() above puts the char in the registrer and checks for error before and after the instruction execution


There isn't any type of iteration so after everything went fine the system will put himself on an endless sleep cicle, and should be manually resetted.

## **License**
This software is distributed under the  GNU General Public License version 3, for further info please read the LICENSE file.