# BiKayaOS
**A variation of the Kaya OS that can run on both uARM and uMPS architechtures, this is the project for the OS Course a.y. 2019/2020**

Large portionsof this project are written by [Enea Guidi](https://github.com/its-hmny), [Marco Tomasone](https://github.com/MarcoTomasone), [Luca Genova](https://github.com/Lucajett99) and [Simone Boldrini](https://github.com/minosse99). The goal of the project is to write a kernel which can run on both uMPS and uARM architechture, and it was divided in 3 main phases.

## **What is**
BiKayaOS is an educational OS, created by Renzo Davoli, that comes directly from KayaOS. It's a simple kernel that uses a round robin scheduler with an aging priority mechanism to prevent starvation of low priority process, it has an API for both Active Semaphores List and Process Control Block manipulation, as well as low level system call implementation. For now there are as well API for simulated terminal and printer devices interaction. There's excepion handling support with the possibility to set custom handler for syscall, breakpoint, TLB, and trap giving the possibility to implement for example custom syscall for specific processes. At the moment it doesn't support virtual memory or more advanced OS mechanism.

## **Usage & Compilation**

**Disclaimer:** this is not to be meant anything near a real OS, nor is supposed to run on real hardware Indeed is a fun project to learn new concept and nothing more.  

In order to use effectively this project you both need the [uARM](http://mellotanica.github.io/uARM/) and [uMPS](https://github.com/tjonjic/umps) simulator and all their dependencies (cross compilers, linkers, etc). You can find all the information needed at the links above. We have currently two methods to build the project:

### CMake
Create and enter in the build-uMPS directory and type:

```console
usr@computer:~/BiKayaOS/build-uMPS$ cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/uMPS.cmake ..
usr@computer:~/BiKayaOS/build-uMPS$ make all 
```
and the same process for uARM:

```console
usr@computer:~/BiKayaOS/build-uARM$ cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/uARM.cmake ..
usr@computer:~/BiKayaOS/build-uARM$ make all 
```

**NOTE:** In our case, for uMPS CMake compiling we had to install/configure a [crosstool-ng](https://crosstool-ng.github.io/) toolchain, after installing the packet to build the toolchain simply type:

```console
usr@computer:~$ mkdir ~/toolchain && cd ~/toolchain
usr@computer:~$ ct-ng mipsel-unknown-linux-gnu
usr@computer:~$ ct-ng build (This will take a while)
usr@computer:~$ mv ~/xtools ~/toolchain
```

This can happen for both uARM and uMPS.
In this case you have to change the ~/BiKayaOS/toolchains/*.cmake at line 7-9 with the path of the toolchains you have created.
In both cases the compiler will eventually signal error/warning. Please note that to recompile a project you have to use "make clean" before make all.

After that remember to set the kernel.core.uarm and the kernel.stab.uarm  (respectively kernel.*.umps) as Core and Symbol Table file in the respective simulators, then boot up the machines

## **Credits** 
Renzo Davoli - BiKayaOS and KayaOS creator/ideator  
Mattia Maldini, Renzo Davoli, others - mantainer of the test files for each phase  
Oscar Barreca - help and support with the CMake building system  

## **License**
This software is distributed under the GNU General Public License version 3, for further info please read the LICENSE file.