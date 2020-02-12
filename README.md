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
The goal of this phase is to implement the basic data structure of the level above (the kernel itself), given the **pcb_t** (Process Control Block) and the **semd_t** (Semaphor's descriptor) data type we had to implement algorithms for these data type.

Specifically we had to manage:
1) The allocation and deallocation of the PCB
2) The PCB priority queue
3) The PCB tree 
4) The allocation and deallocation of Semaphor
5) The insert and remove of blocked PCBs on a given semaphor

### Implementation
To do the thing mentioned above we used the **listx.h** file taken from the Linux kernel and used the macros and functions of it to achieve what requested. The priority queue is in fact a double-linked list with a dummy in wich the PCBs (through the **p_next** field) are inserted maintaining the sorting by priority property of a priority queue. We utilized another another list_head struct (**p_child** and **p_sib**) to manage the tree/gerarchical aspect of the PCBs, specifially in a given PCB the **p_child** field is the dummy of the PCB's child list while the **p_sib** field is used to link in the child list PCBs with the same father.

For the semaphor descriptor we adopted a proess very similar to the above descripted: we used the list_head **s_procQ** as dummy for the list of PCBs blocked on that given semaphor and we used the **s_next** field to link between them the active semaphors in the ASL (Active Semaphors List). The semd are distinguished between them through their **s_key** field that makes them and their blocked PCBs unique. The final result of this is very similar to a simple HashTable.

Further information are provided at the beggining of each function (requested argument, return type, explanation of the algorithms, etc.).

## **License**
This software is distributed under the GNU General Public License version 3, for further info please read the LICENSE file.