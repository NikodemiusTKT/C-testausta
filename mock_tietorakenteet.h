#ifndef MOCK_TIETORAKENTEET
#define MOCK_TIETORAKENTEET
#include "tietorakenteet.h"
#include <cmocka.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <assert.h>

tuotetiedot* __wrap_hae_tuotetiedot_listasta(tuotetiedot *pAlku_tuotteet, int id);
int __wrap_laskepalautukset(palautustiedot *pAlku_palautukset);
char * __wrap_aikaleima(void);
int __wrap_laske_tuotteen_palautukset(palautustiedot *pAlku_palautukset, int id);
float __wrap_laske_pantit(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet);
int __wrap_lisaa_tuote(tuotetiedot **pAlku_tuotteet, int id, char* tyyppi, float koko, float pantti);

char * __real_aikaleima(void);
int __real_laskepalautukset(palautustiedot *pAlku_palautukset);
int __real_laske_tuotteen_palautukset(palautustiedot *pAlku_palautukset, int id);
float __real_laske_pantit(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet);
tuotetiedot* __real_hae_tuotetiedot_listasta(tuotetiedot *pAlku_tuotteet, int id);
int __real_lisaa_tuote(tuotetiedot **pAlku_tuotteet, int id, char* tyyppi, float koko, float pantti);
#endif
