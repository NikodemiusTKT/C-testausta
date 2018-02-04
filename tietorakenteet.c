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
#include "laskepalautukset.h"
#include "laske_tuotteen_palautukset.h" 
#include "hae_tuotetiedot_listasta.h"
#include "laske_pantit.h"
/*******************************************************************/


/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  lisaa_tuote
 *       Kuvaus:  Funktio lisää eli luo uuden alkion tuotelistan loppuun ja tallettaa
 *                tuotteen tuotetiedot listan alkion tietoihin.Funktio
 *                palauttaa arvon 0 onnistuessaan ja arvon 1 epäonnistuessa. Parametreinaan
 *                funktio tarvitsee osoitteen tuotelistan alkuun sekä talletettavat tuotetiedot 
 *                tuotenumeron, tyypin, koon sekä pantin.
 * =====================================================================================
 */
int lisaa_tuote(tuotetiedot **pAlku_tuotteet, int id, char* tyyppi, float koko, float pantti) {
    tuotetiedot *ptr;
    /* Luodaan uusi alkio tuotetiedoille ja varataan sille tarvittava määrä muistia. */
    tuotetiedot *uusi = (tuotetiedot *)malloc(sizeof(tuotetiedot));
    /* Annetaan tarvittaessa virheilmoitus epäonnistuneestä alkion muistinvarauksesta ja palautetaan arvo 1 */
    if (uusi == NULL) {
        perror("Muistin varaus tuotetietojen listalle epäonnistui.\n");
        return 1;
    }
    /* Varataan muistia merkkijonolle pullon tyypin pituuden verran ja lisätään pituuteen yksi lopetusmerkin tallennusta varten. */
    char *nimi = (char *)malloc(sizeof(char)*(strlen(tyyppi)+1)); /*  */
    /* Tarvittaessa annetaan virheilmoitus ja palautetaan arvo 1, jos muistinvaraus merkkijonolle epäonnistui. */
    if (nimi == NULL) {
        perror("Muistin varaus tuotelistan merkkijonolle epäonnistui.\n");
        return 1;
    }
    /* Kopioidaan pullon tyyppi dynaamisesti varattuun nimi merkkijonon alkioon */
   strcpy(nimi,tyyppi); 
   /* Tallennetaan tarvittavat tuotetiedot uuden listan alkion tietoihin */
   uusi->id = id;
   uusi->tyyppi = nimi;
   uusi->koko = koko;
   uusi->pantti = pantti;
   /* Asetetaan uuden alkion seuraava alkio tyhjään alkioon */
   uusi->seuraava = NULL;
   /* Jos tuotelista on tyhjä asetetaan luotu uusi alkio listan ensimmäiseksi alkioksi. */
    if ((*pAlku_tuotteet) == NULL) {
        (*pAlku_tuotteet) = uusi;
    }
    /* Ei tyhjässä tuotelistassa uusi alkio laitetaan listan viimeiseksi alkioksi. */
    else {
        /* Asetetaan väliaikainen osoitin listan alkuun. */
        ptr = (*pAlku_tuotteet);
        /* Etsitään listan viimeinen alkio */
        while (ptr->seuraava != NULL) {
            ptr = ptr->seuraava;
        }
        /* Asetetaan listan viimeisen alkion seuraavaksi alkioksi uusi luotu alkio. */
        ptr->seuraava = uusi;
    }
    /* Palautetaan arvo 0 onnistuneesta tuotteen lisäämisestä tuotelistaan. */
    return 0;
}



/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  tulostus
 *       Kuvaus:  Funktio tulostaa tuotetiedot listasta valikkoa varten sekä
 *                samalla laskee tuotetietojen määrää valinnan vertailua varten. 
 *                Parametreina funktio tarvitsee osoittimen tuotelistan alkuun.
 * =====================================================================================
 */
int tulostus(tuotetiedot *pAlku_tuotteet) {
    /* Alustetaan laskurin arvo arvoon 1, koska ensimmäinen valinta tarvitaan valikosta poistumiseen. */
    int laskuri = 1;
    /* Tarkistetaan, että tuotelista ei ole tyhjä */
    if (pAlku_tuotteet != NULL) {
        /* Asetetaan osoitin listan alkuun */
        tuotetiedot *ptr = pAlku_tuotteet;
        /* Tulostetaan tuotetiedot valikkoon ja lasketaan tuotteiden määrää niin kauan kunnes koko lista on käyty läpi. */
        while (ptr != NULL) {
            printf("%d) %s %.2fl \n", (ptr->id),ptr->tyyppi,ptr->koko);
            /* Siirrytään seuraavaan tuotelistan alkioon */
            ptr = ptr->seuraava;
            /* Kasvatetaan laskurin arvoa jokaisessa käsitellyssä listan alkiossa */
            laskuri++;
        }
        /* Palautetaan laskurin arvo (tuotteiden määrä + 1); */
        return laskuri;
    }
    /* Annetaan virheilmoitus, jos tuotelista on tyhjä. */
    fprintf(stderr,"Tuotetietojen lista on tyhjä!\n");
    return laskuri;
}


/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  lisaa_palautus
 *       Kuvaus:  Funktio lisää eli luo uuden alkion palautuslistan loppuun ja tallettaa
 *                palautettavan tuotteen tuotenumeron listan alkion tietoihin.Funktio
 *                palauttaa arvon 0 onnistuessaan ja arvon 1 epäonnistuessa. Parametreinaan
 *                funktio tarvitsee osoitteen palautuslistan alkuun ja osoittimen tuotelistan
 *                alkuun sekä talletettavan tuotenumeron.
 * =====================================================================================
 */
int lisaa_palautus(palautustiedot **pAlku_palautukset,tuotetiedot *pAlku_tuotteet, int id) {
    palautustiedot *ptr;
    /* Luodaan uusi listan alkio ja varataan sille dynaamisesti tarvittava määrä muistia. */
    if (pAlku_tuotteet == NULL)
        return 1;
    palautustiedot *uusi = (palautustiedot *)malloc(sizeof(palautustiedot));
    /* Annetaan käyttäjälle virheilmoitus, jos muistinvaraus uudelle alkiolle epäonnistui ja palautetaan arvo 1. */
    if (uusi == NULL) {
        perror("Muistin varaus palautuslistalle epäonnistui.\n");
        return 1;
    }
    /* Tallennetaan tuotenumero listan alkion tietohin ja asetetaan seuraavan alkion osoitin osoittamaan tyhjään alkioon. */
   uusi->id = id;
   uusi->seuraava = NULL;
   /* Jos palautuslista on tyhjä asetetaan uusi alkio listan ensimmäiseksi alkioksi */
    if ((*pAlku_palautukset) == NULL) {
        (*pAlku_palautukset) = uusi;
    }
    /* Jos lista ei ole tyhjä, niin asetetaan uusi alkio listan viimeiseksi alkioksi */
    else {
        /* Asetetaan väliaikainen osoitin listan alkuun */
        ptr = (*pAlku_palautukset);
        /* Etsitään listan viimeinen alkio */
        while (ptr->seuraava != NULL) {
            ptr = ptr->seuraava;
        }
        /* Asetetaan listan viimeisen alkion seuraavaksi alkioksi uusi luotu alkio */
        ptr->seuraava = uusi;
    }
    
    tuotetiedot *tiedot = hae_tuotetiedot_listasta(pAlku_tuotteet,uusi->id);
    printf("Syötetty: %s %.2fl\n", tiedot->tyyppi,tiedot->koko);
    /* Palautetaan arvo 0 onnistuneesta palautuksen lisäämisestä listaan */
    return 0;
}







/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  tulosta_kuitti
 *       Kuvaus:  Funktio tulostaa palautettujen tuotteiden palautuskuitin.
 *                Funktio palauttaa onnistuessaan arvon 0 ja epäonnistuessa arvon 1.
 *                Parametreina funktio tarvitsee osoittimet palautuslista sekä tuotelista
 *                alkuun ja valikossa olevien valintojen määrän eli luvun.
 * =====================================================================================
 */
int tulosta_kuitti(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet, int luku) {
    /* Kuittia ei tulosteta, jos tuotelista tai palautuslista on tyhjä */
    if (pAlku_tuotteet != NULL && pAlku_palautukset != NULL) {
        /* Lasketaan kaikkien palautuksien määrä erillisellä funktiolla. */
        int palautukset = laskepalautukset(pAlku_palautukset);
        /* Muodostetaan nykyisen järjestelmän aikaleima ulkoisessa funktiossa ja talletetaan se aika merkkijono muuttujaan. */
        char *aika = aikaleima();
        printf("\nKuitti %s\n", aika);
        /* Aikaleiman varaaman muistin vapautus. */
        free(aika);
        /* Tulostetaan kaikkien palautettujen pullojen ja tölkkien määrä. */
        printf("\nPalautetut pullot ja tölkit yhteensä %d kappaletta.\n\n", palautukset);
        /* Käydään iteratiivisesti kaikki luvut läpi ykkösestä niin pitkälle kuin tuotetietoja luettiin tiedostosta. */
        for (int i = 1; i < luku; i++) { 
            /* Lasketaan jokaisen yksittäisen tuotteen palautusten määrä palautuslistasta ulkoisessa funktiossa. */
            int maara = laske_tuotteen_palautukset(pAlku_palautukset,i);
            /* Jos kyseistä tuotetta ollaan palautettu, voidaan sen tiedot hakea tuotelistasta ja tiedot voidaan tulostaa käyttäjälle */
            if (maara > 0) {
                /* haetaan tuotetiedot tuotelistasta */
                tuotetiedot *ptr = hae_tuotetiedot_listasta(pAlku_tuotteet,i);
                /* tulostetaan palautustiedot käyttäjälle */
                printf("%s %.2fl\tpantit %d x %.2f = %.2f€\n", ptr->tyyppi,ptr->koko,maara,ptr->pantti,ptr->pantti*maara);
            }
        }
        /* Tulostetaan lopuksi panttien yhteissumma, joka lasketaan ulkoisessa funktiossa. */
        printf("\n\nPantit yhteensä %.2f€\n\n\n", laske_pantit(pAlku_palautukset,pAlku_tuotteet));
        /* Palautetaan 0 onnistumisesta */
        return 0;
    }
    else {
        /* Jos tuotelista ja palautuslista ovat tyhjiä, tulostetaan käyttäjälle ehdollinen virheilmoitus siitä mitkä listoista olivat tyhjiä */
        fprintf(stderr, "Kuittia ei voida tulostaa koska %s.\n", (pAlku_tuotteet!=NULL&&pAlku_palautukset==NULL) ? "palautuslista on tyhjä" : "molemmat listat ovat tyhjiä");
        /* Epäonnistumisesta palautetaan 0 */
        return 1;
    }
}


/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  tyhjenna_palautukset
 *       Kuvaus:  Funktio tyhjentaa palautuslistan eli vapauttaa sen varaaman muistin.
 *                Funktio palauttaa onnistuessaan luvun 0 ja luvun 1 epäonnistuessa. 
 *                Parametreinaan funktio tarvitsee osoitteen palautuslistan alkuun.
 * =====================================================================================
 */
int tyhjenna_palautukset(palautustiedot **pAlku_palautukset) {
    if (*pAlku_palautukset != NULL) {
        palautustiedot *ptr = *pAlku_palautukset;
        while (ptr != NULL) {
            /* Asetetaan ensimmäistä alkiota seuraava alkio listan uudeksi ensimmäiseksi alkioksi. */
            *pAlku_palautukset = (*pAlku_palautukset)->seuraava;
            /* Vapautetaan listan alkion varaama muisti */
            free(ptr);
            /* Siirrytään seuraavan palautuslistan alkioon */
            ptr = *pAlku_palautukset;
        }
        /* Palautetaan arvo 0 onnistuneesta palautuslistan tyhjennyksestä */
        return 0;
    }
    return 1;
}

/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  tyhjenna_tuotteet
 *       Kuvaus:  Funktio tyhjentaa tuotetietojen listan eli vapauttaa sen varaaman muistin.
 *                Paremetreina funktio tarvitsee osoitteen tuotetietojen listan alkuun.
 *                Funktio palauttaa onnistuessaan luvun 0 ja luvun 1 epäonnistuessa. 
 * =====================================================================================
 */

int tyhjenna_tuotteet(tuotetiedot **pAlku_tuotteet) {
    /* Asetetaan väliaikainen osoitin listan alkuun */
    tuotetiedot *ptr = *pAlku_tuotteet;
    /* Tarkistetaan, että tyhjennettävä lista ei ole jo tyhjä */
    if (*pAlku_tuotteet != NULL) {
        while (ptr != NULL) {
            /* Asetetaan ensimmäistä alkiota seuraava alkio listan uudeksi ensimmäiseksi alkioksi. */
            *pAlku_tuotteet = (*pAlku_tuotteet)->seuraava;
            /* Vapautetetaan ensin merkkijonon varaama muisti ja sitten vapautetaan koko alkion varaama muisti. */
            free(ptr->tyyppi);
            free(ptr);
            /* Siirrytään seuraavaan listan alkioon. */
            ptr = *pAlku_tuotteet;
        }
        return 0;                               /* Palautetaan 0 onnistuessa */
    }
    return 1;                                   /* Palautetaan 1 epäonnistuessa */
}


