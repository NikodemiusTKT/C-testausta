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
#ifndef TIEDOSTO_H
#define TIEDOSTO_H
/* Tarvittavat kirjastot */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tietorakenteet.h"
#include "apufunktiot.h"


#if UNIT_TESTING
#include "test_tiedosto.h"
#include "test_tietorakenteet.h"
#include "mock_tietorakenteet.h"

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



/* Funktioesittelyt */
void test_lue_tiedosto_luku(void);
int lue_tiedosto(tuotetiedot **pAlku_tuotteet, char *tuotetiedosto);
int kirjoita_lokitiedosto(palautustiedot *pAlku_palautukset, tuotetiedot *pAlku_tuotteet, char* l_nimi);
int kirjoita_valiaikainen(tuotetiedot *pAlku_tuotteet, palautustiedot *pAlku_palautukset, char* v_nimi);


#endif
