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
 
void ajout_produit(struct produit t[], int *n)
{
    if (*n >= MAX)
    {
        printf("\n Tableau plein, impossible d'ajouter.\n");
        return;
    }
    int i = *n;
    *n = *n + 1;
 
    printf("\n Donner le code      : "); scanf("%d", &t[i].code);
    printf(" Donner le nom       : "); scanf("%s", t[i].nom);       
    printf(" Donner la quantite  : "); scanf("%d", &t[i].qte);
    printf(" Donner le seuil_min : "); scanf("%d", &t[i].seuil_min);
    printf(" Donner le prix      : "); scanf("%f", &t[i].prix);
 
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
    printf(" Nouveau prix (ancien = %f) : ", t[i].prix);
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
        printf("%d %s %d %d %.2f\n", t[i].code, t[i].nom, t[i].qte, t[i].seuil_min, t[i].prix);
        printf("\n");
    }
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
    {
        t[j] = t[j + 1];
    }
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
        strcpy(TabM[*nm].mouv, "sortie");           
 
        printf("Donner la date : ");
        scanf("%s", TabM[*nm].date);
 
        TabM[*nm].qt = q;
        TabP[i].qte -= q;
        (*nm)++;
        printf("Sortie enregistree avec succes\n");
        return;
    }
    printf("Produit non trouve\n");
}
 
void afficher_historique(struct mstock TabM[], int n)
{
    if (n == 0)
    {
        printf("Aucun mouvement\n");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        printf("Article  : %s\n",  TabM[i].article.nom);
        printf("Type     : %s\n",  TabM[i].mouv);
        printf("Date     : %s\n",  TabM[i].date);
        printf("Quantite : %d\n",  TabM[i].qt);
        printf("----------------------\n");
    }
}
 
/* ─── Sous-menu Mouvements ───────────────────────────────── */
/* BUG 6/7/9 corrige : np passe par pointeur pour refléter les ajouts de produits */
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
            case 1: Enregistrer_achat(tM, nm, t, *np);   break;
            case 2: Enregistrer_vente(tM, nm, t, *np);   break;
            case 3: afficher_historique(tM, *nm);         break; 
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
 
/* ─── Menu Principal ──────────────────────────────────────── */
int main()
{
    int np = 0, nm = 0;       
    struct produit tp[MAX];
    struct mstock  tM[MAX];
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
            case 1:
                produit(tp, &np);                  
                break;
            case 2:
                M_stock(tp, &np, tM, &nm);         
                break;
            case 3:
                printf("\n Module en cours de developpement.\n");
                break;
            case 0:
                printf("\n Au revoir !\n");
                break;
            default:
                printf("\n Choix invalide.\n");
        }
    } while (choix != 0);
 
    return 0;
}
 
