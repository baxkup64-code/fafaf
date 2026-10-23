# Bauen ohne Mac (GitHub Actions)

1. Neues GitHub-Repo anlegen (privat), den **Inhalt** dieses Ordners hochladen (inkl. `.github/`-Ordner und `laser_patched`).
2. Reiter **Actions** -> "Build kitmod.dylib" laeuft automatisch bei jedem Push (oder "Run workflow").
3. Nach ca. 2-5 Minuten: Lauf anklicken -> unten **Artifacts** -> `kitmod-build` herunterladen.
   Darin: `kitmod.dylib` und `laser_patched`.
4. `laser_patched` in der IPA als Haupt-Binary ersetzen (gleicher Name wie das Original `laser`), `kitmod.dylib` per
   Sideloadly "Inject dylibs" einfuegen (Sideloadly gibt es auch fuer Windows), signieren, installieren.
5. Log auf dem Geraet: `Documents/kitmod.log`

Optional: legst du die Original-`laser` ins Repo-Root, prueft der Workflow zusaetzlich `laser_patched`.
Bei Fehlern im Build: Lauf oeffnen, rotes Log kopieren und mir schicken.
