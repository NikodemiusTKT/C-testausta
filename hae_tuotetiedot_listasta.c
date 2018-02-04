#include "tietorakenteet.h"

/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  hae_tuotetiedot_listasta
 *       Kuvaus:  Funktio hakee tuotelistasta tuotetiedot haetulla tuotenumerolla. Funktio
 *                palauttaa osoittimen haettuun tuotetietoon. Parametreinaan funktio
 *                tarvitsee osoittimen tuotetietojen listan alkuun sekä haettavan tuotteen
 *                tuotenumeron.
 * =====================================================================================
 */
tuotetiedot* hae_tuotetiedot_listasta(tuotetiedot *pAlku_tuotteet,int id) {
    tuotetiedot *ptr = pAlku_tuotteet;
    /* Haetaan tuotetietoja listasta niin kauan kunnes kaikki listan alkiot ollaan käyty läpi tai
    jos halutun tuotteen tuotetiedot löydetään. */
    while (ptr != NULL) {
        /* Jos tuotelistan tuotteen tuotenumero vastaa haettua tuotenumeroa voidaan se palauttaa
        ja funktion suoritus lopettaa. */
        if (id == ptr->id) {
            return ptr;
        }
        /* Siirrytään käsittelemään seuraavaa listan alkiota */
        ptr = ptr->seuraava;
    }
    /* Jos tuotetietoja ei löydetä, niin palautetaan tyhjä osoitin */
    return ptr;
}

