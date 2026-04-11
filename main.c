#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FICHIER_PRODUITS    "produits.txt"
#define FICHIER_MOUVEMENTS  "mouvements.txt"
#define FICHIER_ALERTES     "alertes.txt"
#define FICHIER_HISTORIQUE  "historique_notifications.txt"

struct produit {
    int   code;
    char  nom[20];
    int   qte;
    int   seuil_min;
    float prix;
};

struct mstock {
    struct produit article;
    char   mouv[10];
    char   date[20];
    int    qt;
};

typedef struct {
    struct produit article;
    char type_alerte[20];
    char date[11];
} t_alerte;

typedef struct {
    char date[11];
    char message[100];
} alerte;

/* ================================================================
   MODULE FICHIERS  —  lecture/écriture texte (fprintf / fgets)
   Format produits   : code|nom|qte|seuil_min|prix
   Format mouvements : code|nom|qte|seuil|prix|mouv|date|qt
   Format alertes    : code|nom|qte|seuil|prix|type_alerte|date
   Format historique : date|message
   ================================================================ */

/* ── Produits ─────────────────────────────────────────────── */
void sauvegarder_produits(struct produit t[], int n)
{
    FILE *f = fopen(FICHIER_PRODUITS, "w");
    if (!f) { perror("Erreur produits"); return; }
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%s|%d|%d|%.2f\n",
                t[i].code, t[i].nom, t[i].qte, t[i].seuil_min, t[i].prix);
    fclose(f);
    printf("\n %d produit(s) sauvegardes dans '%s'.\n", n, FICHIER_PRODUITS);
}

int charger_produits(struct produit t[])
{
    FILE *f = fopen(FICHIER_PRODUITS, "r");
    if (!f) { printf("\n Aucun fichier produits, demarrage a vide.\n"); return 0; }

    int  n = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f) != NULL && n < MAX) {
        ligne[strcspn(ligne, "\n")] = '\0';

        char *tok = strtok(ligne, "|"); if (!tok) continue;
        t[n].code = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(t[n].nom, tok, sizeof(t[n].nom) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        t[n].qte = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        t[n].seuil_min = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        t[n].prix = (float)atof(tok);

        n++;
    }
    fclose(f);
    printf("\n %d produit(s) charges depuis '%s'.\n", n, FICHIER_PRODUITS);
    return n;
}

/* ── Mouvements ───────────────────────────────────────────── */
void sauvegarder_mouvements(struct mstock tM[], int nm)
{
    FILE *f = fopen(FICHIER_MOUVEMENTS, "w");
    if (!f) { perror("Erreur mouvements"); return; }
    for (int i = 0; i < nm; i++)
        fprintf(f, "%d|%s|%d|%d|%.2f|%s|%s|%d\n",
                tM[i].article.code, tM[i].article.nom,
                tM[i].article.qte,  tM[i].article.seuil_min,
                tM[i].article.prix, tM[i].mouv,
                tM[i].date,         tM[i].qt);
    fclose(f);
    printf("\n %d mouvement(s) sauvegardes dans '%s'.\n", nm, FICHIER_MOUVEMENTS);
}

int charger_mouvements(struct mstock tM[])
{
    FILE *f = fopen(FICHIER_MOUVEMENTS, "r");
    if (!f) { printf("\n Aucun fichier mouvements, historique vide.\n"); return 0; }

    int  nm = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f) != NULL && nm < MAX) {
        ligne[strcspn(ligne, "\n")] = '\0';

        char *tok = strtok(ligne, "|"); if (!tok) continue;
        tM[nm].article.code = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tM[nm].article.nom, tok, sizeof(tM[nm].article.nom) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tM[nm].article.qte = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tM[nm].article.seuil_min = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tM[nm].article.prix = (float)atof(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tM[nm].mouv, tok, sizeof(tM[nm].mouv) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tM[nm].date, tok, sizeof(tM[nm].date) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tM[nm].qt = atoi(tok);

        nm++;
    }
    fclose(f);
    printf("\n %d mouvement(s) charges depuis '%s'.\n", nm, FICHIER_MOUVEMENTS);
    return nm;
}

/* ── Alertes (t_alerte) ───────────────────────────────────── */
void sauvegarder_alertes(t_alerte tab3[], int n3)
{
    FILE *f = fopen(FICHIER_ALERTES, "w");
    if (!f) { perror("Erreur alertes"); return; }
    for (int i = 0; i < n3; i++)
        fprintf(f, "%d|%s|%d|%d|%.2f|%s|%s\n",
                tab3[i].article.code, tab3[i].article.nom,
                tab3[i].article.qte,  tab3[i].article.seuil_min,
                tab3[i].article.prix, tab3[i].type_alerte,
                tab3[i].date);
    fclose(f);
    printf("\n %d alerte(s) sauvegardee(s) dans '%s'.\n", n3, FICHIER_ALERTES);
}

int charger_alertes(t_alerte tab3[])
{
    FILE *f = fopen(FICHIER_ALERTES, "r");
    if (!f) { printf("\n Aucun fichier alertes.\n"); return 0; }

    int  n3 = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f) != NULL && n3 < MAX) {
        ligne[strcspn(ligne, "\n")] = '\0';

        char *tok = strtok(ligne, "|"); if (!tok) continue;
        tab3[n3].article.code = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tab3[n3].article.nom, tok, sizeof(tab3[n3].article.nom) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tab3[n3].article.qte = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tab3[n3].article.seuil_min = atoi(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        tab3[n3].article.prix = (float)atof(tok);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tab3[n3].type_alerte, tok, sizeof(tab3[n3].type_alerte) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(tab3[n3].date, tok, sizeof(tab3[n3].date) - 1);

        n3++;
    }
    fclose(f);
    printf("\n %d alerte(s) chargee(s) depuis '%s'.\n", n3, FICHIER_ALERTES);
    return n3;
}

/* ── Historique notifications (alerte) ───────────────────── */
void sauvegarder_historique(alerte hist[], int nb_h)
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "w");
    if (!f) { perror("Erreur historique"); return; }
    for (int i = 0; i < nb_h; i++)
        fprintf(f, "%s|%s\n", hist[i].date, hist[i].message);
    fclose(f);
    printf("\n %d notification(s) sauvegardee(s) dans '%s'.\n", nb_h, FICHIER_HISTORIQUE);
}

int charger_historique(alerte hist[])
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "r");
    if (!f) { printf("\n Aucun fichier historique, notifications vides.\n"); return 0; }

    int  nb_h = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f) != NULL && nb_h < MAX) {
        ligne[strcspn(ligne, "\n")] = '\0';

        char *tok = strtok(ligne, "|"); if (!tok) continue;
        strncpy(hist[nb_h].date, tok, sizeof(hist[nb_h].date) - 1);

        tok = strtok(NULL, "|"); if (!tok) continue;
        strncpy(hist[nb_h].message, tok, sizeof(hist[nb_h].message) - 1);

        nb_h++;
    }
    fclose(f);
    printf("\n %d notification(s) chargee(s) depuis '%s'.\n", nb_h, FICHIER_HISTORIQUE);
    return nb_h;
}

/* ================================================================
   MODULE PRODUITS
   ================================================================ */

int pos(struct produit t[], int n, int mcode)
{
    for (int i = 0; i < n; i++)
        if (t[i].code == mcode) return i;
    return -1;
}

void ajout_produit(struct produit t[], int *n)
{
    if (*n >= MAX) { printf("\n Tableau plein.\n"); return; }
    int i = *n; (*n)++;
    printf("\n Donner le code      : "); scanf("%d",  &t[i].code);
    printf(" Donner le nom       : "); scanf("%s",   t[i].nom);
    printf(" Donner la quantite  : "); scanf("%d",  &t[i].qte);
    printf(" Donner le seuil_min : "); scanf("%d",  &t[i].seuil_min);
    printf(" Donner le prix      : "); scanf("%f",  &t[i].prix);
    printf("\n Produit ajoute avec succes !\n");
}

void modif_prix_produit(struct produit t[], int n)
{
    int mcode;
    printf("\n Code du produit a modifier : "); scanf("%d", &mcode);
    int i = pos(t, n, mcode);
    if (i == -1) { printf("\n Code inexistant.\n"); return; }
    float mprix;
    printf(" Nouveau prix (ancien = %.2f) : ", t[i].prix);
    scanf("%f", &mprix);
    t[i].prix = mprix;
    printf("\n Prix modifie avec succes !\n");
}

void afficher_produits(struct produit t[], int n)
{
    if (n == 0) { printf("\n Aucun produit.\n"); return; }
    printf("\n %-6s %-20s %-6s %-10s %-8s\n",
           "Code", "Nom", "Qte", "Seuil_min", "Prix");
    printf(" --------------------------------------------------\n");
    for (int i = 0; i < n; i++)
        printf(" %-6d %-20s %-6d %-10d %.2f\n",
               t[i].code, t[i].nom, t[i].qte, t[i].seuil_min, t[i].prix);
}

void supp_produit(struct produit t[], int *n)
{
    if (*n == 0) { printf(" Aucun produit.\n"); return; }
    int p;
    printf(" Code du produit a supprimer : "); scanf("%d", &p);
    int i = pos(t, *n, p);
    if (i == -1) { printf("\n Code inexistant.\n"); return; }
    for (int j = i; j < *n - 1; j++) t[j] = t[j + 1];
    (*n)--;
    printf("\n Produit supprime avec succes.\n");
}

/* ================================================================
   MODULE MOUVEMENTS
   ================================================================ */

void Enregistrer_achat(struct mstock TabM[], int *nm,
                       struct produit TabP[], int np)
{
    int code;
    printf("Code produit : "); scanf("%d", &code);
    int i = pos(TabP, np, code);
    if (i == -1) { printf("Produit non trouve.\n"); return; }

    TabM[*nm].article = TabP[i];
    strcpy(TabM[*nm].mouv, "achat");
    printf("Date : ");    scanf("%s", TabM[*nm].date);
    printf("Quantite : "); scanf("%d", &TabM[*nm].qt);
    TabP[i].qte += TabM[*nm].qt;
    (*nm)++;
    printf("Achat enregistre.\n");
}

void Enregistrer_vente(struct mstock TabM[], int *nm,
                       struct produit TabP[], int np)
{
    int code;
    printf("Code produit : "); scanf("%d", &code);
    int i = pos(TabP, np, code);
    if (i == -1) { printf("Produit non trouve.\n"); return; }

    int q;
    printf("Quantite : "); scanf("%d", &q);
    if (q > TabP[i].qte) { printf("Stock insuffisant !\n"); return; }

    TabM[*nm].article = TabP[i];
    strcpy(TabM[*nm].mouv, "vente");
    printf("Date : "); scanf("%s", TabM[*nm].date);
    TabM[*nm].qt = q;
    TabP[i].qte -= q;
    (*nm)++;
    printf("Vente enregistree.\n");
}

void afficher_historique(struct mstock TabM[], int n)
{
    if (n == 0) { printf("\n Aucun mouvement.\n"); return; }
    for (int i = 0; i < n; i++) {
        printf("\n Article  : %s\n", TabM[i].article.nom);
        printf(" Type     : %s\n",   TabM[i].mouv);
        printf(" Date     : %s\n",   TabM[i].date);
        printf(" Quantite : %d\n",   TabM[i].qt);
        printf(" ----------------------\n");
    }
}

/* ================================================================
   MODULE ALERTES
   ================================================================ */

void enregister_alerte_memoire(alerte hist[], int *nb_h,
                               char date[], char msg_text[])
{
    int i = *nb_h;
    strcpy(hist[i].date,    date);
    strcpy(hist[i].message, msg_text);
    *nb_h = i + 1;
}

void saisir_date(char d[])
{
    printf("\n Entrez la date (jj/mm/aaaa) : ");
    scanf("%s", d);
}

void alerte_stock_faible(struct produit tab1[], int n1, t_alerte tab3[], int *n3,
                         alerte hist[], int *nb_h, char date[])
{
    int k = 0;
    printf("\n--- Produits a stock faible ---\n");
    for (int i = 0; i < n1; i++) {
        if (tab1[i].qte <= tab1[i].seuil_min && tab1[i].qte > 0) {
            tab3[k].article = tab1[i];
            strcpy(tab3[k].date, date);
            strcpy(tab3[k].type_alerte, "faible");
            char msg[100];
            strcpy(msg, "faible:"); strcat(msg, tab1[i].nom);
            printf("[ALERTE] %s | code:%d | qte:%d | min:%d\n",
                   date, tab1[i].code, tab1[i].qte, tab1[i].seuil_min);
            enregister_alerte_memoire(hist, nb_h, date, msg);
            k++;
        }
    }
    *n3 = k;
}

void alerte_stock_rupture(struct produit tab1[], int n1, t_alerte tab3[], int *n3,
                          alerte hist[], int *nb_h, char date[])
{
    int k = 0;
    printf("\n--- Produits en rupture de stock ---\n");
    for (int i = 0; i < n1; i++) {
        if (tab1[i].qte == 0) {
            tab3[k].article = tab1[i];
            strcpy(tab3[k].date, date);
            strcpy(tab3[k].type_alerte, "rupture");
            char msg[100];
            strcpy(msg, "rupture:"); strcat(msg, tab1[i].nom);
            printf("[RUPTURE] %s | code:%d | nom:%s\n",
                   date, tab1[i].code, tab1[i].nom);
            enregister_alerte_memoire(hist, nb_h, date, msg);
            k++;
        }
    }
    *n3 = k;
}

void recherche_alerte_par_code(struct produit tab1[], int n1, char date[])
{
    int code_rech, trouve = 0;
    printf(" Code du produit : "); scanf("%d", &code_rech);
    for (int i = 0; i < n1; i++) {
        if (tab1[i].code == code_rech) {
            trouve = 1;
            if (tab1[i].qte == 0)
                printf("[RUPTURE] %s | code:%d | nom:%s\n",
                       date, tab1[i].code, tab1[i].nom);
            else if (tab1[i].qte <= tab1[i].seuil_min)
                printf("[FAIBLE]  %s | code:%d | nom:%s | qte:%d (min:%d)\n",
                       date, tab1[i].code, tab1[i].nom,
                       tab1[i].qte, tab1[i].seuil_min);
            else
                printf(" Stock suffisant : %s (code:%d) qte=%d\n",
                       tab1[i].nom, tab1[i].code, tab1[i].qte);
            break;
        }
    }
    if (!trouve) printf(" Produit code %d introuvable.\n", code_rech);
}

void afficher_historique_notification(alerte hist[], int nb_h)
{
    if (nb_h == 0) { printf("\n Historique vide.\n"); return; }
    printf("\n--- Historique des notifications ---\n");
    for (int i = 0; i < nb_h; i++)
        printf(" [%s] %s\n", hist[i].date, hist[i].message);
}

/* ================================================================
   SOUS-MENUS
   ================================================================ */

void M_stock(struct produit t[], int *np, struct mstock tM[], int *nm)
{
    int c;
    do {
        printf("\n--- MODULE MOUVEMENTS ---\n");
        printf("1. Enregistrer un achat\n");
        printf("2. Enregistrer une vente\n");
        printf("3. Afficher l'historique\n");
        printf("0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: Enregistrer_achat(tM, nm, t, *np); break;
            case 2: Enregistrer_vente(tM, nm, t, *np); break;
            case 3: afficher_historique(tM, *nm);       break;
            case 0: break;
            default: printf(" Choix invalide.\n");
        }
    } while (c != 0);
}

void produit_menu(struct produit t[], int *n)
{
    int c;
    do {
        printf("\n--- MODULE PRODUITS ---\n");
        printf("1. Ajouter un produit\n");
        printf("2. Modifier le prix\n");
        printf("3. Afficher les produits\n");
        printf("4. Supprimer un produit\n");
        printf("5. Sauvegarder les produits\n");
        printf("0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: ajout_produit(t, n);         break;
            case 2: modif_prix_produit(t, *n);   break;
            case 3: afficher_produits(t, *n);    break;
            case 4: supp_produit(t, n);          break;
            case 5: sauvegarder_produits(t, *n); break;
            case 0: break;
            default: printf(" Choix invalide.\n");
        }
    } while (c != 0);
}

void m_inventaire_alerte(struct produit tab1[], int n1, t_alerte tab3[], int *n3,
                         alerte hist[], int *nb_h)
{
    int c; char d[11];
    do {
        printf("\n--- MODULE INVENTAIRE & ALERTES ---\n");
        printf("1. Afficher stock actuel\n");
        printf("2. Stock faible\n");
        printf("3. Rupture de stock\n");
        printf("4. Recherche par code\n");
        printf("5. Historique notifications\n");
        printf("0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: afficher_produits(tab1, n1);                                              break;
            case 2: saisir_date(d); alerte_stock_faible(tab1,n1,tab3,n3,hist,nb_h,d);        break;
            case 3: saisir_date(d); alerte_stock_rupture(tab1,n1,tab3,n3,hist,nb_h,d);       break;
            case 4: saisir_date(d); recherche_alerte_par_code(tab1,n1,d);                     break;
            case 5: afficher_historique_notification(hist, *nb_h);                            break;
            case 0: break;
            default: printf(" Choix invalide.\n");
        }
    } while (c != 0);
}

/* ================================================================
   MAIN
   ================================================================ */
int main(void)
{
    int np = 0, nm = 0, na = 0, nb_h = 0;
    struct produit tp[MAX];
    struct mstock  tM[MAX];
    t_alerte       alertes[MAX];
    alerte         historique[MAX];

    /* Chargement automatique au démarrage */
    printf("\n=== Chargement des donnees ===\n");
    np = charger_produits(tp);
    nm = charger_mouvements(tM);
    na = charger_alertes(alertes);
    nb_h = charger_historique(historique);

    int choix;
    do {
        printf("\n=================================\n");
        printf("     APPLICATION SMARTSTOCK\n");
        printf("=================================\n");
        printf("1. Gestion des Produits\n");
        printf("2. Mouvements de Stock\n");
        printf("3. Inventaire et Alertes\n");
        printf("0. Quitter\n");
        printf("\nChoix : "); scanf("%d", &choix);

        switch (choix) {
            case 1: produit_menu(tp, &np);                                          break;
            case 2: M_stock(tp, &np, tM, &nm);                                     break;
            case 3: m_inventaire_alerte(tp, np, alertes, &na, historique, &nb_h);  break;

            case 0:
                /* Sauvegarde automatique en quittant */
                printf("\n Sauvegarde automatique...\n");
                sauvegarder_produits(tp, np);
                sauvegarder_mouvements(tM, nm);
                sauvegarder_alertes(alertes, na);
                sauvegarder_historique(historique, nb_h);
                printf(" Au revoir !\n");
                break;
            default: printf("\n Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
