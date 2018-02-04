#ifndef TEST_APU_H
#define TEST_APU_H
#include "unity.h"
#include "apufunktiot.h" 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void test_aikaleima_tarkista_aikaleima_ei_tyhja(void) {
    TEST_ASSERT_NOT_NULL(aikaleima());
}

void test_aikaleima_tarkista_palautettu_aikaleima(void) {
    FILE *date = popen("date '+%d.%m.%Y %H:%M'", "r");
    char puskuri[255];
    fgets(puskuri,sizeof(puskuri),date);
    char *aika = aikaleima();
    strtok(puskuri,"\n");
    TEST_ASSERT_EQUAL_STRING(puskuri,aika);
    pclose(date);
    free(aika);
}

void test_tarkistasyote_hyvaksy_syote(void) {
    char puskuri[CHAR_MAX];
    for (int i = 0; i<100000; i++) {
        sprintf(puskuri,"%d", i);
        TEST_ASSERT_EQUAL_INT(i,tarkistaSyote(puskuri));
    }
}

void testi_tarkistasyote_hylkaa_syote(void) {
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("ad020afadj"));
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("\n"));
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("akdsafkaj"));
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("akdsafka0"));
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("01dsafka0"));
    TEST_ASSERT_EQUAL_INT(-1,tarkistaSyote("10 10 10"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_aikaleima_tarkista_aikaleima_ei_tyhja);
    RUN_TEST(test_aikaleima_tarkista_palautettu_aikaleima);
    RUN_TEST(test_tarkistasyote_hyvaksy_syote);
    RUN_TEST(testi_tarkistasyote_hylkaa_syote);
    return UNITY_END();
}
#endif
