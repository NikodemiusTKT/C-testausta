#include "unity.h"
#include "Types.h"
#include "tiedosto.h"
#include "tietorakenteet.h"
#include "apufunktiot.h"
const char *name = "tuotetiedosto.txt";

tuotetiedot *pAlku_tuotteet=NULL;
void SetUp (void) {
    tyhjenna_tuotteet(&pAlku_tuotteet);
}

void test_lue_tiedosto_luku_epaonnistuu(void) {
    TEST_ASSERT_EQUAL_INT(1, lue_tiedosto(&pAlku_tuotteet,"testi.txt"));
    TEST_ASSERT_NULL(pAlku_tuotteet);

}
void test_lue_tiedosto_luku_onnistuu(void) {
    TEST_ASSERT_EQUAL_INT(0, lue_tiedosto(&pAlku_tuotteet,"tuotetiedosto.txt"));
    TEST_ASSERT_NOT_NULL(pAlku_tuotteet);
    TEST_ASSERT_EQUAL_STRING("pullo",pAlku_tuotteet->tyyppi);
    TEST_ASSERT_EQUAL_INT(1,pAlku_tuotteet->id);
    TEST_ASSERT_EQUAL_FLOAT(1.0,pAlku_tuotteet->koko);
    TEST_ASSERT_EQUAL_FLOAT(0.5,pAlku_tuotteet->pantti);
    TEST_ASSERT_NOT_NULL(pAlku_tuotteet->seuraava);
}


void lue_tiedosto_testi(void) {
    FILE *tiedosto;
    int id = 1, rivi = 0;                       /* Alustetaan rivi arvoon 0. */
    char *koko, *pantti;
    char tyyppi[CHAR_MAX];
    char puskuri[CHAR_MAX];
    tiedosto = fopen(name,"r");
    if (tiedosto != NULL) {
        TEST_ASSERT_NOT_NULL(tiedosto);
        while (fgets(puskuri, sizeof(puskuri), tiedosto)) {
            TEST_ASSERT_NOT_NULL(puskuri);
            rivi ++;
            puskuri[strcspn(puskuri, "\n")] = 0; /* Poistetaan rivinvaihtomerkki puskurin merkkijonon lopusta. */
            if (sscanf(puskuri,"%s %s %s", tyyppi,koko,pantti) == 3) {
                TEST_ASSERT_TRUE(sscanf(puskuri,"%s %s %s", tyyppi,koko,pantti) == 3);
                TEST_ASSERT_TRUE(atof(koko));
                TEST_ASSERT_TRUE(atof(pantti));
                TEST_ASSERT_TRUE(strstr(puskuri,tyyppi));
                TEST_ASSERT_TRUE(strstr(puskuri,koko));
                TEST_ASSERT_TRUE(strstr(puskuri,pantti));
                TEST_ASSERT_TRUE(rivi > 0);
                TEST_ASSERT_TRUE(id >= 1);
                id++;
            }
            /* Annetaan tarvittaessa virheilmoitus väärin luetuista riveistä. */
            else {
                TEST_ASSERT_FALSE(sscanf(puskuri,"%s %s %s", tyyppi,koko,pantti) == 3);
                fprintf(stderr, "Tiedoston rivi %d: '%s' on vääränlainen.\n", rivi,puskuri);
            }
        }
        TEST_ASSERT_NOT_NULL(tiedosto);
        fclose(tiedosto);
        return;
    }
    /* Annetaan virheilmoitus, jos tiedoston luku epäonnistui ja palautetaan luku 1. */
    else {
        TEST_ASSERT_NULL_MESSAGE(tiedosto,"Tiedosto avattiin virheellisesti");
        perror("Tiedoston lukeminen epäonnistui.\n");
        return;
    }
}



/* int main(void) { */
/*     UNITY_BEGIN(); */
/*     RUN_TEST(test_lue_tiedosto_luku_epaonnistuu); */
/*     RUN_TEST(test_lue_tiedosto_luku_onnistuu); */
/*     RUN_TEST(test_lue_tiedosto_lista_tyhja); */
/*     RUN_TEST(lue_tiedosto_testi); */
/*     return UNITY_END(); */
/* } */
