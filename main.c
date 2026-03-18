#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct produit
{
    int code;
    char nom[20];
    int qte;
    int seuil_min;
    float prix;
};



/* ─── Recherche de la position d'un produit par code ──────── */
int pos(struct produit t[], int n, int mcode)
{
    int i = 0;
    while (i < n)
    {
        if (t[i].code == mcode)
            return i;       /* trouvé : on retourne l'index */
        i++;
    }
    return -1;              /* non trouvé */
}

/* ─── Ajout d'un produit ──────────────────────────────────── */
void ajout_produit(struct produit t[], int *n)
{
    if (*n >= MAX)
    {
        printf("\n Tableau plein, impossible d'ajouter.\n");
        return;
    }
    int i =*n;
    *n =*n +1;

    printf("\n Donner le code      : "); scanf("%d", &t[i].code);
    printf(" Donner le nom       : "); scanf("%s", &t[i].nom);
    printf(" Donner la quantite  : "); scanf("%d", &t[i].qte);
    printf(" Donner le seuil_min : "); scanf("%d", &t[i].seuil_min);
    printf(" Donner le prix      : "); scanf("%f", &t[i].prix);

    printf("\n Produit ajoute avec succes !\n");
}

/* ─── Modification du prix d'un produit ──────────────────── */
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

/* ─── Affichage de tous les produits ──────────────────────── */
void afficher_produits(struct produit t[], int n)
{
    if (n == 0)
    {
        printf("\n Aucun produit en stock.\n");
        return;
    }
    printf("\n %s %s %s %s %s\n","Code", "Nom", "Qte", "Seuil_min", "Prix");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d %s %d %d %f\n",t[i].code, t[i].nom, t[i].qte, t[i].seuil_min, t[i].prix);
        printf("\n");
    }
}
/* ─── supprimer un prosuit  ──────────────────────── */
void supp_produit(struct produit t[],int *n)
{
    if (*n==0){printf("il n'ya pas de produits a supprimer \n"); return;}
    int p;
    printf("donner le code de produit a supprimer : ");scanf("%d",&p);
    int i = pos(t, *n, p);

    if (i == -1)
    {
        printf("\n Ce code n'existe pas.\n");
        return;
    }
    for(int j=i;j<*n-1;j++)
    {
        t[j]=t[j+1];
    }
    *n=*n-1;
    printf("\n le produit est supprime avec succes \n");
    return;
}


void M_produit(struct produit t[], int* n)
{
    int c;

    do
    {
        printf("\n--- MODULE GESTION DES PRODUITS ---\n");
        printf("1. Ajouter un produit\n");
        printf("2. Modifier le prix d'un produit\n");
        printf("3. Afficher la liste des produits\n");
        printf("4.supprimer un produit\n ");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &c);

        switch (c)
        {
            case 1: ajout_produit(t, n);        break;
            case 2: modif_prix_produit(t, *n);   break;
            case 3: afficher_produits(t, *n);   break;
            case 4: supp_produit(t,n); break;
            case 0: break;
            default: printf("\n Choix invalide.\n");
        }
    } while (c != 0);
}

/* ─── Menu Principal ──────────────────────────────────────── */
int main()
{
    int n=0;
    struct produit t[MAX];
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
                M_produit(t, &n);
                break;
            case 2:
                printf("\n Module en cours de developpement.\n");
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
