# Optinen Theremin (Arduino & Mozzi)

Tämä projekti muuttaa Arduinon valolla ohjattavaksi syntetisaattoriksi. Soitin hyödyntää kahta LDR-valovastusta: toinen ohjaa äänen taajuutta (pitch) ja toinen äänenvoimakkuutta (volume). Äänisynteesistä vastaa tehokas Mozzi-audiokirjasto, joka tuottaa puhdasta aaltomuotoa.

## 🛠 Osaluettelo

* **Arduino** (Uno, Nano tai vastaava)
* **2x LDR-valovastus**
* **2x 10kΩ vastus**
* **Vahvistin ja kaiutin** (tai aktiivikaiutin)
* **Koekytkentälevy (Breadboard)** ja hyppylankoja

## 🔌 Kytkentäkaavio

Mozzi-kirjasto käyttää oletuksena **digitaalista nastaa 9** äänen ulostuloon (Arduino Uno/Nano).

1.  **Taajuus (Pitch):** * Kytke LDR:n toinen jalka Arduinon `5V`-nastaan.
    * Kytke LDR:n toinen jalka analogiseen nastaan `A0`.
    * Kytke 10kΩ vastus nastasta `A0` maahan (`GND`).
2.  **Äänenvoimakkuus (Volume):**
    * Kytke toisen LDR:n toinen jalka `5V`-nastaan.
    * Kytke LDR:n toinen jalka analogiseen nastaan `A1`.
    * Kytke 10kΩ vastus nastasta `A1` maahan (`GND`).
3.  **Ääniulostulo (Audio Out):**
    * Kytke hyppylanka Arduinon digitaalisesta nastasta `9` vahvistimen audiosisääntuloon (Audio In / Tip).
    * Kytke Arduinon `GND` vahvistimen maahan (Ground / Sleeve).

## 🚀 Käyttöönotto

1. Avaa Arduino IDE.
2. Asenna Mozzi-kirjasto: `Sketch` -> `Include Library` -> `Manage Libraries...` -> Etsi **Mozzi** ja asenna.
3. Kopioi projektin `.ino`-tiedoston koodi ja lataa se Arduinoon.
4. Liikuta käsiäsi LDR-vastusten yllä muuttaaksesi ääntä!

## ⚙️ Kalibrointi

Huoneen valaistus vaikuttaa LDR-vastusten toimintaan. Etsi koodista funktio `updateControl()` ja muokkaa `map()`-funktioiden anturiarvoja (`0, 1023`), jotta soitin reagoi oikein tilasi valaistukseen:

```cpp
// Esimerkki kalibroidusta taajuudesta, jos huoneessa on hämärämpää:
int freq = map(pitchSensorVal, 200, 800, 100, 1500);
