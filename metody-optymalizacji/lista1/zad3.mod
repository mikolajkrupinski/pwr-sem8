set JednostkiDestylacji;
set Produkty;
set ProduktyKoncowe;
param WydajnoscDestylacji{JednostkiDestylacji, Produkty};
param WydajnoscKrakowania{Produkty};
param MimimalnyPopyt{ProduktyKoncowe};
param KosztJednostkiDestylacji{JednostkiDestylacji};
param KosztDestylacji;
param KosztKrakowania;
param SiarkaWOleju{JednostkiDestylacji};
param SiarkaWOlejuPoKrakowaniu{JednostkiDestylacji};
param MaksymalnaSiarkaWOlejuDomowym;

var Ropa{JednostkiDestylacji} >= 0;

var IloscProduktow{JednostkiDestylacji, Produkty} >= 0;
s.t. ilosc_produktow_koncowych {JD in JednostkiDestylacji, P in Produkty}:
    IloscProduktow[JD, P] = Ropa[JD] * WydajnoscDestylacji[JD, P];

var IloscDestylatuDoKrakowania{JednostkiDestylacji} >= 0;
s.t. ilosc_produktow_do_krakowania {JD in JednostkiDestylacji}:
    IloscDestylatuDoKrakowania[JD] <= IloscProduktow[JD, "Destylat"];

var IloscProduktowPoKrakowaniu{JednostkiDestylacji, Produkty} >= 0;
s.t. ilosc_produktow_po_krakowaniu {JD in JednostkiDestylacji, P in Produkty}:
    IloscProduktowPoKrakowaniu[JD, P] = IloscDestylatuDoKrakowania[JD] * WydajnoscKrakowania[P];

var IloscOlejuDoDomowego{JednostkiDestylacji} >= 0;
s.t. ilosc_oleju_do_domowego {JD in JednostkiDestylacji}:
    IloscOlejuDoDomowego[JD] <= IloscProduktow[JD, "Olej"];

var IloscProduktowKoncowych{ProduktyKoncowe} >= 0;
s.t. ilosc_produktow_koncowych_paliwa_silnikowe:
    IloscProduktowKoncowych["Paliwa silnikowe"] =
        sum{JD in JednostkiDestylacji} (IloscProduktow[JD, "Benzyna"] + IloscProduktowPoKrakowaniu[JD, "Benzyna"]);
s.t. ilosc_produktow_koncowych_domowe_paliwa_olejowe:
    IloscProduktowKoncowych["Domowe paliwa olejowe"] =
        sum{JD in JednostkiDestylacji} (IloscOlejuDoDomowego[JD] + IloscProduktowPoKrakowaniu[JD, "Olej"]);
s.t. ilosc_produktow_koncowych_ciezkie_paliwa_olejowe:
    IloscProduktowKoncowych["Ciezkie paliwa olejowe"] =
        sum{JD in JednostkiDestylacji} (IloscProduktow[JD, "Resztki"] + IloscProduktow[JD, "Olej"] - IloscOlejuDoDomowego[JD] +
        IloscProduktowPoKrakowaniu[JD, "Resztki"]);
s.t. popyt_na_produkty_koncowe {P in ProduktyKoncowe}:
    IloscProduktowKoncowych[P] >= MimimalnyPopyt[P];

minimize koszt_produkcji:
    sum{JD in JednostkiDestylacji}
        (Ropa[JD] * (KosztJednostkiDestylacji[JD] + KosztDestylacji) + IloscDestylatuDoKrakowania[JD] * KosztKrakowania);

s.t. maksymalna_siarka_w_oleju_domowym:
    sum{JD in JednostkiDestylacji} (IloscOlejuDoDomowego[JD] * SiarkaWOleju[JD] +
    IloscProduktowPoKrakowaniu[JD, "Olej"] * SiarkaWOlejuPoKrakowaniu[JD]) <=
    IloscProduktowKoncowych["Domowe paliwa olejowe"] * MaksymalnaSiarkaWOlejuDomowym;

solve;

display koszt_produkcji;
display IloscOlejuDoDomowego;
display IloscProduktow;
display Ropa;
display IloscProduktowKoncowych;

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
