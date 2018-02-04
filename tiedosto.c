
/*******************************************************************/
/* CT60A0220 C-ohjelmoinnin ja testauksen perusteet 
 * Otsikkotiedot: C-ohjelmoinnin harjoitustyö
 * Tekijät: Teemu Tanninen 0508505
 *          Joonas Hakkarainen 0438374
 *          Henrik Peteri 0452868
 * Päivämäärä: 12.03.17
 * Yhteistyö ja lähteet, nimi ja yhteistyön muoto:
 * C-kieli ohjelmointiopas, Kasurinen & Nikula 2012
 */
/*******************************************************************/


#include "tietorakenteet.h"
#include "apufunktiot.h" 
#include "tiedosto.h"
#include "laskepalautukset.h"
#include "laske_pantit.h"
#include "hae_tuotetiedot_listasta.h"
#include "laske_tuotteen_palautukset.h"

/*******************************************************************/

/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  lue_tiedosto
 *       Kuvaus:  Funktio lukee kansiossa olevan tuotetiedot sisältävän tekstitiedoston rivit
 *                ja lisää oikein luetut tuotetiedot tuotelistaan. Onnistumisesta palautetaan
 *                arvo 0 ja epäonnistumisesta arvo 1. Parametreina funktio tarvitsee osoitteen
 *                tuotelistan alkuun ja luettavan tiedoston nimen.
 * =====================================================================================
 */
int lue_tiedosto(tuotetiedot **pAlku_tuotteet, char* tuotetiedosto) {
    FILE *tiedosto;
    int id = 1, rivi = 0;                       /* Alustetaan rivi arvoon 0. */
    float koko, pantti;
    char tyyppi[CHAR_MAX];
    /* Puskuri tiedoston rivien lukemiseen */
    char puskuri[CHAR_MAX];
    tiedosto = fopen(tuotetiedosto,"r");
    if (tiedosto != NULL) {
        /* Luetaan tiedoston rivit puskuriin */
        while (fgets(puskuri, sizeof(puskuri), tiedosto)) {
            /* Kasvatetaan rivinumeron arvoa. */
            rivi ++;
            puskuri[strcspn(puskuri, "\n")] = 0; /* Poistetaan rivinvaihtomerkki puskurin merkkijonon lopusta. */
            /* Tarkistetaan, että luettu rivi sisältää halutut kolme tietoa. */
            if (sscanf(puskuri,"%s %f %f", tyyppi,&koko,&pantti) == 3) {
                /* Lisätään onnistuneesti luettu tuote tuotelistaan. */
                lisaa_tuote(pAlku_tuotteet,id,tyyppi,koko,pantti);
                /* Kasvatetaan tuotenumeron arvoa. */
                id++;
            }
            /* Annetaan tarvittaessa virheilmoitus väärin luetuista riveistä. */
            else {
                fprintf(stderr, "Tiedoston rivi %d: '%s' on vääränlainen.\n", rivi,puskuri);
            }
        }
    }
    /* Annetaan virheilmoitus, jos tiedoston luku epäonnistui ja palautetaan luku 1. */
    else {
        fprintf(stderr, "Tiedoston lukeminen epäonnistui.\n");
        return 1;
    }
    fclose(tiedosto);
    /* Annetaan virheilmoitus ja palautetaan arvo 1, jos tuotelista on vielä tyhjä tiedoston lukemisen jälkeen. */
    if (*pAlku_tuotteet == NULL) {
        perror("Tuotetietojen lisäys listalle epäonnistui.\n");
        return 1;
    }
    /* Palautetaan arvo 0 onnistuneesta tuotetietojen lukemisesta ja niiden lisäämisestä tuotelistaan. */
    return 0;

}



/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  kirjoita_lokitiedosto
 *       Kuvaus:  Funktio kirjoittaa tehtyjen palautusten tiedot kansiossa olevaan
 *                lokitiedosto tekstitiedostoon. Funktio palauttaa onnistumisesta arvon 0
 *                ja epäonnistumisesta arvon 1. Parametreinaan funktio tarvitsee osoittimet
 *                tuotelistan ja palautuslistan alkuun, valikon valintojen määrän luvun
 *                sekä kirjoitettavan tiedoston nimen.
 * =====================================================================================
 */
int kirjoita_lokitiedosto(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet, char* l_nimi) {
	FILE* tiedosto;
	/* Avataan tiedosto kirjoittamista varten, jos avaus epäonnistuu tulostetaan virheilmoitus ja palautetaan arvo 1. */
	if ((tiedosto = fopen(l_nimi, "a")) == NULL) {
		fprintf(stderr, "Tiedoston avaaminen epäonnistui!");
		return 1;
	}
	/* Kirjoittaa lokitiedostoon tiedot, jos pulloja on palautettu*/
	if (pAlku_tuotteet != NULL && pAlku_palautukset != NULL) {
	    int palautukset = laskepalautukset(pAlku_palautukset);
        char *aika = aikaleima();
		fprintf(tiedosto, "%s - Palautukset %d kpl. Pantit %.2f€.\n",  aika, palautukset, laske_pantit(pAlku_palautukset, pAlku_tuotteet));
	    free(aika);
	}
    else {
        /* fprintf(stderr, "Lokitiedostoa ei voida kirjoittaa koska %s.\n", (pAlku_tuotteet!=NULL&&pAlku_palautukset==NULL) ? "palautuslista on tyhjä" : "molemmat listat ovat tyhjiä"); */
        if(pAlku_tuotteet==NULL && pAlku_palautukset != NULL) fprintf(stderr,"Lokitiedostoa ei voida kirjoittaa koska tuotelista on tyhjä.\n");
        else if(pAlku_tuotteet!=NULL && pAlku_palautukset == NULL) fprintf(stderr,"Lokitiedostoa ei voida kirjoittaa koska palautuslista on tyhjä.\n");
        else fprintf(stderr,"Lokitiedostoa ei voida kirjoittaa koska molemmat listat ovat tyhjiä.\n");
        return 1;
    }
	fclose(tiedosto);
	return 0;
}



/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  kirjoita_valiaikainen
 *       Kuvaus:  Funktio kirjoittaa väliaikaiset palautustiedot kansion tilapaistiedostoon.
 *                Kansiossa oleva tekstitiedosto kirjoitetaan uudelleen joka kerta funktiota
 *                käytettäessä. Funktio palauttaa onnistumisesta arvon 0 ja epäonnistumisesta
 *                arvon 1. Parametreina funktio tarvitsee osoittimet tuotelistan ja palautuslistan
 *                alkuun sekä kirjoitettavan tiedoston nimi.
 * =====================================================================================
 */

int kirjoita_valiaikainen(tuotetiedot *pAlku_tuotteet, palautustiedot *pAlku_palautukset, char* v_nimi) {
    FILE *v_tiedosto;
    /* Tiedoston avaamisen epäonnistuessa tulostetaan virheilmoitus ja palautetaan arvo 1. */
	if ((v_tiedosto = fopen(v_nimi, "w")) == NULL) {
		fprintf(stderr, "Tiedoston avaaminen epäonnistui!");
		return 1;
	}
    /* Asetetaan väliaikainen osoitin palautuslistan alkuun. */
    /* Tarkistetaan, että kumpikaan listoista ei ole tyhjä. */
    if (pAlku_tuotteet != NULL && pAlku_palautukset != NULL) {
        palautustiedot *ptr = pAlku_palautukset;
        fputs("Tilapäinen lokitiedosto\n\n\n", v_tiedosto);
        /* Käydään kaikki palautuslistan tuotteet läpi. */
        while (ptr != NULL) {
            /* Haetaan palautuslistan tuotteen tiedot tuotelistasta */
            tuotetiedot *tiedot = hae_tuotetiedot_listasta(pAlku_tuotteet,ptr->id);
            if (tiedot) {
            char *aika = aikaleima();
            /* Luodaan nykyisen hetken aikaleima. */
            /* Kirjoitetaan halutut tiedot muotoiltuna tekstitiedostoon. */
            fprintf(v_tiedosto,"%s:%s-%.2fl:%.2f€.\n", aika,tiedot->tyyppi,tiedot->koko,tiedot->pantti);
            free(aika);
            }
            /* Siirrytään seuraavaan palautuslistan tuotteeseen. */
            ptr = ptr->seuraava;
            /* Vapautetaan aina aikaleiman varaama muisti. */
        }
    }
    /* Tulostetetaan tarvittaessa virheilmoitus ja palautetetaan arvo 1, mikäli tuotelista ja/tai palautuslista ovat tyhjiä. */
    else {
        fprintf(stderr, "Väliaikaista lokitiedostoa ei voida kirjoittaa koska %s.\n", (pAlku_tuotteet!=NULL&&pAlku_palautukset==NULL) ? "palautuslista on tyhjä" : "molemmat listat ovat tyhjiä");
        return 1;
    }
    /* Suljetetaan tiedosto. */
    fclose(v_tiedosto);
    /* Palautetetaan arvo 0 onnistuneesta tietojen kirjoittamisesta */
    return 0;
}
