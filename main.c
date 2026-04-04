#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct produit
{
    int code;
    char nom[20];
    int qte;
    int seuil_min;
    float prix;
};

struct mstock {
    struct produit article;
    char mouv[10];
    char date[20];
    int qt;
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


/* ─── Recherche de la position d'un produit par code ──────── */
int pos(struct produit t[], int n, int mcode)
{
    int i = 0;
    while (i < n)
    {
        if (t[i].code == mcode)
            return i;
        i++;
    }
    return -1;
}

/* ─── MODULE PRODUITS ────────────────────────────────────── */

void ajout_produit(struct produit t[], int *n)
{
    if (*n >= MAX)
    {
        printf("\n Tableau plein, impossible d'ajouter.\n");
        return;
    }
    int i = *n;
    *n = *n + 1;

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
    printf("\n Donner le code du produit a modifier : ");
    scanf("%d", &mcode);

    int i = pos(t, n, mcode);

    if (i == -1)
    {
        printf("\n Ce code n'existe pas.\n");
        return;
    }

    float mprix;
    printf(" Nouveau prix (ancien = %.2f) : ", t[i].prix);
    scanf("%f", &mprix);
    t[i].prix = mprix;
    printf("\n Prix modifie avec succes !\n");
}

void afficher_produits(struct produit t[], int n)
{
    if (n == 0)
    {
        printf("\n Aucun produit en stock.\n");
        return;
    }
    printf("\n %s %s %s %s %s\n", "Code", "Nom", "Qte", "Seuil_min", "Prix");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d %s %d %d %f\n",
               t[i].code, t[i].nom, t[i].qte, t[i].seuil_min, t[i].prix);
    }
}


void afficher_stock(struct produit t[], int n)
{
    afficher_produits(t, n);
}

void supp_produit(struct produit t[], int *n)
{
    if (*n == 0) { printf("il n'y a pas de produits a supprimer\n"); return; }
    int p;
    printf("donner le code de produit a supprimer : "); scanf("%d", &p);
    int i = pos(t, *n, p);

    if (i == -1)
    {
        printf("\n Ce code n'existe pas.\n");
        return;
    }
    for (int j = i; j < *n - 1; j++)
        t[j] = t[j + 1];
    *n = *n - 1;
    printf("\n le produit est supprime avec succes\n");
}

/* ─── MODULE MOUVEMENTS ──────────────────────────────────── */

void Enregistrer_achat(struct mstock TabM[], int *nm, struct produit TabP[], int np)
{
    int code;
    printf("Donner le code produit : ");
    scanf("%d", &code);

    int i = pos(TabP, np, code);

    if (i != -1)
    {
        TabM[*nm].article = TabP[i];
        strcpy(TabM[*nm].mouv, "achat");

        printf("Donner la date : ");
        scanf("%s", TabM[*nm].date);

        printf("Donner la quantite : ");
        scanf("%d", &TabM[*nm].qt);

        TabP[i].qte += TabM[*nm].qt;
        (*nm)++;
        printf("Entree enregistree avec succes\n");
        return;
    }
    printf("Produit non trouve\n");
}

void Enregistrer_vente(struct mstock TabM[], int *nm, struct produit TabP[], int np)
{
    int code;
    printf("Donner le code produit : ");
    scanf("%d", &code);

    int i = pos(TabP, np, code);

    if (i != -1)
    {
        printf("Donner la quantite : ");
        int q;
        scanf("%d", &q);

        if (q > TabP[i].qte)
        {
            printf("Stock insuffisant !\n");
            return;
        }

        TabM[*nm].article = TabP[i];
        strcpy(TabM[*nm].mouv, "vente");

        printf("Donner la date : ");
        scanf("%s", TabM[*nm].date);

        TabM[*nm].qt = q;
        TabP[i].qte -= q;
        (*nm)++;
        printf("vente enregistree avec succes\n");
        return;
    }
    printf("Produit non trouve\n");
}

void afficher_historique(struct mstock TabM[], int n)
{
    if (n == 0)
    {
        printf("\n Aucun mouvement\n");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        printf("\n Article  : %s\n", TabM[i].article.nom);
        printf("Type     : %s\n",   TabM[i].mouv);
        printf("Date     : %s\n",   TabM[i].date);
        printf("Quantite : %d\n",   TabM[i].qt);
        printf("----------------------\n");
    }
}

/* ─── MODULE ALERTES ET NOTIFICATIONS ───────────────────── */


void enregister_alerte_memoire(alerte hist[], int *nb_h, char date[], char msg_text[])
{
    int i = *nb_h;
    strcpy(hist[i].date,    date);
    strcpy(hist[i].message, msg_text);
    *nb_h = i + 1;
}

void afficher_historique_notification(alerte hist[], int nb_h)
{
    if (nb_h == 0)
    {
        printf("\n historique vide\n");
        return;
    }
    printf("\n---historique---\n");
    for (int i = 0; i < nb_h; i++)
        printf("[%s] %s\n", hist[i].date, hist[i].message);
}

void saisir_date(char d[])
{
    printf("\n saisie de la date");
    printf("\n entrez la date (jj/mm/aaaa) : ");
    scanf("%s", d);
}


void alerte_stock_faible(struct produit tab1[], int n1, t_alerte tab3[], int *n3,
                         alerte hist[], int *nb_h, char date[])
{
    int k = 0;
    printf("\n---liste des produits a faible stock---\n");
    for (int i = 0; i < n1; i++)
    {
        if (tab1[i].qte <= tab1[i].seuil_min && tab1[i].qte > 0)
        {
            tab3[k].article = tab1[i];
            strcpy(tab3[k].date, date);
            strcpy(tab3[k].type_alerte, "faible");          

            char msg[100];
            strcpy(msg, "faible:");
            strcat(msg, tab1[i].nom);

            printf("[critique!!] date:%s | code:%d | nom:%s | quantite:%d | min:%d | prix:%.2f | alerte:%s\n",
                   tab3[k].date,
                   tab3[k].article.code,
                   tab3[k].article.nom,
                   tab3[k].article.qte,
                   tab3[k].article.seuil_min,
                   tab3[k].article.prix,          
                   tab3[k].type_alerte);

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
    printf("\n---liste des produits en rupture de stock---\n");
    for (int i = 0; i < n1; i++)
    {
        if (tab1[i].qte == 0)
        {
            tab3[k].article = tab1[i];
            strcpy(tab3[k].date, date);
            strcpy(tab3[k].type_alerte, "rupture");          

            char msg[100];
            strcpy(msg, "rupture:");
            strcat(msg, tab1[i].nom);

            printf("[critique!!] date:%s | code:%d | nom:%s | quantite:%d | min:%d | prix:%f | alerte:%s\n",
                   tab3[k].date,
                   tab3[k].article.code,
                   tab3[k].article.nom,
                   tab3[k].article.qte,
                   tab3[k].article.seuil_min,
                   tab3[k].article.prix,           
                   tab3[k].type_alerte);

            enregister_alerte_memoire(hist, nb_h, date, msg); 
            k++;
        }
    }
    *n3 = k;
}

void recherche_alerte_par_code(struct produit tab1[], int n1, char date[])
{
    int code_rech, trouve = 0;
    printf("\n---recherche du statut d'alerte---\n");
    printf("entrez le code du produit a verifier : ");
    scanf("%d", &code_rech);

    for (int i = 0; i < n1; i++)
    {
        if (tab1[i].code == code_rech)
        {
            trouve = 1;
            if (tab1[i].qte == 0)
                printf("[alerte rupture] date:%s | code:%d | nom:%s | quantite:0\n",
                       date, tab1[i].code, tab1[i].nom);
            else if (tab1[i].qte < tab1[i].seuil_min)
                printf("[alerte faible] date:%s | code:%d | nom:%s | quantite:%d (min:%d)\n",
                       date, tab1[i].code, tab1[i].nom, tab1[i].qte, tab1[i].seuil_min);
            else
                printf("le produit %s (code:%d) est en stock suffisant\n",
                       tab1[i].nom, tab1[i].code);
            break;
        }
    }
    if (trouve == 0)
        printf("aucun produit trouve avec le code %d\n", code_rech);
}

/* ─── Sous-menu Mouvements ───────────────────────────────── */
void M_stock(struct produit t[], int *np, struct mstock tM[], int *nm)
{
    int c;
    do
    {
        printf("\n--- MODULE MOUVEMENT DU STOCK ---\n");
        printf("1. Ajouter un achat\n");
        printf("2. Ajouter une vente\n");
        printf("3. Afficher l'historique\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &c);

        switch (c)
        {
            case 1: Enregistrer_achat(tM, nm, t, *np);  break;
            case 2: Enregistrer_vente(tM, nm, t, *np);  break;
            case 3: afficher_historique(tM, *nm);        break;
            case 0: break;
            default: printf("\n Choix invalide.\n");
        }
    } while (c != 0);
}

/* ─── Sous-menu Produits ─────────────────────────────────── */
void produit(struct produit t[], int *n)
{
    int c;
    do
    {
        printf("\n--- MODULE GESTION DU PRODUIT ---\n");
        printf("1. Ajouter un produit\n");
        printf("2. Modifier le prix d'un produit\n");
        printf("3. Afficher la liste des produits\n");
        printf("4. Supprimer un produit\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &c);

        switch (c)
        {
            case 1: ajout_produit(t, n);       break;
            case 2: modif_prix_produit(t, *n); break;
            case 3: afficher_produits(t, *n);  break;
            case 4: supp_produit(t, n);        break;
            case 0: break;
            default: printf("\n Choix invalide.\n");
        }
    } while (c != 0);
}

/* ─── Sous-menu Alertes et Notifications ─────────────────── */

void m_inventaire_alerte(struct produit tab1[], int n1, t_alerte tab3[], int *n3,
                         alerte hist[], int *nb_h)
{
    int c3;
    char d[11];

    do
    {
        printf("\n--- module 3 : Inventaire et Alertes ---\n");
        printf("  1. Afficher stock actuel\n");
        printf("  2. Filtrer : stock faible\n");
        printf("  3. Filtrer : rupture de stock\n");
        printf("  4. Recherche par code\n");
        printf("  5. Afficher historique notifications\n");
        printf("  0. Quitter\n");
        printf("  Votre choix : ");
        scanf("%d", &c3);

        switch (c3)
        {
            case 1: afficher_stock(tab1, n1);                                              break;
            case 2: saisir_date(d); alerte_stock_faible(tab1, n1, tab3, n3, hist, nb_h, d);  break;
            case 3: saisir_date(d); alerte_stock_rupture(tab1, n1, tab3, n3, hist, nb_h, d); break;
            case 4: saisir_date(d); recherche_alerte_par_code(tab1, n1, d);                   break;
            case 5: afficher_historique_notification(hist, *nb_h);                        break;
            case 0: printf("retour au menu principal\n");                                  break;
            default: printf("\n  erreur : choix invalide !\n");                            break;
        }
    } while (c3 != 0);
}

/* ─── Menu Principal ──────────────────────────────────────── */
int main()
{
    int np = 0, nm = 0, na = 0, nb_h = 0;
    struct produit  tp[MAX];
    struct mstock   tM[MAX];
    t_alerte        alertes[MAX];
    alerte          historique[MAX];   
    int choix;

    do
    {
        printf("\n=================================\n");
        printf("        APPLICATION SMARTSTOCK\n");
        printf("=================================\n");
        printf("1. Gestion des Produits\n");
        printf("2. Mouvements de Stock\n");
        printf("3. Inventaire et Alertes\n");
        printf("0. Quitter\n");
        printf("\nChoisir un module : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1: produit(tp, &np);                                          break;
            case 2: M_stock(tp, &np, tM, &nm);                                break;
            case 3: m_inventaire_alerte(tp, np, alertes, &na, historique, &nb_h); break;
            case 0: printf("\n Au revoir !\n");                                break;
            default: printf("\n Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
