# BiKayaOS
**A variation of the Kaya OS that can run on both uARM and uMPS architechtures, this is the project for the OS course during the academic year 2019/2020**

This project is written by [Enea Guidi](https://github.com/its-hmny), [Marco Tomasone](https://github.com/MarcoTomasone), [Luca Genova](https://github.com/Lucajett99) and [Simone Boldrini](https://github.com/minosse99). The goal of the project is to write a kernel which can run on both uMPS and uARM architechture, the project is divided in 3 main phases:

## **Compilation**
In order to use effectively this project you both need the [uARM](http://mellotanica.github.io/uARM/) and [uMPS](https://github.com/tjonjic/umps) simulator and all their dependencies (cross compilers, linkers, etc). You can find all the information needed at the links above. We have currently two methods to build the project:

### Makefile
Simply go to the project directory and type:
```console
usr@computer:~/BiKayaOS$ make uarm
usr@computer:~/BiKayaOS$ make umps
```
to build respectively the uARM version or the uMPS version, if everything has been set correctly the compilation should go fine.

### CMake
Create and enter in the build-umps directory and type:
```console
usr@computer:~/BiKayaOS/build-umps$ cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/umps.cmake ..
usr@computer:~/BiKayaOS/build-umps$ make all 
```
and the same process for uARM:
```console
usr@computer:~/BiKayaOS/build-uarm$ cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/uarm.cmake ..
usr@computer:~/BiKayaOS/build-uarm$ make all 
```

**NOTE:** In our case, for uMPS CMake compiling we had to install/configure a [crosstool-ng](https://crosstool-ng.github.io/) toolchain, after installing the packet to build the toolchain simply type:
```console
usr@computer:~$ mkdir ~/toolchain && cd ~/toolchain
usr@computer:~$ ct-ng mipsel-unknown-linux-gnu
// This will take a while
usr@computer:~$ ct-ng build
usr@computer:~$ mv ~/xtools ~/toolchain
```
This can happen for both uARM and uMPS.
In this case you have to change the ~/BiKayaOS/toolchains/*.cmake at line 7-9 with the path of the toolchains you have created.
In both cases the compiler will eventually signal error/warning. Please note that to recompile a project you have to use "make clean" before make all.

After that remember to set the kernel.core.uarm and the kernel.stab.uarm  (respectively kernel.*.umps) as Core and Symbol Table file in the respective simulators, then boot up the machines

## **License**
This software is distributed under the GNU General Public License version 3, for further info please read the LICENSE file.