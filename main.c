#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIG_COMMAND 2048
#define FIRST_COMMAND 16
#define NAME_LENGHT 256
#define CONTENT_LIMIT 256


typedef struct Risorsa {
    char filename[NAME_LENGHT];
    _Bool isFile;
    struct Risorsa* firstChild;
    struct Risorsa* firstBrother;
    char content[CONTENT_LIMIT];
} Risorsa;

Risorsa Root;
int atleastone;

Risorsa* walktree(Risorsa *node, char percorso[], int value) {                            // Ritorna il padre di dove dobbiamo operare
    char* p = percorso;
    
    if (percorso[0]=='/' && value > 0) {                                                 // Siamo nel caso in cui abbiamo ancora il prefisso / e abbiamo altra roba dopo
        if (node->firstChild == NULL && value > 0) { return 0; }
        p++;
        node = node->firstChild;
    }
    
    if (value == 0) { return node;}         // Stiamo lavorando con il livello subito sotto la radice e dobbiamo rimanere li
    
    p = strtok(percorso, "/");
    
    while (value > 1) {
        if (strcmp(node->filename, p) == 0) {
            if (node->firstChild != NULL) { node = node->firstChild; value--; p = strtok(NULL, "/"); }
            else  { return 0; }
        }
        else {
            if (node->firstBrother != NULL && strcmp(node->filename, p) < 0 ) { node = node->firstBrother; }
            else { return 0;    }
        }
    }
    
    if (value == 1) {
        while (node->firstBrother != NULL && strcmp(node->filename, p) < 0)
        { node = node->firstBrother; }
        if (strcmp(node->filename, p) == 0) { return node; }
        else { return 0; }
    }
}

int read(Risorsa* Node, char percorso[], char* Target, int value) {
    
    Risorsa* currNode = walktree(&Root, percorso, value-1);
    if (currNode == 0) { printf("no\n"); return 0; }                        // Caso in cui non si è scesi in maniera lecita prima
    if (currNode->firstChild == NULL) { printf("no\n"); return 0; }        // Se il padre di dove dobbiamo operare non ha figli...
    
    currNode = currNode->firstChild;                                                        // ... se no scendiamo
    while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0)        // Spostiamoci fino a che non arriviamo al figlio che dobbiamo leggere
    { currNode = currNode->firstBrother; }
    if (currNode != NULL && currNode->isFile == 1 && strcmp(currNode->filename, Target) == 0 ) { printf("contenuto %s\n", currNode->content); return 1; }    // Se è leggibile stampa contenuto
    else { printf("no\n");    }
    return 0;
}

int write(Risorsa* Node, char contenuto[], char percorso[], char* Target, int value) {
    
    Risorsa* currNode = walktree(&Root, percorso, value-1);
    if (currNode == 0) { printf("no\n"); return 0; }                    // Caso in cui non si è scesi in maniera lecita prima
    if (currNode->firstChild == NULL) { printf("no\n"); return 0; }        // Se il padre di dove dobbiamo operare non ha figli...
    
    currNode = currNode->firstChild;                                                        // ... se no scendiamo
    while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0)         // Ci spostiamo lateralmente una volta giunti al livello giusto
    { currNode = currNode->firstBrother; }
    if (currNode != NULL && strcmp(currNode->filename, Target) == 0 && currNode->isFile == 1) { strcpy(currNode->content, contenuto); printf("ok %d\n", strlen(contenuto)); }
    else { printf("no\n"); }
    return 0;
}

int find(Risorsa* Node,char target[], char currentpath[]) {
    
    if (strcmp(Node->filename, target) == 0) {     printf("ok %s/%s\n", currentpath, Node->filename); atleastone = 1; }
    if (Node->firstChild != NULL) {    char temp[2048];    strcpy(temp, currentpath );    find(Node->firstChild, target, strcat(strcat(temp, "/"), Node->filename)); }
    if (Node->firstBrother != NULL) { find(Node->firstBrother, target, currentpath); }
}

int deleteNode(Risorsa *node, char percorso[], char* Target, int value) {
    
    Risorsa* currNode = walktree(&Root, percorso, value-1);
    if (currNode == 0) { printf("no\n"); return 0; }
    if (currNode->firstChild == NULL) { printf(("no\n")); return 0;} // Siamo arrivati al padre della risorsa che dobbiamo cancellare ma lui non c'è
    if (strcmp(currNode->firstChild->filename, Target) == 0 && currNode->firstChild->firstChild == NULL) {
        Risorsa* tempNode = currNode->firstChild->firstBrother; free(currNode->firstChild); currNode->firstChild = tempNode; printf("ok\n"); return 1;
    } // Siamo nel caso in cui dobbiamo fare fuori il figlio unico del padre
    Risorsa* Previous = currNode;
    currNode = currNode->firstChild;
    while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0)  // Ci spostiamo laterlmente una volta giunti al livello corretto
    { Previous = currNode; currNode = currNode->firstBrother; }
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
    
    Risorsa* currNode = walktree(&Root, percorso, value-1);
    if (currNode == 0) { printf("no\n"); return 0; }
    if (currNode->firstChild == NULL) { printf("no\n"); return 0;    }
    if (strcmp(currNode->firstChild->filename, Target) == 0) {
        Risorsa* tempNode = currNode->firstChild->firstBrother;
        freerecursive(currNode->firstChild);
        currNode->firstChild = tempNode;
        printf("ok\n");
    return 1;    } // Siamo nel caso in cui dobbiamo fare fuori il figlio del padre
    Risorsa* Previous = currNode;
    currNode = currNode->firstChild;
    while (currNode->firstBrother != NULL && strcmp(currNode->filename, Target) < 0)
    { Previous = currNode; currNode = currNode->firstBrother; }
    if (strcmp(currNode->filename, Target) == 0) {
        Previous->firstBrother = currNode->firstBrother; freerecursive(currNode->firstChild); free(currNode);
    printf("ok\n"); return 1; }
    else { printf("no\n"); return 0; }
    
}

int insertiontree(Risorsa* Node, char percorso[], char* toAdd, int value, _Bool isFile){ /* Va bene sia per create che per create.dir, in base a _Bool isFile*/
    
    Risorsa* currNode = walktree(&Root, percorso, value-1);
    if (currNode == 0 || currNode->isFile == 1) { printf("no\n"); return 0;    } // Se c'è stato un errore nella discesa o il supposto padre è un FILE e non una DIRECTORY non possiamo proseguire
    else {
        if (currNode->firstChild == NULL) { // Caso inserimento come primo e unico figlio
            Risorsa* newNode = malloc(sizeof(Risorsa));
            strcpy(newNode->filename, toAdd);        newNode->isFile = isFile;    newNode->firstBrother = NULL;    newNode->firstChild = NULL; if(isFile) { strcpy(newNode->content, "");} // Facciamo una basic declaration per il nuovo nodo
        currNode->firstChild = newNode; printf("ok\n"); return 0;    }
        if (strcmp(toAdd, currNode->firstChild->filename) <= 0 ) { // Caso di inserimento in cui dobbiamo inserire il nuovo nodo come primo dei figli!
            if (strcmp(toAdd, currNode->firstChild->filename) == 0)  {    printf("no\n"); return 0;    }
            Risorsa* newNode = malloc(sizeof(Risorsa));
            strcpy(newNode->filename, toAdd);        newNode->isFile = isFile;    newNode->firstBrother = NULL;    newNode->firstChild = NULL; if(isFile) { strcpy(newNode->content, "");} // Facciamo una basic declaration per il nuovo nodo
            newNode->firstBrother = currNode->firstChild; currNode->firstChild = newNode;
            printf("ok\n");
            return 1;
        }
        else { // Inserimento in mezzo ai fratelli
            currNode = currNode->firstChild;
            while(currNode->firstBrother != NULL && strcmp(toAdd, currNode->firstBrother->filename) > 0)
            { currNode = currNode->firstBrother;    }
            if (strcmp(toAdd, currNode->filename) == 0)  { printf("no\n"); return 0;    }
            Risorsa* newNode = malloc(sizeof(Risorsa));
            strcpy(newNode->filename, toAdd);        newNode->isFile = isFile;    newNode->firstBrother = NULL;    newNode->firstChild = NULL; if(isFile) { strcpy(newNode->content, "");}//Facciamo una basic declaration per il nuovo nodo
            newNode->firstBrother = currNode->firstBrother; currNode->firstBrother = newNode;
            printf("ok\n");
            return 1;
        }
    }
}

int main(){
    
    char firstpart[FIRST_COMMAND];
    int exit_command = 0;
    
    strcpy(Root.filename, "");
    
    do {                                    // Main loop logic
        
        scanf("%s", firstpart);             // Storing first part of command
        
        /*
        _____                _
        / ____|              | |
        | |     _ __ ___  __ _| |_ ___
        | |    | '__/ _ \/ _` | __/ _ \
        | |____| | |  __/ (_| | ||  __/
        \_____|_|  \___|\__,_|\__\___|
        */
        if (strcmp(firstpart, "create") == 0) {
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }  // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){      before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
            insertiontree(&Root, temp, before, count, 1);
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
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }   // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){        before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
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
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }   // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){    before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
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
            
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            char temp1[CONTENT_LIMIT];
            scanf("%s", temp1);
            
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }   // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){       before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
            // Saltiamo il primo ed ultimo carattere in quanto sono apici brutti >""<
            
            char* tempp = temp1;
            tempp++;
            tempp[strlen(tempp)-1] = 0;
            
            
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
            
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }   // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){    before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
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
            char temp[BIG_COMMAND];
            scanf("%s", temp);
            
            int i, count;    for (i=0, count=0; temp[i]; i++) {  count += (temp[i] == '/'); }   // Count number of delimiter '/' inside the target_path of the command
            char str[BIG_COMMAND];    strcpy(str, temp);      char * pch; char * before;
            pch = strtok (str,"/");
            while (pch != NULL){      before = pch;        pch = strtok (NULL, "/");} // Tokenizer the string, now used as a Debug, need before to keep the filename
            
            deleteRecursive(&Root, temp, before, count);
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
            char currpath[BIG_COMMAND];
            char temp[BIG_COMMAND];
            char buffer[BIG_COMMAND];
            scanf("%s", temp);
            strcpy(buffer, "");
            atleastone = 0; // To check if we don't find matches
            printf("%s", buffer);
            find(Root.firstChild, temp, buffer);
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
            // The end is COMING
            exit_command = 1;
        }
        
        /*else {
            printf("Huston we have a problem! A parsing problem with %s", firstpart);
            return -1;
        }*/
    }
    while (exit_command != 1);
    return 0;
}
