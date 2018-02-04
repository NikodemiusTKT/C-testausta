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

#include "apufunktiot.h"
#define MAX_KOKO 30

/*******************************************************************/


/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  aikaleima
 *       Kuvaus:  Hankkii tietokoneen nykyisen ajan ja palauttaa sen paikallisena aikana muotoiltuna merkkijonona.
 * =====================================================================================
 */
char* aikaleima(void) {
    char buff[MAX_KOKO];                              /* Puskuri nykyisen ajan säilytykseen */
    time_t nykyinen_aika;                       /* time_t tyyppinen muuttuja nykyisen ajan säilytykseen*/
    struct tm *paikallinen_aika;                /* Tietorakenne nykyisen ajan tietojen säilytykseen */

    /* Hanki nykyinen aika */
    nykyinen_aika = time(NULL);

    /* Muutetaan nykyinen aika paikalliseen aikaan */
    paikallinen_aika = localtime(&nykyinen_aika);

    /* Muotoillaan aika halutun muotoiseksi strftime funktiolla päivä.kuukasi.vuosi ja kellonaika muodossa tunti:minuutti. */
    strftime(buff,sizeof(buff), "%d.%m.%Y %R", paikallinen_aika);

    /* Varataan muistia aika -merkkijonolle. */
    char* aika = (char *)malloc(sizeof(char)*strlen(buff)+1); /* Varatun muistialueen kokoon lisätään 1, lopetusmerkin '\0' säilytykseen. */

    /* Kopioidaan nykyinen aika puskurista, muistista varattuun aika merkkijonoon. */
    strcpy(aika,buff);

    /* Nykyisen ajan merkkijono palautetaan funktiosta. */
    return aika;
/* Muista vapauttaa aikaleiman varaama muisti funktion ulkopuolella! */
}
/* 
 * ===  FUNKTIO  ======================================================================
 *         Nimi:  tarkistaSyote
 *       Kuvaus:  Ottaa merkkijonon ja tarkistaa onko kyseessä numero. Palauttaa muutetun arvon, palauttaa -1 jos syöte ei kelpaa
 * =====================================================================================
 */
int tarkistaSyote(char merkkijono[]){
	/*Tarkistaa syötteen ja muuttaa sen, palauttaa -1 jos ei ole numero */
	int valinta = -1;
	int len = strlen(merkkijono);
    /* Tarkistus mikäli merkkijonoa ei ole annettu ts. '\n' */
    if (merkkijono[0] == '\n' || merkkijono == " ") {
        return valinta;
    }
	for(int i = 0;i < len-1;i++){
		if(!isdigit(merkkijono[i])){
			return valinta;
		}
	}
	valinta = atoi(&merkkijono[0]);
	return valinta;

}
