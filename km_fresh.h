// km_fresh.h -- "Frische Installation": erkennt eine neue Installation und leert dann die App-Sandbox + Keychain,
// BEVOR das Spiel irgendwas liest (laeuft im dylib-Konstruktor, vor main()).
//
// Erkennung: Kennung = Pfad der ausfuehrbaren Datei (enthaelt die Bundle-Container-UUID, die iOS bei jeder Installation neu vergibt)
//            + Groesse + Aenderungszeit der Datei. Gespeichert in $HOME/Library/kitmod_install_id.
//            Weicht die gespeicherte Kennung ab (oder fehlt sie) -> neue Installation -> alles loeschen -> Kennung speichern.
#ifndef KM_FRESH_H
#define KM_FRESH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mach-o/dyld.h>

#ifndef KM_FRESH_ON_INSTALL
#define KM_FRESH_ON_INSTALL 1          // 0 = Funktion aus
#endif
#ifndef KM_FRESH_KEEP_ACTIVATION
#define KM_FRESH_KEEP_ACTIVATION 1     // 1 = Telegram-Aktivierung (Library/Caches/kitmod_act.json) bleibt, 0 = wirklich alles weg
#endif
#ifndef KM_FRESH_KEEP_LOG
#define KM_FRESH_KEEP_LOG 1            // 1 = Documents/kitmod.log bleibt
#endif
#ifndef KM_FRESH_KEYCHAIN
#define KM_FRESH_KEYCHAIN 1            // 1 = auch Keychain (gespeicherte Spiel-Accounts) leeren
#endif

#define KM_MAXKEEP 4
static char km_keep_list[KM_MAXKEEP][1100];
static int  km_keep_n;

static int km_is_kept(const char *p) {
    for (int i = 0; i < km_keep_n; i++) if (strcmp(p, km_keep_list[i]) == 0) return 1;
    return 0;
}
static int km_keep_below(const char *p) {              // liegt ein zu behaltender Pfad UNTER p?
    size_t n = strlen(p);
    for (int i = 0; i < km_keep_n; i++)
        if (strncmp(km_keep_list[i], p, n) == 0 && km_keep_list[i][n] == '/') return 1;
    return 0;
}
// Inhalt von dir loeschen (dir selbst bleibt). Rueckgabe: Anzahl geloeschter Eintraege
static int km_wipe_dir(const char *dir, int depth) {
    if (depth > 24) return 0;
    DIR *d = opendir(dir);
    if (!d) return 0;
    int removed = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        char full[1200];
        if (snprintf(full, sizeof full, "%s/%s", dir, e->d_name) >= (int)sizeof full) continue;
        if (km_is_kept(full)) continue;
        struct stat st;
        if (lstat(full, &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) {
            removed += km_wipe_dir(full, depth + 1);
            if (!km_keep_below(full) && rmdir(full) == 0) removed++;
        } else if (unlink(full) == 0) removed++;
    }
    closedir(d);
    return removed;
}
static void km_keychain_wipe(void) {
#if KM_FRESH_KEYCHAIN
    typedef void *(*dict_fn)(void *, const void **, const void **, long, const void *, const void *);
    typedef int (*del_fn)(void *);
    dict_fn mk = (dict_fn)dlsym(RTLD_DEFAULT, "CFDictionaryCreate");
    del_fn  del = (del_fn)dlsym(RTLD_DEFAULT, "SecItemDelete");
    void **pclass = (void **)dlsym(RTLD_DEFAULT, "kSecClass");
    if (!mk || !del || !pclass) return;                 // Security/CoreFoundation nicht da -> ueberspringen
    static const char *classes[] = { "kSecClassGenericPassword", "kSecClassInternetPassword", "kSecClassKey", "kSecClassIdentity", "kSecClassCertificate" };
    for (unsigned i = 0; i < sizeof classes / sizeof *classes; i++) {
        void **pv = (void **)dlsym(RTLD_DEFAULT, classes[i]);
        if (!pv) continue;
        const void *keys[1] = { *pclass }, *vals[1] = { *pv };
        void *q = mk(NULL, keys, vals, 1, NULL, NULL);
        if (q) del(q);
    }
#endif
}
// Rueckgabe: 1 = es wurde geloescht (neue Installation), 0 = nichts zu tun
static int km_fresh_install_check(void) {
#if KM_FRESH_ON_INSTALL
    const char *home = getenv("HOME");
    char exe[1024]; uint32_t sz = sizeof exe;
    if (!home || !*home || _NSGetExecutablePath(exe, &sz) != 0) return 0;
    struct stat est;
    if (stat(exe, &est) != 0) return 0;
    char id[1300];
    snprintf(id, sizeof id, "%s|%lld|%lld", exe, (long long)est.st_size, (long long)est.st_mtime);

    char marker[1100], lib[1100];
    snprintf(lib, sizeof lib, "%s/Library", home);
    snprintf(marker, sizeof marker, "%s/Library/kitmod_install_id", home);
    char old[1300] = { 0 };
    FILE *f = fopen(marker, "r");
    if (f) { size_t n = fread(old, 1, sizeof old - 1, f); old[n] = 0; fclose(f); }
    if (strcmp(old, id) == 0) return 0;                 // gleiche Installation -> Daten bleiben

    km_keep_n = 0;
#if KM_FRESH_KEEP_ACTIVATION
    snprintf(km_keep_list[km_keep_n++], sizeof km_keep_list[0], "%s/Library/Caches/kitmod_act.json", home);
#endif
#if KM_FRESH_KEEP_LOG
    snprintf(km_keep_list[km_keep_n++], sizeof km_keep_list[0], "%s/Documents/kitmod.log", home);
#endif
    static const char *subs[] = { "Documents", "Library", "tmp" };
    for (int pass = 0; pass < 2; pass++)
        for (unsigned i = 0; i < 3; i++) {
            char p[1100];
            snprintf(p, sizeof p, "%s/%s", home, subs[i]);
            km_wipe_dir(p, 0);
        }
    km_keychain_wipe();

    mkdir(lib, 0755);
    f = fopen(marker, "w");                             // Kennung NACH dem Loeschen schreiben
    if (f) { fputs(id, f); fclose(f); }
    return 1;
#else
    return 0;
#endif
}
#endif
