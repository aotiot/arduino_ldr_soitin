/*
 * OPTINEN THEREMIN MOZZI-KIRJASTOLLA
 * * Tämä koodi muuttaa Arduinon syntetisaattoriksi. 
 * Vasen käsi (A0) ohjaa äänen taajuutta (pitch) ja 
 * oikea käsi (A1) ohjaa äänenvoimakkuutta (volume).
 * * Varoitus: Älä käytä Serial.print() tai delay() -funktioita 
 * tämän koodin kanssa, sillä ne rikkovat ääniaallon ajoituksen!
 */

#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h> // Tuodaan 2048 askeleen siniaaltotaulukko

// ---------------------------------------------------------
// 1. ASETUKSET JA MUUTTUJAT
// ---------------------------------------------------------

// Määritellään oskillaattori nimeltä "aSin".
// Se käyttää siniaaltotaulukkoa ja päivittyy järjestelmän audiotaajuudella.
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// Määritellään, mihin nastoihin LDR-vastukset on kytketty
const int PITCH_PIN = A0; 
const int VOL_PIN = A1;   

// Muuttuja äänenvoimakkuuden tallentamiseen. 
// Käytämme 'byte'-tyyppiä (0-255), koska se on nopeampi käsitellä kuin 'int'.
byte volume = 0; 

// ---------------------------------------------------------
// 2. ALUSTUS (SETUP)
// ---------------------------------------------------------
void setup() {
  // Mozzi käyttää oletuksena digitaalista nastaa 9 äänen ulostuloon
  // Arduino Unossa ja Nanossa. Sitä ei tarvitse erikseen määritellä pinMode:lla.
  
  startMozzi(); // Käynnistetään Mozzin äänimoottori
}

// ---------------------------------------------------------
// 3. KONTROLLIEN PÄIVITYS (UPDATE CONTROL)
// ---------------------------------------------------------
// Tätä funktiota ajetaan oletuksena 64 kertaa sekunnissa (CONTROL_RATE).
// Täällä luetaan anturit ja tehdään hitaammat laskutoimitukset prosessorin säästämiseksi.
void updateControl() {
  
  // --- TAAJUUDEN LUKEMINEN JA ASETUS ---
  // Luetaan taajuus-LDR:n arvo (0 - 1023)
  int pitchSensorVal = mozziAnalogRead(PITCH_PIN); 
  
  // Muutetaan anturin lukema halutuiksi äänitaajuuksiksi (Hertseinä).
  // Parametrit: (luettu_arvo, anturin_min, anturin_max, taajuus_min, taajuus_max)
  // KALIBROINTI: Jos huone on hämärä, anturi ei ehkä koskaan anna arvoa 1023. 
  // Voit joutua muuttamaan arvoja, esim: map(pitchSensorVal, 200, 800, 100, 1500);
  int freq = map(pitchSensorVal, 0, 1023, 100, 1500); 
  
  // Kerrotaan oskillaattorille uusi taajuus
  aSin.setFreq(freq); 


  // --- ÄÄNENVOIMAKKUUDEN LUKEMINEN JA ASETUS ---
  // Luetaan voimakkuus-LDR:n arvo (0 - 1023)
  int volSensorVal = mozziAnalogRead(VOL_PIN); 
  
  // Skaalataan lukema voimakkuudeksi (0 - 255).
  // KALIBROINTI: Jotta saat soittimen täysin hiljaiseksi peittämällä anturin,
  // säädä anturin minimiarvoa (tässä oletuksena 0) hieman ylemmäs, esim. 150.
  // Esim: volume = map(volSensorVal, 150, 900, 0, 255);
  int tempVolume = map(volSensorVal, 0, 1023, 0, 255);
  
  // Varmistetaan constrain-funktiolla, ettei voimakkuus koskaan mene yli 255 tai alle 0.
  // Tämä estää äänen "ympäripyörähtämisen" särinäksi.
  volume = constrain(tempVolume, 0, 255); 
}

// ---------------------------------------------------------
// 4. ÄÄNEN PÄIVITYS (UPDATE AUDIO)
// ---------------------------------------------------------
// Tämä on koodin kriittisin osa. Sitä ajetaan yli 16 000 kertaa sekunnissa!
// Täällä ei saa tehdä mitään raskaita laskutoimituksia.
int updateAudio() {
  
  // 1. aSin.next() hakee seuraavan askeleen siniaallosta (arvo väliltä -128 ... 127).
  // 2. Kerrotaan se äänenvoimakkuudella (arvo väliltä 0 ... 255).
  // 3. Jaetaan tulos 256:lla käyttämällä bittisiirtoa (>> 8). Tämä on prosessorille
  //    paljon nopeampaa kuin tavallinen jakolasku (/ 256), ja se palauttaa
  //    äänisignaalin takaisin Mozzin vaatimalle tasolle.
  
  return (aSin.next() * volume) >> 8; 
}

// ---------------------------------------------------------
// 5. PÄÄLOOPPI (LOOP)
// ---------------------------------------------------------
void loop() {
  // audioHook() on pakollinen ja sen pitää pyöriä jatkuvasti.
  // Se huolehtii siitä, että ääntä pusketaan ulos oikeaan tahtiin.
  // ÄLÄ laita tähän looppiin mitään muuta koodia!
  audioHook(); 
}
