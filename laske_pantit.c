#include "tietorakenteet.h"
/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  laske_pantit
 *       Kuvaus:  Funktio laskee kaikkien palautuslistassa olevien tuotteiden yhteispantin
 *                ja palauttaa sen liukulukuna. Funktio tarvitsee parametreinaan osoittimet
 *                palautuslistan ja tuotelistan alkuun.
 * =====================================================================================
 */
float laske_pantit(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet) {
    float pantit=0;

    /* Luodaan väliaikainen ptr osoitin ja asetetaan se palautuslistan alkuun */

    if (pAlku_palautukset == NULL || pAlku_tuotteet == NULL)
        return 0;
    palautustiedot *ptr = pAlku_palautukset;

    /* Lasketaan pantteja niin kauan kunnes käsiteltävä alkio ei ole tyhjä */

    while (ptr != NULL) {

        /* Haetaan palautuslistan tuotteen tuotetiedot tuotelistasta */

        tuotetiedot *tiedot = hae_tuotetiedot_listasta(pAlku_tuotteet,ptr->id);
        
        /* Lisätään aikaisempaan panttiin haetun tuotteen pantti */

        pantit +=  tiedot->pantti;

        /* Siirrytään seuraavaan listan alkioon */

        ptr = ptr->seuraava;
    }
    /* Palautetaan pantit */

    return pantit;
}

