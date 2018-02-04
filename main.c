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

/*******************************************************************/


/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  main
 *       Kuvaus:  Funktio tulostaa käyttäjälle valikon ja kutsuu muita
 *                moduuleissa olevia funktioita.
 * =====================================================================================
 */
int main(void) {
    /* Alustetaan tuote- ja palautuslistat tyhjiksi. */
    tuotetiedot *pAlku_tuotteet = NULL;
    palautustiedot *pAlku_palautukset = NULL;
    /* Valinta muuttujat valikoille 1 ja 2 */
    int valinta1, valinta2;
    /* Puskuri syötteelle */
	char syote[255];
    char *tuotetiedosto = "tuotetiedosto.txt", *l_nimi = "lokitiedosto.txt", *v_nimi = "tilapaistiedosto.txt";
    printf("\n");
    printf("PULLONPALAUTUSAUTOMAATTI\n");
    /* Tulostetaan ensimmäinen valikko */
    while (1) {
        printf("\n");
        printf("Automaatti on valmis ottamaan vastaan pullot ja tölkit.\n");
        printf("\n");
        printf("1) Aloita palautus\n");
        printf("0) Lopeta\n");
        printf("\n");
        printf("Valitse: ");
        /* Luetaan käyttäjän syöte puskuriin. */
        fgets(syote,254,stdin);
        /* Tarkistetaan syöte erillisellä funktiolla, joka palauttaa kokonaisluvun. */
        valinta1 = tarkistaSyote(syote);		
        printf("\n");
        /* Valinnalla 0 suljetaan ohjelma */
        if (valinta1 == 0) {
            printf("Suljetaan pullonpalautusautomaatti.\n");
            break;
        }
        /* Valinnalla 1 luetaan tiedot tiedostosta tuotelistaan ja tulostetaan valikko 2 */
        else if (valinta1 == 1) {
            /* Luetaan tuotetiedot tiedostosta tuotelistaan */
			if(lue_tiedosto(&pAlku_tuotteet,tuotetiedosto) == 0) {
            /* Toisen valikon tulostus */
                while (1) {
                    printf("Syötä uusi pullo tai tölkki.\n");
                    printf("\n");
                    /* Tuotevalintojen tulostus ja viimeisen valinnan luvun laskeminen*/
                    int luku = tulostus(pAlku_tuotteet);
                    printf("%d) Lopeta syöttö ja tulosta kuitti.\n", luku);
                    printf("--> ");
                    /* Valinnan kysyminen toiselle valikolle */
                    fgets(syote,254,stdin);
                    printf("\n");
                    /* Toisen valinnan syötteen tarkistus */
                    valinta2 = tarkistaSyote(syote);
                    /* Jos käyttäjä valitsee viimeisen valinnan */
                    if (valinta2 == luku) {
                        /* Palautuskuitin tulostus käyttäjälle */
                        tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,luku);
                        /* Tiedostojen kirjoitukset */
                        kirjoita_valiaikainen(pAlku_tuotteet,pAlku_palautukset, v_nimi);
                        kirjoita_lokitiedosto(pAlku_palautukset, pAlku_tuotteet,l_nimi);
                        /* Listojen tyhjennys toimenpiteet */
                        tyhjenna_palautukset(&pAlku_palautukset);
                        tyhjenna_tuotteet(&pAlku_tuotteet);
                        printf("\n");
                        printf("PULLONPALAUTUSAUTOMAATTI\n");
                        /* Toisesta valikosta poistuminen takaisin ensimmäiseen valikkoon. */
                        break;
                    }
                    /* Muulla kuin viimeisella valinnalla lisätään tuote palautuslistaan */
                    else if (valinta2 > 0 && valinta2 < luku) {
                        lisaa_palautus(&pAlku_palautukset,pAlku_tuotteet,valinta2);
                    }
                    /* Ilmoitetaan väärästä valinnasta valikossa 2 */
                    else {
                        printf("Väärä valinta!\n");
                    }
                }
            }
        }
        /* ilmoitetaan väärästä valinnasta valikossa 1 */
        else {
            printf("Väärä valinta!\n");
        }
    }

    /* Varmistetaan, että molemmat listat vapautetaan muistista. */
    tyhjenna_tuotteet(&pAlku_tuotteet);
    tyhjenna_palautukset(&pAlku_palautukset);
    return 0;
}
/* eof */
