#include "tietorakenteet.h"
#include "mock_tietorakenteet.h"
#include "test_tietorakenteet.h"
#include "laskepalautukset.h"
#include "laske_tuotteen_palautukset.h"
#include "hae_tuotetiedot_listasta.h"
#include "laske_pantit.h"


extern int disable_laskepalautukset_mocking;
extern int disable_aikaleima_mocking;
extern int disable_lasketuotteet_mocking;
extern int disable_haetiedot_mocking;
extern int disable_laskepantit_mocking;
extern int disable_lisaatuote_mocking;

int __real_laskepalautukset(palautustiedot *pAlku_palautukset);
int __wrap_laskepalautukset(palautustiedot *pAlku_palautukset) {
    if(disable_laskepalautukset_mocking) 
        return __real_laskepalautukset(pAlku_palautukset);
    else return (int)mock();
}

char * __wrap_aikaleima(void) {

    if(disable_aikaleima_mocking)
        return __real_aikaleima();
    char *aika = mock_ptr_type(char *);
    char *aikaleima = (char*)malloc(sizeof(char)*strlen(aika)+1);
    strcpy(aikaleima,aika);
    return aikaleima;
}

int __wrap_laske_tuotteen_palautukset(palautustiedot *pAlku_palautukset, int id) {
    if(disable_lasketuotteet_mocking)
        return __real_laske_tuotteen_palautukset(pAlku_palautukset,id);
    else return (int)mock();
}

float __wrap_laske_pantit(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet) {
    if(disable_laskepantit_mocking) return __real_laske_pantit(pAlku_palautukset,pAlku_tuotteet);
    else return (float)mock();
}

tuotetiedot* __wrap_hae_tuotetiedot_listasta(tuotetiedot *pAlku_tuotteet, int id) {
    if(disable_haetiedot_mocking) return __real_hae_tuotetiedot_listasta(pAlku_tuotteet,id);
    else return (tuotetiedot*)mock();
}

int __wrap_lisaa_tuote(tuotetiedot **pAlku_tuotteet, int id, char* tyyppi, float koko, float pantti) {
    check_expected(id);
    check_expected_ptr(tyyppi);
    check_expected(koko);
    check_expected(pantti);
    if(disable_lisaatuote_mocking) return __real_lisaa_tuote(pAlku_tuotteet,id,tyyppi,koko,pantti);
    else { 
        tuotetiedot uusi={.id=id,.tyyppi=tyyppi,.koko=koko,.pantti=pantti,.seuraava=NULL};
        if (*pAlku_tuotteet == NULL) {
            *pAlku_tuotteet=&uusi;
        } else {
            tuotetiedot *viimeinen = *pAlku_tuotteet;
            while(viimeinen->seuraava!=NULL) {
                viimeinen = viimeinen->seuraava;
            }
            viimeinen->seuraava=&uusi;
        }

        return (int)mock();
    }
    
}
