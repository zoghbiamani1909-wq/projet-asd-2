#include <stdio.h>
#include <stdlib.h>


int main()
{
    int choix;

    printf("=================================\n");
    printf("        APPLICATION SMARTSTOCK\n");
    printf("=================================\n");

    printf("1. Gestion des Produits\n");
    printf("2. Mouvements de Stock\n");
    printf("3. Inventaire et Alertes\n");
    printf("4. Quitter\n");

    printf("\nChoisir un module : ");
    scanf("%d",&choix);

    switch(choix)
    {
        case 1:
            printf("\n--- MODULE GESTION DES PRODUITS ---\n");
            printf("1. Ajouter un produit\n");
            printf("2. Modifier un produit\n");
            printf("3. Supprimer un produit\n");
            printf("4. Afficher la liste des produits\n");
            break;

        case 2:
            printf("\n--- MODULE MOUVEMENTS DE STOCK ---\n");
            printf("1. Enregistrer une entree\n");
            printf("2. Enregistrer une sortie\n");
            printf("3. Historique des mouvements\n");
            break;

        case 3:
            printf("\n--- MODULE INVENTAIRE ET ALERTES ---\n");
            printf("1. Afficher stock actuel\n");
            printf("2. Produits en stock faible\n");
            printf("3. Produits en rupture\n");
            break;

        case 4:
            printf("\nFin du programme.\n");
            break;

        default:
            printf("\nChoix invalide.\n");
    }

    return 0;
}
