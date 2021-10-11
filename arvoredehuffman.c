#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TADHuffman.h"

//Algoritmo para gerar os códigos de Huffman:

int codigoHuffman(Huffman *raiz, char str[], int i, FILE *arq) {
	
	if(raiz -> dir != raiz -> esq) {
		
		str[i++] = '0';
		i = codigoHuffman(raiz -> esq, str, i, arq);
		str[i++] = '1';
		i = codigoHuffman(raiz -> dir, str, i, arq);
	}
	else
		fprintf(arq,"%s\t\t%s\n", raiz -> info, str);
	str[i] = '\0';
	
	return --i;
}

void geraFraseCodificada(char str[]) {
	
	int i, j = 0, k = 0;
	char vetstr[200][30], palavra[30], cod[30], codaux[30];
	FILE *arqtabela = fopen("tabelaCodificada.txt","r"), *arqfrase = fopen("fraseCodificada.txt","w");
	
	for(i = 0; i <= strlen(str); i++) {
		
		if(str[i] != ' ')
			vetstr[j][k++] = str[i];
		else {
			
			vetstr[j][k] = '\0';
			j++;
			k = 0;
		}
	}
	i = 0;
	while(i <= j) {
		
		fscanf(arqtabela, "%s %s", palavra, cod);
		while(!feof(arqtabela) && strcmp(palavra, "\n") != 0) {
		
			if(strcmp(palavra, vetstr[i]) == 0)	
				strcpy(codaux, cod);
			fscanf(arqtabela, "%s %s", palavra, cod);	
		}
		fprintf(arqfrase, "%s", codaux);	
		fseek(arqtabela, 0, 0);
		i++;
	}
}

//Algoritmo para construção da Árvore de Huffman:

void constroiHuffman(Huffman **raiz, char str[]) {
	
	Huffman *h, *auxh1, *auxh2;
	Lista *listafreq;
	int i, j = 0, k = 0, freq, auxfreq1, auxfreq2, qtde = 0;
	char vetstr[200][30], auxstr[] = "", flag = 0;
	FILE *arqword = fopen("tabelaFrequencia.txt","w"), *arqcod= fopen("tabelaCodificada.txt","w+");
	
	initList(&listafreq);	
	for(i = 0; i <= strlen(str); i++) {
		
		if(str[i] != ' ')
			vetstr[j][k++] = str[i];
		else {
			
			vetstr[j][k] = '\0';
			j++;
			k = 0;
			qtde++;
		}
	}
	fputs("Palavras\tFrequencia\n\n", arqword);
	for(i = 0; i < qtde; i++) {
		
		if(stricmp(vetstr[i], ".") != 0) {
			
			freq = 1;
			for(j = i + 1; j <= qtde; j++) {
				
				if(stricmp(vetstr[i],vetstr[j]) == 0) {
					
					freq++;
					strcpy(vetstr[j], ".");
				}
			}
			if(!flag)
				fprintf(arqword, "%s\t\t%.2lf", vetstr[i], freq / (double)qtde);
			else	
				fprintf(arqword, "\n%s\t\t%.2lf", vetstr[i], freq / (double)qtde);
			flag++;	
			criaNo(&h, &vetstr[i]);
			insereLista(&listafreq, h, freq);
		}
	}
	while(!penultimo(listafreq)) {
		
		retiraLista(&listafreq, &auxh1, &auxfreq1);
		retiraLista(&listafreq, &auxh2, &auxfreq2);
		freq = auxfreq2 + auxfreq1;
		juntaArvore(&h, auxh1, auxh2);
		insereLista(&listafreq, h, freq);
	}
	retiraLista(&listafreq, &*raiz, &freq);
	codigoHuffman(*raiz, auxstr, 0, arqcod);
	fclose(arqword);
	fclose(arqcod);
}

//Algoritmo que abra os arquivos e mostre a frase decodificada na tela:

void geraFraseDecodificada() {
	
	int i, j = 0;
	char str[200], straux[30], palavra[30], auxpalavra[30], cod[30], flag = -1;
	FILE *arqtabela = fopen("tabelaCodificada.txt","r"), *arqfrase = fopen("fraseCodificada.txt","r");
	
	fscanf(arqfrase, " %[^\n]", str);
	for(i = 0; i < strlen(str); i++) {	
		
		straux[j] = str[i];
		straux[j+1] = '\0';
		fscanf(arqtabela, "%s %s", palavra, cod);
		while(!feof(arqtabela) && strcmp(palavra,"\n") != 0){
			
			if(strcmp(cod, straux) == 0) {
				
				strcpy(auxpalavra, palavra);
				flag++;
			}
			fscanf(arqtabela, "%s %s", palavra, cod);
		}
		if(!flag) {
			
			j = 0;
			printf("%s ", auxpalavra);
		}
		else
			j++;
		fseek(arqtabela,0,0);
		flag = -1;
	}
}

int main() {
	
	Huffman *raiz;
	char str[] = "amo como ama o amor nao conheco nenhuma outra razao para amar senao amar que queres que te diga alem de que te amo se o que quero dizer-te e que te amo";
	
	constroiHuffman(&raiz,str);
	geraFraseCodificada(str);
	geraFraseDecodificada();
	
	return 0;
}