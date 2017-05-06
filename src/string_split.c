#include <stdlib.h>
// #include <stdio.h>

/**
 * @brief Découpe une string en fonction d'un substring
 * @details Par exemple,
 * @author Nicolas Argyriou
 * 
 * ```C
 * string_split("A | B | C", " | ");
 * ```
 * Renverrait un tableau de cette forme:
 * t[0] = "A"
 * t[1] = "B"
 * t[2] = "C"
 * t[3] = (null)
 * 
 * Pourquoi un pointeur null: pour signifier au programmeur
 * la borne du tableau.
 * 
 * **EXEMPLE:**
 * ```C
 *		char** tab = string_split("A, B, C", ", ");
 *		for (int i = 0; tab[i] != NULL; ++i)
 *			printf("%s\n", tab[i]);
 * ```
 * @param str String à découper
 * @param substring Substring de séparation
 * 
 * @return Un tableau de char* n'allant pas au delà de t[0].
 */
char** string_split(char* str, char* substring){
	char** mots;
	unsigned int iMot = 0;      // Numéro du mot
	//char* iMot = 0x1;          // 
	unsigned int i = 0;			 // Position dans la string
	unsigned int j = 0;			 // Position dans la substring
	unsigned int tailleMot = 0;	 // Taille du mot courant

	mots = malloc(sizeof(char*));

	/*
	 * Nous parcourons le string et comparons chaque caractère
	 * par rapport au début du substring.
	 * Si nous trouvons que str[i] == substring[j], alors nous
	 * incrémentons j
	 * 
	 * Nous ajoutons aussi virtuellement un match à la toute fin
	 * de notre chaîne; car on souhaite aussi découper C dans "A,B,C"
	 * Même si C n'est pas suivi d'une virgule.
	 */

	while (1) {
		// Si finalement on ne trouve pas notre substring
		if (str[i] != substring[j] && str[i] != '\0') {
			// Nous revenons avant ces cases que nous avons cru être le début
			// de notre substring et reprenons la détection à la case d'après
			i = i - j;
			i++;
			tailleMot += 1;
			j = 0;
		}
		// Sinon, si ce que nous avons en face ressemble à notre substring
		else if (str[i] == substring[j] || str[i] == '\0'){
			j++;
			// Si nous avons un match complet
			if (substring[j] == '\0' || str[i] == '\0'){
				char c = str[i];
				j--;

				// On set remet au début de notre mot
				i -= j; 		// en enlevant la taille de notre substring
				i -= tailleMot; // et de notre mot
				
				/* 
				 * Si nous avons atteint la fin de notre string mais que nous
				 * étions en train de matcher quelque-chose, alors,
				 * ce que nous croyions être un match n'était en fait rien.
				 * Notre mot est donc plus grand que prévu.
				 */
				if (c == '\0')
					tailleMot += j;

				if (tailleMot > 0){
					// On rajoute un pointeur dans le tableau de mots
					mots = realloc(mots, (iMot+2) * sizeof(char*));
					// (le tableau est 1 case trop grande, pour le NULL)

					// Et on alloue de l'espace pour ce mot
					mots[iMot] = malloc(tailleMot);
//					printf("mots[%d] = %p\n", iMot, mots[iMot]);

					// On copie le mot dans notre tableau et on finit par un '\0'
					for (unsigned int n = 0; n < tailleMot; ++n)
						mots[iMot][n] = str[i+n];
					mots[iMot][tailleMot] = '\0';

					// Ca fait un mot de plus.
					iMot++;
				}
				if (c == '\0')
					break;

				// Réinitisalisation de la recherche
				i += j;
				i += tailleMot;
				j = 0;
				tailleMot = 0; 
			}
			i++;
		} 
	}


//	printf("mots[%d] = %p\n", iMot, mots[iMot]);
	mots[iMot] = NULL;
	//mots[0] = (char*)iMot;

	return mots;
}