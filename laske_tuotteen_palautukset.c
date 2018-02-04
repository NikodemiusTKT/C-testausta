#include "tietorakenteet.h"
/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  laske_tuotteen_palautukset
 *       Kuvaus:  Funktio laskee yksittäisen tuotteen palautusten määrän palautuslistasta
 *                ja palauttaa lasketun kokonaisluvun. Funktio tarvitsee parametreinaan
 *                osoittimen palautuslistan alkuun sekä palautetun tuotteen id numeron.
 * =====================================================================================
 */
int laske_tuotteen_palautukset(palautustiedot *pAlku_palautukset, int id) {
    /* Alustetaan palautusten määrä arvoon 0. */
    int maara = 0;
    /* Väliaikainen osoitin asetetaan palautuslistan alkuun */
    palautustiedot *ptr = pAlku_palautukset;
    /* Palautuksien määrä lasketaan niin kauan kunnes kaikki listan alkiot on käyty läpi */
    while (ptr != NULL) {
        /* Lisätään palautusten määrää, jos palautuslistan tuotteen id numero vastaa haettua id numeroa */
        if (ptr->id == id) {
            maara += 1;
        }
        /* Siirrytään käsittelemään seuraavaa listan alkiota */
        ptr = ptr->seuraava;
    }
    /* Palautetaan laskettujen tuotepalautusten määrä */
    return maara;
}

