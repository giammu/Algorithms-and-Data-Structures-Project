#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_COMANDO 18
#define ALTEZZA_BUF 10000 //si può cambiare
#define DIM_ALFABETO 78
#define RED 0
#define BLACK 1

//Nodo RB
struct Nodo{
    struct Nodo *left;
    struct Nodo *right;
    struct Nodo *padre;
    int color;
    char* parola;
};

//Nodo BST
struct NodoBST{
    struct NodoBST *left;
    struct NodoBST *right;
    struct NodoBST *padre;
    char* parola;
};

//struttura per i vincoli
struct Vincolo{
    int appartiene; //dice se il carattere è nella parola (=1), o non è nella parola (=0) [fino a prova contraria è nella parola, quindi all'inizio sono tutti 1]
    int num_min; //numero minimo di volte che compare -> mi serve per salvare contatore dopo ogni parola (se aumenta)
    int num_esatto; //numero esatto di volte che compare (lo capisco se trovo / dopo + o | )
    int contatore; //conto quante volte compare il carattere nella parola che sto confrontando, fino a num_esatto
    int occorrenze_riferimento; //numero di volte che il carattere compare nella parola di riferimento
    int *non_compare; //lungo quanto la parola, dice dove non compare (=1 dove non compare) (quando trova '|')
};

//fatta da me al posto di strcmp:
//confrontastringhe: se prima parola è > seconda, ritorna 1; se seconda parola è > prima, ritorna 0.
int confrontaStringheNodo(struct Nodo nodo1, struct Nodo nodo2, int lunghezza_parole2){
    int i=0;
    for(i=0;i<lunghezza_parole2;i++){
        if(nodo1.parola[i] > nodo2.parola[i] ){
            return 1;
        }
        else{
            if(nodo2.parola[i] > nodo1.parola[i] ){
                return 0;
            }
        }
    }
    return 2; //le parole confrontate sono uguali
}

int confrontaStringheNodoBST(struct NodoBST nodo1, struct NodoBST nodo2, int lunghezza_parole2){
    int i=0;
    for(i=0;i<lunghezza_parole2;i++){
        if(nodo1.parola[i] > nodo2.parola[i] ){
            return 1;
        }
        else{
            if(nodo2.parola[i] > nodo1.parola[i] ){
                return 0;
            }
        }
    }
    return 2; //le parole confrontate sono uguali
}

//leftRotate: presa da pseudocodice
void leftRotate(struct Nodo *alfabeto[], int indice_alfabeto, struct Nodo* x, struct Nodo *nil2){
    struct Nodo* y = x->right;
    x->right = y->left;
    if(y->left != nil2){
        y->left->padre = x;
    }
    y->padre=x->padre;
    if(x->padre == nil2){
        alfabeto[indice_alfabeto] = y;
    }
    else if(x == x->padre->left){
        x->padre->left = y;
    }
    else{
        x->padre->right = y;
    }

    y->left = x;
    x->padre = y;
}

//rightRotate: presa da pseudocodice
void rightRotate(struct Nodo *alfabeto[], int indice_alfabeto, struct Nodo* y, struct Nodo *nil3){
    struct Nodo* x = y->left;
    y->left = x->right;
    if(x->right != nil3){
        x->right->padre = y;
    }
    x->padre=y->padre;
    if(y->padre == nil3){
        alfabeto[indice_alfabeto] = x;
    }
    else if(y == y->padre->left){
        y->padre->left = x;
    }
    else{
        y->padre->right = x;
    }

    x->right = y;
    y->padre = x;
}

//riparaRB: presa da pseudocodice
void riparaRBinserisci(struct Nodo *alfabeto[], int indice_alfabeto, struct Nodo* z, struct Nodo* nil4){ //z è il nodo inserito
    struct Nodo *y;
    while(z->padre->color == RED){
        if(z->padre == z->padre->padre->left){ //Se z è figlio sinistro del nonno
            y = z->padre->padre->right; //y è lo zio
            if(y->color == RED){
                z->padre->color = BLACK;
                y->color = BLACK;
                z->padre->padre->color = RED;
                z = z->padre->padre;
            }
            else{
                if(z == z->padre->right){
                    z = z->padre;
                    leftRotate(alfabeto, indice_alfabeto, z, nil4);
                }
                z->padre->color = BLACK;
                z->padre->padre->color = RED;
                rightRotate(alfabeto, indice_alfabeto, z->padre->padre, nil4);
            }
        }
        else{ //Se z è figlio destro del nonno
            y = z->padre->padre->left; //y è lo zio
            if(y->color == RED){
                z->padre->color = BLACK;
                y->color = BLACK;
                z->padre->padre->color = RED;
                z = z->padre->padre;
            }
            else{
                if(z == z->padre->left){
                    z = z->padre;
                    rightRotate(alfabeto, indice_alfabeto, z, nil4);
                }
                z->padre->color = BLACK;
                z->padre->padre->color = RED;
                leftRotate(alfabeto, indice_alfabeto, z->padre->padre, nil4);
            }
        }
    }
    alfabeto[indice_alfabeto]->color = BLACK;
}

//inserisci Red-Black Tree: presa da pseudocodice
void inserisciRB(struct Nodo *alfabeto[], int indice_alfabeto, struct Nodo* daInserire, int lunghezza_parole3, struct Nodo* nil1){
    struct Nodo* radice = alfabeto[indice_alfabeto];
    struct Nodo* pre = nil1;
    struct Nodo* cur = radice;

    while(cur != nil1){
        pre = cur;
        if(confrontaStringheNodo(*cur, *daInserire, lunghezza_parole3)){ //ritorna 1 se cur>daInserire
            cur = cur->left;
        }
        else{
            cur = cur->right;
        }
    }
    daInserire->padre=pre;
    if(pre==nil1){ //se la cur=radice è NIL allora anche pre è NIL, allora inserisco l'elemento come radice
        alfabeto[indice_alfabeto] = daInserire;
        daInserire->color = BLACK; //radice è nera
        daInserire->padre = nil1; //il padre della radice è nil
    }
    else{
        if(confrontaStringheNodo(*pre, *daInserire, lunghezza_parole3)){ //ritorna 1 se pre>daInserire
            pre->left=daInserire;
        }
        else
        {
            pre->right=daInserire;
        }
    }
    daInserire->left= nil1;
    daInserire->right= nil1; //left e right sono nil perchè il nodo viene inserito come nei BST alla fine
    //non coloro il nodo di rosso perchè l'ho già fatto nella funzione inserisci_dizionario

    riparaRBinserisci(alfabeto, indice_alfabeto, daInserire, nil1);
}

struct NodoBST* minimoBST(struct NodoBST *radice){ //trova il minimo: presa da pseudocodice
    struct NodoBST *cur = radice;
    while(cur->left != NULL){
        cur = cur->left;
    }
    return cur;
}

struct NodoBST* successoreBST(struct NodoBST *nodo){ //trova il successore: presa da pseudocodice

    if(nodo->right != NULL){
        return minimoBST(nodo->right);
    }
    struct NodoBST *y = nodo->padre;
    while((y!=NULL)&&(y->right==nodo)){
        nodo =y;
        y=y->padre;
    }
    return y;
}

//cancellazione: presa da pseudocodice
void cancellaNodo(struct NodoBST* filtrate[], int indice, struct NodoBST *nodo){ //elimina il nodo "nodo" o una foglia
    struct NodoBST* da_cancellare;
    struct NodoBST* sotto;

    if((nodo->left == NULL)||(nodo->right==NULL)){ // se ha solo un figlio o nessun figlio
        da_cancellare = nodo;
    }
    else{ //se ha 2 figli
        da_cancellare = successoreBST(nodo);
    }

    if(da_cancellare->left != NULL){//se ha un figlio a sinistra
        sotto = da_cancellare->left;
    }
    else{
        sotto = da_cancellare->right; //potrebbe essere nil
    }

    if(sotto != NULL){ //se il figlio non è nil
        sotto->padre=da_cancellare->padre; //sistemo il padre del figlio
    }

    if(da_cancellare->padre == NULL){ //se sto cancellando la radice
        filtrate[indice] = sotto; //sistemo la nuova radice (può essere anche nil se sotto=nil)
    }
    else if(da_cancellare == da_cancellare->padre->left){ //se sto cancellando il figlio sinistro
        da_cancellare->padre->left = sotto; //sistemo il figlio
    }
    else{ //altrimenti se sto cancellando il figlio destro
        da_cancellare->padre->right = sotto; //sistemo il figlio
    }

    if(da_cancellare != nodo){ //questo lo fa nel caso 3 quando ci sono 2 figli
        nodo->parola = da_cancellare->parola;
    }

    free(da_cancellare);
}

//inserimento: preso da pseudocodice
void inserisciBST(struct NodoBST *filtrate[], int indice_alfabeto, struct NodoBST* daInserire, int lunghezza_parole3){

    struct NodoBST* radice = filtrate[indice_alfabeto];
    struct NodoBST* pre = NULL;
    struct NodoBST* cur = radice;

    while(cur != NULL){
        pre = cur;
        if(confrontaStringheNodoBST(*cur, *daInserire, lunghezza_parole3)){ //ritorna 1 se cur>daInserire
            cur = cur->left;
        }
        else{
            cur = cur->right;
        }
    }
    daInserire->padre=pre;
    if(pre==NULL){ //se la cur=radice è NIL allora anche pre è NIL, allora inserisco l'elemento come radice
        filtrate[indice_alfabeto] = daInserire;
        daInserire->padre = NULL; //il padre della radice è nil
    }
    else{
        if(confrontaStringheNodoBST(*pre, *daInserire, lunghezza_parole3)){ //ritorna 1 se pre>daInserire
            pre->left=daInserire;
        }
        else
        {
            pre->right=daInserire;
        }
    }
}


void freeBST(struct NodoBST *radice){
    //-----cancella ricorsivamente tutto l'albero
    if(radice->left != NULL){
        freeBST(radice->left);
    }

    if(radice->right != NULL){
        freeBST(radice->right);
    }

    free(radice);
}

//visita in ordine e stampa il RB
void inOrderPrint(struct Nodo radice, struct Nodo *nil5, int lunghezza_parole){

    if(radice.left != nil5){
        inOrderPrint(*radice.left, nil5, lunghezza_parole);
    }

    int i;
    for(i=0;i<lunghezza_parole;i++){
        printf("%c",radice.parola[i]);
    }
    printf("\n");

    if(radice.right != nil5){
        inOrderPrint(*radice.right, nil5, lunghezza_parole);
    }
}

void inOrderPrintBST(struct NodoBST radice, struct NodoBST *nilBST, int lunghezza_parole){

    if(radice.left != nilBST){
        inOrderPrintBST(*radice.left, nilBST, lunghezza_parole);
    }

    int i;
    for(i=0;i<lunghezza_parole;i++){
        printf("%c",radice.parola[i]);
    }
    printf("\n");

    if(radice.right != nilBST){
        inOrderPrintBST(*radice.right, nilBST, lunghezza_parole);
    }
}

//legge il primo numero dell'input = lunghezza parole
int lettura_numero(){
    int num=0, input;
    while( (input=getchar_unlocked()) != '\n' ){
        num = (num<<3) + (num<<1) + (input - '0') ; //lo shift equivale a moltiplicare per 10 ma è più veloce
    }
    return num ;
}

//inizializzo il buffer con il dollaro
void buffer_dollaro(int larghezza_buf1, char buffer[][larghezza_buf1]){
    int i,f;
    for(i=0;i<ALTEZZA_BUF;i++){
        for(f=0;f<larghezza_buf1;f++){
            buffer[i][f]='$';
        }
    }
}

//riempie il buffer con lo stdin fino a EOF o alla dimensione massima del buffer
void input_buffer(int larghezza_buf2, char buffer[][larghezza_buf2]){
    int a=0, b=0, bool=0;
    buffer[a][b]='$'; //serve per far rispettare la condizione nel while
    while( (buffer[a][b] != EOF) && (a<(ALTEZZA_BUF)) && (bool == 0) ){
        while( b<(larghezza_buf2) && (bool == 0) ){
            buffer[a][b]=(char)getchar_unlocked();
            if(buffer[a][b] == '\n'){
                buffer[a][b] = '$';//alla fine della parola c'è uno o più asterischi e non c'è più \n
                break;
            }
            else{
                if(buffer[a][b] == EOF)
                {
                    buffer[a][b]='%'; //se trovo EOF metto %
                    bool=1; //se raggiungo la fine del file, uso bool per interrompere entrambi i cicli
                }
                else{
                    b++;
                }
            }
        }
        b=0;
        a++;
        if(a == (ALTEZZA_BUF-1)){
            buffer[a][b] ='!';   //se arrivo alla fine del buffer ma non ho ancora letto tutto l'input metto un !
            break;
        }
    }
}

void riempi_buffer(int larghezza_buf5, char buffer[][larghezza_buf5], int *indice ){
    buffer_dollaro(larghezza_buf5, buffer); //pulisco con $
    //riempie il buffer con lo stdin fino a EOF o alla dimensione massima del buffer
    input_buffer(larghezza_buf5, buffer); //leggo l'input
    *indice=0; //perchè altrimenti il prossimo ciclo termina il for
}

//caso in cui è la prima parola e devo costruire l'albero delle filtrate, scansionando il dizionario in ordine
int filtra_nodo1(struct Nodo *nodo, struct NodoBST* filtrate[], struct Vincolo *vincoli,
                 int *conta_filtrate, int lunghezza_parole, char giuste_posto_giusto[],
                 int *prima_parola, struct NodoBST *massimoBST[]){

    int h;
    int indice_alfabeto;
    int contatore[DIM_ALFABETO] = {0};

    //Provo a filtrare ogni caratteristica
    for(h=0;h<lunghezza_parole;h++){ //per ogni carattere della parola

        indice_alfabeto = nodo->parola[h] - '-';

        if(vincoli[indice_alfabeto].appartiene != 1){
            return 0;
        }

        if(vincoli[indice_alfabeto].non_compare[h] == 1){
            return 0;
        }

        if( (nodo->parola[h] != giuste_posto_giusto[h])&&(giuste_posto_giusto[h] != '$') ){
            return 0;
        }

        //Altrimenti se il carattere va bene:
        contatore[indice_alfabeto] = contatore[indice_alfabeto] +1; //incremento le occorrenze di quel carattere
    }

    //se arrivo qui significa che tutti i caratteri potrebbero andare bene se presi singolarmente nella loro posizione
    //Ora ne Controllo le occorrenze:
    for(h=0; h<DIM_ALFABETO; h++){

        //se conosco il numero esatto ma è sbagliato + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto>0)&&(contatore[h] != vincoli[h].num_esatto) ){
            return 0;
        }

        //se conosco il numero minimo ma non ho abbastanza occorrenze + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto==0)&&(vincoli[h].num_min>0)&&(contatore[h] < vincoli[h].num_min) ){
            return 0;
        }
    }
    //se dopo tutte le condizioni la parola non è stata scartata, allora può andare bene

    //faccio la malloc e lo inserisco nell'albero delle filtrate
    struct NodoBST *inserisci = malloc( sizeof(struct Nodo) );
    inserisci->left = NULL;
    inserisci->right = NULL;
    inserisci->padre = NULL;
    inserisci->parola = nodo->parola; //il nodo filtrato punta alla parola del nodo nel dizionario
    *conta_filtrate = *conta_filtrate +1;
    int indice = nodo->parola[0] -'-';

    if(filtrate[indice] != NULL){ //lo metto nel massimo
        massimoBST[indice]->right=inserisci;
        inserisci->padre = massimoBST[indice];
        massimoBST[indice] = inserisci;
    }
    else{ //è il primo elemento (la radice)
        filtrate[indice] = inserisci;
        massimoBST[indice] = inserisci;
    }
    return 1;
}

//caso in cui ho l'inserimento durante una partita
int filtra_nodo3(struct Nodo *nodo, struct NodoBST* filtrate[], struct Vincolo *vincoli,
                 int *conta_filtrate, int lunghezza_parole, char giuste_posto_giusto[]){

    int h;
    int indice_alfabeto;
    int contatore[DIM_ALFABETO] = {0};

    //Provo a filtrare ogni caratteristica
    for(h=0;h<lunghezza_parole;h++){ //per ogni carattere della parola

        indice_alfabeto = nodo->parola[h] - '-';
        if(vincoli[indice_alfabeto].appartiene != 1){
            return 0;
        }
        if(vincoli[indice_alfabeto].non_compare[h] == 1){
            return 0;
        }
        if( (nodo->parola[h] != giuste_posto_giusto[h])&&(giuste_posto_giusto[h] != '$') ){
            return 0;
        }
        //Altrimenti se il carattere va bene:
        contatore[indice_alfabeto] = contatore[indice_alfabeto] +1; //incremento le occorrenze di quel carattere
    }

    //se arrivo qui significa che tutti i caratteri potrebbero andare bene se presi singolarmente nella loro posizione
    //Ora ne Controllo le occorrenze:
    for(h=0; h<DIM_ALFABETO; h++){
        //se conosco il numero esatto ma è sbagliato + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto>0)&&(contatore[h] != vincoli[h].num_esatto) ){
            return 0;
        }
        //se conosco il numero minimo ma non ho abbastanza occorrenze + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto==0)&&(vincoli[h].num_min>0)&&(contatore[h] < vincoli[h].num_min) ){
            return 0;
        }
    }
    //se dopo tutte le condizioni la parola non è stata scartata, allora può andare bene

    //faccio la malloc e lo inserisco nell'albero delle filtrate
    struct NodoBST *inserisci = malloc( sizeof(struct Nodo) );
    inserisci->left = NULL;
    inserisci->right = NULL;
    inserisci->padre = NULL;
    inserisci->parola = nodo->parola; //il nodo filtrato punta alla parola del nodo nel dizionario
    *conta_filtrate = *conta_filtrate +1;
    int indice = nodo->parola[0] -'-';
    inserisciBST(filtrate, indice, inserisci, lunghezza_parole);
    return 1;
}

//caso in cui devo filtrare l'albero delle filtrate    --- NB: gli devo passare il nodo dell'albero di filtrate
int filtra_nodo2(struct NodoBST *nodo, struct NodoBST* filtrate[], struct Vincolo *vincoli,
                 int *conta_filtrate, int lunghezza_parole, const char giuste_posto_giusto[]){

    int h;
    int indice_alfabeto;
    int indice = nodo->parola[0] -'-';
    int contatore[DIM_ALFABETO] = {0};

    //Provo a filtrare ogni caratteristica
    for(h=0;h<lunghezza_parole;h++){ //per ogni carattere della parola

        indice_alfabeto = nodo->parola[h] - '-';

        if(vincoli[indice_alfabeto].appartiene != 1){
            cancellaNodo(filtrate, indice, nodo); //cancella il nodo e fa la free
            *conta_filtrate = *conta_filtrate -1;
            return 0;
        }

        if(vincoli[indice_alfabeto].non_compare[h] == 1){
            cancellaNodo(filtrate, indice, nodo); //cancella il nodo e fa la free
            *conta_filtrate = *conta_filtrate -1;
            return 0;
        }

        if( (nodo->parola[h] != giuste_posto_giusto[h])&&(giuste_posto_giusto[h] != '$') ){
            cancellaNodo(filtrate, indice, nodo); //cancella il nodo e fa la free
            *conta_filtrate = *conta_filtrate -1;
            return 0;
        }

        //Altrimenti se il carattere va bene:
        contatore[indice_alfabeto] = contatore[indice_alfabeto] +1; //incremento le occorrenze di quel carattere
    }

    //se arrivo qui significa che tutti i caratteri potrebbero andare bene se presi singolarmente nella loro posizione
    //Ora ne Controllo le occorrenze:
    for(h=0; h<DIM_ALFABETO; h++){

        //se conosco il numero esatto ma è sbagliato + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto>0)&&(contatore[h] != vincoli[h].num_esatto) ){
            cancellaNodo(filtrate, indice, nodo); //cancella il nodo e fa la free
            *conta_filtrate = *conta_filtrate -1;
            return 0;
        }

        //se conosco il numero minimo ma non ho abbastanza occorrenze + se dovrebbe esserci ma non c'è
        if( (vincoli[h].num_esatto==0)&&(vincoli[h].num_min>0)&&(contatore[h] < vincoli[h].num_min) ){
            cancellaNodo(filtrate, indice, nodo); //cancella il nodo e fa la free
            *conta_filtrate = *conta_filtrate -1;
            return 0;
        }
    }
    //se dopo tutte le condizioni la parola non è stata scartata, allora può andare bene
    //lascio il nodo dov'è
    return 1;
}

//Leggo dal buffer le parole da inserire nel dizionario e fa una grossa malloc per tutte
void inserisci_dizionario(int larghezza_buf3, int lunghezza_parole1, char buffer[][larghezza_buf3], int *indice_inizio_inserimento,
                          struct Nodo* alfabeto[], struct Nodo *nil, int *conta_dizionario, int *conta_filtrate, int vincoli_attivi, int nella_partita,
                          struct Vincolo *vincoli, char giuste_posto_giusto[], struct NodoBST* filtrate[]){
    //indico la riga di inizio e fine del buffer dove sono le parole da inserire e quante sono
    int numero_parole;
    int indice_fine_inserimento = *indice_inizio_inserimento;
    while( (buffer[indice_fine_inserimento][0] != '%') && (buffer[indice_fine_inserimento][0] != '!')
           && (buffer[indice_fine_inserimento][0] != '+') ){ //NB funziona anche se incontra +inserisci_fine
        indice_fine_inserimento++;//alla fine è = al primo elemento da non inserire
    }
    numero_parole = indice_fine_inserimento - *indice_inizio_inserimento;
    *conta_dizionario = *conta_dizionario + numero_parole; //conta il numero di parole totali nel dizionario fin'ora

    //alloco spazio in memoria per la struttura
    struct Nodo* da_inserire = malloc( numero_parole*( sizeof(struct Nodo)  ) );

    //alloco spazio in memoria per le parole
    char *parole = malloc(numero_parole*lunghezza_parole1);

    //inserisco nella struttura appena allocata le parole, ed inizializzo gli altri parametri
    int indice_parole=0;
    int i,j;
    for(i=0;i<numero_parole;i++){
        da_inserire[i].left = nil;
        da_inserire[i].right = nil;
        da_inserire[i].padre = NULL;
        da_inserire[i].parola = &parole[indice_parole];
        da_inserire[i].color = RED;
        for(j=0; j<lunghezza_parole1; j++){
            da_inserire[i].parola[j] = buffer[*indice_inizio_inserimento+i][j];
        }
        indice_parole += lunghezza_parole1;
    }

    //chiamo la funzione di inserimento nel Tree su ogni parola (ma prima devo calcolare in che cella dell'array 'alfabeto' si trova)
    int indice_alfabeto;
    struct Nodo * nodo;
    for(i=0; i<numero_parole; i++){
        //calcolo il numero della cella dell'array in cui si trova la parola in base al primo carattere
        indice_alfabeto = da_inserire[i].parola[0] - '-';

        if(vincoli_attivi == 1){ //Se devo controllare che la parola rispetti i vincoli
            nodo = &da_inserire[i];
            filtra_nodo3(nodo, filtrate, vincoli, conta_filtrate, lunghezza_parole1, giuste_posto_giusto);
        }
        //funzione di inserimento della parola nell'albero del dizionario
        inserisciRB(alfabeto, indice_alfabeto, &da_inserire[i], lunghezza_parole1, nil);
    }

    //scelgo come cambiare l'indice i nel main:
    if( (buffer[indice_fine_inserimento][0]=='!')||(buffer[indice_fine_inserimento][0]=='%') ){
        *indice_inizio_inserimento=indice_fine_inserimento-1; //cambia l'indice 'i' del main che indica dove finiscono le parole da inserire
        //e diventa = all'ultima parola inserita
    }
    if(buffer[indice_fine_inserimento][0]=='+'){
        *indice_inizio_inserimento=indice_fine_inserimento; //cambia l'indice 'i' del main che indica dove finiscono le parole da inserire
        //e diventa = al comando
    }
}

//confronta_parole: se sono uguali ritorna 1, altrimenti ritorna 0
int parole_uguali(const char parola_rif[], const char parola_buf[], int lunghezza_parole){
    int i;
    for(i=0;i<lunghezza_parole;i++){
        if(parola_rif[i] != parola_buf[i]){
            return 0;
        }
    }
    return 1;
}

//ricerca nel RB scritta da me
int ricerca(struct Nodo *radice, struct Nodo *parola, struct Nodo *nil6, int lunghezza_parole4){
    if(radice != nil6){
        while(1){
            if(confrontaStringheNodo(*radice, *parola, lunghezza_parole4) == 2){ //se è uguale
                return 1;
            }

            if(confrontaStringheNodo(*parola, *radice, lunghezza_parole4)){ //se parola > radice
                if(radice->right != nil6){
                    radice = radice->right;
                }
                else{
                    return 0; //arriva alla fine dell'albero senza trovarlo
                }
            }
            else{ //se parola < radice
                if(radice->left != nil6){
                    radice = radice->left;
                }
                else{
                    return 0; //arriva alla fine dell'albero senza trovarlo
                }
            }
        }
    }
    return 0;
}

//COSTRUISCO L'ALBERO DI FILTRATE RICORSIVO: scorro il dizionario
void inOrder_filtra1(struct Nodo *radice_alfabeto, struct NodoBST *filtrate[], struct Nodo *nil6, struct Vincolo *vincoli,
                     int *conta_filtrate, int lunghezza_parole, char giuste_posto_giusto[], int *prima_parola, struct NodoBST* massimoBST[]){

    if(radice_alfabeto->left != nil6){
        inOrder_filtra1(radice_alfabeto->left, filtrate, nil6, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto, prima_parola, massimoBST);
    }
    //verifica le condizioni e filtra
    filtra_nodo1( radice_alfabeto, filtrate, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto, prima_parola, massimoBST);

    if(radice_alfabeto->right != nil6){
        inOrder_filtra1(radice_alfabeto->right, filtrate, nil6, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto, prima_parola, massimoBST);
    }
}

//FILTRO LE FILTRATE RICORSIVO: scorro le filtrate
void inOrder_filtra4(struct NodoBST *radice_filtrate,struct NodoBST *filtrate[], struct Vincolo *vincoli,
                     int *conta_filtrate, int lunghezza_parole, char giuste_posto_giusto[]){

    if(radice_filtrate->left != NULL){
        inOrder_filtra4(radice_filtrate->left, filtrate, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto);
    }

    if(radice_filtrate->right != NULL){
        inOrder_filtra4(radice_filtrate->right, filtrate, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto);
    }

    //verifica le condizioni e filtra
    filtra_nodo2( radice_filtrate, filtrate, vincoli, conta_filtrate, lunghezza_parole, giuste_posto_giusto);
}

//questa funz confronta la parola, ne trova i vincoli, applica i vincoli (= chiama la funzione che filtra le parole)
// e calcola la stringa da stampare di confronto
void parole_partita(int larghezza_buf4, int lunghezza_parole5, char buffer[][larghezza_buf4], int *indice_inizio_parole,
                    struct Nodo *alfabeto[], struct Nodo *nil, char parola_riferimento[], int *numero_tentativi,
                    struct Vincolo *vincoli, char giuste_posto_giusto[], int *conta_filtrate,
                    int *nella_partita, int* vincoli_attivi, int *prima_parola, struct NodoBST *filtrate[],
                    struct NodoBST* massimoBST[]){

    int h;
    int indice_fine_parole = *indice_inizio_parole; //trovo quante sono le parole da confrontare
    while( (buffer[indice_fine_parole][0] != '%') && (buffer[indice_fine_parole][0] != '!')
           && (buffer[indice_fine_parole][0] != '+') ){
        indice_fine_parole++;//alla fine è = al primo elemento da non inserire
    }
    int numero_parole = indice_fine_parole - *indice_inizio_parole;

    int indice_alfabeto;
    int num_occorrenze_restanti; //conta le occorrenze che mancano di ogni carattere che appartiene, per dedurne il numero esatto
    struct Nodo parolaPartita; //ci metto dentro la parola, mi serve per fare la ricerca nell'albero
    char da_stampare[lunghezza_parole5]; //parola che contiene l'esito del confronto da stampare
    int i;
    for(i=0; i<numero_parole; i++){//ciclo per ogni parola da confrontare
        if(*numero_tentativi>0){

            if(parole_uguali(parola_riferimento, buffer[*indice_inizio_parole+i], lunghezza_parole5)){
                printf("ok\n");//se la parola è = a quella di riferimento
                *nella_partita=0;
                *vincoli_attivi=0; //questi due servono se c'è inserisci_inizio dopo
                *prima_parola=0;
            }
            else{//se la parola non è uguale a quella di riferimento
                indice_alfabeto = buffer[*indice_inizio_parole+i][0] - '-';

                //copio nella struttura (che mi serve dopo per la ricerca) il riferimento alla parola da cercare
                parolaPartita.parola = &buffer[*indice_inizio_parole+i][0];

                //la cerco nell'albero e deve essere ammissibile(=dentro al dizionario)
                if( ricerca(alfabeto[indice_alfabeto], &parolaPartita, nil, lunghezza_parole5) ){
                    *numero_tentativi = *numero_tentativi -1;

                    for(h=0;h<lunghezza_parole5;h++){ //pulisco la parola da stampare
                        da_stampare[h]='$';
                    }
                    //   ---Ora confronta, apprendi i vincoli e stampa il confronto---

                    int indice_contatore;
                    for(h=0;h<lunghezza_parole5;h++){ //azzero il contatore dei caratteri: Solo quelli usati
                        indice_contatore = buffer[*indice_inizio_parole + i][h] - '-';
                        vincoli[indice_contatore].contatore =0;
                    }

                    //Prima controllo le giuste al posto giusto
                    for(h=0;h<lunghezza_parole5;h++) {//ciclo per ogni carattere della parola
                        indice_alfabeto = buffer[*indice_inizio_parole + i][h] - '-';//dice l'indice del carattere nella struttura dei vincoli

                        //se è giusta al posto giusto: '+'
                        if (buffer[*indice_inizio_parole + i][h] == parola_riferimento[h]) {
                            da_stampare[h] = '+';

                            vincoli[indice_alfabeto].appartiene = 1;
                            vincoli[indice_alfabeto].contatore = vincoli[indice_alfabeto].contatore + 1; //conta quante volte compare quel carattere nella parola

                            giuste_posto_giusto[h] = parola_riferimento[h]; //inserisce il carattere giusto nella parola

                            //aggiorno il numero minimo di occorrenze del carattere
                            if(vincoli[indice_alfabeto].num_min<vincoli[indice_alfabeto].contatore){
                                vincoli[indice_alfabeto].num_min=vincoli[indice_alfabeto].contatore;
                            }
                        }
                    }

                    for(h=0;h<lunghezza_parole5;h++){//ciclo per ogni carattere della parola
                        indice_alfabeto = buffer[*indice_inizio_parole + i][h] - '-';

                        if(da_stampare[h] != '+'){ //se prima in quella posizione non c'era il carattere giusto

                            //se almeno il carattere compare nella parola di riferimento: '|'
                            if( vincoli[indice_alfabeto].occorrenze_riferimento >0  ){
                                vincoli[indice_alfabeto].appartiene=1;
                                vincoli[indice_alfabeto].non_compare[h] = 1; //so che quel carattere non compare in quella posizione
                                vincoli[indice_alfabeto].contatore = vincoli[indice_alfabeto].contatore +1;

                                num_occorrenze_restanti = vincoli[indice_alfabeto].occorrenze_riferimento - vincoli[indice_alfabeto].contatore;

                                if(num_occorrenze_restanti>=0){ //se ci possono ancora essere occorrenze di quel carattere
                                    da_stampare[h] = '|';
                                }
                                else{
                                    da_stampare[h]='/';
                                    //se ho appena sforato di 1 le occorrenze
                                    if(num_occorrenze_restanti == -1){ //NB: posso dire che ho trovato quelle esatte solo se trovo il '/'
                                        vincoli[indice_alfabeto].num_esatto = vincoli[indice_alfabeto].contatore-1;
                                    }
                                }
                            }
                            else{//se il carattere non compare nella parola di riferimento: '/'
                                da_stampare[h] = '/';
                                vincoli[indice_alfabeto].appartiene=0;
                                vincoli[indice_alfabeto].non_compare[h]=1;
                            }
                        }
                        else{//il carattere era giusto '+' e quindi lo ignoro
                        }

                        //aggiorno il numero minimo di occorrenze del carattere (lo fa per ogni carattere incontrato?)
                        if(vincoli[indice_alfabeto].num_min<vincoli[indice_alfabeto].contatore){
                            vincoli[indice_alfabeto].num_min=vincoli[indice_alfabeto].contatore;
                        }
                    }

                    //stampare il confronto
                    for(h=0;h<lunghezza_parole5;h++){
                        printf("%c",da_stampare[h]);
                    }
                    printf("\n");


                    //filtro il dizionario
                    if(*prima_parola==0){ //se ho il 1° vincolo -> creo l'albero di filtrate
                        for(h=0;h<DIM_ALFABETO;h++){
                            if(alfabeto[h] != nil){
                                //chiama la funzione che filtra quell'albero
                                inOrder_filtra1(alfabeto[h], filtrate, nil,
                                                vincoli, conta_filtrate, lunghezza_parole5,
                                                giuste_posto_giusto, prima_parola, massimoBST);
                            }
                        }
                        *prima_parola=1;
                    }
                    else{ //ho già l'albero di filtrate -> lo filtro ulteriormente
                        for(h=0;h<DIM_ALFABETO;h++){
                            if(filtrate[h] != NULL){
                                //chiama la funzione che filtra quell'albero
                                inOrder_filtra4(filtrate[h], filtrate,
                                                vincoli, conta_filtrate, lunghezza_parole5, giuste_posto_giusto);
                            }
                        }
                    }

                    //stampa il numero di parole
                    printf("%d\n",*conta_filtrate);

                    if(*numero_tentativi == 0){
                        printf("ko\n");
                        *nella_partita=0;
                        *vincoli_attivi=0; //questi due servono se c'è inserisci_inizio dopo
                        *prima_parola=0;
                    }

                    *vincoli_attivi=1; //devo segnalare se l'inserimento deve rispettarei vincoli
                }
                else{ //altrimenti se non lo trova nell'albero (nel dizionario)
                    printf("not_exists\n");
                }
            }
        }
        else{//Ho finito i tentativi, l'input è corretto quindi si ferma qui e non devo fare niente.
        }
    }

    //ora sistemo l'indice del buffer
    if( (buffer[indice_fine_parole][0]=='!')||(buffer[indice_fine_parole][0]=='%') ){
        *indice_inizio_parole=indice_fine_parole-1; //cambia l'indice 'i' del main che indica dove finiscono le parole da inserire
        //e diventa = all'ultima parola inserita
    }
    if(buffer[indice_fine_parole][0]=='+'){
        *indice_inizio_parole=indice_fine_parole; //cambia l'indice 'i' del main che indica dove finiscono le parole da inserire
        //e diventa = al comando
    }
}


int main() {

    //acquisisco la lunghezza delle parole
    int lunghezza_parole=lettura_numero();

    //scelgo la larghezza del buffer in base alla lunghezza delle parole
    int larghezza_buf;
    if(lunghezza_parole<DIM_COMANDO){
        larghezza_buf=DIM_COMANDO;
    }
    else{
        larghezza_buf = lunghezza_parole+1;
    }

    //definisco il buffer per l'input e lo inizializzo a $
    char buffer[ALTEZZA_BUF][larghezza_buf];
    int i;
    riempi_buffer(larghezza_buf, buffer, &i);

    //Definisco l'array di puntatori sul quale creo gli alberi RB, sarebbero le "Radici" degli alberi per ogni lettera
    struct Nodo* alfabeto[DIM_ALFABETO]; //78 celle di puntatori a struct
    //numero di caratteri ascii da '-' a 'z'
    //indici inutilizzati: 1,2,13,14,15,16,17,18,19,46,47,48,49,51

    struct NodoBST* filtrate[DIM_ALFABETO];

    // Definisco T.NIL: il singolo nodo foglia per gli RBTree
    struct Nodo nil;
    nil.left = NULL;
    nil.right = NULL;
    nil.padre = NULL;
    nil.parola = NULL;
    nil.color = BLACK;

    int h; int k;
    for(h=0; h<DIM_ALFABETO;h++){ //inizializzo tutto l'array di puntatori
        alfabeto[h] = &nil;
        filtrate[h] = NULL;
    }

    //struct Vincolo vincoli[DIM_ALFABETO];//dichiaro la struttura che tiene i vincoli
    //struct Vincolo* vincoli = malloc( DIM_ALFABETO *  sizeof(struct Vincolo) );
    struct Vincolo vincoli[DIM_ALFABETO];

    int* posizioni = malloc(DIM_ALFABETO * lunghezza_parole* sizeof(int) ); //alloco memoria per le stringhe "non_compare"
    int indice_posizioni=0;
    for(h=0; h<DIM_ALFABETO;h++){
        vincoli[h].non_compare = &posizioni[indice_posizioni];
        indice_posizioni += lunghezza_parole;
    }

    char parola_riferimento[lunghezza_parole]; int leggo_parola=0; //diventa 1 se ha letto la parola
    int numero_tentativi;  int leggo_numero=0; //diventa 1 se ha letto il numero
    int vincoli_attivi=0; //serve per capire quando faccio l'inserimento se devo considerare i vincoli
    int nella_partita=0; //diventa 1 se sono dentro a una partita
    int sto_inserendo=1; //all'inizio è 1 perchè sto inserendo il dizionario
    int conta_dizionario=0, conta_filtrate=0;
    int indice_alfabeto=0; //serve per calcolare il numero ascii del carattere
    char giuste_posto_giusto[lunghezza_parole];
    int prima_parola=0; //serve per capire se ho già filtrato il dizionario con la 1° parola della partita
    struct NodoBST* massimoBST[DIM_ALFABETO]; //serve per fre l'inserimento rapido nel BST quando creo il BST

    //Parsing dell'input nel buffer:
    for(i=0;i<ALTEZZA_BUF;i++){

        if(buffer[i][0] == '!'){ //devo riempire il buffer:
            riempi_buffer(larghezza_buf, buffer, &i);
        }

        if(buffer[i][0] == '%'){//se ho trovato EOF
            break; //esco dal ciclo per terminare
        }

        //Dopo aver letto nuova_partita, nei cicli successivi leggo parola_riferimento e numero tentativi nel buffer
        if(nella_partita == 1){
            if( leggo_parola == 0){ //se devo ancora leggere parola di riferimento
                for(h=0; h<lunghezza_parole;h++){
                    parola_riferimento[h] = buffer[i][h];
                }
                leggo_parola=1;

                for(h=0;h<lunghezza_parole;h++){ //conto quante volte compaiono i caratteri nella parola di riferimento
                    indice_alfabeto = parola_riferimento[h] - '-';
                    vincoli[indice_alfabeto].occorrenze_riferimento +=1;
                }
            }
            else{
                if(leggo_numero == 0){ //se devo ancora leggere il numero di tentativi
                    h=0;
                    numero_tentativi=0;
                    while( buffer[i][h] != '$' ){
                        numero_tentativi = (numero_tentativi<<3) + (numero_tentativi<<1) + (buffer[i][h] - '0');
                        h++;
                    }
                    leggo_numero=1;
                }
                else{
                    if( buffer[i][0] != '+' ){ //se sono nella partita ma non è un comando -> è una parola da confrontare
                        //chiamola funzione che confronta le parole e ne ricava i vincoli
                        //la funzione ritorna un tipo di indice se trova % o !, un'altro se trova + (simile a inserisci dizionario)

                        parole_partita(larghezza_buf, lunghezza_parole, buffer, &i,
                                       alfabeto, &nil, parola_riferimento, &numero_tentativi, vincoli, giuste_posto_giusto,
                                       &conta_filtrate, &nella_partita,
                                       &vincoli_attivi, &prima_parola, filtrate, massimoBST);
                    }
                }
            }
        }

        if(sto_inserendo==1){
            inserisci_dizionario(larghezza_buf, lunghezza_parole, buffer, &i,
                                 alfabeto, &nil, &conta_dizionario, &conta_filtrate, vincoli_attivi, nella_partita,
                                 vincoli, giuste_posto_giusto, filtrate); //NON SO SE IL PASSAGGIO DI PARAMETRI È GIUSTO
        }

        if( buffer[i][0] == '+'){//se c'è un comando

            if( buffer[i][12]=='n' ){//+inserisci_inizio
                sto_inserendo=1;
                if(nella_partita == 1){
                    nella_partita=2; //ho 'messo in pausa la partita'
                }
            }

            if( buffer[i][12]=='i'){//+inserisci_fine
                sto_inserendo=0;
                if(nella_partita==2){
                    nella_partita=1;//'riprendo' la partita
                }
            }

            if( buffer[i][12]=='t'){//+nuova_partita

                //AZZERO I VINCOLI PRECEDENTI
                for(h=0;h<DIM_ALFABETO;h++){
                    vincoli[h].num_esatto=0;
                    vincoli[h].num_min=0;
                    vincoli[h].occorrenze_riferimento=0;
                    vincoli[h].appartiene=1;
                    vincoli[h].contatore=0;
                    for(k=0;k<lunghezza_parole;k++){
                        vincoli[h].non_compare[k]=0;
                    }
                    massimoBST[h] = NULL;

                    if(filtrate[h] != NULL){ //faccio la free del vecchio albero di filtrate
                        freeBST(filtrate[h]);
                        filtrate[h] = NULL;
                    }
                }

                for(h=0;h<lunghezza_parole;h++){ //ad inizio partita pulisco le giuste al posto giusto
                    giuste_posto_giusto[h] = '$';
                }

                nella_partita=1;
                leggo_numero=0;
                leggo_parola=0;
                vincoli_attivi=0;
                sto_inserendo=0;
                conta_filtrate=0;
                prima_parola=0;
            }

            if( buffer[i][12]=='r'){//+stampa_filtrate

                if(vincoli_attivi == 1){ //se ci sono vincoli-> stampo l'albero delle filtrate
                    for(h=0;h<DIM_ALFABETO;h++){
                        if(filtrate[h] != NULL){
                            inOrderPrintBST(*filtrate[h], NULL, lunghezza_parole);
                        }
                    }
                }
                else{ //se non ci sono vincoli-> stampo l'albero del dizionario
                    for(h=0;h<DIM_ALFABETO;h++){
                        if(alfabeto[h] != &nil){
                            inOrderPrint(*alfabeto[h], &nil, lunghezza_parole);
                        }
                    }
                }
            }
        }
    }

    return 0;
}