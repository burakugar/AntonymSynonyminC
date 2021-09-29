/* program designed assuming that synonym of a words cannot be antonym of that word as well */
/* main method of this program: */
 /* create a linked list with using antonyms.txt and synonyms.txt. each first word in the txt files and that first word's antonyms and synonyms
  * form a linked list and we store that linked list in the nodes and nodes1 linked list arrays.
  * then we are combining this link list with a main linked list. every time a new user joins to the program, we are storing it's number of
  * answer etc. and writing to the file. after that user come back, using our main linked list, we are adding the user's answers on it.*/
/* i use different formats for the linked lists when saving them to the user's file*/
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define LINE 150
#define MAX_BUFFER 200
#define WORDS 1000 /* number of words in the files */
#define NO 10 /* maximum number of synonyms antonyms of a word */
#define LEN 15 /* maximum number of characters a single word can be as a antonym or synonym */
#define ANS 50 /* maximum number of session with single user */
typedef struct node{ /* a linked list node for a single word */
    char name[LEN];
    int index;
    float pa; /* probability of a word to be asked as antonym question */
    float ps; /* probability of a word to be asked as synonym question */
    int wa; 	/* how many times a word has answered wrong as a antonym*/
    int ws;	 /* how many times a word has answered wrong as a synonym*/
    int na; /* how many times a word is asked as antonym question */
    int ns;	/* how many times a word is asked as synonym question */
    char synonyms [NO][LEN]; /* antonyms and synonyms of a node */
    char antonyms [NO][LEN];
    char diffant[NO][LEN];
    char diffsyn[NO][LEN];
    struct node* next;
}node;
int calculateLine(FILE* fp) { /* this function returns the number of lines in given file */
    char a;
    int no=0; /* get a char at each iteration until reaching the eof and everytime see the newline increase the counter */
    for (a = getc(fp); a != EOF; a = getc(fp))
        if (a == '\n')
            no+=1;
    return no;
}
void addAntonymList1(char enter[30], node **iter){
    int i; /* this function adds a new antonym word to the given node */
    for(i=0;i<NO;i++){
        if(strcmp((*iter)->antonyms[i]," ")!= 0){ /* empty indexes of the array has been filled with " " */
            strcpy((*iter)->antonyms[i],enter);
            return;
        }
    }
}
void addSynonymList1(char enter[30], node **iter){
    int i; /* this function does the same job for the synonyms */
    for(i=0;i<NO;i++){
        if(strcmp((*iter)->synonyms[i]," ")!= 0){
            strcpy((*iter)->synonyms[i],enter);
            return;
        }
    }
}
void addAntonymList(char enter[30], node **iter){   /* this function does the same job with above functions but adds a antonym for the
  new user */
    int i;  /* this function does the same job with above functions but adds a synonym for the
  new user */
    for(i=0;i<NO;i++){
        if(strcmp((*iter)->antonyms[i],"\0")!= 0){ /* empty indexes of the antonym's array for the first user is indicated with null */
            strcpy((*iter)->antonyms[i],enter);
            return;
        }
    }
}
void addSynonymList(char enter[30], node **iter){
    int i;
    for(i=0;i<NO;i++){
        if(strcmp((*iter)->synonyms[i],"\0")!= 0){
            strcpy((*iter)->synonyms[i],enter);
            return;
        }
    }
}
void calculateNumberOfSA(char** str, int* numbers, int l){ /* this function calculates the number of synonym and antonym in the first file */
    int i, count=0,k;
    char** str1= (char**) malloc(sizeof(char*)*l); /* copying the str array in order not to lose */
    for(i=0;i<l;i++){
        str1[i]= (char*) malloc(sizeof(char)*LEN);
    }

    for(i=0;i<l;i++){
        strcpy(str1[i],str[i]);
        count=0; /* in the file, if we count the number of spaces we will get number of antonyms and synonyms */
        while(*(str1[i]) != '\n'){
            if(*(str1[i]) == ' ')
                count++;
            str1[i]+=1;
        }
        count= count+1;
        numbers[i]= count; /* numbers[i] holds the number of antonyms for each line */
    }
    free(str1);
}
node* copyList(node* list) { /* this function copies the elements of two linked lists */
    int i;
    if (list == NULL)
        return NULL;
    node* final = (node*) malloc(sizeof(node));
    strcpy(final->name,list->name);
    final->index = list->index;
    final->pa = list->pa;
    final->ps = list->ps;
    final->wa= list->wa;
    final->ws= list->ws;
    final->na= list->na;
    final->ns= list->ns;
    for(i=0;i<NO;i++){
        strcpy(final->antonyms[i],list->antonyms[i]);
        strcpy(final->synonyms[i],list->synonyms[i]);
    }
    final->next = copyList(list->next);
    node* iter8= final;
    return final;
}
void appendEnd(node** headcpy,node** added){ /* this function add the given node at the end of the linked list */
    node* iter= *headcpy;
    while(iter->next!=NULL){
        iter= iter->next;
    }
    (*added)->next= NULL;
    iter->next= *added;
}
void pushFront(node **head, char* name,int index,float pa,float ps,int a,int s)
{ /* this function adds the given node at front of the linked list */
    node *temp = (node*) malloc(sizeof(node));
    temp->index = index;
    strcpy(temp->name, name);
    temp->pa=pa;
    temp->ps=ps;
    temp->wa=a;
    temp->ws=s;
    temp->next = *head;
    *head = temp;
}
void addFirst(node* Nodes[WORDS],int l,char** str,char** names,int* numbers){
    int i,k,j,key=0; /* this function fills the first linked list */
    int index=0,a=0,s=0;
    float pa=0,ps=0;
    int len;
    char* temp= (char*) malloc(sizeof(char)* LEN);
    char** bfr= (char**) malloc(sizeof(char*)*LEN);
    char delim[] = " "; /* we are deleting the spaces */
    for(i=0;i<l;i++){
        for(j=0;j<numbers[i];j++){
            bfr[j]= (char*) malloc(sizeof(char)*LEN);
        }
    }
    /* parsing implementation for determining the synonyms */
    for(i=0;i<l;i++){
        node* head= (node*) malloc(sizeof(node));
        node* iter= head;
        strcpy(temp, str[i]);
        key=0;
        for(k=0;k<numbers[i];k++){
            //printf("%d",numbers[i]);
            char *ptr = strtok(temp, delim);
            while(ptr != NULL && *ptr!= '\0')
            {
                if(k==0 && key==0 ){
                    strcpy(iter->name,ptr);
                    iter->next= NULL;
                    key=1;
                }
                else{
                    while(iter->next !=NULL)
                        iter= iter->next;
                    node* temp1= (node*) malloc(sizeof(node));
                    strcpy(temp1->name,ptr);
                    temp1->next= NULL;
                    iter->next= temp1;
                }
                //printf("ptr %s\n", ptr);
                ptr = strtok(NULL, delim);
            }
            free(ptr);
        }
        Nodes[i]= head; /* assigning each line's pointer to a pointer array */
    }
    for(i=0;i<l;i++){
        pushFront(&Nodes[i],names[i],index,pa,ps,a,s);
    }
}
void addFirst1(node* Nodes1[WORDS],int l1,char** str1,char** names1, int* numbers1,node* temporary[WORDS])  {
    int i,k,j,key=0; /* same function above but this time we are creating node pointer for the synonyms txt */
    int z;
    int index = 0,a=0,s=0;
    float pa=0,ps=0;
    int len;
    char* temp= (char*) malloc(sizeof(char)* LEN);
    char** bfr= (char**) malloc(sizeof(char*)*LEN);
    char delim[] = " ";
    for(i=0;i<l1;i++){
        for(j=0;j<numbers1[i];j++){
            bfr[j]= (char*) malloc(sizeof(char)*LEN);
        }
    }
    /* parsing implementation for determining the synonyms */
    for(i=0;i<l1;i++){
        node* head= (node*) malloc (sizeof(node));
        node* iter= head;
        strcpy(temp, str1[i]);
        key=0;
        for(k=0;k<numbers1[i];k++){
            //printf("%d",numbers[i]);
            char *ptr = strtok(temp, delim);
            while(ptr != NULL && *ptr!= '\0')
            {
                if(k==0 && key==0){
                    strcpy(iter->name,ptr);
                    iter->next= NULL;
                    key=1;
                }
                else{
                    while(iter->next !=NULL)
                        iter= iter->next;
                    node* temp1= (node*) malloc(sizeof(node));
                    strcpy(temp1->name,ptr);
                    temp1->next= NULL;
                    iter->next= temp1;
                }
                //printf("ptr %s\n", ptr);
                ptr = strtok(NULL, delim);
            }
            free(ptr);
        }
        Nodes1[i]= head;
    }
    for(i=0;i<l1;i++){
        temporary[i]= Nodes1[i];
    }
    for(i=0;i<l1;i++){
        pushFront(&temporary[i],names1[i],index,pa,ps,a,s);
    }
    free(temp);
}
void calculateProb(node** headcpy){ /* this function calculates each word's probability and assing them to a linked list */
    float pa,ps;
    int i;
    node* iter = *headcpy;
    while(iter!=NULL){
        iter-> pa= (float) (1/(float)(iter->na+iter->ns))*iter->wa*iter->ns; /* probability formula according to pdf */
        iter-> ps= (float) (1/(float)(iter->na+iter->ns))*iter->ws*iter->na;
        iter=iter->next;
    }
}
int checkA(node** iter,char enter[LEN]){
    int i;
    char name[LEN];
    for(i=0;i<NO;i++){
        printf("antonymi : %s\n", (*iter)->antonyms[i]);
        if(strcmp(enter,(*iter)->antonyms[i])== 0)
        {
            (*iter)-> na +=1;
            return 1; /* we found an antonym in the given node then return 1 */
        }
    }

    return 0;
}
int checkS(node** iter,char enter[LEN]){ /* this function checks whether given node has a special synonym or not */
    int i;
    char name[LEN];
    for(i=0;i<NO;i++){
        if(strcmp(enter,(*iter)->synonyms[i])== 0)
        {
            (*iter)-> ns +=1;
            return 1; /* we found an synonym in the given node then return 1 */
        }
    }

    return 0;
}
void askFirstQuestions(FILE* fp,node** headcpy){ /* if the first user enters to the program this function will ask the related questions and will
 * create the main linked list */
    int i,n;
    char user_anton[ANS][LEN]; /* this array saves the user's wrong answers according to antonyms questions(there is no need to save true answers) */
    char user_synon[ANS][LEN]; /* this array saves the user's wrong answers according to synonym questions */
    char enter[LEN*2]; /* ANSWER THAT USER ENTERED FOR A QUESTION */
    node* iter= *headcpy;
    iter= *headcpy;
    while(iter!=NULL){ /* IN THE BEGINNING ITERATING TROUGH END OF THE LINKED LIST AND ASK BOTH SYNONYM AND ANTONYMS OF THE NODES */
        printf("\nWhat is the antonym of the %s?: ", iter->name);
        scanf("%s",enter);
        if(checkA(&iter,enter)==1){
            printf("True Answer !");
        }
        else{
            er:
            printf("\n%s is not one of the antonyms of the %s! Should we add it to the list?(Yes:1, No:0):  ",enter,iter->name);
            scanf("%d",&n);
            switch(n)
            {	case 0:
                    break;
                case 1:
                    printf("\n%s is added to the antonyms list of %s!", enter, iter->name);
                    addAntonymList(enter,&iter);
                    break;
                default:
                    printf("Please choose a valid answer!");
                    goto er;
            }
        }
        printf("\nWhat is the synonyms of the %s?: ", iter->name);
        scanf("%s",enter);
        if(checkS(&iter,enter)==1){
            printf("True Answer !");
        }
        else{
            er1:
            printf("\n%s is not one of the synonyms of the %s! Should we add it to the list?(Yes:1, No:0):  ",enter,iter->name);
            scanf("%d",&n);
            switch(n)
            {   case 0:
                    break;
                case 1:
                    printf("\n%s is added to the antonyms list of %s!", enter, iter->name);
                    addSynonymList(enter,&iter);
                    break;
                default:
                    printf("\nPlease choose a valid answer!");
                    goto er1;

            }
        }
        iter=iter->next;
    }
}
void addnewfirstVocab(FILE* fp,node** head,node** headcpy){ /* this function adds a vocabulary to the linked list */
    int i,n,n1,count=0;
    char name[LEN];
    char synonym[LEN];
    char antonym[LEN];
    int index=0,indexs,indexa;
    node* iter= *headcpy;
    while (iter!=NULL) /* iterating trough end of the list and counting the number of nodes */
    {
        iter= iter->next;
        count++;
    }
    count+=1;
    node* iter1= *headcpy;
    node* temp= (node*) malloc(sizeof (node));
    er2:
    printf("Choose the following menu: ");
    printf("\n1. Add a new word:");
    printf("\n2. Add a new synonym for a word:");
    printf("\n3. Add a new antonym for a word:");
    scanf("%d",&n);
    switch(n)
    {
        case 1:
            printf("Enter the name of the word:");
            scanf("%s", name);
            strcpy(temp->name,name);
            printf("\nEnter the number of synonyms of the word: ");
            scanf("%d",&n1);
            for(i=0;i<n1;i++){
                printf("\nEnter the %d'th synonym of the %s",i+1,name);
                scanf("%s",temp->synonyms[i]);
            }
            printf("\nEnter the number of antonyms of the word: ");
            scanf("%d",&n1);
            for(i=0;i<n1;i++){
                printf("\nEnter the %d'th antonyms of the %s",i+1,name);
                scanf("%s",temp->antonyms[i]);
            }
            temp->index= count;
            temp->na=0;
            temp->ns=0;
            temp->wa=0;
            temp->ws=0;
            appendEnd(headcpy,&temp);
            break;
        case 2:
            iter1= *headcpy;
            printf("Enter the name of the word:");
            scanf("%s", name);
            printf("Enter the synonym of the word:");
            scanf("%s", synonym);
            while(iter1!= NULL){
                if(strcmp(name, iter1->name)==0) {
                     for(i=0;i<NO;i++){
                         if(strcmp(iter1->synonyms[i],"\0")==0){
                             strcpy(iter1->synonyms[i],synonym);
                             break;
                         }
                     }
                    goto er4;
                }
                iter1= iter1->next;
            }
        er4:
            iter= *headcpy;
            break;
        case 3:
            printf("Enter the name of the word:");
            scanf("%s", name);
            printf("Enter the antonym of the word:");
            scanf("%s", antonym);
            while(iter1!= NULL){
                if(strcmp(name, iter1->name)==0) {
                    for(i=0;i<NO;i++){
                        if(strcmp(iter1->antonyms[i],"\0")==0){
                            strcpy(iter1->antonyms[i],antonym);
                            break;
                        }
                    }
                    goto er4;
                }
                iter1= iter1->next;
            }
            iter= *headcpy;
            break;
        default:
            printf("Please choose a valid menu!");
            goto er2;
    }

}
void askQuestions(FILE* fp, node** head, node** headcpy){ /* this function asks the questions about linked lists */
    node* iter;
    node* biggest;
    node* biggest1;
    node* biggest2;
    float temp,temp1,temp2;
    int flag=0, flag1=0;
    int number;
    char enter[LEN*2]; /* ANSWER THAT USER ENTERED FOR A QUESTION */
    calculateProb(headcpy); /* calculating the all nodes probability and assigning them to linked list */
    iter= *headcpy;
    int maxa= iter->pa;
    int maxs= iter->ps;
    /* in a node, we have a prob.of asking a ant. question and syn. question */
    /* in the below, i'm comparing the prb. of asking a antonym question of the nodes and same is
     * valid for syn.s as well */
    /* at the and we are comparing the biggest prob of antonyms and biggest of the synonyms */
    while(iter!=NULL){ /* we are controlling one by one the probability of the next node */
        if(iter->next!=NULL){
            if(iter->next-> pa >= iter->pa ){
                maxa= iter->next->pa;
                biggest= iter->next;
            }
            else{
                maxa= iter->pa;
                biggest= iter;
            }
        }
        iter= iter->next;
    }
    iter= *headcpy;
    while(iter!=NULL){
        if(iter->next!=NULL){
            if(iter->next-> ps > iter->ps ){
                maxs= iter->next->ps;
                biggest1= iter->next;
            }
            else{
                maxs= iter->ps;
                biggest1= iter;
            }
        }
        iter= iter->next;
    }
    if(maxs>= maxa){
        biggest2= biggest1; /* flag= synonym question will be asked */
        flag=1;
    }
    else{
        biggest2= biggest;
        flag1=1; /* flag1= antonym question will be asked */
    }
    if(flag==1){
        biggest2->ns+=1;
        /* ask a synonym question */
        printf("\nWhat is the synonym of the %s?: ", biggest2->name);
        scanf("%s",enter);
            if(checkS(&biggest2,enter)==1){
                printf("True Answer !");
            }
            else{
                biggest2->ws+=1;
                er7:
                printf("\n%s is not one of the synonym of the %s! Should we add it to the list?(Yes:1, No:0):  ",enter,biggest2->name);
                scanf("%d",&number);
                switch(number)
                {	case 0:
                        break;
                    case 1:
                        printf("\n%s is added to the synonym list of %s!", enter, biggest2->name);
                        addSynonymList(enter,&biggest2);
                        break;
                    default:
                        printf("Please choose a valid answer!");
                        goto er7;
                }
            }
    }
    else{
        biggest2->na +=1;
        /* ask a antonym question */
        printf("\nWhat is the antonym of the %s?: ", biggest2->name);
        scanf("%s",enter);
        if(checkA(&biggest2,enter)==1){
            printf("True Answer !");
        }
        else{ /* if user's answer is wrong, increment the number of wrong questions */
            biggest2->wa+=1;
            er8:
            printf("\n%s is not one of the antonyms of the %s! Should we add it to the list?(Yes:1, No:0):  ",enter,biggest2->name);
            scanf("%d",&number);
            switch(number)
            {	case 0:
                    break;
                case 1:
                    printf("\n%s is added to the antonyms list of %s!", enter, biggest2->name);
                    addAntonymList(enter,&biggest2);
                    break;
                default:
                    printf("Please choose a valid answer!");
                    goto er8;
            }
        }
    }
}
void addnewVocab(FILE* fp, node **headcpy){ /* this function adds a new vocabulary to the list */
    int i,n,n1,count=0;
    char name[LEN];
    char synonym[LEN];
    char antonym[LEN];
    int index=0,indexs,indexa;
    node* iter= *headcpy;
    while (iter!=NULL) /* iterating trough end of the list and counting the number of nodes */
    {
        iter= iter->next;
        count++;
    }
    count+=1;
    node* iter1= *headcpy; /* copying the head pointer */
    node* temp= (node*) malloc(sizeof (node));
    er11:
    printf("Choose the following menu: ");
    printf("\n1. Add a new word:");
    printf("\n2. Add a new synonym for a word:");
    printf("\n3. Add a new antonym for a word:");
    scanf("%d",&n);
    switch(n)
    {
        case 1:
            printf("Enter the name of the word:");
            scanf("%s", name);
            strcpy(temp->name,name);
            printf("\nEnter the number of synonyms of the word: ");
            scanf("%d",&n1);
            for(i=0;i<n1;i++){
                printf("\nEnter the %d'th synonym of the %s",i+1,name);
                scanf("%s",temp->synonyms[i]);
            }
            printf("\nEnter the number of antonyms of the word: ");
            scanf("%d",&n1);
            for(i=0;i<n1;i++){
                printf("\nEnter the %d'th antonyms of the %s",i+1,name);
                scanf("%s",temp->antonyms[i]);
            }
            temp->index= count;
            temp->na=0;
            temp->ns=0;
            temp->wa=0;
            temp->ws=0;
            appendEnd(headcpy,&temp);
            break;
        case 2:
            iter1= *headcpy;
            printf("Enter the name of the word:");
            scanf("%s", name);
            printf("Enter the synonym of the word:");
            scanf("%s", synonym);
            while(iter1!= NULL){
                if(strcmp(name, iter1->name)==0) {
                    for(i=0;i<NO;i++){
                        if(strcmp(iter1->synonyms[i]," ")==0){
                            strcpy(iter1->synonyms[i],synonym);
                            break;
                        }
                    }
                    goto er11;
                }
                iter1= iter1->next;
            }
        er12:
            iter= *headcpy;
            break;
        case 3:
            printf("Enter the name of the word:");
            scanf("%s", name);
            printf("Enter the antonym of the word:");
            scanf("%s", antonym);
            while(iter1!= NULL){
                if(strcmp(name, iter1->name)==0) {
                    for(i=0;i<NO;i++){
                        if(strcmp(iter1->antonyms[i]," ")==0){
                            strcpy(iter1->antonyms[i],antonym);
                            break;
                        }
                    }
                    goto er12;
                }
                iter1= iter1->next;
            }
            iter= *headcpy;
            break;
        default:
            printf("Please choose a valid menu!");
            goto er11;
    }
}
void updateFile(FILE* fp,node** head,node** headcpy){/* we need this function when we create a user and
 * save it's answer to the linked list then we have to save it to the file in a special format */
    int index, wa,ws,na,ns,row,i=0,y=0,important_index=0;
    char name[LEN];
    char anton[MAX_BUFFER];
    char synon[MAX_BUFFER];
    char temp[WORDS+MAX_BUFFER]=" ";
    char addeda[NO][30];
    char addeds[NO][30];
    for(y=0;y<NO;y++){
        strcpy(addeda[y]," ");
        strcpy(addeds[y]," ");
    }
    char* tok;
    const char deleted[2]="-";
    row=calculateLine(fp);
    fseek(fp,0,SEEK_SET);
    while(i!= row){
        sscanf(temp,"%d,%s,%d,%d,%d,%d,%s,%s",&index,name,&wa,&ws,&na,&ns,anton,synon);
        i++;
    }
    node* iter= *headcpy;
    node* iter1= *head;
    node* temp1 = (node*) malloc(sizeof(node));
   while(iter!=NULL){
       if(strcmp(name,"0")==0){
           iter->wa=wa;
           iter->ws=ws;
           iter->na=na;
           iter->ns=ns;
           tok = strtok(anton, deleted);
            while(tok!=NULL){
                strcpy(addeda[important_index],tok);
                important_index++;
                tok=strtok(NULL,deleted);
            }
            important_index=0;
           tok = strtok(synon, deleted);
           while(tok!=NULL){
               strcpy(addeds[important_index],tok);
               important_index++;
               tok=strtok(NULL,deleted);
           }
           for(i=0;i<NO;i++)
              addAntonymList1(addeds[i],&iter);
           for(i=0;i<NO;i++)
               addAntonymList1(addeds[i],&iter);
            }
        else {
           important_index = 0;
           temp1->na = na;
           temp1->ns = ns;
           temp1->ws = ws;
           temp1->wa = wa;
           temp1->index = index;
           tok = strtok(anton, deleted);
           while (tok != NULL) {
               strcpy(addeda[important_index], tok);
               important_index++;
               tok = strtok(NULL, deleted);
           }
           for (i = 0; i < important_index; i++) {
               strcpy(temp1->antonyms[i], addeda[i]);
           }
           important_index = 0;
           tok = strtok(synon, deleted);
           while (tok != NULL) {
               strcpy(addeds[important_index], tok);
               important_index++;
               tok = strtok(NULL, deleted);
           }
           for (i = 0; i < important_index; i++) {
               strcpy(temp1->synonyms[i], addeds[i]);
           }
           appendEnd(&iter, &temp1);
       }
       iter=iter->next;
   }
}
void chooseMenu(FILE *fp, node **head, node **headcpy) { /* this function is crating the main linked lists based on txt files */
    updateFile(fp,head,headcpy);
    int i,j,n,number=1;
    char* name= (char*) malloc(sizeof(char)* NO);
do {
    y:
    printf("Please choose a menu: \n");
    printf("1. Guess synonyms and antonyms: \n");
    printf("2. Add a new synonym or antonym:\n");
    printf("3. Exit the program: ");
    scanf("%d", &n);
    switch (n) {
        case 1:
            askQuestions(fp, head, headcpy);
            break;
        case 2:
            addnewVocab(fp, headcpy);
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Please choose a valid menu!");
            goto y;
        }
    printf("Dou you want to continue ? YES: 1 NO: 0");
    scanf("%d",&number);
    } while(number==1);
    if(number==0)
     exit(1);
}
void writeFile(FILE *fp, node **head, node **headcpy) { /* we saved our information in the linked list, now we will write them to the
 * user's file */
    fseek(fp,0,SEEK_SET);
    int i,j=0,k=0,count=0,count1=0,diff,rcount=0;
    node* iter= *head;
    node* iter1= *headcpy;
    while(iter!=NULL){
        count++;
        iter=iter->next;
    }
    while(iter1!=NULL){
        count1++;
        strcpy(iter1->diffant[0],"\0");
        strcpy(iter1->diffsyn[0],"\0");
        iter1=iter1->next;
    }
    diff= count1-count;
    diff++;
    iter= *head;
    iter1= *headcpy;
    while(iter1!=NULL){
        while(iter!= NULL){
            for(i=0;i<NO;i++) {
                for(j=0;j<NO;j++){
                    if (strcmp(iter->antonyms[i], "\0") != 0 && strcmp(iter1->antonyms[i], "\0") != 0 && strcmp(iter->name,iter1->name)==0) {
                        if (strcmp(iter->antonyms[j], iter1->antonyms[i]) == 0) {
                            goto y1;
                        }
                    }
                 }
                strcpy(iter1->diffant[k],iter1->antonyms[i]);
                k++;
                y1:;
            }
            k=0;
            iter= iter->next;
        }
        iter1= iter1->next;
    }
    iter= *head;
    iter1= *headcpy;
    k=0;
    while(iter1!=NULL){
        while(iter!= NULL){
            for(i=0;i<NO;i++) {
                for(j=0;j<NO;j++){
                    if (strcmp(iter->synonyms[i], "\0") != 0 && strcmp(iter1->synonyms[i], "\0") != 0 && strcmp(iter->name,iter1->name)==0) {
                        if (strcmp(iter->synonyms[j], iter1->synonyms[i]) == 0) {
                            goto y2;
                        }
                    }
                }
                strcpy(iter1->diffsyn[k],iter1->synonyms[i]);
                k++;
                y2:;
            }
            k=0;
            iter= iter->next;
        }
        iter1= iter1->next;
    }
    iter1= *headcpy;
    while(iter1!=NULL){
        if(rcount<diff){ /* in this part we are printing the differences between head linked list and headcpy linked list */
            fprintf(fp,"%d,0,%d,%d,%d,%d,",iter1->index,iter1->wa,iter1->ws,iter1->na,iter1->ns);
            for(i=0;i<NO;i++){
                if(strcmp(iter1->diffant[i],"\0")!=0 && strcmp(iter1->diffant[i+1],"\0") !=0 ){
                    fprintf(fp,"%s-",iter1->diffant[i]);
                }
                else if(strcmp(iter1->diffant[i],"\0")!=0 && strcmp(iter1->diffant[i+1],"\0")==0 && i!= NO){
                    fprintf(fp,"%s,",iter1->diffant[i]);
                }
            }
            for(i=0;i<NO;i++){
                if(strcmp(iter1->diffsyn[i],"\0")!=0 && strcmp(iter1->diffsyn[i+1],"\0") !=0 ){
                    fprintf(fp,"%s-",iter1->diffsyn[i]);
                }
                else if(strcmp(iter1->diffsyn[i],"\0")!=0 && strcmp(iter1->diffsyn[i+1],"\0")==0 && i!= NO){
                    fprintf(fp,"%s\n",iter1->diffsyn[i]);
                }
            }
        }
        else{
            fprintf(fp,"%d,%s,0,%d,%d,%d,%d,",iter1->index,iter1->name,iter1->wa,iter1->ws,iter1->na,iter1->ns);
            for(i=0;i<NO;i++){
                if(strcmp(iter1->diffant[i],"\0")!=0 && strcmp(iter1->diffant[i+1],"\0") !=0 ){
                    fprintf(fp,"%s-",iter1->diffant[i]);
                }
                else if(strcmp(iter1->diffant[i],"\0")!=0 && strcmp(iter1->diffant[i+1],"\0")==0 && i!= NO){
                    fprintf(fp,"%s,",iter1->diffant[i]);
                }
            }
            for(i=0;i<NO;i++){
                if(strcmp(iter1->diffsyn[i],"\0")!=0 && strcmp(iter1->diffsyn[i+1],"\0") !=0 ){
                    fprintf(fp,"%s-",iter1->diffsyn[i]);
                }
                else if(strcmp(iter1->diffsyn[i],"\0")!=0 && strcmp(iter1->diffsyn[i+1],"\0")==0 && i!= NO){
                    fprintf(fp,"%s\n",iter1->diffsyn[i]);
                }
            }
        }
        rcount++;
        iter1= iter1->next;
    }
}
void chooseMenu1(FILE* fp,node** head,node** headcpy){
    int i,j,n;
    char* name= (char*) malloc(sizeof(char)* NO);
    y:
    printf("Please choose a menu: \n");
    printf("1. Guess synonyms and antonyms: \n");
    printf("2. Add a new synonym or antonym: \n");
    printf("3. Exit the program: ");
    scanf("%d",&n);
    switch(n)
    {
        case 1:
            askFirstQuestions(fp,headcpy);
            printf("\n");
            break;
        case 2:
            addnewfirstVocab(fp,head,headcpy);
            break;
        case 3:
            writeFile(fp,head,headcpy);
            exit(1);
        default:
            printf("Please choose a valid menu!" );
            goto y;
    }
}
 void printMenu(node** head,node** headcpy,int l,int l1) {
    int i, j, number = 1,number1=1;
    char *name = (char *) malloc(sizeof(char) * NO);
    char file_extension[] = ".worddat";
    int flag = 0;
    FILE *fp;
    printf("Please enter the user name: ");
    scanf("%s", name);
    DIR *user;
    struct dirent *name_user;
    user = opendir(".");
     strcat(name,file_extension);
    if (user != 0) {
        while ((name_user = readdir(user)) != NULL) { /* check if the given username is already in the directory, if not create a new file */
            if (strcmp(name, name_user->d_name) == 0) {
                flag = 1;
                fp = fopen(name, "rb+");
                do {
                    chooseMenu(fp, head, headcpy);
                    printf("Do you want to continue?( YES: 1 NO: 0");
                    scanf("%d", &number1);
                } while (number1 == 1);
            }
        }
    }
    closedir(user);
    if (flag == 0) {
        strcat(name, file_extension); /* adding the .worddat extension at the end of the file */
        fp = fopen(name, "wb+");
        do {
            chooseMenu1(fp, head, headcpy);
            printf("Do you want to continue?( YES: 1 NO: 0");
            scanf("%d", &number);
        } while (number == 1);
    }
}
void createFirm(node* Nodes[WORDS],node* temporary[WORDS],int l, int l1,node** head,int* numbers,int* numbers1) {
    /* this function merges two linked list. We know first linked list is the antnoyms linked list which is indicated with N
    Nodes and the other one indicated with Nodes1. This are the linked list arrays which holds the first words of the
    antonyms.txt and synonyms.txt with their accordance words */

    int i, j, flag = 0, k = 0, y, d;
    (*head)->next = NULL;
    node *iter = *head;

    node *iter5;
    node *iter4;
    /* this will assign null to antonyms and synonyms of a node initially array elements */
    int z, w = 0;
    char **added = (char **) malloc(sizeof(char *) * (WORDS) * (l + l1 + WORDS));
    for (i = 0; i < l + l1 + WORDS; i++) {
        added[i] = (char *) malloc(sizeof(char) * LEN);
    }
    for (i = 0; i < l; i++) {
        while (iter->next != NULL) {
            iter = iter->next;
        }
        if (i == 0) {
            strcpy(iter->name, Nodes[i]->name);
            iter4= Nodes[i];
            for(z=0;z<numbers[w];z++){
                strcpy(iter->antonyms[z],iter4->next->name);
               if(iter4->next!=NULL)
                    iter4= iter4->next;
            }
            strcpy(added[k],iter->name);
        }
        else{
            iter4= Nodes[i];
            node* temp= (node*) malloc(sizeof(node));
            for(z=0;z<numbers[w];z++){
                strcpy(temp->antonyms[z],iter4->next->name);
                    iter4= iter4->next;
            }
            for(z=numbers[w];z<NO;z++){
                strcpy(temp->antonyms[z],"\0");
            }
            strcpy(temp->name, Nodes[i]->name);

            temp->next=NULL;
            iter->next =temp;
            strcpy(added[k],temp->name);
        }
        k++;
        w++;
    }
    iter=*head;
    for(i=0;i<l1;i++){
        node* iter6= temporary[i];
        iter5=*head;
        for(j=0;j<k;j++){
            if(strcmp(added[j], temporary[i]->name)==0){
                while(strcmp(iter5->name, temporary[i]->name)!=0&& iter5->next!= NULL){
                    iter5= iter5->next;
                }
                for(y=0;y<numbers1[i];y++){
                    for(d=0;d<numbers1[i];d++){
                            iter6= iter6->next;
                        strcpy(iter5->synonyms[d],iter6->next->name);
                    }
                    goto w;
                }

            }
        }
        while(iter->next!=NULL){
            iter=iter->next;
        }
        node* temp= (node*) malloc(sizeof(node));
        strcpy(temp->name, temporary[i]->name);
        temp->next=NULL;
        iter->next =temp;
        strcpy(added[k],temp->name);
        k++;
        iter6= temporary[i];
        iter6=iter6->next;
        for(d=0;d<numbers1[i];d++){
            strcpy(temp->synonyms[d],iter6->next->name);
            iter6=iter6->next;
        }
        for(d=numbers1[i];d<NO;d++){
            strcpy(temp->synonyms[d],"\0");
        }
        w:;
    }
    iter=*head;
    i=0;
    while(iter!=NULL){
        iter->index= i;
        iter= iter->next;
        i++;
    }
    iter= *head;
    while(iter!=NULL){
        iter->wa=0;
        iter->ws=0;
        iter->na=0;
        iter->ns=0;
        iter= iter->next;
    }
    free(added);
}
void handleFile(FILE* fp, FILE* fp1, int l,int l1){ /* this function handles the parsing implemantation in the files */
    int i,k;
    node* Nodes[WORDS] = {0};
    node* Nodes1[WORDS] = {0};
    node* temporary[WORDS] = {0};
    node* head= (node*) malloc(sizeof(node));
     if(head == NULL){
         fprintf(stderr, "Unable to allocate memory \n");
         exit(-1);
     }
    int count;
    char** words= (char**) malloc(sizeof(char*)*WORDS*2);
    for(i=0;i<WORDS*2;i++){
        words[i]= (char*) malloc(sizeof(char)*LEN);
    }
    char** names= (char**) malloc(sizeof(char*)*l); /* that array holds the first vocabularies such as in example of happy <> sad gloomy unhappy miserable this array will hold the happy */
    char** names1= (char**) malloc(sizeof(char*)*l1);
    for(i=0;i<l;i++){
        names[i]= (char*) malloc(sizeof(char)*LEN);
    }
    for(i=0;i<l1;i++){
        names1[i]= (char*) malloc(sizeof(char)*LEN);
    }
    char* temp= (char*) malloc(sizeof(char)*LINE);
    char* final= (char*) malloc(sizeof(char)*LINE);
    char* temp1= (char*) malloc(sizeof(char)*LINE);
    char* final1= (char*) malloc(sizeof(char)*LINE);
    int* numbers= (int*) malloc(sizeof(int)*l); /* this int array holds the number of synonyms that a word has in each line */
    char** str= (char**) malloc(sizeof(char*)*l);
    int* numbers1= (int*) malloc(sizeof(int)*l1); /* this int array holds the number of synonyms that a word has in each line */
    char** str1= (char**) malloc(sizeof(char*)*l1);
    for(i=0;i<l;i++){
        str[i]= (char*) malloc(sizeof(char)*LEN*3);
    }
    for(i=0;i<l1;i++){
        str1[i]= (char*) malloc(sizeof(char)*LEN*3);
    }
    const char deleted[2]= "<>"; /*  tokens that will be deleted */
    const char deleted1[2]= "=";

    for(i=0;i<l;i++){
        fgets(temp,LINE,fp);
        sscanf(temp,"%s",names[i]); /* scanning whole line then using strtok to parse the string */
        final= strtok(temp, deleted);
        while( final != NULL ){
            strcpy(str[i],final);
            final = strtok(NULL, deleted);
        }

        str[i]=str[i]+1;
    }
    calculateNumberOfSA(str,numbers,l);
    addFirst(Nodes,l,str,names,numbers);
    for(i=0;i<l1;i++){
        fgets(temp1,LINE,fp1);
        sscanf(temp1,"%s",names1[i]);
        final1= strtok(temp1, deleted1);
        while( final1 != NULL ){
            strcpy(str1[i],final1);
            final1 = strtok(NULL, deleted1);
        }
        str1[i]=str1[i]+1;
    }
    calculateNumberOfSA(str1,numbers1,l1);
    addFirst1(Nodes1,l1,str1,names1,numbers1,temporary);
    createFirm(Nodes,temporary,l,l1,&head,numbers,numbers1);
    node* headcpy= copyList(head);
    printMenu(&head,&headcpy,l,l1);
    free(final);
    free(temp);
    free(names);
    free(numbers);
    free(str);
    free(words);
}
void countcharsinLine(FILE* fp, int l, int* numboflines) {
    int x, i, j;
    int len;
    int count = 0;
    char* temp = (char*)malloc(sizeof(char) * MAX_BUFFER); /* since we can not read the lines without knowing how much
	character in that line, we have to determine a proper guess about what can it be.For example, fgetc or getc is not new line
	sensitive functions. There is no way us to find the new line with that functions. Same fact is valid for fscanf and fgets too.
	So, reading whole line with a temp buffer and after using strlen to find number of character is the most valid solution. */
    for (i = 0; i < l; i++) { /* Until reaching end of file, we are reading the file line by line and saving the number of characters
	in a line in the int** numboflines */
        fgets(temp, MAX_BUFFER, fp);
        len =(int) strlen(temp);
        *(numboflines + i) = len;
    }
    free(temp);
}
int main() {
    int l,l1;/* number of lines in the file */
    FILE* fp = fopen("antonyms.txt","r"); /* opening the files in reading mode */
    FILE* fp1 = fopen("synonyms.txt","r");

    if(fp== NULL || fp1 == NULL){
        printf("Error happened during opening the files!"); /* print error message when failing occurs */
        exit(1);
    }
    l= calculateLine(fp); /* calculating the number of lines for each file */
    l1= calculateLine(fp1);
    int* numboflines= (int*) malloc(sizeof(int)*l);
    int* numboflines1=(int*) malloc(sizeof(int)*l);
    fseek(fp,0,SEEK_SET);
    fseek(fp1,0,SEEK_SET);
    countcharsinLine(fp,l,numboflines);
    countcharsinLine(fp1,l1,numboflines1); /* filling the numboflines array which holds the number of characters in each line in the file */
    fseek(fp,0,SEEK_SET);
    fseek(fp1,0,SEEK_SET);
    handleFile(fp,fp1,l,l1);
    fclose(fp);
    fclose(fp1);
    free(numboflines);
    free(numboflines1);
    return 0;
}
