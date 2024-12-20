# SO Monitor
## Proiect la sisteme de operare, FMI an 2, sem 1.

*Echipa (toti de la 252): Pescariu Matei-Alexandru, Popescu Vladut-Daniel si Duzi Mihai-Nicolae.*

## Cerinta:
>8. Monitor (2–3 studenti) – Implementati un obiect de sincronizare de tip monitor. Folositi
>variabile conditionale pentru a permite mai multor procese sa coexiste in interiorul monitorului la un moment dat.

## Explicatie ce e un monitor:

Un monitor este o metoda abstracta de sincronizare high-level. Are variabile share-uite, functii care le accesseaza si variabile de legatura care blockeaza executia pana sunt indeplinite.

Monitoarele garanteaza excluziunea mutuala pe variabilele share-uite (cu un mutex comun). Variabile share-uite detin informatie. Variabilele conditionale nu detin informatie, ci doar daca o conditie a fost indeplinita. Astfel, ele pot fi accesate si din exteriorul monitorului.

Pentru sincronizare exista solutiile low-level de mutexi si semafoare. Problema cu mutexii si semafoarele este ca este foarte usor sa se strecoare o eroare, daca uitam sa dam lock/unlock unde trebuie. Astfel, monitorul este un wrapper pentru mutexi si semafoare care simplifica codarea.

## Organizarea echipei:

-  Pescariu Matei-Alexandru:
    - A scris acest README.md
    - A programat structura, initializarea monitorului

- Popescu Vladut-Daniel:
    - A codat functiile de utilizare ale monitorului

- Duzi Mihai-Nicolae:
    - A facut exemplul de folosire al monitorului