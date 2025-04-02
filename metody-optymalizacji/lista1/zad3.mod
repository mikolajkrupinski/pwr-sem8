# Mikołaj Krupiński, zadanie 3

set JednostkiDestylacji; # Nazwy jednostek destylacji
set Produkty; # Nazwy produktów wyjściowych z jednostek destylacji
set ProduktyKoncowe; # Nazwy produktów końcowych
param WydajnoscDestylacji{JednostkiDestylacji, Produkty}; # Wydajność destylacji według jednostek destylacji i produktów
param WydajnoscKrakowania{Produkty}; # Wydajność krakowania według produktów
param MimimalnyPopyt{ProduktyKoncowe}; # Minimalny popyt na produkty końcowe
param KosztJednostkiDestylacji{JednostkiDestylacji}; # Koszt destylacji 1t ropy w jednostkach destylacji
param KosztDestylacji; # Koszt destylacji 1t ropy
param KosztKrakowania; # Koszt krakowania 1t destylatu
param SiarkaWOleju{JednostkiDestylacji}; # Zawartość siarki w oleju w jednostkach destylacji
param SiarkaWOlejuPoKrakowaniu{JednostkiDestylacji}; # Zawartość siarki w oleju po krakowaniu
param MaksymalnaSiarkaWOlejuDomowym; # Maksymalna zawartość siarki w oleju domowym

var Ropa{JednostkiDestylacji} >= 0; # Ilość ropy do przetworzenia w jednostkach destylacji

var IloscProduktow{JednostkiDestylacji, Produkty} >= 0; # Ilość produktów wyjściowych z jednostek destylacji
s.t. ilosc_produktow {JD in JednostkiDestylacji, P in Produkty}: # Ilość produktów wyjściowych proporcjonalna do wydajności destylacji
    IloscProduktow[JD, P] = Ropa[JD] * WydajnoscDestylacji[JD, P];

var IloscDestylatuDoKrakowania{JednostkiDestylacji} >= 0; # Ilość destylatu do krakowania w jednostkach destylacji
s.t. ilosc_produktow_do_krakowania {JD in JednostkiDestylacji}: # Ilość destylatu do krakowania mniejsza od ilości destylatu
    IloscDestylatuDoKrakowania[JD] <= IloscProduktow[JD, "Destylat"];

var IloscProduktowPoKrakowaniu{JednostkiDestylacji, Produkty} >= 0; # Ilość produktów po krakowaniu
s.t. ilosc_produktow_po_krakowaniu {JD in JednostkiDestylacji, P in Produkty}: # Ilość produktów po krakowaniu proporcjonalna do wydajności krakowania
    IloscProduktowPoKrakowaniu[JD, P] = IloscDestylatuDoKrakowania[JD] * WydajnoscKrakowania[P];

var IloscOlejuDoDomowego{JednostkiDestylacji} >= 0; # Ilość oleju do domowego oleju według jednostek destylacji
s.t. ilosc_oleju_do_domowego {JD in JednostkiDestylacji}: # Ilość oleju do domowego użytku mniejsza od ilości oleju
    IloscOlejuDoDomowego[JD] <= IloscProduktow[JD, "Olej"];

var IloscProduktowKoncowych{ProduktyKoncowe} >= 0; # Ilość produktów końcowych
s.t. ilosc_produktow_koncowych_paliwa_silnikowe: # Ilość paliw silnikowych
    IloscProduktowKoncowych["Paliwa silnikowe"] =
        sum{JD in JednostkiDestylacji} (IloscProduktow[JD, "Benzyna"] + IloscProduktowPoKrakowaniu[JD, "Benzyna"]);
s.t. ilosc_produktow_koncowych_domowe_paliwa_olejowe: # Ilość domowych paliw olejowych
    IloscProduktowKoncowych["Domowe paliwa olejowe"] =
        sum{JD in JednostkiDestylacji} (IloscOlejuDoDomowego[JD] + IloscProduktowPoKrakowaniu[JD, "Olej"]);
s.t. ilosc_produktow_koncowych_ciezkie_paliwa_olejowe: # Ilość ciężkich paliw olejowych
    IloscProduktowKoncowych["Ciezkie paliwa olejowe"] =
        sum{JD in JednostkiDestylacji} ((IloscProduktow[JD, "Destylat"] - IloscDestylatuDoKrakowania[JD]) + (IloscProduktow[JD, "Olej"] - IloscOlejuDoDomowego[JD]) + IloscProduktow[JD, "Resztki"] + IloscProduktowPoKrakowaniu[JD, "Resztki"]);
s.t. popyt_na_produkty_koncowe {P in ProduktyKoncowe}: # Popyt na produkty końcowe spełniony
    IloscProduktowKoncowych[P] >= MimimalnyPopyt[P];

var WagaSiarkiWOlejuDomowym >= 0; # Waga siarki w oleju domowym
s.t. waga_siarki_w_oleju_domowym: # Waga siarki w oleju domowym
    WagaSiarkiWOlejuDomowym =
        sum{JD in JednostkiDestylacji} (IloscOlejuDoDomowego[JD] * SiarkaWOleju[JD] +
        IloscProduktowPoKrakowaniu[JD, "Olej"] * SiarkaWOlejuPoKrakowaniu[JD]);

s.t. maksymalna_waga_siarki_w_oleju_domowym: # Maksymalna waga siarki w oleju domowym
    WagaSiarkiWOlejuDomowym <= IloscProduktowKoncowych["Domowe paliwa olejowe"] * MaksymalnaSiarkaWOlejuDomowym;

minimize koszt_produkcji: # Całkowity koszt produkcji
    sum{JD in JednostkiDestylacji}
        (Ropa[JD] * (KosztJednostkiDestylacji[JD] + KosztDestylacji) + IloscDestylatuDoKrakowania[JD] * KosztKrakowania);

solve;

display Ropa;
display koszt_produkcji;
display IloscProduktowKoncowych;
display IloscDestylatuDoKrakowania;
display IloscOlejuDoDomowego;
display WagaSiarkiWOlejuDomowym;

data;

set JednostkiDestylacji := "B1" "B2";
set Produkty := "Benzyna" "Olej" "Destylat" "Resztki";
set ProduktyKoncowe := "Paliwa silnikowe" "Domowe paliwa olejowe" "Ciezkie paliwa olejowe";
param WydajnoscDestylacji :=
    ["B1", "Benzyna"] 0.15
    ["B1", "Olej"] 0.4
    ["B1", "Destylat"] 0.15
    ["B1", "Resztki"] 0.15
    ["B2", "Benzyna"] 0.1
    ["B2", "Olej"] 0.35
    ["B2", "Destylat"] 0.2
    ["B2", "Resztki"] 0.25
    ;
param WydajnoscKrakowania :=
    "Benzyna" 0.5
    "Olej" 0.2
    "Destylat" 0
    "Resztki" 0.06
    ;
param MimimalnyPopyt :=
    "Paliwa silnikowe" 200000
    "Domowe paliwa olejowe" 400000
    "Ciezkie paliwa olejowe" 250000
    ;
param KosztJednostkiDestylacji :=
    "B1" 1300
    "B2" 1500
    ;
param KosztDestylacji := 10;
param KosztKrakowania := 20;
param SiarkaWOleju :=
    "B1" 0.002
    "B2" 0.012
    ;
param SiarkaWOlejuPoKrakowaniu :=
    "B1" 0.003
    "B2" 0.025
    ;
param MaksymalnaSiarkaWOlejuDomowym := 0.005;

end;
