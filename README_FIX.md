# kitmod v59 – Fix: Startscreen "Launch game" / "Clear data"

## Was im Log auffiel
- Nach "Login-Screen angefordert" kam nie `[LS] >>> LoginScreenBauen` oder `[LS] Login-Button umbenannt`, obwohl der State 8 erreicht wurde.
  Die Builder-/Button-Hooks (0x31bacc / 0x31c1d4) haben also nicht gefeuert. Danach lief `8 -> 2 -> 0 -> 2 -> 4`:
  das ist der Original-Button "New game" (Spiel startet normal), nicht dein "Launch game"/"Clear data".
- Offsets, vtable (0x1010d88), State 8 = Login-Screen und die gepatchte Binary habe ich per Disassembly geprueft: alles korrekt.
  Warum die statischen Patches auf dem Geraet nicht greifen, laesst sich von hier aus nicht sehen (haeufig: die Original-`laser`
  steckt noch in der IPA, oder die falsche Datei wurde ersetzt).

## Was geaendert wurde (nur `bundle.js`, Funktion `lsInstall`)
1. **vtable-Weg**: Builder (vtable-Slot +0x40) und ButtonPressed (primaer Slot +0xc0, sekundaer +8) der Login-Screen-Klasse werden
   zur Laufzeit per NativeCallback umgebogen. Braucht keine Binary-Patches. Die Hooks schweigen dann (kein Doppel-Handling).
   Faellt der Weg aus (Eintraege unerwartet / Schreibfehler), bleibt der alte Hook-Weg.
2. **Selbsttest** beim Start: `[LS] Patch-Check` zeigt, ob 0x31bacc / 0x31c1d4 / 0x315cc0 in der laufenden Binary gepatcht sind,
   `[LS] Hook-Slot[0]` ob das __KMDATA-Segment da ist, und nach 10 s `[LS] Treffer ...` wie oft welcher Weg feuerte.
3. Startscreen-Texte (HUD) und ALWAYS_LOGIN laufen auch ohne ScreenManager-Hook (Timer, Haupt-Thread).

## Nicht geaendert
Die Lizenzpruefung. "Geraetelimit erreicht" kommt vom Server (/poll -> denied) und muss dort zurueckgesetzt werden
(Geraet entfernen / Limit erhoehen). Solange nicht aktiviert, startet "Launch game" bewusst die Telegram-Aktivierung (LOCK_LAUNCH).
"Clear data" funktioniert unabhaengig davon.

## Bauen
Wie gehabt (README_CI.md): Repo-Inhalt hochladen -> Actions -> `kitmod.dylib` laden und neu injizieren. `laser_patched` bleibt gleich.
Auf dem Geraet in `Documents/kitmod.log` nachsehen: `[LS] Patch-Check`, `[LS] vtable-Weg aktiv`, `[LS] >>> LoginScreenBauen (vtable)`.

## Getestet / nicht getestet
Linux-Simulation der Laufzeit mit nachgebautem vtable-Image: Umbiegen, Builder-Aufruf, Umbenennen, Press-Unterdrueckung
(primaer + sekundaer), Clear-data-Pfad bis zum App-Ende funktionieren. Auf dem iPhone selbst: ungetestet.
