# Hello world configurabile per UARM/UMPS

Questa repository contiene un esempio di programma compilabile sia per l'emulatore uMPS2 (https://github.com/tjonjic/umps) che uARM (https://github.com/mellotanica/uARM). 
I due emulatori offrono librerie ROM e dispositivi mappati in memoria molto simili, per cui ottenere un risultato cross-platform e' relativamente semplice. Tramite delle macro `#ifdef` si includono gli header delle rispettive routine ROM e gli indirizzi dei dispositivi (in questo caso, solo il terminale).

A scopo di esempio sono implementati almeno due possibili metodi per la configurazione dell'architettura: make e scons.

## Requisiti

Perche' la compilazione vada a buon fine sono necessari i seguenti pacchetti:

- arm-none-eabi-gcc
- mipsel-linux-gnu-gcc
- uarm (per la compilazione su uarm)
- umps (per la compilazione su umps)
- make (per utilizzare i makefile)
- python-scons (per utilizzare SConstruct)
- python-kconfiglib (per utilizzare SConstruct)

## Make

Molto semplicemente vengono forniti due makefile separati per la compilazione, `uarmmake` e `umpsmake`. Invocando `make` sul file corrispondente si compila l'esempio per l'emulatore richiesto:

```
$ make -f uarmmake
$ make -f umpsmake
```

Dietro le quinte le differenze tra i due makefile sono:

 - utilizzo di un compilatore e di flag di compilazione appropriati
 - compilazione di diverse librerie di base
 - inclusione di diversi header
 - definizione delle macro `TARGET_UMPS` o `TARGET_UARM` per ottenere un comportamento diverso (in questo semplice esempio la cosa si riduce all'includere degli header diversi)

## Esecuzione

Per l'esecuzione dell'esempio fare riferimento ai manuali di uARM e uMPS2, rispettivamente.
Serve innanzitutto aprire i due simulatori, se la compilazione è andata a buon fine allora si dovrebbero trovare nella directory principale 4 nuovi file due di Symbol Table (.stab.uarm, .stab.umps) e due di core (.core.uarm, .core.umps) dal simulatore si devono poi impostare i due path nei simulatori e avviare le macchine