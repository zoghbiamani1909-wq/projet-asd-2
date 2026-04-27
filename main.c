#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FICHIER_PRODUITS      "produits.txt"
#define FICHIER_MOUVEMENTS    "mouvements.txt"
#define FICHIER_NOTIFICATIONS "notifications.txt"

/* ================================================================
   STRUCTURES DE BASE
   ================================================================ */

typedef struct produit {
    int   code;
    char  nom[20];
    int   qte;
    int   seuil_min;
    float prix;
} Produit;

/* ── Noeuds des listes chainees ─────────────────────────────── */

typedef struct noeud_produit {
    Produit               article;
    struct noeud_produit *suiv;
} NoeudProduit;

typedef struct noeud_mouv {
    Produit            article;
    char               mouv[10];
    char               date[20];
    int                qt;
    struct noeud_mouv *suiv;
} NoeudMouv;


typedef struct noeud_notif {
    Produit              article;       /* produit complet           */
    char                 type_alerte[20]; /* "faible" ou "rupture"   */
    char                 date[11];      /* date du declenchement     */
    char                 message[100];  /* ex: "faible:Sucre"        */
    struct noeud_notif  *suiv;
} NoeudNotif;

/* ── Listes (pointeurs de tete) ─────────────────────────────── */

typedef struct { NoeudProduit *tete; int n;   } ListeProduits;
typedef struct { NoeudMouv   *tete; int nm;  } ListeMouvs;
typedef struct { NoeudNotif  *tete; int nn;  } ListeNotif;

/* ================================================================
   UTILITAIRES LISTES
   ================================================================ */

/* -- Produits -- */
NoeudProduit *creer_noeud_produit(Produit p)
{
    NoeudProduit *n = malloc(sizeof(NoeudProduit));
    if (!n) { perror("malloc produit"); exit(1); }
    n->article = p;
    n->suiv    = NULL;
    return n;
}

void ajouter_produit_liste(ListeProduits *lp, Produit p)
{
    NoeudProduit *nv = creer_noeud_produit(p);
    if (!lp->tete) { lp->tete = nv; }
    else {
        NoeudProduit *cur = lp->tete;
        while (cur->suiv) cur = cur->suiv;
        cur->suiv = nv;
    }
    lp->n++;
}

NoeudProduit *chercher_produit(ListeProduits *lp, int code)
{
    NoeudProduit *cur = lp->tete;
    while (cur) {
        if (cur->article.code == code) return cur;
        cur = cur->suiv;
    }
    return NULL;
}

void liberer_produits(ListeProduits *lp)
{
    NoeudProduit *cur = lp->tete, *tmp;
    while (cur) { tmp = cur->suiv; free(cur); cur = tmp; }
    lp->tete = NULL; lp->n = 0;
}

/* -- Mouvements -- */
void ajouter_mouv_liste(ListeMouvs *lm, NoeudMouv donnees)
{
    NoeudMouv *nv = malloc(sizeof(NoeudMouv));
    if (!nv) { perror("malloc mouv"); exit(1); }
    *nv = donnees; nv->suiv = NULL;
    if (!lm->tete) { lm->tete = nv; }
    else {
        NoeudMouv *cur = lm->tete;
        while (cur->suiv) cur = cur->suiv;
        cur->suiv = nv;
    }
    lm->nm++;
}

void liberer_mouvs(ListeMouvs *lm)
{
    NoeudMouv *cur = lm->tete, *tmp;
    while (cur) { tmp = cur->suiv; free(cur); cur = tmp; }
    lm->tete = NULL; lm->nm = 0;
}

/* -- Notifications  -- */
void ajouter_notif_liste(ListeNotif *ln, NoeudNotif donnees)
{
    NoeudNotif *nv = malloc(sizeof(NoeudNotif));
    if (!nv) { perror("malloc notif"); exit(1); }
    *nv = donnees; nv->suiv = NULL;
    if (!ln->tete) { ln->tete = nv; }
    else {
        NoeudNotif *cur = ln->tete;
        while (cur->suiv) cur = cur->suiv;
        cur->suiv = nv;
    }
    ln->nn++;
}

void liberer_notifs(ListeNotif *ln)
{
    NoeudNotif *cur = ln->tete, *tmp;
    while (cur) { tmp = cur->suiv; free(cur); cur = tmp; }
    ln->tete = NULL; ln->nn = 0;
}

/* ================================================================
   MODULE FICHIERS
   ================================================================ */

/* -- Produits -- */
void sauvegarder_produits(ListeProduits *lp)
{
    FILE *f = fopen(FICHIER_PRODUITS, "w");
    if (!f) { perror("Erreur produits"); return; }
    NoeudProduit *cur = lp->tete;
    while (cur) {
        fprintf(f, "%d|%s|%d|%d|%.2f\n",
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min,
                cur->article.prix);
        cur = cur->suiv;
    }
    fclose(f);
    printf("\n %d produit(s) sauvegardes dans '%s'.\n", lp->n, FICHIER_PRODUITS);
}

void charger_produits(ListeProduits *lp)
{
    FILE *f = fopen(FICHIER_PRODUITS, "r");
    if (!f) { printf("\n Aucun fichier produits, demarrage a vide.\n"); return; }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        Produit p; memset(&p, 0, sizeof(p));
        char *tok = strtok(ligne, "|"); if (!tok) continue; p.code = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(p.nom, tok, sizeof(p.nom)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; p.qte = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; p.seuil_min = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; p.prix = (float)atof(tok);
        ajouter_produit_liste(lp, p);
    }
    fclose(f);
    printf("\n %d produit(s) charges depuis '%s'.\n", lp->n, FICHIER_PRODUITS);
}

/* -- Mouvements -- */
void sauvegarder_mouvements(ListeMouvs *lm)
{
    FILE *f = fopen(FICHIER_MOUVEMENTS, "w");
    if (!f) { perror("Erreur mouvements"); return; }
    NoeudMouv *cur = lm->tete;
    while (cur) {
        fprintf(f, "%d|%s|%d|%d|%.2f|%s|%s|%d\n",
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min,
                cur->article.prix, cur->mouv,
                cur->date, cur->qt);
        cur = cur->suiv;
    }
    fclose(f);
    printf("\n %d mouvement(s) sauvegardes dans '%s'.\n", lm->nm, FICHIER_MOUVEMENTS);
}

void charger_mouvements(ListeMouvs *lm)
{
    FILE *f = fopen(FICHIER_MOUVEMENTS, "r");
    if (!f) { printf("\n Aucun fichier mouvements, historique vide.\n"); return; }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        NoeudMouv m; memset(&m, 0, sizeof(m));
        char *tok = strtok(ligne, "|"); if (!tok) continue; m.article.code = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.article.nom,  tok, sizeof(m.article.nom)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.qte       = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.seuil_min = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.prix      = (float)atof(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.mouv, tok, sizeof(m.mouv)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.date, tok, sizeof(m.date)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; m.qt = atoi(tok);
        ajouter_mouv_liste(lm, m);
    }
    fclose(f);
    printf("\n %d mouvement(s) charges depuis '%s'.\n", lm->nm, FICHIER_MOUVEMENTS);
}


void sauvegarder_notifications(ListeNotif *ln)
{
    FILE *f = fopen(FICHIER_NOTIFICATIONS, "w");
    if (!f) { perror("Erreur notifications"); return; }
    NoeudNotif *cur = ln->tete;
    while (cur) {
        fprintf(f, "%s|%d|%s|%d|%d|%.2f|%s|%s\n",
                cur->date,
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min,
                cur->article.prix,
                cur->type_alerte,
                cur->message);
        cur = cur->suiv;
    }
    fclose(f);
    printf("\n %d notification(s) sauvegardee(s) dans '%s'.\n",
           ln->nn, FICHIER_NOTIFICATIONS);
}

void charger_notifications(ListeNotif *ln)
{
    FILE *f = fopen(FICHIER_NOTIFICATIONS, "r");
    if (!f) { printf("\n Aucun fichier notifications, demarrage a vide.\n"); return; }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        NoeudNotif nv; memset(&nv, 0, sizeof(nv));
        char *tok = strtok(ligne, "|"); if (!tok) continue;
        strncpy(nv.date, tok, sizeof(nv.date)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; nv.article.code = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(nv.article.nom, tok, sizeof(nv.article.nom)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; nv.article.qte       = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; nv.article.seuil_min = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; nv.article.prix      = (float)atof(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(nv.type_alerte, tok, sizeof(nv.type_alerte)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(nv.message,     tok, sizeof(nv.message)-1);
        ajouter_notif_liste(ln, nv);
    }
    fclose(f);
    printf("\n %d notification(s) chargee(s) depuis '%s'.\n",
           ln->nn, FICHIER_NOTIFICATIONS);
}

/* ── Date du jour ─────────────────────────────────────────── */
void date_du_jour(char buf[11])
{
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    strftime(buf, 11, "%d/%m/%Y", lt);
}

/* ================================================================
   MODULE PRODUITS
   ================================================================ */

void ajout_produit(ListeProduits *lp)
{
    Produit p; memset(&p, 0, sizeof(p));
    printf("\n Donner le code      : "); scanf("%d",   &p.code);
    printf(" Donner le nom       : "); scanf("%19s",   p.nom);
    printf(" Donner la quantite  : "); scanf("%d",   &p.qte);
    printf(" Donner le seuil_min : "); scanf("%d",   &p.seuil_min);
    printf(" Donner le prix      : "); scanf("%f",   &p.prix);
    ajouter_produit_liste(lp, p);
    printf("\n Produit ajoute avec succes !\n");
}

void modif_prix_produit(ListeProduits *lp)
{
    int mcode;
    printf("\n Code du produit a modifier : "); scanf("%d", &mcode);
    NoeudProduit *n = chercher_produit(lp, mcode);
    if (!n) { printf("\n Code inexistant.\n"); return; }
    float mprix;
    printf(" Nouveau prix (ancien = %.2f) : ", n->article.prix);
    scanf("%f", &mprix);
    n->article.prix = mprix;
    printf("\n Prix modifie avec succes !\n");
}

void afficher_produits(ListeProduits *lp)
{
    if (lp->n == 0) { printf("\n Aucun produit.\n"); return; }
    printf("\n %s %s %s %s %s\n",
           "Code", "Nom", "Qte", "Seuil_min", "Prix");
    printf(" --------------------------------------------------\n");
    NoeudProduit *cur = lp->tete;
    while (cur) {
        printf(" %-6d %-20s %-6d %-10d %.2f\n",
               cur->article.code, cur->article.nom,
               cur->article.qte,  cur->article.seuil_min,
               cur->article.prix);
        cur = cur->suiv;
    }
}

void supp_produit(ListeProduits *lp)
{
    if (lp->n == 0) { printf(" Aucun produit.\n"); return; }
    int code;
    printf(" Code du produit a supprimer : "); scanf("%d", &code);
    NoeudProduit *cur = lp->tete, *prec = NULL;
    while (cur && cur->article.code != code) { prec = cur; cur = cur->suiv; }
    if (!cur) { printf("\n Code inexistant.\n"); return; }
    if (!prec) lp->tete = cur->suiv;
    else       prec->suiv = cur->suiv;
    free(cur);
    lp->n--;
    printf("\n Produit supprime avec succes.\n");
}

/* ================================================================
   MODULE MOUVEMENTS
   ================================================================ */

void Enregistrer_achat(ListeMouvs *lm, ListeProduits *lp)
{
    int code;
    printf("Code produit : "); scanf("%d", &code);
    NoeudProduit *np = chercher_produit(lp, code);
    if (!np) { printf("Produit non trouve.\n"); return; }

    NoeudMouv m; memset(&m, 0, sizeof(m));
    m.article = np->article;
    strcpy(m.mouv, "achat");
    date_du_jour(m.date);
    printf("Quantite : "); scanf("%d", &m.qt);
    np->article.qte += m.qt;
    ajouter_mouv_liste(lm, m);
    printf("Achat enregistre le %s.\n", m.date);
}

void Enregistrer_vente(ListeMouvs *lm, ListeProduits *lp)
{
    int code;
    printf("Code produit : "); scanf("%d", &code);
    NoeudProduit *np = chercher_produit(lp, code);
    if (!np) { printf("Produit non trouve.\n"); return; }

    int q;
    printf("Quantite : "); scanf("%d", &q);
    if (q > np->article.qte) { printf("Stock insuffisant !\n"); return; }

    NoeudMouv m; memset(&m, 0, sizeof(m));
    m.article = np->article;
    strcpy(m.mouv, "vente");
    date_du_jour(m.date);
    m.qt = q;
    np->article.qte -= q;
    ajouter_mouv_liste(lm, m);
    printf("Vente enregistree le %s.\n", m.date);
}

void afficher_historique_mouvements(ListeMouvs *lm)
{
    if (lm->nm == 0) { printf("\n Aucun mouvement.\n"); return; }
    NoeudMouv *cur = lm->tete;
    while (cur) {
        printf("\n Article  : %s\n", cur->article.nom);
        printf(" Type     : %s\n",   cur->mouv);
        printf(" Date     : %s\n",   cur->date);
        printf(" Quantite : %d\n",   cur->qt);
        printf(" ----------------------\n");
        cur = cur->suiv;
    }
}

/* ================================================================
   MODULE ALERTES
   ================================================================ */



void alerte_stock_faible(ListeProduits *lp, ListeNotif *ln, char date[])
{
    printf("\n--- Produits a stock faible ---\n");
    int trouve = 0;
    NoeudProduit *cur = lp->tete;
    while (cur) {
        Produit *p = &cur->article;
        if (p->qte <= p->seuil_min && p->qte > 0) {
            NoeudNotif nv; memset(&nv, 0, sizeof(nv));
            nv.article = *p;
            strncpy(nv.date,        date,     sizeof(nv.date)-1);
            strncpy(nv.type_alerte, "faible",  sizeof(nv.type_alerte)-1);
            snprintf(nv.message, sizeof(nv.message), "faible:%s", p->nom);
            ajouter_notif_liste(ln, nv);
            printf("[ALERTE] %s | code:%d | qte:%d | min:%d\n",
                   date, p->code, p->qte, p->seuil_min);
            trouve = 1;
        }
        cur = cur->suiv;
    }
    if (!trouve) printf(" Aucun produit a stock faible.\n");
}

void alerte_stock_rupture(ListeProduits *lp, ListeNotif *ln, char date[])
{
    printf("\n--- Produits en rupture de stock ---\n");
    int trouve = 0;
    NoeudProduit *cur = lp->tete;
    while (cur) {
        Produit *p = &cur->article;
        if (p->qte == 0) {
            NoeudNotif nv; memset(&nv, 0, sizeof(nv));
            nv.article = *p;
            strncpy(nv.date,        date,      sizeof(nv.date)-1);
            strncpy(nv.type_alerte, "rupture",  sizeof(nv.type_alerte)-1);
            snprintf(nv.message, sizeof(nv.message), "rupture:%s", p->nom);
            ajouter_notif_liste(ln, nv);
            printf("[RUPTURE] %s | code:%d | nom:%s\n",
                   date, p->code, p->nom);
            trouve = 1;
        }
        cur = cur->suiv;
    }
    if (!trouve) printf(" Aucun produit en rupture.\n");
}

void recherche_alerte_par_code(ListeProduits *lp, char date[])
{
    int code_rech;
    printf(" Code du produit : "); scanf("%d", &code_rech);
    NoeudProduit *n = chercher_produit(lp, code_rech);
    if (!n) { printf(" Produit code %d introuvable.\n", code_rech); return; }
    Produit *p = &n->article;
    if (p->qte == 0)
        printf("[RUPTURE] %s | code:%d | nom:%s\n", date, p->code, p->nom);
    else if (p->qte <= p->seuil_min)
        printf("[FAIBLE]  %s | code:%d | nom:%s | qte:%d (min:%d)\n",
               date, p->code, p->nom, p->qte, p->seuil_min);
    else
        printf(" Stock suffisant : %s (code:%d) qte=%d\n",
               p->nom, p->code, p->qte);
}

/* Affiche toutes les notifications enregistrees (historique complet) */
void afficher_historique_notifications(ListeNotif *ln)
{
    if (ln->nn == 0) { printf("\n Historique vide.\n"); return; }
    printf("\n--- Historique des notifications (%d entree(s)) ---\n", ln->nn);
    printf(" %s %s %s %s %s %s\n",
           "Date", "Type", "Nom", "Qte", "Min", "Message");
    printf(" ---------------------------------------------------------------\n");
    NoeudNotif *cur = ln->tete;
    while (cur) {
        printf(" %-11s %-10s %-20s %-6d %-6d %s\n",
               cur->date,
               cur->type_alerte,
               cur->article.nom,
               cur->article.qte,
               cur->article.seuil_min,
               cur->message);
        cur = cur->suiv;
    }
}

/* Affiche uniquement les alertes du dernier scan (meme date) */
void afficher_dernieres_alertes(ListeNotif *ln)
{
    if (ln->nn == 0) { printf("\n Aucune notification.\n"); return; }

    /* Trouver la date la plus recente (derniere de la liste) */
    NoeudNotif *cur = ln->tete;
    char derniere_date[11] = "";
    while (cur) {
        strncpy(derniere_date, cur->date, sizeof(derniere_date)-1);
        cur = cur->suiv;
    }

    printf("\n--- Dernieres alertes (%s) ---\n", derniere_date);
    cur = ln->tete;
    int trouve = 0;
    while (cur) {
        if (strcmp(cur->date, derniere_date) == 0) {
            printf(" [%s] code:%d | nom:%s | qte:%d | min:%d\n",
                   cur->type_alerte,
                   cur->article.code, cur->article.nom,
                   cur->article.qte,  cur->article.seuil_min);
            trouve = 1;
        }
        cur = cur->suiv;
    }
    if (!trouve) printf(" Aucune alerte pour cette date.\n");
}

/* ================================================================
   SOUS-MENUS
   ================================================================ */

void M_stock(ListeProduits *lp, ListeMouvs *lm)
{
    int c;
    do {
        printf("\n--- MODULE MOUVEMENTS ---\n");
        printf(" 1. Enregistrer un achat\n");
        printf(" 2. Enregistrer une vente\n");
        printf(" 3. Afficher l'historique des mouvements\n");
        printf(" 0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: Enregistrer_achat(lm, lp);            break;
            case 2: Enregistrer_vente(lm, lp);            break;
            case 3: afficher_historique_mouvements(lm);   break;
            case 0: break;
            default: printf(" Choix invalide.\n");
        }
    } while (c != 0);
}

void produit_menu(ListeProduits *lp)
{
    int c;
    do {
        printf("\n--- MODULE PRODUITS ---\n");
        printf(" 1. Ajouter un produit\n");
        printf(" 2. Modifier le prix\n");
        printf(" 3. Afficher les produits\n");
        printf(" 4. Supprimer un produit\n");
        printf(" 0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: ajout_produit(lp);      break;
            case 2: modif_prix_produit(lp); break;
            case 3: afficher_produits(lp);  break;
            case 4: supp_produit(lp);       break;
            case 0: break;
            default: printf(" Choix invalide.\n");
        }
    } while (c != 0);
}

void m_inventaire_alerte(ListeProduits *lp, ListeNotif *ln)
{
    int c; char d[11];
    do {
        printf("\n--- MODULE INVENTAIRE & ALERTES ---\n");
        printf(" 1. Afficher stock actuel\n");
        printf(" 2. Detecter stock faible\n");
        printf(" 3. Detecter rupture de stock\n");
        printf(" 4. Recherche par code\n");
        printf(" 5. Historique complet des notifications\n");
        printf(" 6. Dernieres alertes uniquement\n");
        printf(" 0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: afficher_produits(lp);                              break;
            case 2: date_du_jour(d); alerte_stock_faible(lp, ln, d);   break;
            case 3: date_du_jour(d); alerte_stock_rupture(lp, ln, d);  break;
            case 4: date_du_jour(d); recherche_alerte_par_code(lp, d); break;
            case 5: afficher_historique_notifications(ln);              break;
            case 6: afficher_dernieres_alertes(ln);                     break;
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
    ListeProduits lp = { NULL, 0 };
    ListeMouvs    lm = { NULL, 0 };
    ListeNotif    ln = { NULL, 0 };

    printf("\n=== Chargement des donnees ===\n");
    charger_produits(&lp);
    charger_mouvements(&lm);
    charger_notifications(&ln);

    int choix;
    do {
        printf("\n=================================\n");
        printf("       APPLICATION SMARTSTOCK\n");
        printf("=================================\n");
        printf(" 1. Gestion des Produits\n");
        printf(" 2. Mouvements de Stock\n");
        printf(" 3. Inventaire et Alertes\n");
        printf(" 0. Quitter\n");
        printf("\nChoix : "); scanf("%d", &choix);

        switch (choix) {
            case 1: produit_menu(&lp);               break;
            case 2: M_stock(&lp, &lm);               break;
            case 3: m_inventaire_alerte(&lp, &ln);   break;
            case 0:
                printf("\n Sauvegarde automatique...\n");
                sauvegarder_produits(&lp);
                sauvegarder_mouvements(&lm);
                sauvegarder_notifications(&ln);
                liberer_produits(&lp);
                liberer_mouvs(&lm);
                liberer_notifs(&ln);
                printf(" Au revoir !\n");
                break;
            default: printf("\n Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}

