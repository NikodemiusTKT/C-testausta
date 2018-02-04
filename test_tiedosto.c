#include "tiedosto.h"
#include "tietorakenteet.h"
#include "test_tietorakenteet.h"
#include "mock_tietorakenteet.h"
#include "hae_tuotetiedot_listasta.h"
#include "laske_pantit.h"
#include "laske_tuotteen_palautukset.h"
#include "laskepalautukset.h"

int disable_laskepalautukset_mocking=0;
int disable_aikaleima_mocking=0;
int disable_lasketuotteet_mocking=0;
int disable_haetiedot_mocking=0;
int disable_laskepantit_mocking=0;
int disable_lisaatuote_mocking=0;

int test_fprintf(FILE* const file, const char *format, ...) {
	int return_value;
	va_list args;
	char temporary_buffer[256];
	va_start(args, format);
	return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer), format, args);
	check_expected(temporary_buffer);
	va_end(args);
	return return_value;
}

int test_printf(const char *format, ...) {
	int return_value;
	va_list args;
	char temporary_buffer[256];
	va_start(args, format);
	return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer), format, args);
	check_expected(temporary_buffer);
	va_end(args);
	return return_value;
}
void test_lokitiedosto_ei_tiedostoa(void **state) {
    tuotetiedot *pAlku_tuotteet=NULL;
    palautustiedot *pAlku_palautukset=NULL;
    expect_string(test_fprintf, temporary_buffer, "Tiedoston avaaminen epäonnistui!");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,NULL),1);
}
void test_lokitiedosto_tyhjat_listat(void **state) {
    tuotetiedot *pAlku_tuotteet=NULL;
    palautustiedot *pAlku_palautukset=NULL;
    expect_string(test_fprintf, temporary_buffer, "Lokitiedostoa ei voida kirjoittaa koska molemmat listat ovat tyhjiä.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,"lokitiedosto.txt"),1);
}
void test_lokitiedosto_tyhja_palautuslista(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku = &a;
    palautustiedot *pAlku_palautukset = NULL;
    expect_string(test_fprintf, temporary_buffer, "Lokitiedostoa ei voida kirjoittaa koska palautuslista on tyhjä.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku,"lokitiedosto.txt"),1);

}
void test_lokitiedosto_onnistunut_kirjoitus(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    will_return(__wrap_aikaleima,"21.4.2017 23:55");
    will_return(__wrap_laskepalautukset,6);
    will_return(__wrap_laske_pantit,4.0);
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55 - Palautukset 6 kpl. Pantit 4.00€.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,"lokitiedosto.txt"),0);

}
void test_lokitiedosto_tyhja_tuotelista(void **state) {
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    tuotetiedot *pAlku_tuotteet = NULL;
    expect_string(test_fprintf, temporary_buffer, "Lokitiedostoa ei voida kirjoittaa koska tuotelista on tyhjä.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,"lokitiedosto.txt"),1);
}
void test_valiaikainen_olematon_tiedosto(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    palautustiedot *pAlku_palautukset = NULL;
    expect_string(test_fprintf, temporary_buffer, "Tiedoston avaaminen epäonnistui!");
    assert_int_equal(kirjoita_valiaikainen(pAlku_tuotteet,pAlku_palautukset,NULL),1);
}

void test_valiaikainen_tyhjat_listat(void **state) {
    tuotetiedot *pAlku_tuotteet=NULL;
    palautustiedot *pAlku_palautukset=NULL;
    expect_string(test_fprintf, temporary_buffer, "Väliaikaista lokitiedostoa ei voida kirjoittaa koska molemmat listat ovat tyhjiä.\n");
    assert_int_equal(kirjoita_valiaikainen(pAlku_tuotteet,pAlku_palautukset,"lokitiedosto.txt"),1);
}
void test_valiaikainen_tyhja_palautuslista(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot *pAlku_palautukset = NULL;
    expect_string(test_fprintf, temporary_buffer, "Väliaikaista lokitiedostoa ei voida kirjoittaa koska palautuslista on tyhjä.\n");
    assert_int_equal(kirjoita_valiaikainen(pAlku_tuotteet,pAlku_palautukset,"lokitiedosto.txt"),1);
}
void test_valiaikainen_kirjoitus_onnistuus(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *palku_tuotteet = &a;
    palautustiedot pf = {.id=3,.seuraava=NULL};
    palautustiedot pe = {.id=1,.seuraava=&pf};
    palautustiedot pd = {.id=2,.seuraava=&pe};
    palautustiedot pc = {.id=1,.seuraava=&pd};
    palautustiedot pb = {.id=2,.seuraava=&pc};
    palautustiedot pa = {.id=1,.seuraava=&pb};
    palautustiedot *palku_palautukset = &pa;
    will_return_count(__wrap_aikaleima,"21.4.2017 23:55",6);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &c);
    /* expect_string(test_printf, temporary_buffer, "tilapäinen lokitiedosto\n\n"); */
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:1.00€.\n");
    assert_int_equal(kirjoita_valiaikainen(palku_tuotteet,palku_palautukset,"lokitiedosto.txt"),0);

}
void test_valiaikainen_ei_loydy_tuotetiedoja(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *palku_tuotteet = &a;
    palautustiedot pf = {.id=3,.seuraava=NULL};
    palautustiedot pe = {.id=1,.seuraava=&pf};
    palautustiedot pd = {.id=2,.seuraava=&pe};
    palautustiedot pc = {.id=1,.seuraava=&pd};
    palautustiedot pb = {.id=2,.seuraava=&pc};
    palautustiedot pa = {.id=1,.seuraava=&pb};
    palautustiedot *palku_palautukset = &pa;
    will_return_count(__wrap_aikaleima,"21.4.2017 23:55",5);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, NULL);
    /* expect_string(test_printf, temporary_buffer, "tilapäinen lokitiedosto\n\n"); */
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    /* expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:1.00€.\n"); */
    assert_int_equal(kirjoita_valiaikainen(palku_tuotteet,palku_palautukset,"lokitiedosto.txt"),0);
}

void test_luetiedot_lisays_onnistuu(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    /* expect_any(__wrap_lisaa_tuote,pAlku_tuotteet); */
    expect_value(__wrap_lisaa_tuote,id,1);
    expect_value(__wrap_lisaa_tuote,id,2);
    expect_value(__wrap_lisaa_tuote,koko,0);
    expect_value(__wrap_lisaa_tuote,koko,0);
    expect_value(__wrap_lisaa_tuote,pantti,5);
    expect_value(__wrap_lisaa_tuote,pantti,4);
    expect_string(__wrap_lisaa_tuote,tyyppi,"lasipullo");
    expect_string(__wrap_lisaa_tuote,tyyppi,"tölkki");
    will_return(__wrap_lisaa_tuote,0);
    will_return(__wrap_lisaa_tuote,0);
    assert_int_equal(lue_tiedosto(&pAlku_tuotteet,"tuotetiedosto.txt"),0);
    /* except_any(__wrap_lisaa_tuote,koko); */
}
void test_luetiedot_olematon_tiedosto(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    expect_string(test_fprintf, temporary_buffer, "Tiedoston lukeminen epäonnistui.\n");
    assert_int_equal(lue_tiedosto(&pAlku_tuotteet,NULL),1);
    assert_ptr_equal(pAlku_tuotteet,NULL);
}
void test_luetiedot_lukusuojattu_tiedosto(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    system("chmod -r tuotetiedosto.txt");
    expect_string(test_fprintf, temporary_buffer, "Tiedoston lukeminen epäonnistui.\n");
    assert_int_equal(lue_tiedosto(&pAlku_tuotteet,"tuotetiedosto.txt"),1);
    assert_ptr_equal(pAlku_tuotteet,NULL);
    system("chmod +r tuotetiedosto.txt");
}


/* INTEGRAATIOTESTAUKSET */
void test_valiaikainen_integraatio_haetuotetiedot(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *palku_tuotteet = &a;
    palautustiedot pf = {.id=3,.seuraava=NULL};
    palautustiedot pe = {.id=1,.seuraava=&pf};
    palautustiedot pd = {.id=2,.seuraava=&pe};
    palautustiedot pc = {.id=1,.seuraava=&pd};
    palautustiedot pb = {.id=2,.seuraava=&pc};
    palautustiedot pa = {.id=1,.seuraava=&pb};
    palautustiedot *palku_palautukset = &pa;
    disable_haetiedot_mocking=1;
    will_return_count(__wrap_aikaleima,"21.4.2017 23:55",6);
    /* expect_string(test_printf, temporary_buffer, "tilapäinen lokitiedosto\n\n"); */
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:tölkki-0.30l:0.20€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:0.40€.\n");
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55:pullo-1.00l:1.00€.\n");
    assert_int_equal(kirjoita_valiaikainen(palku_tuotteet,palku_palautukset,"lokitiedosto.txt"),0);

}
void test_lokitiedosto_integraatio_laskepalautukset(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    will_return(__wrap_aikaleima,"21.4.2017 23:55");
    disable_laskepalautukset_mocking=1;
    will_return(__wrap_laske_pantit,4.0);
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55 - Palautukset 6 kpl. Pantit 4.00€.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,"lokitiedosto.txt"),0);

}
void test_lokitiedosto_integraatio_laskepantit(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    will_return(__wrap_aikaleima,"21.4.2017 23:55");
    disable_laskepalautukset_mocking=1;
    disable_laskepantit_mocking=1;
    expect_string(test_fprintf, temporary_buffer, "21.4.2017 23:55 - Palautukset 6 kpl. Pantit 2.60€.\n");
    assert_int_equal(kirjoita_lokitiedosto(pAlku_palautukset,pAlku_tuotteet,"lokitiedosto.txt"),0);

}
/* void test_lisaatuote_lisays_onnistuu(void **state) { */
/*     tuotetiedot *pAlku_tuotteet = NULL; */
/*     /1* expect_any(__wrap_lisaa_tuote,pAlku_tuotteet); *1/ */
/*     expect_value(__wrap_lisaa_tuote,id,1); */
/*     expect_value(__wrap_lisaa_tuote,id,2); */
/*     expect_value(__wrap_lisaa_tuote,koko,1); */
/*     expect_value(__wrap_lisaa_tuote,koko,1); */
/*     expect_value(__wrap_lisaa_tuote,pantti,0); */
/*     expect_value(__wrap_lisaa_tuote,pantti,0); */
/*     expect_string(__wrap_lisaa_tuote,tyyppi,"pullo"); */
/*     expect_string(__wrap_lisaa_tuote,tyyppi,"tolkki"); */
/*     will_return(__wrap_lisaa_tuote,0); */
/*     will_return(__wrap_lisaa_tuote,0); */
/*     assert_int_equal(lue_tiedosto(&pAlku_tuotteet,"tuotetiedosto.txt"),0); */
/*     /1* except_any(__wrap_lisaa_tuote,koko); *1/ */
/* } */

int main(void) {
	const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lokitiedosto_ei_tiedostoa),
        cmocka_unit_test(test_lokitiedosto_tyhjat_listat),
        cmocka_unit_test(test_lokitiedosto_tyhja_palautuslista),
        cmocka_unit_test(test_lokitiedosto_tyhja_tuotelista),
        cmocka_unit_test(test_lokitiedosto_onnistunut_kirjoitus),
        cmocka_unit_test(test_valiaikainen_olematon_tiedosto),
        cmocka_unit_test(test_valiaikainen_tyhja_palautuslista),
        cmocka_unit_test(test_valiaikainen_tyhjat_listat),
        cmocka_unit_test(test_valiaikainen_kirjoitus_onnistuus),
        cmocka_unit_test(test_valiaikainen_ei_loydy_tuotetiedoja),
        cmocka_unit_test(test_valiaikainen_integraatio_haetuotetiedot),
        cmocka_unit_test(test_lokitiedosto_integraatio_laskepalautukset),
        cmocka_unit_test(test_lokitiedosto_integraatio_laskepantit),
        cmocka_unit_test(test_luetiedot_lisays_onnistuu),
        cmocka_unit_test(test_luetiedot_olematon_tiedosto),
        cmocka_unit_test(test_luetiedot_lukusuojattu_tiedosto),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}

