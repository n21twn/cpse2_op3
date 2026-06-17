# CPSE2 - Opdracht O3 - Nick Teeuwen 1774888

## Project
Factory Opdracht 3 — een SFML-applicatie die schermobjecten leest uit `factory.txt`,
deze toont, laat selecteren en verplaatsen, en bij afsluiten opslaat met de nieuwe posities.

## Type
Visual Studio 2022 project (SFML 2.5.1, C++17, MSVC, x64)

## Vereisten
- Visual Studio 2022
- SFML 2.5.1 (Windows VC17 64-bit) — download via https://www.sfml-dev.org/download.php

## Installatie SFML

1. Download en pak SFML 2.5.1 uit naar een vaste map, bijv. `C:\SFML-2.5.1\`

2. Voeg de SFML `bin`-map toe aan de **PATH** omgevingsvariabele van Windows:
   - Zoek op: *"Omgevingsvariabelen bewerken"*
   - Voeg toe aan **Path** (gebruiker of systeem): `C:\SFML-2.5.1\bin`
   - Herstart Visual Studio na deze wijziging

   > Hierdoor vindt Windows de SFML DLL-bestanden automatisch — je hoeft ze niet handmatig te kopiëren.

3. Koppel SFML aan het project via **Project → Properties** (zorg dat Configuration = Release, Platform = x64):
   - **C/C++ → General → Additional Include Directories:** `C:\SFML-2.5.1\include`
   - **Linker → General → Additional Library Directories:** `C:\SFML-2.5.1\lib`
   - **Linker → Input → Additional Dependencies:** voeg toe:
     ```
     sfml-graphics.lib
     sfml-window.lib
     sfml-system.lib
     ```

## Bouwen

1. Open `cpse2-op3-nick-teeuwen.sln` in Visual Studio 2022

2. Stel rechtsboven in op **Release** en **x64**

3. **Build → Build Solution** (`Ctrl+Shift+B`)

## Working Directory instellen

Het project leest `factory.txt` en afbeeldingen uit de `assets/`-map relatief aan de
**working directory**. Stel deze in op de projectroot:

- **Project → Properties → Debugging → Working Directory:** `$(ProjectDir)`

Hierdoor werken alle bestandspaden zonder hardcoded of relatieve paden.

## Runnen

Run vanuit Visual Studio met **Debug → Start Without Debugging** (`Ctrl+F5`),
of dubbelklik op de `.exe` mits de working directory correct is ingesteld.

## Configuratiebestand: `factory.txt`

Elk object staat op een aparte regel. Lege regels worden genegeerd.

### Formaat

```
(x,y) CIRCLE kleur diameter
(x,y) RECTANGLE kleur (x2,y2)
(x,y) LINE kleur (x2,y2)
(x,y) PICTURE bestandsnaam.png
```

- `(x,y)` — startpositie in pixels (linkerbovenhoek of startpunt)
- `kleur` — één van: `black`, `white`, `red`, `green`, `blue`, `yellow`, `magenta`, `cyan`
- `diameter` — geheel positief getal (pixels); alleen voor CIRCLE
- `(x2,y2)` — eindpositie of tweede hoekpunt; voor RECTANGLE en LINE
- `bestandsnaam.png` — bestandsnaam van een PNG in de `assets/`-map; voor PICTURE

### Objecten

| Type        | Beschrijving                                                         |
|-------------|----------------------------------------------------------------------|
| `CIRCLE`    | Gevulde cirkel. Positie is linkerbovenhoek van het omsluitende kader.|
| `RECTANGLE` | Gevuld rechthoek tussen twee hoekpunten.                             |
| `LINE`      | Lijn van startpunt naar eindpunt.                                    |
| `PICTURE`   | PNG-afbeelding geladen uit de `assets/`-map.                        |

### Voorbeeld

```
(50,50) RECTANGLE blue (150,100)
(50,90) CIRCLE red 30
(50,150) LINE yellow (150,250)
(50,300) PICTURE hu-logo.png
(300,90) CIRCLE green 70
(50,450) RECTANGLE white (450,500)
```

Afbeeldingen (PNG) moeten in de `assets/`-map staan in de **projectroot**.

## Gebruik

| Actie                | Effect                                              |
|----------------------|-----------------------------------------------------|
| Klik op object       | Selecteer het object (wit kader verschijnt)         |
| Sleep object         | Verplaats het geselecteerde object                  |
| Klik op leeg vlak    | Deselecteer alles                                   |
| `ESC`                | Deselecteer alles                                   |
| Venster sluiten      | Sla nieuwe posities op naar `factory.txt` en sluit  |

Bij een fout in `factory.txt` wordt de fout getoond (regelnummer + beschrijving)
en wordt het bestand **niet** overschreven.
