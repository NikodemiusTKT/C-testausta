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

#ifndef TIETORAKENTEET_H
#define TIETORAKENTEET_H

/* Tarvittavat kirjastot */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>


// If -D UNIT_TESTING is defined
#if UNIT_TESTING
#include "test_tietorakenteet.h"

/* Redirect printf to a function in the test application so it's possible to
 * test the standard output. */
#ifdef printf
#undef printf
#endif // printf
#define printf test_printf
extern void print_message(const char *format, ...);
extern int test_printf(const char *format, ...);

/* Redirect fprintf to a function in the test application so it's possible to
 * test error messages. */
#ifdef fprintf
#undef fprintf
#endif // fprintf
#define fprintf test_fprintf
extern int test_fprintf(FILE * const file, const char *format, ...);

// Redirect assert to mock_assert() so assertions can be caught by cmockery.
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);

/* Redirect calloc and free to test_calloc() and test_free() so cmockery can
 * check for memory leaks. */
#ifdef calloc
#undef calloc
#endif // calloc
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#ifdef free
#undef free
#endif // free
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
void* _test_calloc(const size_t number_of_elements, const size_t size,
                   const char* file, const int line);
void _test_free(void* const ptr, const char* file, const int line);

/* main is defined in the unit test so redefine name of the the main function
 * here. */

/* All functions in this object need to be exposed to the test application,
 * so redefine static to nothing. */
#define static

#endif // UNIT_TESTING

// Basic program definitions start here
//
/* Rakenne linkitetylle tuotelistalle */
struct tuotelista {
    int id;
    char *tyyppi;
    float koko;
    float pantti;
    struct tuotelista *seuraava;
};

/* Rakenne linkitetylle palautelistalle */
struct palautelista {
    int id;
    int maara;
    struct palautelista *seuraava;
};


/* Uudet nimet tietorakenteille */
typedef struct tuotelista tuotetiedot;
typedef struct palautelista palautustiedot;

/* Funktioesittelyt */
int lisaa_tuote(tuotetiedot **pAlku_tuotteet, int id, char* tyyppi, float koko, float pantti);
int tulostus(tuotetiedot *pAlku_tuotteet);
int lisaa_palautus(palautustiedot **pAlku_palautukset, tuotetiedot *pAlku_tuotteet, int id);
/* int laskepalautukset(palautustiedot *pAlku_palautukset); */
tuotetiedot* hae_tuotetiedot_listasta(tuotetiedot *pAlku_tuotteet, int id);
/* int laske_tuotteen_palautukset(palautustiedot *pAlku_palautukset, int id); */
int tulosta_kuitti(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet, int luku);
/* float laske_pantit(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet); */
int tyhjenna_palautukset(palautustiedot **pAlku_palautukset);
int tyhjenna_tuotteet(tuotetiedot **Palku_tuotteet);

#endif
