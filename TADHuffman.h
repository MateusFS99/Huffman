struct huffman{
	
	char info[30];
	struct huffman *esq, *dir;
};

typedef struct huffman Huffman;

struct lista{
	
	int freq;
	struct huffman *tree;
	struct lista *prox;
};

typedef struct lista Lista;

void initTree(Huffman **h){
	
	*h = NULL;
}

void initList(Lista **l){
	
	*l = NULL;
}

void criaNo(Huffman **h, char *str){
	
	*h = (Huffman*)malloc(sizeof(Huffman));
	strcpy((*h) -> info,str);
	(*h) -> dir = (*h) -> esq = NULL;
}

void criaLista(Lista **l, Huffman *h, int freq){
	
	*l = (Lista*)malloc(sizeof(Lista));
	(*l) -> tree = h;
	(*l) -> prox = NULL;
	(*l) -> freq = freq;
}

void juntaArvore(Huffman **raiz, Huffman *t1, Huffman *t2){
	
	*raiz = (Huffman*)malloc(sizeof(Huffman));
	(*raiz) -> esq = t1;
	(*raiz) -> dir = t2;
}

void insereLista(Lista **l, Huffman *h, int freq){
	
	Lista *novo, *aux = *l, *ant = NULL;
	
	criaLista(&novo,h,freq);
	if(*l == NULL)
		*l = novo;
	else{
		
		while(aux != NULL && freq < aux -> freq){
			
			ant = aux;
			aux = aux -> prox;
		}
		if(aux == NULL)
			ant -> prox = novo;
		else if(ant == NULL){
			
			novo -> prox = *l;
			*l = novo;
		}
		else{
			
			novo -> prox = aux;
			ant -> prox = novo;
		}
	}
}

char penultimo(Lista *listafreq){
	
	return listafreq -> prox == NULL;
}

void retiraLista(Lista **l, Huffman **h, int *freq){
	
	Lista *aux = *l, *ant = NULL;
	
	while(aux -> prox != NULL){
		
		ant = aux;
		aux = aux -> prox;
	}
	*h = aux -> tree;
	*freq = aux -> freq;
	free(aux);
	if(ant != NULL)
		ant -> prox = NULL;
	else 
		*l = NULL;
}
