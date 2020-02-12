# BiKayaOS
**A variation of the Kaya OS that can run on both uARM and uMPS architechtures, this is the project for the OS course during the academic year 2019/2020**

This project is written by [Enea Guidi](https://github.com/its-hmny), [Marco Tomasone](https://github.com/MarcoTomasone), [Luca Genova](https://github.com/Lucajett99) and [Simone Boldrini](https://github.com/minosse99). The goal of the project is to write a kernel which can run on both uMPS and uARM architechture, the project is divided in 3 main phases:

## **Compilation**
In order to use effectively this project you both need the [uARM](http://mellotanica.github.io/uARM/) and [uMPS](https://github.com/tjonjic/umps) simulator and all their dependencies (cross compilers, linkers, etc). You can find all the information needed at the links above. To build the project just go to the project main directory and type:

```console
usr@computer:~/BiKayaOS$ make uarm
usr@computer:~/BiKayaOS$ make umps
```
to build respectively the uARM version or the uMPS version, if everything has been set correctly the compilation should go fine. After that remember to set the kernel.core.uarm and the kernel.stab.uarm  (respectively kernel.*.umps) as Core and Symbol Table file in the respective simulators, then boot up the machines

## **Phase 1**

### Goal

### Implementation

## **License**
This software is distributed under the GNU General Public License version 3, for further info please read the LICENSE file.