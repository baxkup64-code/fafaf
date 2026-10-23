# v60 – HUD nur auf dem Launch-Screen + frische Installation

## 1) kitmod-Text / Server-Verbindung oben bleibt nicht mehr stehen (bundle.js)
Ursache: Beim Verlassen des Screens (oder wenn ein anderer Hook dazwischenfunkte) wurden die Text-Objekte nur aus der
Liste `hud.items` vergessen (Position NaN), aber nie wirklich ausgeblendet -> sie blieben im Root-Container sichtbar.
Jetzt:
- Jeder angelegte HUD-Text wird in `hud.all` gemerkt und beim Verlassen wirklich geleert (Text "" + Breite 1 + weit ausserhalb).
- Die Parent-Pruefung, die das Ausblenden manchmal verhindert hat, ist raus (es zaehlt nur, ob das Objekt noch lebt).
- Sicherheitsnetz im 250-ms-Timer: steht der ScreenManager auf einem anderen State als dem Login-Screen (8), werden alle Texte entfernt.
- Beim erneuten Aufbau des Launch-Screens werden sie neu angelegt.
Log: `[HUD] Startscreen-Texte entfernt (...)`.

## 2) Immer frische Installation (dylib/km_fresh.h, aufgerufen in dylib/kitmod.c)
Im dylib-Konstruktor (vor main, bevor das Spiel Dateien liest) wird eine Installationskennung gebildet
(Pfad der ausfuehrbaren Datei inkl. Bundle-UUID + Groesse + Datum) und mit `Library/kitmod_install_id` verglichen.
Weicht sie ab / fehlt sie -> neue Installation -> Documents, Library, tmp und Keychain werden geleert, dann Kennung gespeichert.
Gleiche Installation -> nichts wird angefasst (normale Starts verlieren keine Daten).
Schalter oben in `km_fresh.h` (oder per -D im Makefile):
- `KM_FRESH_ON_INSTALL` 1/0 (ganze Funktion)
- `KM_FRESH_KEEP_ACTIVATION` 1 = Telegram-Aktivierung bleibt (0 = wirklich alles weg, danach neu aktivieren)
- `KM_FRESH_KEEP_LOG` 1 = Documents/kitmod.log bleibt
- `KM_FRESH_KEYCHAIN` 1 = Keychain (gespeicherte Spiel-Accounts) auch leeren

Getestet: Logik auf Linux (Dateien weg, Aktivierung + Log bleiben, 2. Start loescht nichts, neue Installation loescht wieder).
NICHT getestet: Build mit iOS-clang und das Verhalten auf dem Geraet.
