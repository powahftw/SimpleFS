#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_LENGHT 256
#define CONTENT_LIMIT 256
#define print printf

#if 0
  #define SPAM(a) printf a
#else
  #define SPAM(a) (void)0
#endif

// POWDERED BY CAFFEINE, SPERANZE DI LAUREA E BLACK MAGIC

*/
typedef struct Risorsa {
	char filename[NAME_LENGHT];
	_Bool isFile;
    struct Risorsa* firstChild;
	struct Risorsa* firstBrother;
	char content[CONTENT_LIMIT];
} Risorsa;

Risorsa Root;
int atleastone;

void printtree(Risorsa* node){ //QUESTA FUNZIONE E' COME LA VIA VITA = INUTILE
	if (node==NULL) { printf("-");	}
	else {
		printf("[%s|%d]",node->filename, node->isFile);
		printtree(node->firstBrother);
		printtree(node->firstChild);
	}
}

Risorsa* walktree(Risorsa *node, char percorso[], int value) {

	// Utility function. Given the root of the tree and the target path return the PARENT just up that! 
	// IN: Risorsa* Root, Percorso, Value OUT: Risorsa* Parent
	
	SPAM(("CurrNodo: |%s| Percorso: |%s| Valore: |%d|", node->filename, percorso, value)); // Debugging

	if (percorso[0]=='/' && value>0) { 												// Siamo nel caso in cui abbiamo ancora il prefisso / e abbiamo altra roba dopo 
	if (node->firstChild == NULL && value > 0) { return 0; }						// Siamo nel caso in cui non abbiamo neanche un figlio della radice, e stiamo provando a aggiungere troppe sottorisorse
	char* p = percorso; p++; SPAM(("\t #Good boyz don't play with ROOT#\n")); return walktree(node->firstChild, p, value); }

	if (value==0) { return node;} // Stiamo lavorando con la radice e dobbiamo rimanere li
	
	char *target = strtok(percorso, "/");	SPAM(("\nTarget = |%s|\n", target));
	
	while (strcmp(node->filename, target) < 0) {
		SPAM(("|%s| ha come 1F |%s|\n",node->filename, node->firstBrother->filename));
		if (node->firstBrother == NULL) {
			SPAM(("Errore mentre scende"));
			return 0; // questo è l'errore mentre scende
		}
		else {
			//printf("Ci spostiamo al fratello |%s|\n", node->firstBrother->filename);
			node = node->firstBrother;
		}
	}
		
	if (value == 1) { SPAM(("Trovato! %s\n", node->filename)); return node;	}
	if (node->firstChild != NULL) { SPAM(("Scendiamo!\n"));	walktree(node->firstChild, strtok(NULL, "!"), value-1); } //Controllo se possiamo scendere vericalmente
	else { SPAM(("Non possiamo scendere verticalmente\n")); return 0; }
	} 

int read(Risorsa* Node, char percorso[], char* Target, int value) {
	
	/* DA COMPLETARE, QUELLO CHE DOVREBBE FARE E' CHE LEGGE IL FILE PASSATO COME PERCORSO E SE E' UN FILE STAMPA IL CONTENUTO, E' IMPORTANTE FARE QUALCOSA PER TUTTI 
	I PARAM INUTILI CHE VENGONO PASSATI OGNI VOLTA*/
	
	SPAM(("\nWe are trying to READ %s\n", percorso));
	Risorsa* currNode = walktree(&Root, percorso, value-1);
	if (currNode == 0) { SPAM(("Errore")); printf("no\n"); return 0; }
	currNode = currNode->firstChild;
	SPAM(("\n\n%s\n\n", currNode->filename));
	while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0) { currNode = currNode->firstBrother;	}
	SPAM(("%d %d %d\n", (currNode == NULL), (currNode->isFile == 1), (strcmp(currNode->filename, Target)== 0)));
	if (currNode != NULL && currNode->isFile == 1 && strcmp(currNode->filename, Target) == 0 ) { printf("contenuto %s\n", currNode->content); return 1; }
	else { printf("no\n");	}
	return 0;
	}

int write(Risorsa* Node, char contenuto[], char percorso[], char* Target, int value) {
	
	SPAM(("\nWe are trying to WRITE %s with %s\n", percorso, contenuto));
	Risorsa* currNode = walktree(&Root, percorso, value-1);
	SPAM(("We returned %s", currNode->filename));
	if (currNode == 0) { SPAM(("Errore\n")); return 0; }
	currNode = currNode->firstChild;
	//print("\n%s\n%s\n", currNode->filename, Target);
	while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0) { currNode = currNode->firstBrother;	}
	if (currNode != NULL && strcmp(currNode->filename, Target) == 0 && currNode->isFile == 1) { strcpy(currNode->content, contenuto); printf("ok %u\n", strlen(contenuto)); }
	else { printf("no\n"); }
	return 0;
	}
	
int find(Risorsa* Node,char target[]) {
	if (strcmp(Node->filename, target) == 0) { printf("ok %s\n"); atleastone = 1;	}
	if (Node->firstChild != NULL) {	find(Node->firstChild, target); }
	if (Node->firstBrother != NULL) { find(Node->firstBrother, target); }
	}


int deleteNode(Risorsa *node, char percorso[], char* Target, int value) {
	SPAM(("\nWe are trying to DELETE %s\n", percorso));
	Risorsa* currNode = walktree(&Root, percorso, value-1);
	if (currNode == 0) { print("no\n"); return 0; }
	SPAM(("\n\n Lavorando con delete siamo arrivati a lavorare con il padre: %s\n\n", currNode->filename));
	if (currNode->firstChild == NULL) { SPAM(("no\n")); return 0;} // Siamo arrivati al padre della risorsa che dobbiamo cancellare ma lui non c'è
	if (strcmp(currNode->firstChild->filename, Target) == 0) { Risorsa* tempNode = currNode->firstChild->firstBrother; free(currNode->firstChild); currNode->firstChild = tempNode; printf("ok\n"); return 1;	} // Siamo nel caso in cui dobbiamo fare fuori il figlio unico del padre
	Risorsa* Previous = currNode;
	currNode = currNode->firstChild;
	while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0) {	Previous = currNode; currNode = currNode->firstBrother; }
	if (strcmp(currNode->filename, Target) == 0 && currNode->firstChild == NULL) { Previous->firstBrother = currNode->firstBrother; free(currNode); printf("ok\n"); return 1; }
	else { printf("no\n"); return 0; }
	}

void freerecursive(Risorsa *node) {
	if (node!=NULL) {
		freerecursive(node->firstBrother);
		freerecursive(node->firstChild);
		free(node);
	} 
}

int deleteRecursive(Risorsa *node, char percorso[], char* Target, int value) {
	SPAM(("\n We will try to DELETE RECURSEVELY OR HOW THE FUCK IT'S TYPED %s\n", percorso));
	Risorsa* currNode = walktree(&Root, percorso, value-1);
	if (currNode == 0) { printf("no\n"); return 0; }
	if (strcmp(currNode->firstChild->filename, Target) == 0) { 
		Risorsa* tempNode = currNode->firstChild->firstBrother; 
		freerecursive(currNode->firstChild);
		currNode->firstChild = tempNode;
        printf("ok\n"); 
		return 1;	} // Siamo nel caso in cui dobbiamo fare fuori il figlio del padre
	Risorsa* Previous = currNode;
	currNode = currNode->firstChild;
	while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0) {	Previous = currNode; currNode = currNode->firstBrother; }
	if (strcmp(currNode->filename, Target) == 0) { 
        SPAM(("PREVIOUS : %s PREVIOUS BROTHER: %s CURRENT: %s CURRENT BROTHER: %s\n", Previous->filename, Previous->firstBrother, currNode->filename, currNode->firstBrother->filename));
        Previous->firstBrother = currNode->firstBrother; freerecursive(currNode->firstChild); free(currNode);
        printf("ok\n"); return 1; }
	else { printf("no\n"); return 0; }
		
		
}
int insertiontree(Risorsa* Node, char percorso[], char* toAdd, int value, _Bool isFile){ /* Va bene sia per create che per create.dir, in base se si è fatta l'impl accennata sotto*/
	
	/* Qualche commento a caldo su questo codice, per ora funziona, bisogna fare dei test più approfonditi. Uno dei problemi più gravi è l'uguale. Andranno implementati
	degli if per individuare l'eventuale uguaglianza e ritornare errore: non possiamo fare un inserimento di qualcosa già presente. Un altra piccola cosa utile sarebbe
	implementare un argomento di funzione extra al fine di lavorare sia con inserimento file che per inserimento cartelle, quello che cambia è che durante la creazione con la 
	malloc va settato il parametro isfile UPDATE: Sembra che abbia aggiunto isBool, bisogna vedere se funzionerà*/
	
	//#########################################################################################################################################################################################
	/* Qualche commento a Freddo su questo codice. Probabilmente qua è dove parte la roba di SIGKILL11, che è il numero di volte che ucciderei chi ha pensato di rompere cosi tanto il *****
	Potrebbe essere sensato fare la malloc solo l'istante prima di inserire, in modo da evitare aggiunta di roba che non viene smaltita correttamente. Usiamo il toAdd per fare i confronti invece di lavorare con il node
	(Che secondo me è anche più veloce) Può anche avere senso aggiungere un fantastico parametro per tenere il percorso salvato e fare la find con meno rotture */
	Risorsa* currNode = walktree(&Root, percorso, value-1);
	if (currNode == 0 || currNode->isFile == 1) { SPAM(("Abbiamo sbagliato prima, o si tratta di un file!!!\n\n")); printf("no\n"); return 0;	}
	else {
		Risorsa* newNode = malloc(sizeof(Risorsa)); //Allochiamo spazio per il nuovo nodo 
		strcpy(newNode->filename, toAdd);		newNode->isFile = isFile;	newNode->firstBrother = NULL;	newNode->firstChild = NULL; if(isFile) { strcpy(newNode->content, "");}//Facciamo una basic declaration per il nuovo nodo
		
		SPAM(("\n\nWe are trying to add |%s|\n", toAdd));
		if (currNode->firstChild == NULL) { currNode->firstChild = newNode; printf("ok\n"); return 0;} // Caso di inserimento in cui dobbiamo semplicemente metterlo come figli
		if (strcmp(toAdd, currNode->firstChild->filename) <= 0 ) { // Caso di inserimento in cui dobbiamo inserire il nuovo nodo come primo dei figli!
			if (strcmp(toAdd, currNode->firstChild->filename) == 0)  {	SPAM(("Abbiamo gia' il figlio!")); printf("no\n"); free(newNode); return 0;	}
			SPAM(("\nStiamo lavorando con |%s| e suo figlio |%s|, in mezzo metteremo |%s|\n\n", currNode->filename, currNode->firstChild->filename, newNode->filename));
			newNode->firstBrother = currNode->firstChild; currNode->firstChild = newNode;
			SPAM(("Vediamo ora le cose! Il padre e' |%s|, il figlio inserito e' |%s| ed il suo vecchio figlio era |%s|\n", currNode->filename, currNode->firstChild->filename, currNode->firstChild->firstBrother->filename));
			printf("ok\n");
			return 1;
			}
		else {
			currNode = currNode->firstChild;
			while(currNode->firstBrother != NULL && strcmp(toAdd, currNode->firstBrother->filename) > 0) { SPAM(("looping"));	currNode = currNode->firstBrother;	}
			SPAM((" in the sky with diamonds\n")); 
			if (strcmp(toAdd, currNode->filename) == 0)  {	SPAM(("Abbiamo già il figlio!\n")); printf("no\n"); free(newNode); return 0;	}
			SPAM(("HELLO"));
			newNode->firstBrother = currNode->firstBrother; currNode->firstBrother = newNode;
			SPAM(("\nIl vecchio nodo era |%s|, stiamo aggiungendo |%s| in mezzo a lui e |%s|\n", currNode->filename, currNode->firstBrother->filename, currNode->firstBrother->firstBrother->filename));
			printf("ok\n");
			return 1;
			}
			
	}
}

int main(){
	
	char firstpart[50];
	int exit_command = 0;
	
	strcpy(Root.filename, "/");
	
	SPAM(("BENVENUTO NEL FANTASTICO FILESYSTEM\n\n"));
	
	SPAM(("Il nome del coso e' %s", Root.filename));
	do {
		SPAM(("\n\\(^^)/   T A K E   M Y   E N E R G Y     \\(^^)/ \n"));
		
		scanf("%s", firstpart); //scanf per il primo, vado di strcmp e poi 
		//printf("%s\n", firstpart);
		//firstpart = strtok(command, " ");
		//printf("%s", firstpart);
		/*
		     _____                _       
 		   / ____|              | |      
 		  | |     _ __ ___  __ _| |_ ___ 
		 | |    | '__/ _ \/ _` | __/ _ \
		 | |____| | |  __/ (_| | ||  __/
 		 \_____|_|  \___|\__,_|\__\___|
		*/
		if (strcmp(firstpart, "create") == 0) {
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo CREATE con %s\n", temp));//strtok(NULL, " "));
			
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			insertiontree(&Root, temp, before, count, 1);
			// create(percorso)
		}
		/*
		  _____                _            _ _      
 		 / ____|              | |          | |(_)     
 		| |     _ __ ___  __ _| |_ ___   __| |_ _ __ 
 		| |    | '__/ _ \/ _` | __/ _ \ / _` | | '__|
 		| |____| | |  __/ (_| | ||  __/| (_| | | |   
 		 \_____|_|  \___|\__,_|\__\___(_)__,_|_|_|  
		*/
		if (strcmp(firstpart, "create_dir") == 0 || strcmp(firstpart, "cd") == 0) {
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo CREATE DIRECTORY con %s\n", temp));//strtok(NULL, " "));
			
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			insertiontree(&Root, temp, before, count, 0);
			//insertiontree(&Node, char percorso[], char* toAdd, int value, _Bool 0);
			// create.dir(percorso)
		}
		/*
		  _____                _ 
 		 |  __ \              | |
		 | |__) |___  __ _  __| |
		 |  _  // _ \/ _` |/ _` |
		 | | \ \  __/ (_| | (_| |
		 |_|  \_\___|\__,_|\__,_|
		*/
		if (strcmp(firstpart, "read") == 0) {
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo READ con %s\n", temp));//strtok(NULL, " "));
			
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			read(&Root, temp, before, count);
			// read(percorso)
		}
		/*
		 __          __   _ _       
		 \ \        / /  (_) |      
		  \ \  /\  / / __ _| |_ ___ 
   		   \ \/  \/ / '__| | __/ _ \
   		    \  /\  /| |  | | ||  __/
     		\/  \/ |_|  |_|\__\___|
	    */
		if (strcmp(firstpart, "write") == 0) {
			
			char temp[1024];
			scanf("%s", temp);
			char temp1[1024];
			scanf("%s", temp1);
			
			
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			// Saltiamo il primo ed ultimo carattere in quanto sono apici brutti >""<
			
			char* tempp = temp1;
			tempp++;
			tempp[strlen(tempp)-1] = 0;
		
			SPAM(("Stiamo eseguendo WRITE con %s e CONTENUTO %s\n", temp, tempp));//strtok(NULL, " "), strtok(NULL, " "));
			
			write(&Root,tempp, temp, before, count);
			// write(percorso, contenuto)
		}
		/*
		  _____       _      _       
		 |  __ \     | |    | |      
		 | |  | | ___| | ___| |_ ___ 
 		| |  | |/ _ \ |/ _ \ __/ _ \
 		| |__| |  __/ |  __/ ||  __/
 		|_____/ \___|_|\___|\__\___|
		*/
		if (strcmp(firstpart, "delete") == 0) {
			
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo DELETE con %s\n", temp));// strtok(NULL, " "));
			
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			deleteNode(&Root, temp, before, count);
			// elimina(percorso)
		}
		/*
		  _____       _      _              
 		 |  __ \     | |    | |             
 		 | |  | | ___| | ___| |_ ___   _ __ 
		 | |  | |/ _ \ |/ _ \ __/ _ \ | '__|
 		 | |__| |  __/ |  __/ ||  __/_| |   
 		|_____/ \___|_|\___|\__\___(_)_|  
		*/
		if (strcmp(firstpart, "delete_r") == 0) {
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo DELETE RECURSIVO con %s\n", temp));// strtok(NULL, " "));
			
			// elimina_recursivo(percorso)
			int i, count;	for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Conta il numero di '/' dentro il path di temp
			//printf("\nConta '/' 1st Command |%d|\n", count);		//	printf("\n\n###### DISSECTING PATH ###### STEP-ASIDE ######\n\n"); // Dissezione del path con strtok
			char str[1024];	strcpy(str, temp);  	char * pch; char * before;
    		//printf ("Splitting string \"%s\" into tokens:\n", temp);
 			pch = strtok (str,"/");
    		while (pch != NULL){    	SPAM(("%s\n",pch));	before = pch;    	pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
			
			deleteRecursive(&Root, temp, before, count);
			// create(percorso)
		}
		/*
		  ______ _           _ 
 		|  ____(_)         | |
 		| |__   _ _ __   __| |
 		|  __| | | '_ \ / _` |
 		| |    | | | | | (_| |
	    |_|    |_|_| |_|\__,_|
		*/
		if (strcmp(firstpart, "find") == 0) {
			char currpath[1024];
			char temp[1024];
			scanf("%s", temp);
			SPAM(("Stiamo eseguendo FIND con %s\n", temp));//strtok(NULL, " "));
			
            atleastone = 0;
			find(&Root, temp);
            if (atleastone == 0) { printf("no\n"); }
			// find(percorso)
		}
		/*
		 |  ____|    (_) |  
	 	 | |__  __  ___| |_ 
	 	 |  __| \ \/ / | __|
		 | |____ >  <| | |_ 
		 |______/_/\_\_|\__|
        */
		if (strcmp(firstpart, "exit") == 0) {
			SPAM(("Stiamo terminando questo abomino\n"));
			SPAM(("Python do it better"));
			exit_command = 1;
		}
		if (strcmp(firstpart, "print") == 0) {
			printf("\n\n###### PRINT-TREE###### STEP-ASIDE ######\n\n");+
			printf("CURR - BROTHER - SON\n");
			printtree(&Root);
			printf("\n\n");
		}
		/*else {
		printf("Huston we have a problem! A parsing problem with %s", firstpart);
		return -1;
		}*/
	}
	while (exit_command != 1);
	return 0;
}
