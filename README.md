# CPSE2 - Opdracht B3 - Nick Teeuwen

## Project
Factory Opdracht 3 — een SFML-applicatie die schermobjecten leest uit `factory.txt`,
deze toont, laat selecteren en verplaatsen, en bij afsluiten opslaat met de nieuwe posities.

## Type
Visual Studio 2022 project (SFML 2.5.1, C++17, MSVC, x64)

## Vereisten
- Visual Studio 2022
- SFML 2.5.1 (Windows VC17 64-bit) — download via https://www.sfml-dev.org/download.php

## Bouwen
1. Open `cpse2-op3-nick-teeuwen.sln` in Visual Studio 2022

2. Stel in Visual studio rechtsboven in op **Release** en **x64**

3. **Build → Build Solution** (`Ctrl+Shift+B`)  (Negeer de error deze pakken we in de volgende stappen aan)

4. Kopieer de volgende SFML 2.5.1 DLL bestanden naar `cpse2-op3-nick-teeuwen\x64\Release\`: 
   - `sfml-graphics-2.dll`
   - `sfml-window-2.dll`
   - `sfml-system-2.dll`
   
   Deze zijn te vinden in `SFML-2.5.1\bin\`

5. Koppel SFML aan het project via **Project → Properties**:
   - **C/C++ → General → Additional Include Directories:** `[pad naar SFML]\include`
   - **Linker → General → Additional Library Directories:** `[pad naar SFML]\lib`
   - **Linker → Input → Additional Dependencies:** voeg toe:
     ```
     sfml-graphics.lib
     sfml-window.lib
     sfml-system.lib
     ```

**Het project is nu ready on te runnen !!**

## Runnen

Dubbelklik op `cpse2-op3-nick-teeuwen.exe` in `x64\Release\` of run vanuit Visual Studio met
**Debug → Start Without Debugging** (`Ctrl+F5`).

`factory.txt` wordt automatisch gevonden — het bestand staat in de root van het project.

## Configuratiebestand: `factory.txt`

Elk object staat op een aparte regel. Lege regels en regels met `#` worden genegeerd.

```
(x,y) CIRCLE kleur diameter
(x,y) RECTANGLE kleur (x2,y2)
(x,y) LINE kleur (x2,y2)
(x,y) PICTURE bestandsnaam.png
```

**Beschikbare kleuren:** black, white, red, green, blue, yellow, magenta, cyan

**Voorbeeld:**
```
(50,50) RECTANGLE blue (150,100)
(50,90) CIRCLE red 30
(50,150) LINE yellow (150,250)
(50,300) PICTURE hu-logo.png
(300,90) CIRCLE green 70
(50,450) RECTANGLE white (450,500)
```

Afbeeldingen (PNG) moeten naast `factory.txt` in de root van het project staanen altijd met ../ beginen

**Voorbeeld**
```
(100,100) PICTURE ../Coin.png
```

## Gebruik

**Acties**

Klik op object:      Selecteer het object (wit kader verschijnt)        
Sleep object:        Verplaats het geselecteerde object                 
Klik op leeg vlak:   Deselecteer alles                                  
ESC:                 Deselecteer alles                                  
Venster sluiten:     Sla nieuwe posities op naar `factory.txt` en sluit 

Bij een fout in `factory.txt` wordt de fout getoond (regelnummer + beschrijving)
en wordt het bestand **niet** overschreven.
