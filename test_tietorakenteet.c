#include "tietorakenteet.h"
#include "test_tietorakenteet.h"
#include "mock_tietorakenteet.h"
#include "laskepalautukset.h"
#include "laske_tuotteen_palautukset.h"
#include "hae_tuotetiedot_listasta.h"
#include "laske_pantit.h"


int disable_laskepalautukset_mocking=0;
int disable_aikaleima_mocking=0;
int disable_lasketuotteet_mocking=0;
int disable_haetiedot_mocking=0;
int disable_laskepantit_mocking=0;
int disable_lisaatuote_mocking=0;
/* A mock fprintf function that checks the value of strings printed to the
 * standard error stream. */
int test_fprintf(FILE* const file, const char *format, ...) {
	int return_value;
	va_list args;
	char temporary_buffer[256];
	assert_true(file == stderr || file == stdout);
	va_start(args, format);
	return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer), format, args);
	check_expected(temporary_buffer);
	va_end(args);
	return return_value;
}

/* A mock printf function that checks the value of strings printed to the
 * standard output stream. */
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

void test_lisaa_kaksi_tuotetta_tuotelistalle(void **state) {
    tuotetiedot *pAlku = NULL;
    assert_int_equal(__real_lisaa_tuote(&pAlku,1,"pullo",1.0,0.4),0);
    assert_int_equal(pAlku->id,1);
    assert_string_equal(pAlku->tyyppi,"pullo");
    assert_true(pAlku->koko==1.0);
    assert_int_equal(__real_lisaa_tuote(&pAlku,2,"tölkki",0.4,0.2),0);
    assert_true(pAlku->seuraava != NULL);
    tuotetiedot *ptr = pAlku->seuraava;
    assert_int_equal(ptr->id,2);
    assert_string_equal(ptr->tyyppi,"tölkki");
    assert_true(ptr->seuraava == NULL);
    tyhjenna_tuotteet(&pAlku);
}
void test_listan_tulostus_onnistuu(void **state) {
    char puskuri[255];
    char *odotetut[2] = {"1) pullo 1.00l \n","2) tölkki 0.40l \n"};
    tuotetiedot *pAlku = NULL;
    assert_int_equal(__real_lisaa_tuote(&pAlku,1,"pullo",1.0,0.4),0);
    assert_int_equal(__real_lisaa_tuote(&pAlku,2,"tölkki",0.4,0.2),0);
    expect_string(test_printf, temporary_buffer, odotetut[0]);
    expect_string(test_printf, temporary_buffer, odotetut[1]);
    assert_int_equal(tulostus(pAlku),3);
    tyhjenna_tuotteet(&pAlku);
}

void test_listan_tulostus_epaonnistuu(void **state) {
    char puskuri[255];
    tuotetiedot *pAlku = NULL;
    expect_string(test_fprintf, temporary_buffer, "Tuotetietojen lista on tyhjä!\n");
    assert_int_equal(tulostus(pAlku),1);
}

void test_tuotelistan_tyhjennys_onnistuu(void **state) {
    tuotetiedot *pAlku = NULL;
    assert_int_equal(__real_lisaa_tuote(&pAlku,1,"pullo",1.0,0.4),0);
    assert_int_equal(__real_lisaa_tuote(&pAlku,2,"pullo",1.0,0.4),0);
    assert_int_equal(__real_lisaa_tuote(&pAlku,3,"pullo",1.0,0.4),0);
    assert_int_equal(__real_lisaa_tuote(&pAlku,4,"pullo",1.0,0.4),0);
    assert_int_equal(tyhjenna_tuotteet(&pAlku),0);
    assert_ptr_equal(pAlku,NULL);
}
void test_tuotelistan_tyhjennys_epaonnistuu(void **state) {
    tuotetiedot *pAlku = NULL;
    assert_int_equal(tyhjenna_tuotteet(&pAlku),1);
}

void test_hae_tuotetiedot_listasta_loytyy(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku = &a;
    assert_ptr_equal(__real_hae_tuotetiedot_listasta(pAlku,3),&c);
}

void test_hae_tuotetiedot_listasta_ei_loydy(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku = &a;
    assert_ptr_equal(__real_hae_tuotetiedot_listasta(pAlku,4),NULL);
}

void test_hae_tuotetiedot_listasta_tyhja_tuotelista(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    assert_ptr_equal(__real_hae_tuotetiedot_listasta(pAlku_tuotteet,2),NULL);
}
void test_lisaa_palautus_onnistuu(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="lasipullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *ptr = &a;
    palautustiedot *pAlku;
    will_return(__wrap_hae_tuotetiedot_listasta, &c);
    expect_string(test_printf, temporary_buffer, "Syötetty: lasipullo 1.00l\n");
    assert_int_equal(lisaa_palautus(&pAlku,ptr,3),0);
    tyhjenna_palautukset(&pAlku);
}

void test_lisaa_palautus_tyhja_tuotelista(void **state) {
    tuotetiedot *ptr = NULL;
    palautustiedot *pAlku = NULL;
    assert_int_equal(lisaa_palautus(&pAlku,ptr,1),1);
    assert_ptr_equal(pAlku,NULL);
}

void test_laskepalautukset_ei_tyhja(void **state) {
    palautustiedot d = {.id=4,.seuraava=NULL};
    palautustiedot c = {.id=3,.seuraava=&d};
    palautustiedot b = {.id=2,.seuraava=&c};
    palautustiedot a = {.id=1,.seuraava=&b};
    palautustiedot *pAlku = &a;
    assert_int_equal(__real_laskepalautukset(pAlku),4);
}
void test_laskepalautukset_tyhja(void **state) {
    palautustiedot *pAlku = NULL;
    assert_int_equal(__real_laskepalautukset(pAlku),0);
}
void test_laske_tuotteen_palautukset_ei_tyhja(void **state) {
    palautustiedot f = {.id=5,.seuraava=NULL};
    palautustiedot e = {.id=1,.seuraava=&f};
    palautustiedot d = {.id=2,.seuraava=&e};
    palautustiedot c = {.id=1,.seuraava=&d};
    palautustiedot b = {.id=2,.seuraava=&c};
    palautustiedot a = {.id=1,.seuraava=&b};
    palautustiedot *pAlku = &a;
    assert_int_equal(__real_laske_tuotteen_palautukset(pAlku,1),3);
    assert_int_equal(__real_laske_tuotteen_palautukset(pAlku,2),2);
    assert_int_equal(__real_laske_tuotteen_palautukset(pAlku,5),1);
}

void test_laske_tuotteen_palautukset_tyhja(void **state) {
    palautustiedot *pAlku = NULL;
    assert_int_equal(__real_laske_tuotteen_palautukset(pAlku,0),0);
}
void test_laske_tuotteen_palautukset_ei_loydy_listalta(void **state) {
    palautustiedot f = {.id=5,.seuraava=NULL};
    palautustiedot e = {.id=1,.seuraava=&f};
    palautustiedot d = {.id=2,.seuraava=&e};
    palautustiedot c = {.id=1,.seuraava=&d};
    palautustiedot b = {.id=2,.seuraava=&c};
    palautustiedot a = {.id=1,.seuraava=&b};
    palautustiedot *pAlku = &a;
    assert_int_equal(__real_laske_tuotteen_palautukset(pAlku,6),0);

}
void test_laske_pantit_ei_tyhja(void **state) {
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
    will_return(__wrap_hae_tuotetiedot_listasta,&a);
    will_return(__wrap_hae_tuotetiedot_listasta,&b);
    will_return(__wrap_hae_tuotetiedot_listasta,&a);
    will_return(__wrap_hae_tuotetiedot_listasta,&b);
    will_return(__wrap_hae_tuotetiedot_listasta,&a);
    will_return(__wrap_hae_tuotetiedot_listasta,&c);
    assert_int_equal(__real_laske_pantit(pAlku_palautukset,pAlku_tuotteet)-2.6,0);
}
void test_laske_pantit_tyhja_tuotelista(void **state) {
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    tuotetiedot *pAlku_tuotteet = NULL;
    assert_int_equal(__real_laske_pantit(pAlku_palautukset,pAlku_tuotteet),0);
}
void test_laske_pantit_tyhja_palautuslista(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot *pAlku_palautukset = NULL;
    assert_int_equal(__real_laske_pantit(pAlku_palautukset,pAlku_tuotteet),0);
}
void test_tulosta_kuitti(void **state) {
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
    expect_string(test_printf, temporary_buffer, "\nKuitti 17.04.2017 22:00\n");
    expect_string(test_printf, temporary_buffer, "\nPalautetut pullot ja tölkit yhteensä 6 kappaletta.\n\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 3 x 0.40 = 1.20€\n");
    expect_string(test_printf, temporary_buffer, "tölkki 0.30l\tpantit 2 x 0.20 = 0.40€\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 1 x 1.00 = 1.00€\n");
    expect_string(test_printf, temporary_buffer, "\n\nPantit yhteensä 2.00€\n\n\n");
    will_return(__wrap_laskepalautukset,6);
    will_return(__wrap_aikaleima, "17.04.2017 22:00");
    will_return(__wrap_laske_tuotteen_palautukset, 3);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_laske_tuotteen_palautukset, 2);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_laske_tuotteen_palautukset, 1);
    will_return(__wrap_hae_tuotetiedot_listasta, &c);
    will_return(__wrap_laske_pantit, 2.60);

    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,4),0);
}
void test_tulosta_kuitti_tyhjat_listat(void **state) {
    tuotetiedot *pAlku_tuotteet = NULL;
    palautustiedot *pAlku_palautukset = NULL;
    expect_string(test_fprintf, temporary_buffer, "Kuittia ei voida tulostaa koska molemmat listat ovat tyhjiä.\n");
    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,1),1);
}
void test_tulosta_kuitti_tyhja_palautuslista(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    palautustiedot *pAlku_palautukset = NULL;
    expect_string(test_fprintf, temporary_buffer, "Kuittia ei voida tulostaa koska palautuslista on tyhjä.\n");
    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,1),1);
}
void test_tulosta_kuitti_tyhja_tuotelista(void **state) {
    palautustiedot Pc = {.id=1,.seuraava=NULL};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    tuotetiedot *pAlku_tuotteet = NULL;
    expect_string(test_fprintf, temporary_buffer, "Kuittia ei voida tulostaa koska palautuslista on tyhjä.\n");
    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,1),1);
}

 /* INTEGRAATIOTESTAUKSET */ 
void test_tulosta_kuitti_integraatio_laskepalautukset(void **state) {
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
    disable_laskepalautukset_mocking=1;
    will_return(__wrap_aikaleima, "17.04.2017 22:00");
    will_return(__wrap_laske_tuotteen_palautukset, 3);
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_laske_tuotteen_palautukset, 2);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_laske_tuotteen_palautukset, 1);
    will_return(__wrap_hae_tuotetiedot_listasta, &c);
    will_return(__wrap_laske_pantit, 2.60);
    expect_string(test_printf, temporary_buffer, "\nKuitti 17.04.2017 22:00\n");
    expect_string(test_printf, temporary_buffer, "\nPalautetut pullot ja tölkit yhteensä 6 kappaletta.\n\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 3 x 0.40 = 1.20€\n");
    expect_string(test_printf, temporary_buffer, "tölkki 0.30l\tpantit 2 x 0.20 = 0.40€\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 1 x 1.00 = 1.00€\n");
    expect_string(test_printf, temporary_buffer, "\n\nPantit yhteensä 2.00€\n\n\n");

    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,4),0);
}

void test_tulosta_kuitti_integraatio_lasketuotteet(void **state) {
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
    disable_laskepalautukset_mocking=1;
    disable_lasketuotteet_mocking=1;
    will_return(__wrap_aikaleima, "17.04.2017 22:00");
    will_return(__wrap_hae_tuotetiedot_listasta, &a);
    will_return(__wrap_hae_tuotetiedot_listasta, &b);
    will_return(__wrap_hae_tuotetiedot_listasta, &c);
    will_return(__wrap_laske_pantit, 2.60);
    expect_string(test_printf, temporary_buffer, "\nKuitti 17.04.2017 22:00\n");
    expect_string(test_printf, temporary_buffer, "\nPalautetut pullot ja tölkit yhteensä 6 kappaletta.\n\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 3 x 0.40 = 1.20€\n");
    expect_string(test_printf, temporary_buffer, "tölkki 0.30l\tpantit 2 x 0.20 = 0.40€\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 1 x 1.00 = 1.00€\n");
    expect_string(test_printf, temporary_buffer, "\n\nPantit yhteensä 2.00€\n\n\n");

    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,4),0);
}
void test_tulosta_kuitti_integraatio_haetiedot(void **state) {
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
    disable_laskepalautukset_mocking=1;
    disable_lasketuotteet_mocking=1;
    disable_haetiedot_mocking=1;
    will_return(__wrap_aikaleima, "17.04.2017 22:00");
    will_return(__wrap_laske_pantit, 2.60);
    expect_string(test_printf, temporary_buffer, "\nKuitti 17.04.2017 22:00\n");
    expect_string(test_printf, temporary_buffer, "\nPalautetut pullot ja tölkit yhteensä 6 kappaletta.\n\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 3 x 0.40 = 1.20€\n");
    expect_string(test_printf, temporary_buffer, "tölkki 0.30l\tpantit 2 x 0.20 = 0.40€\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 1 x 1.00 = 1.00€\n");
    expect_string(test_printf, temporary_buffer, "\n\nPantit yhteensä 2.00€\n\n\n");

    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,4),0);
}
void test_tulosta_kuitti_integraatio_laskepantit(void **state) {
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
    disable_laskepalautukset_mocking=1;
    disable_lasketuotteet_mocking=1;
    disable_haetiedot_mocking=1;
    disable_laskepantit_mocking=1;
    will_return(__wrap_aikaleima, "17.04.2017 22:00");
    expect_string(test_printf, temporary_buffer, "\nKuitti 17.04.2017 22:00\n");
    expect_string(test_printf, temporary_buffer, "\nPalautetut pullot ja tölkit yhteensä 6 kappaletta.\n\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 3 x 0.40 = 1.20€\n");
    expect_string(test_printf, temporary_buffer, "tölkki 0.30l\tpantit 2 x 0.20 = 0.40€\n");
    expect_string(test_printf, temporary_buffer, "pullo 1.00l\tpantit 1 x 1.00 = 1.00€\n");
    expect_string(test_printf, temporary_buffer, "\n\nPantit yhteensä 2.60€\n\n\n");

    assert_int_equal(tulosta_kuitti(pAlku_palautukset,pAlku_tuotteet,4),0);
}
void test_lisaa_palautus_integraatio_haetuotetiedot(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="lasipullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *ptr = &a;
    palautustiedot *pAlku;
    int disable_haetiedot_mocking=1;
    expect_string(test_printf, temporary_buffer, "Syötetty: lasipullo 1.00l\n");
    expect_string(test_printf, temporary_buffer, "Syötetty: pullo 1.00l\n");
    expect_string(test_printf, temporary_buffer, "Syötetty: tölkki 0.30l\n");
    assert_int_equal(lisaa_palautus(&pAlku,ptr,3),0);
    assert_int_equal(lisaa_palautus(&pAlku,ptr,1),0);
    assert_int_equal(lisaa_palautus(&pAlku,ptr,2),0);
    tyhjenna_palautukset(&pAlku);
}

void test_laske_pantit_integraatio_haetuotetiedot(void **state) {
    tuotetiedot c = {.id=3,.tyyppi="pullo",.koko=1.0,.pantti=1.0,.seuraava=NULL};
    tuotetiedot b = {.id=2,.tyyppi="tölkki",.koko=0.3,.pantti=0.2,.seuraava=&c};
    tuotetiedot a = {.id=1,.tyyppi="pullo",.koko=1.0,.pantti=0.4,.seuraava = &b};
    tuotetiedot *pAlku_tuotteet = &a;
    int disable_haetiedot_mocking=1;
    palautustiedot Pf = {.id=3,.seuraava=NULL};
    palautustiedot Pe = {.id=1,.seuraava=&Pf};
    palautustiedot Pd = {.id=2,.seuraava=&Pe};
    palautustiedot Pc = {.id=1,.seuraava=&Pd};
    palautustiedot Pb = {.id=2,.seuraava=&Pc};
    palautustiedot Pa = {.id=1,.seuraava=&Pb};
    palautustiedot *pAlku_palautukset = &Pa;
    assert_int_equal((__real_laske_pantit(pAlku_palautukset,pAlku_tuotteet)-2.6),0);
}

void test_main(void **state) {
    expect_string(test_printf, temporary_buffer, "Syötetty: tölkki 0.30l\n");
    system("./testi.sh 10 10");
}
int main(void) {
	const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lisaa_kaksi_tuotetta_tuotelistalle),
        cmocka_unit_test(test_listan_tulostus_onnistuu),
        cmocka_unit_test(test_listan_tulostus_epaonnistuu),
        cmocka_unit_test(test_tuotelistan_tyhjennys_onnistuu),
        cmocka_unit_test(test_tuotelistan_tyhjennys_epaonnistuu),
        cmocka_unit_test(test_tuotelistan_tyhjennys_epaonnistuu),
        cmocka_unit_test(test_hae_tuotetiedot_listasta_loytyy),
        cmocka_unit_test(test_hae_tuotetiedot_listasta_ei_loydy),
        cmocka_unit_test(test_hae_tuotetiedot_listasta_tyhja_tuotelista),
        cmocka_unit_test(test_lisaa_palautus_onnistuu),
        cmocka_unit_test(test_lisaa_palautus_tyhja_tuotelista),
        cmocka_unit_test(test_laskepalautukset_ei_tyhja),
        cmocka_unit_test(test_laskepalautukset_tyhja),
        cmocka_unit_test(test_laske_tuotteen_palautukset_ei_tyhja),
        cmocka_unit_test(test_laske_tuotteen_palautukset_tyhja),
        cmocka_unit_test(test_laske_tuotteen_palautukset_ei_loydy_listalta),
        cmocka_unit_test(test_laske_pantit_ei_tyhja),
        cmocka_unit_test(test_laske_pantit_tyhja_tuotelista),
        cmocka_unit_test(test_laske_pantit_tyhja_palautuslista),
        cmocka_unit_test(test_tulosta_kuitti),
        cmocka_unit_test(test_tulosta_kuitti_tyhjat_listat),
        cmocka_unit_test(test_tulosta_kuitti_tyhja_palautuslista),
        // Integraatiotestaukset
        cmocka_unit_test(test_tulosta_kuitti_integraatio_laskepalautukset),
        cmocka_unit_test(test_tulosta_kuitti_integraatio_lasketuotteet),
        cmocka_unit_test(test_tulosta_kuitti_integraatio_haetiedot),
        cmocka_unit_test(test_tulosta_kuitti_integraatio_laskepantit),
        cmocka_unit_test(test_lisaa_palautus_integraatio_haetuotetiedot),
        cmocka_unit_test(test_laske_pantit_integraatio_haetuotetiedot),
	};
	return cmocka_run_group_tests_name("tietorakenteet", tests, NULL, NULL);
}


