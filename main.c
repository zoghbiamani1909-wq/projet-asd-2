#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FICHIER_PRODUITS    "produits.txt"
#define FICHIER_MOUVEMENTS  "mouvements.txt"
#define FICHIER_ALERTES     "alertes.txt"
#define FICHIER_HISTORIQUE  "historique_notifications.txt"

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

typedef struct noeud_alerte {
    Produit               article;
    char                  type_alerte[20];
    char                  date[11];
    struct noeud_alerte  *suiv;
} NoeudAlerte;

typedef struct noeud_hist {
    char               date[11];
    char               message[100];
    struct noeud_hist *suiv;
} NoeudHist;

/* ── Listes (pointeurs de tete) ─────────────────────────────── */

typedef struct { NoeudProduit  *tete; int n;    } ListeProduits;
typedef struct { NoeudMouv    *tete; int nm;   } ListeMouvs;
typedef struct { NoeudAlerte  *tete; int na;   } ListeAlertes;
typedef struct { NoeudHist    *tete; int nb_h; } ListeHist;

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
    while (cur) { if (cur->article.code == code) return cur; cur = cur->suiv; }
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

/* -- Alertes -- */
void ajouter_alerte_liste(ListeAlertes *la, NoeudAlerte donnees)
{
    NoeudAlerte *nv = malloc(sizeof(NoeudAlerte));
    if (!nv) { perror("malloc alerte"); exit(1); }
    *nv = donnees; nv->suiv = NULL;
    if (!la->tete) { la->tete = nv; }
    else {
        NoeudAlerte *cur = la->tete;
        while (cur->suiv) cur = cur->suiv;
        cur->suiv = nv;
    }
    la->na++;
}

void liberer_alertes(ListeAlertes *la)
{
    NoeudAlerte *cur = la->tete, *tmp;
    while (cur) { tmp = cur->suiv; free(cur); cur = tmp; }
    la->tete = NULL; la->na = 0;
}

/* -- Historique -- */
void ajouter_hist_liste(ListeHist *lh, const char *date, const char *msg)
{
    NoeudHist *nv = malloc(sizeof(NoeudHist));
    if (!nv) { perror("malloc hist"); exit(1); }
    strncpy(nv->date,    date, sizeof(nv->date)    - 1); nv->date[sizeof(nv->date)-1]    = '\0';
    strncpy(nv->message, msg,  sizeof(nv->message) - 1); nv->message[sizeof(nv->message)-1] = '\0';
    nv->suiv = NULL;
    if (!lh->tete) { lh->tete = nv; }
    else {
        NoeudHist *cur = lh->tete;
        while (cur->suiv) cur = cur->suiv;
        cur->suiv = nv;
    }
    lh->nb_h++;
}

void liberer_hist(ListeHist *lh)
{
    NoeudHist *cur = lh->tete, *tmp;
    while (cur) { tmp = cur->suiv; free(cur); cur = tmp; }
    lh->tete = NULL; lh->nb_h = 0;
}

/* ================================================================
   MODULE FICHIERS
   ================================================================ */

void sauvegarder_produits(ListeProduits *lp)
{
    FILE *f = fopen(FICHIER_PRODUITS, "w");
    if (!f) { perror("Erreur produits"); return; }
    NoeudProduit *cur = lp->tete;
    while (cur) {
        fprintf(f, "%d|%s|%d|%d|%.2f\n",
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min, cur->article.prix);
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

void sauvegarder_mouvements(ListeMouvs *lm)
{
    FILE *f = fopen(FICHIER_MOUVEMENTS, "w");
    if (!f) { perror("Erreur mouvements"); return; }
    NoeudMouv *cur = lm->tete;
    while (cur) {
        fprintf(f, "%d|%s|%d|%d|%.2f|%s|%s|%d\n",
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min, cur->article.prix,
                cur->mouv, cur->date, cur->qt);
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
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.article.nom, tok, sizeof(m.article.nom)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.qte = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.seuil_min = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; m.article.prix = (float)atof(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.mouv, tok, sizeof(m.mouv)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(m.date, tok, sizeof(m.date)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; m.qt = atoi(tok);
        ajouter_mouv_liste(lm, m);
    }
    fclose(f);
    printf("\n %d mouvement(s) charges depuis '%s'.\n", lm->nm, FICHIER_MOUVEMENTS);
}

void sauvegarder_alertes(ListeAlertes *la)
{
    FILE *f = fopen(FICHIER_ALERTES, "w");
    if (!f) { perror("Erreur alertes"); return; }
    NoeudAlerte *cur = la->tete;
    while (cur) {
        fprintf(f, "%d|%s|%d|%d|%f|%s|%s\n",
                cur->article.code, cur->article.nom,
                cur->article.qte,  cur->article.seuil_min, cur->article.prix,
                cur->type_alerte, cur->date);
        cur = cur->suiv;
    }
    fclose(f);
    printf("\n %d alerte(s) sauvegardee(s) dans '%s'.\n", la->na, FICHIER_ALERTES);
}

void charger_alertes(ListeAlertes *la)
{
    FILE *f = fopen(FICHIER_ALERTES, "r");
    if (!f) { printf("\n Aucun fichier alertes.\n"); return; }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        NoeudAlerte a; memset(&a, 0, sizeof(a));
        char *tok = strtok(ligne, "|"); if (!tok) continue; a.article.code = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(a.article.nom, tok, sizeof(a.article.nom)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; a.article.qte = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; a.article.seuil_min = atoi(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; a.article.prix = (float)atof(tok);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(a.type_alerte, tok, sizeof(a.type_alerte)-1);
        tok = strtok(NULL, "|"); if (!tok) continue; strncpy(a.date, tok, sizeof(a.date)-1);
        ajouter_alerte_liste(la, a);
    }
    fclose(f);
    printf("\n %d alerte(s) chargee(s) depuis '%s'.\n", la->na, FICHIER_ALERTES);
}

void sauvegarder_historique(ListeHist *lh)
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "w");
    if (!f) { perror("Erreur historique"); return; }
    NoeudHist *cur = lh->tete;
    while (cur) {
        fprintf(f, "%s|%s\n", cur->date, cur->message);
        cur = cur->suiv;
    }
    fclose(f);
    printf("\n %d notification(s) sauvegardee(s) dans '%s'.\n", lh->nb_h, FICHIER_HISTORIQUE);
}

void charger_historique(ListeHist *lh)
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "r");
    if (!f) { printf("\n Aucun fichier historique, notifications vides.\n"); return; }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        char *tok = strtok(ligne, "|"); if (!tok) continue;
        char date[11]; strncpy(date, tok, sizeof(date)-1); date[sizeof(date)-1] = '\0';
        tok = strtok(NULL, "|"); if (!tok) continue;
        ajouter_hist_liste(lh, date, tok);
    }
    fclose(f);
    printf("\n %d notification(s) chargee(s) depuis '%s'.\n", lh->nb_h, FICHIER_HISTORIQUE);
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
    printf("\n Donner le code      : "); scanf("%d",  &p.code);
    printf(" Donner le nom       : "); scanf("%19s",  p.nom);
    printf(" Donner la quantite  : "); scanf("%d",  &p.qte);
    printf(" Donner le seuil_min : "); scanf("%d",  &p.seuil_min);
    printf(" Donner le prix      : "); scanf("%f",  &p.prix);
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
    printf("\n %-6s %-20s %-6s %-10s %-8s\n",
           "Code", "Nom", "Qte", "Seuil_min", "Prix");
    printf(" --------------------------------------------------\n");
    NoeudProduit *cur = lp->tete;
    while (cur) {
        printf(" %-6d %-20s %-6d %-10d %.2f\n",
               cur->article.code, cur->article.nom,
               cur->article.qte,  cur->article.seuil_min, cur->article.prix);
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

void afficher_historique(ListeMouvs *lm)
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

void alerte_stock_faible(ListeProduits *lp, ListeAlertes *la,
                         ListeHist *lh, char date[])
{
    /* Vider l'ancienne liste d'alertes */
    liberer_alertes(la);

    printf("\n--- Produits a stock faible ---\n");
    NoeudProduit *cur = lp->tete;
    while (cur) {
        Produit *p = &cur->article;
        if (p->qte <= p->seuil_min && p->qte > 0) {
            NoeudAlerte a; memset(&a, 0, sizeof(a));
            a.article = *p;
            strcpy(a.date, date);
            strcpy(a.type_alerte, "faible");
            ajouter_alerte_liste(la, a);

            char msg[100];
            snprintf(msg, sizeof(msg), "faible:%s", p->nom);
            ajouter_hist_liste(lh, date, msg);

            printf("[ALERTE] %s | code:%d | qte:%d | min:%d\n",
                   date, p->code, p->qte, p->seuil_min);
        }
        cur = cur->suiv;
    }
}

void alerte_stock_rupture(ListeProduits *lp, ListeAlertes *la,
                          ListeHist *lh, char date[])
{
    liberer_alertes(la);

    printf("\n--- Produits en rupture de stock ---\n");
    NoeudProduit *cur = lp->tete;
    while (cur) {
        Produit *p = &cur->article;
        if (p->qte == 0) {
            NoeudAlerte a; memset(&a, 0, sizeof(a));
            a.article = *p;
            strcpy(a.date, date);
            strcpy(a.type_alerte, "rupture");
            ajouter_alerte_liste(la, a);

            char msg[100];
            snprintf(msg, sizeof(msg), "rupture:%s", p->nom);
            ajouter_hist_liste(lh, date, msg);

            printf("[RUPTURE] %s | code:%d | nom:%s\n",
                   date, p->code, p->nom);
        }
        cur = cur->suiv;
    }
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

void afficher_historique_notification(ListeHist *lh)
{
    if (lh->nb_h == 0) { printf("\n Historique vide.\n"); return; }
    printf("\n--- Historique des notifications ---\n");
    NoeudHist *cur = lh->tete;
    while (cur) {
        printf(" [%s] %s\n", cur->date, cur->message);
        cur = cur->suiv;
    }
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
        printf(" 3. Afficher l'historique\n");
        printf(" 0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: Enregistrer_achat(lm, lp); break;
            case 2: Enregistrer_vente(lm, lp); break;
            case 3: afficher_historique(lm);   break;
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

void m_inventaire_alerte(ListeProduits *lp, ListeAlertes *la, ListeHist *lh)
{
    int c; char d[11];
    do {
        printf("\n--- MODULE INVENTAIRE & ALERTES ---\n");
        printf(" 1. Afficher stock actuel\n");
        printf(" 2. Stock faible\n");
        printf(" 3. Rupture de stock\n");
        printf(" 4. Recherche par code\n");
        printf(" 5. Historique notifications\n");
        printf(" 0. Retour\n");
        printf("Choix : "); scanf("%d", &c);
        switch (c) {
            case 1: afficher_produits(lp);                                              break;
            case 2: date_du_jour(d); alerte_stock_faible(lp,la,lh,d);                  break;
            case 3: date_du_jour(d); alerte_stock_rupture(lp,la,lh,d);                 break;
            case 4: date_du_jour(d); recherche_alerte_par_code(lp,d);                  break;
            case 5: afficher_historique_notification(lh);                               break;
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
    ListeProduits lp  = { NULL, 0 };
    ListeMouvs    lm  = { NULL, 0 };
    ListeAlertes  la  = { NULL, 0 };
    ListeHist     lh  = { NULL, 0 };

    printf("\n=== Chargement des donnees ===\n");
    charger_produits(&lp);
    charger_mouvements(&lm);
    charger_alertes(&la);
    charger_historique(&lh);

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
            case 1: produit_menu(&lp);                         break;
            case 2: M_stock(&lp, &lm);                        break;
            case 3: m_inventaire_alerte(&lp, &la, &lh);       break;
            case 0:
                printf("\n Sauvegarde automatique...\n");
                sauvegarder_produits(&lp);
                sauvegarder_mouvements(&lm);
                sauvegarder_alertes(&la);
                sauvegarder_historique(&lh);
                liberer_produits(&lp);
                liberer_mouvs(&lm);
                liberer_alertes(&la);
                liberer_hist(&lh);
                printf(" Au revoir !\n");
                break;
            default: printf("\n Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
