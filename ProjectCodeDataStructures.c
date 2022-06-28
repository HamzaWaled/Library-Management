// This program aims to help a Library Assistant to keep track of the borrowed books.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 50

typedef struct Book_d{
    char BookName[SIZE];
    char BookISBN[SIZE];
    int status;
    char BorrowerName[SIZE];
    char DueDate[SIZE];
    char field[SIZE];
    struct Book_d *next, *previous;
}Book;


Book *head = NULL;

int MenuF(void);
void InsertatFirst(Book *newnode);
void InsertatMiddleEnd(Book *newnode);
void DisplayBooks(void);
void createLinkedList(FILE *fdata);
int UpdateBook(char ISBN[]);
int DeleteBook(char ISBN[]);
void DeleteFromBeg(void);
int DeleteFromBegEnd(char ISBN[]);
int SearchMenu(void);
int SearchbyISBN(char ISBN[]);
int SearchbyField(char Field[]);
int SearchbyName(char Name[]);
int SearchbyBorrower(char BName[]);

int main(void){
  FILE *fdata,*fassendingdata;
  char FilePath[SIZE];
  Book *newnode;
  Book* newbook, *walker;
  int x, flag,n;
  char ISBN[SIZE], Field[SIZE],Name[SIZE],BName[SIZE];

  printf("Please enter your File Path: \n ");
  gets(FilePath);


  fdata = fopen(FilePath,"r");


  if(fdata == NULL){
    printf("\nThe file does not exist\n");
    return 0;
  }

  printf("\n\t\t\tHello, welcome to our library! \n");
  printf("\n\t\t\t******************************** \n\n");

  //Creating the head
  head = (Book *) malloc(sizeof(Book));
  fgets(head->BookName, SIZE , fdata);

  fgets(head->BookISBN, SIZE ,fdata);
  fscanf(fdata,"%d \n",&head->status); 
  fgets(head->BorrowerName, SIZE ,fdata);
  fgets(head->DueDate, SIZE , fdata);
  fgets(head->field, SIZE , fdata);
  head->next = NULL;
  head->previous = NULL;
  createLinkedList(fdata); // This function is for creating our linked list

  do{
    x = MenuF(); //Menu has to be displaced until the users exit the program
    if(x == 1){
      if(head == NULL){
        printf("\n\nThere are no books!\n\n"); //This line is in case the file has no books, or in case the user deleted all the books.
      }
      else{
        DisplayBooks(); //Function to display the books
      }
    }
    else if(x == 2){ //Insering new book 
        int flag1 = 0;
        newbook = (Book*)malloc(sizeof(Book));
        printf("\nPlease enter the new book criterias: \n");
        getchar(); 
        printf("\nBook Name: ");
        fgets(newbook->BookName, SIZE,stdin);
        printf("\nBook ISBN: ");
        fgets(newbook->BookISBN, SIZE, stdin);
        for(walker = head; walker != NULL; walker = walker->next){
            if(strcmp(newbook->BookISBN, walker->BookISBN) == 0){
                printf("\n\nThis ISBN already exists!\n\n"); // if the users enters an ISBN that does already exist
                flag1 = 1;
            }
        }
        if(flag1 == 0){ // if the flag1 did not change to 1 and stays 0, means that our ISBN does not exist, hence we can insert the new book
            printf("\nBook status: ");
            scanf("%d",&newbook->status);

            printf("\nBook Borrower Name: ");
            getchar();
            fgets(newbook->BorrowerName, SIZE, stdin);

            printf("\nBook DueDate: ");
            fgets(newbook->DueDate, SIZE, stdin);

            printf("\nBook Field: ");
            fgets(newbook->field, SIZE, stdin);

            newbook->next = NULL;
            newbook->previous = NULL;
            if(strcasecmp(newbook->BookName,head->BookName) < 0 || head == NULL){ // the function strcasecmp is used to make our program not a sensitive case
                InsertatFirst(newbook);                       //head == NULL is added here in case there is no books, or the user deleted all the books
            }
            else{
                InsertatMiddleEnd(newbook);
            }
        }
    }
    else if(x == 3){ //Updating a book
      if(head == NULL){ //in case there is no books, or the user deletd all the book, there is no book to be updated
        printf("\nThere are no books!\n");
      }
      else{
        printf("\nEnter the ISBN of the book to be updated: ");
        getchar();
        fgets(ISBN, SIZE, stdin);
        flag = UpdateBook(ISBN);
        if(flag == 0){
        printf("\n\nThe ISBN entered does not exist!\n\n");
        }
        else{
        printf("\n\nThe book was updated successfully!\n\n");
        }
      }
    }
    else if(x == 4){ //Deleting a book
      if(head == NULL){ // in case there is no books, there is no book to be deleated
        printf("\nThere are no books!\n");
      }
      else{
        printf("\nEnter the ISBN of the book to be deleted: ");
        getchar();
        fgets(ISBN, SIZE, stdin);
        flag = DeleteBook(ISBN);
        if(flag == 0){
          printf("\n\nThe ISBN entered does not exist!\n\n");
        }
        else{
          printf("\n\nThe book was deleted successfully!\n\n");
        }
      }
    }
    else if(x == 5){ //Searching for a book
      n=SearchMenu();
      if(n==1){ //Search by ISBN
        printf("\n\nEnter the ISBN of the book to be searched: ");
        getchar();
        fgets(ISBN, SIZE, stdin);
        flag = SearchbyISBN(ISBN);
        if(flag == 0){
          printf("\n\nThe ISBN entered does not exist!\n\n");
        }
      }
      else if(n == 2){ //Search by Field
        printf("\nEnter the Field of the book to be searched: ");
        getchar();
        fgets(Field, SIZE,stdin);
        flag = SearchbyField(Field);
        if(flag == 0){
          printf("\n\nThe Field entered does not exist!\n\n");
        }
      }
      else if(n == 3){ // Search by Book Name
        printf("\nEnter the name of the book to be searched: ");
        getchar();
        fgets(Name, SIZE,stdin);
        flag = SearchbyName(Name);
        if(flag == 0){
          printf("\n\nThe name entered does not exist!\n\n");
        }
      }
      else if(n == 4){ //Search by a borrower name of a borrowed book
        printf("\nEnter the Borrower Name of the Borrowed book to be searched: ");
        getchar();
        fgets(BName, SIZE,stdin);
        flag = SearchbyBorrower(BName);
        if(flag == 0){
            printf("NONE!"); 
        }
      }
      else{
        printf("Invalid Input!\n"); // In case the user enters a wrong input for the sub-menu
      }
    }
    else if(x == 6){ // Exit from the program
        printf("\nCome back again!\n");
        fclose(fdata);
        fdata = fopen("bookData.txt","w");
      for(walker = head; walker!= NULL; walker = walker->next){
        fprintf(fdata,"%s",walker->BookName);
        fprintf(fdata,"%s",walker->BookISBN);
        fprintf(fdata,"%d\n",walker->status);
        fprintf(fdata,"%s",walker->BorrowerName);
        fprintf(fdata,"%s",walker->DueDate);
        fprintf(fdata,"%s",walker->field);
      }  
    }
    else{
        printf("Invalid input!"); // In case the user enters a wrong input for the Menu
    }
  }while(x != 6);
  fclose(fdata);
  return 0;
}

// Menu Function:

int MenuF(void){
  int x;
  printf(" \n\t\t\t******** Menu ******** \n\n");
  printf(" \t\t\t1. Display all Books \n\t\t\t2. Insert a new Book \n\t\t\t3. Update a book by ISBN \n\t\t\t4. Delete a book by ISBN \n\t\t\t5. Search \n\t\t\t6. Exit \n\n");
  scanf("%d", &x);
  return x;
}

//Creation of the linked list function:

void createLinkedList(FILE *fdata){
  Book *newnode;
  char field[SIZE];
  int i;
  while(!feof(fdata)){
    newnode = (Book *) malloc(sizeof(Book));
    fgets(newnode->BookName, SIZE , fdata);
    fgets(newnode->BookISBN, SIZE , fdata);
    fscanf(fdata,"%d \n",&newnode->status);
    fgets(newnode->BorrowerName, SIZE , fdata);
    fgets(newnode->DueDate, SIZE , fdata);
    fgets(newnode->field, SIZE , fdata);
    if(feof(fdata)){
        for(i = 0; i < strlen(newnode->field); i++){ // This loop in here add an /n for the last element of the file as long as the other elements have a /n
            field[i] = newnode->field[i];
        }
        field[i] = '\n';
        field[i+1] = '\0'; // \0 is added to stop copying from the file after the \n
        strcpy(newnode->field, field);
    }
    newnode->next = NULL;
    newnode->previous = NULL;
    if(strcasecmp(newnode->BookName,head->BookName) < 0){ //strcasecmp is a function that compares two strings without case sensitivity
      InsertatFirst(newnode);
    }
    else{
      InsertatMiddleEnd(newnode);
    }
  }
}

// Function to insert a book in the begining of the linked list (conserving the alphabetical order and without case sensitivity):

void InsertatFirst(Book *newnode){
  if(head == NULL){
    head = newnode;
    newnode->next = NULL;
  }
  else{
    newnode -> next = head;
    head -> previous = newnode;
    head = newnode;
  }
}

// Function to insert a book in the middle or end of the linked list (conserving the alphabetical order and without case sensitivity):

void InsertatMiddleEnd(Book *newnode){
  Book* walker;
  //Insertion at the middle by alphabetical order
  for(walker = head; walker->next!= NULL; walker = walker->next){
    if(strcasecmp(newnode->BookName, walker->next->BookName) < 0){
      newnode->next = walker->next;
      newnode->previous = walker;
      walker->next = newnode;
      walker->next->previous = newnode;
      return;
    }
  }
  //Insertion at the end
  walker->next = newnode;
  newnode->previous = walker;
  newnode->next = NULL;
}

// Function that diplays the books

void DisplayBooks(void){
  int i=1;
  Book *walker;
  for(walker = head; walker != NULL; walker = walker->next){
    printf("\n******** Book %d ********",i);
    printf("\nBook Name:%sBook ISBN: %sBook Status: %d\nBorrower Name: %sDue Date: %sField: %s\n\n", walker->BookName, walker->BookISBN, walker->status,walker->BorrowerName,walker->DueDate,walker->field);
    i++;
  }
}

// Function that updates the books (by ISBN)

int UpdateBook(char ISBN[]){
  Book *walker;
  for(walker = head; walker != NULL; walker = walker->next){
    if(strcmp(ISBN, walker->BookISBN) == 0){
      printf("\nEnter the new criteria of the book: ");
      printf("\n\nThe status of the book : ");
      scanf("%d", &walker->status);
      printf("\nThe Borrower Name : ");
      getchar();
      fgets(walker->BorrowerName, SIZE, stdin);
      printf("\nThe Due Date : ");
      fgets(walker->DueDate, SIZE, stdin);
      return 1;
    }
  }
  return 0;
}

// Function that deletes the books (by ISBN)

int DeleteBook(char ISBN[]){
  int flag;
  if(strcmp(ISBN,head->BookISBN)==0){
    DeleteFromBeg();
    return 1;
  }
  else{
    flag = DeleteFromBegEnd(ISBN);
    return flag;
  }
}

// Function to delete the book from the begining (if the book is situated at the begining)

void DeleteFromBeg(void){
  Book *DeletedBook = head;
  if(head->next == NULL){
    head = NULL;
  }
  else{
    head = head->next;
    head->previous=NULL;
    DeletedBook->next = NULL;
    free(DeletedBook);
  }
}

// Function to delete the book from the middle or end (if the book is situated at the middle or end)

int DeleteFromBegEnd(char ISBN[]){
  Book *walker, *DeletedBook;
  for(walker = head; walker->next->next != NULL; walker = walker->next){
    if(strcmp(ISBN,walker->next->BookISBN)==0){
      DeletedBook = walker->next;
      walker->next=DeletedBook->next;
      DeletedBook->next->previous=walker;
      DeletedBook->next=NULL;
      DeletedBook->previous=NULL;
      free(DeletedBook); // to free the book
      return 1;
    }
  }
    if(strcmp(ISBN,walker->next->BookISBN)==0){
      DeletedBook = walker->next;
      walker->next = NULL;
      DeletedBook->previous = NULL;
      free(DeletedBook);
      return 1;
  }
  return 0;
}

//Search Menu

int SearchMenu(void){
  int x;
  printf(" \n\t\t\t******* Sub-Menu *******");
  printf("\t\n\n\t\tTo search for your book, you want to: \n");
  printf(" \t\t1. Search by ISBN \n \t\t2. Search by Field \n \t\t3. Search by name \n \t\t4. Search for a borrowed book by its Borrower name \n\n");
  scanf("%d", &x);
  return x;
}

// Search by ISBN Function

int SearchbyISBN(char ISBN[]){
    Book *walker;
    for(walker = head; walker != NULL; walker = walker->next){
        if(strcmp(ISBN, walker->BookISBN) == 0){
            printf("\n\nThe book you're searching for is: \n");
            printf("\nBook Name:%sBook ISBN: %sBook Status: %d\nBorrower Name: %sDue Date: %sField: %s\n\n", walker->BookName, walker->BookISBN, walker->status,walker->BorrowerName,walker->DueDate,walker->field);
            return 1;
        }
    }
    return 0;
}

// Search by Field Function (without case sensitivity)

int SearchbyField(char Field[]){
    Book *walker;
    int flag = 0;
    for(walker = head; walker != NULL; walker = walker->next){
        if(strcasecmp(Field, walker->field) == 0){
            printf("\n\nThe book you're searching for is: \n");
            printf("\nBook Name:%sBook ISBN: %sBook Status: %d\nBorrower Name: %sDue Date: %sField: %s\n\n", walker->BookName, walker->BookISBN, walker->status,walker->BorrowerName,walker->DueDate,walker->field);
            flag = 1;
        }
    }
    return flag;
}

// Search by Name Function (without case sensitivity)

int SearchbyName(char Name[]){
    Book *walker;
    int flag = 0;
    for(walker = head; walker != NULL; walker = walker->next){
        if(strcasecmp(Name, walker->BookName) == 0){
            printf("\n\nThe book you're searching for is: \n");
            printf("\nBook Name:%sBook ISBN: %sBook Status: %d\nBorrower Name: %sDue Date: %sField: %s\n\n", walker->BookName, walker->BookISBN, walker->status,walker->BorrowerName,walker->DueDate,walker->field);
            flag = 1;
        }
    }
    return flag;
}

// Search by Borrower Name of borrowed books Function (without case sensitivity)

int SearchbyBorrower(char BName[]){
  Book *walker;
  int flag = 0;
  printf("\n\nHere are the book(s) borrowed by %s\n", BName);
  for(walker = head; walker != NULL; walker = walker->next){
    if(walker->status==1){
      if(strcasecmp(BName,walker->BorrowerName)==0){
        flag = 1;
        printf("\nBook Name:%sBook ISBN: %sBook Status: %d\nBorrower Name: %sDue Date: %sField: %s\n\n", walker->BookName, walker->BookISBN, walker->status,walker->BorrowerName,walker->DueDate,walker->field);
      }
    }
  }
  return flag;
}
