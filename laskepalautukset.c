#include "tietorakenteet.h"

/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  laskepalautukset
 *       Kuvaus:  Funktio laskee kaikkien palautuslistalla olevien tuotteiden määrän ja 
 *                palauttaa lasketun kokonaisluvun.Parametreinaan funktio tarvisee osoittimen
 *                palautuslistan alkuun.
 * =====================================================================================
 */
int laskepalautukset(palautustiedot *pAlku_palautukset) {
    palautustiedot *ptr = pAlku_palautukset;
    /* Alustetaan palautusten määrän arvoon 0 */
    int palautukset = 0;
    /* Lasketaan palautusten määrää kunnes kaikki listan alkiot on käyty läpi */
    while (ptr != NULL) {
        /* Lisätään palautusten määrää jokaisesta ei tyhjästä listan alkiosta */
        palautukset++;
        /* Siirrytään seuraavaan listan alkioon */
        ptr = ptr->seuraava;
    }
    /* Palautetaan laskettujen palautusten määrä */
    return palautukset;
}
