#include "../include/types_bikaya.h"
#include "../include/listx.h"
#include "../include/const.h"
#include "asl.h"

#define HIDDEN static;
struct list_head *semd_h; //sentinella asl
struct list_head *semdFree_h; //sentinella lista libera


semd_t* getSemd(int *key) {

    return(NULL);
}

void initASL() {}

int insertBlocked(int *key,pcb_t* p) {
    return(0);
}

pcb_t* removeBlocked(int *key) {
    return(NULL);
}

/*DESCRIZIONE: Rimuove il PCB puntato da p dalla coda del
semaforo su cui è bloccato (indicato da p->p_semKey). Se il
PCB non compare in tale coda, allora restituisce NULL
(condizione di errore). Altrimenti, restituisce p. Se la coda
dei processi bloccati per il semaforo diventa vuota,x
rimuove il descrittore corrispondente dalla ASL e lo
inserisce nella coda dei descrittori liberi (semdFree).*/

pcb_t* outBlocked(pcb_t *p) {
    struct semd_t* semd = getSemd(p->p_semkey);
    struct list_head* pos;
    int guardia=1;
    
    if(semd==NULL)
        return NULL;
    
    //NON SO SE È POSSIBILE CHE LA CHIAVE COMBACI MA NON SI TROVA ALL'INTERNO DELLA CODA, IN QUEL CASO BISOGNA FARE ANCHE IL CONTROLLO ALL'INTERNO DELLA CODA

    else{
        list_for_each(pos,&semd->s_procQ){  //scorro finchè non trovo il pcb che voglio eliminare
            if((pos->next==p) && (guardia=1)){
                pos=pos->next;
                list_del(pos);
                guardia=0;
                }
        }
    }

    if(list_empty(&semd->s_procQ)){ //controllo se posso spostare il semd da quelli attivi a quelli liberi
        list_del(semd); 
        list_add_tail(semd,semdFree_h);
    }

    return(pos);
}

/*DESCRIZIONE: Restituisce (senza rimuovere) il puntatore al
PCB che si trova in testa alla coda dei processi associata al
SEMD con chiave key. Ritorna NULL se il SEMD non
compare nella ASL oppure se compare ma la sua coda dei
processi è vuota.*/

pcb_t* headBlocked(int *key) {
    struct semd_t* semd = getSemd(key);
    struct list_head* pos;
    
    //controllo se semd appartiene agli ASL o se ha la coda vuota, non so se basta solo la prima codizione dato che se non appartiene agli ASL allora ha la coda sicuramente vuota
    if((semd==NULL) || (list_empty(&semd->s_procQ))) //non so se la sentinella ci sia a prescindere e quindi la conta come elemento della lista
        return NULL;
    else
    {
        return (container_of(list_next(&semd->s_procQ), struct pcb_t, p_next)); //list_next(semd->s_procQ) perchè il primo è l'elemento sentinella
    }
    //ritorna il puntatore ad una struttura contenete quel PCB


}

/*DESCRIZIONE: Rimuove il PCB
puntato da p dalla coda del semaforo
su cui è bloccato (indicato da
p->p_semKey). Inoltre, elimina tutti i
processi dell’albero radicato in p (ossia
tutti i processi che hanno come avo p)
dalle eventuali code dei semafori su
cui sono bloccati.*/

void outChildBlocked(pcb_t *p) {
    struct pcb_t *avo=outBlocked(p);
  
    if(list_empty(&avo->p_child)==FALSE){
        struct list_head *tmp=&avo->p_child;
        struct list_head *pos;

        for (pos=(tmp)->next; pos != tmp; pos=pos->next)  //l'avevo fatta con list_for_each(pos,&tmp) solo che mi dava errore e mi diceva di espanderla così
            outChildBlocked(container_of(pos,struct pcb_t, p_sib)); //richiamo la funzione per tutti i figli
    }
}
