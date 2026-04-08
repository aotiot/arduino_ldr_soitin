### 3. Bonus: Siniaaltotaulukon generointiskripti (`generate_sine.py`)

Tätä tiedostoa et pakollisesti tarvitse soittimen toimimiseen (koska Mozzi sisältää taulukon valmiina), mutta laitan sen talteen siltä varalta, että haluat myöhemmin kokeilla luoda omia aaltomuotoja! Tämä on Python-skripti.

```python
import math

# Asetukset
table_size = 2048
amplitude = 127.5 # 8-bittisen audion maksimiarvo (-128 ... 127)

print(f"// Automaattisesti generoitu {table_size} askeleen siniaaltotaulukko")
print("#include <avr/pgmspace.h>\n")
print(f"const int8_t SIN{table_size}_DATA[] PROGMEM = {{")

for i in range(table_size):
    # Lasketaan siniaallon vaihe ja arvo
    # Vähennetään 0.5, jotta pyöristys int-tyyppiin osuu oikein
    val = int(math.sin(2.0 * math.pi * i / table_size) * amplitude - 0.5)
    
    # Tulostetaan luku ja pilkku
    print(f"{val:4},", end="")
    
    # Rivinvaihto 16 numeron välein luettavuuden vuoksi
    if (i + 1) % 16 == 0:
        print()

print("};")
