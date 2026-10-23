# kitmod v59 (laser) + Telegram-Aktivierung aus v54

`bundle.js` = dein v59-Script, davor die komplette Telegram-Aktivierung aus v54 (Ed25519-Token, Pairing-Code, Bot-Link,
HUD-Texte, Button im Settings-Popup, Login-Screen mit "Launch game"/"Clear data"). Das v59-Script startet erst nach erfolgreicher
Aktivierung (`Act.run(kitmodMain)`), genau wie in v54.

## Neu gefundene Offsets (v59 / "laser", alle per Disassembly geprueft)
| Zweck | v54 | v59 |
|---|---|---|
| LoginScreen-Builder (button_scid = this+0x30, button_guest = this+0x38) | 0x2b6398 (+0x20/+0x28) | 0x31bacc |
| LoginScreen ButtonPressed (Thunk 0x31c250 -> 0x31c1d4) | 0x2b6ab0 | 0x31c1d4 |
| GameButton::setText(this, "txt", String*) | 0x8f390 | 0x9bd7c |
| ScreenManager::update (Hook, +0x2c = angeforderter State) | 0x2b0ef4 | 0x315cc0 |
| ScreenManager-Singleton (Zeiger) | 0x2606cf0 | 0x1182d10 |
| State fuer Login-Screen (Factory 0x3158f8) | 9 | 8 |
| SettingsPopup-Ctor (Root-MovieClip this+0x70) | 0x2a81b8 (+0x68) | 0x307494 |
| GameMain-Singleton / Root / Kinderzahl | 0x2606320 / +0xe8 / +0x46 | 0x1182100 / +0x108 / +0x4e |
| GameButton: Init-Slot / Press-Slot / Text-MC / Parent | 0x150 / 0x178 / 0x58 / 0x30 | 0x160 / 0x188 / 0x60 / 0x38 |
Weitere Funktionen (GetMovieClip, SetText, SetXY, ...) kommen aus der v59-Tabelle `O`. `hooks.json` hat jetzt 22 Hook-Stellen
(16 vom Script + 6 neue: 0x31bacc, 0x31c1d4, 0x315cc0, 0x307494, 0x8ab4, 0xb88b54).
Die v54-Diagnosehooks (0x17bc1e4, 0x17edb20, 0x2b5688) sind abgeschaltet (`LS_LOG: false`, Block deaktiviert).

## Bauen
`laser_patched` und `dylib/kitmod_gen.h` / `kitmod_tramp.S` sind schon erzeugt und mit `tools/verify_patched.py` geprueft.
Dylib bauen: Repo auf GitHub hochladen (siehe README_CI.md) oder auf dem Mac `cd dylib && make`.
Dann `laser_patched` als Haupt-Binary in die IPA (Name wie das Original), `kitmod.dylib` per Sideloadly injizieren, signieren.

## Nicht geprueft
`make` / ARM-Assembler (kein Compiler hier), Verhalten auf dem Geraet. Das Script laedt in der Linux-Testlaufzeit bis zu den
iOS-spezifischen ObjC-Aufrufen (dort fehlen die Symbole, das ist auf Linux normal).
Vor dem Benutzen in `ACT_CFG` am Anfang von `bundle.js`: BOT / API / PUBKEY (stehen schon drin, aus deinem v54).
