#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 50

typedef struct lib{
    int id;
    char name[80];
    int quantity;
}Book;
Book book[SIZE];
Book SortedBook[SIZE]; // for displaying the books unsorted


int capacity = 3; // the number of records in the array (number of books)

FILE *library, *testFile; // the test file is for replacing the original file after modification "deleting"
char file_name[20] = "library.txt";

void InsertBook();
void DeleteById();
void SearchById(int id, int startingIndex);
void SearchByName(char name[], int low, int high);
void DisplaySortedByName();
void DisplayUnSorted();

int main()
{
    library = fopen(file_name, "r");
    if(library == NULL){
        printf("The File can\'t be opened. \n");
    }
    else{
        int i=0;
        while(fscanf(library, "%d ", &book[i].id) != EOF){ // loading the books from the file
            char c;
            int j=0;
            while((c = fgetc(library)) != '\n'){
                book[i].name[j] = c;
                j++;
            }
            fscanf(library, "%d ", &book[i].quantity);
            i++;
        }
    }
    fclose(library);


    int numberOfLines=0; // calculating the number of lines and
                         // setting the capacity to the right number
    char c;
    library = fopen(file_name, "r");
    while((c=fgetc(library))!=EOF){
        if(c == '\n'){
            numberOfLines++;
        }
    }
    fclose(library);
    capacity = numberOfLines / 3;

    for(int i=0 ; i<SIZE ; i++){  // for coping data into the sorted book
        SortedBook[i].id = book[i].id;
        strcpy(SortedBook[i].name, book[i].name);
        SortedBook[i].quantity = book[i].quantity;
    }


    int flag=0; // to control the repetition of the menu

    do{  // menu loop
        int check;
        system("cls");
        printf("\nWelcome to the library\n\n\n");
        printf("Enter the number of the operation you want to perform : \n\n");
        printf("1. Insert a book.\n");
        printf("2. Delete a book by id.\n");
        printf("3. Search a book by id.\n");
        printf("4. Search a book by name.\n");
        printf("5. Display all books sorted by name.\n");
        printf("6. Display all books unsorted.\n");
        printf("7. Exit.\n\n");
        scanf("%d", &check);

        switch(check){
            case 1: system("cls"); printf("\n");
                    InsertBook(); break;
            case 2: system("cls"); printf("\n");
                    DeleteById(); break;
            case 3: system("cls"); printf("\n"); int id;
                    printf("Enter the id you want to search for : ");
                    scanf("%d", &id);
                    SearchById(id, 0); break;
            case 4: system("cls"); printf("\n"); char name[80];
                    printf("Enter the name of the book : ");
                    fflush(stdin);
                    gets(name);
                    SearchByName(name, 0, capacity-1); break;
            case 5: system("cls"); printf("\n");
                    DisplaySortedByName(); break;
            case 6: system("cls"); printf("\n");
                    DisplayUnSorted(); break;
            case 7: return 0;
            default: printf("Invalid choice.\n");

        }

        char repeat;
        printf("\nDo you want to perform another operation?[Y/N]: ");
        fflush(stdin);
        scanf("%c", &repeat);
        repeat = toupper(repeat);
        if(repeat != 'Y'){
            flag = 1;
        }

    }while(flag != 1);

    return 0;
}


void InsertBook(){

    library = fopen(file_name, "a");

    printf("Enter the book id : ");
    scanf("%d", &book[capacity].id);
    fflush(stdin);
    printf("Enter the book name : ");
    fflush(stdin);
    gets(book[capacity].name);
    printf("Enter the book quantity : ");
    scanf("%d", &book[capacity].quantity);
    fprintf(library, "%d\n%s\n%d\n", book[capacity].id, book[capacity].name, book[capacity].quantity);
    printf("The book has been inserted successfully.\n");
    capacity++;

    fclose(library);

    for(int i=0 ; i<SIZE ; i++){  // for coping data into the sorted book
        SortedBook[i].id = book[i].id;
        strcpy(SortedBook[i].name, book[i].name);
        SortedBook[i].quantity = book[i].quantity;
    }
}

void DeleteById(){
    int id;
    printf("Enter the id you want to delete : ");
    scanf("%d", &id);
    int lineNumber = 0;
    int delLineNumber;
    int flag = 0;
    char str[100]; // for reading the file lines

    for(int i=0 ; i<capacity; i++){ // specifying the line we want to delete
            if(id == book[i].id){
                delLineNumber = i*3; // (i*3) because every id in the file after 3 lines from the one before
                flag = 1;
                capacity--;
            }
    }

    library = fopen(file_name, "r");
    testFile = fopen("replace.txt", "w");

    while(fgets(str, 99, library) != NULL && flag == 1){
        if(lineNumber != delLineNumber && lineNumber != (delLineNumber+1) && lineNumber != (delLineNumber+2)){
            fputs(str, testFile);
        }
        lineNumber++;
    }


    fclose(library);
    fclose(testFile);

    remove(file_name);
    rename("replace.txt", file_name);

    if(flag == 1){
        printf("The book has been deleted successfully.\n");

        library = fopen(file_name, "r");
        int i=0;
        while(fscanf(library, "%d ", &book[i].id) != EOF){ // updating the info in the array of structure
            char c;
            int j=0;
            while((c = fgetc(library)) != '\n'){
                book[i].name[j] = c;
                j++;
            }
            fscanf(library, "%d ", &book[i].quantity);
            i++;
        }
        fclose(library);
    }
    else{
        printf("Not found.\n");
    }


}

void SearchById(int id, int startingIndex){ // recursively // linear search
    int i = startingIndex;
    if(book[i].id == id){
        printf("Book name : %s\n", book[i].name);
        printf("Book quantity : %d\n", book[i].quantity);
        return;
    }
    else if(startingIndex == SIZE){
        printf("Not found.\n");
        return;
    }
    SearchById(id, i+1);
}

void SearchByName(char name[], int low, int high){ // recursively // binary search

    for(int pass=1 ; pass<capacity ; pass++){ // for sorting
        for(int i=0 ; i<capacity-pass ; i++){
            if(strcmp(SortedBook[i].name, SortedBook[i+1].name) > 0){
                char temp[80];
                strcpy(temp, SortedBook[i].name);
                strcpy(SortedBook[i].name, SortedBook[i+1].name);
                strcpy(SortedBook[i+1].name, temp);
                int tempi;
                tempi = SortedBook[i].id;
                SortedBook[i].id = SortedBook[i+1].id;
                SortedBook[i+1].id = tempi;
                tempi = SortedBook[i].quantity;
                SortedBook[i].quantity = SortedBook[i+1].quantity;
                SortedBook[i+1].quantity = tempi;
            }
        }
    }

    int middle = (low+high)/2;

    if(low > high){
        printf("Not found\n");
        return;
    }
    else if(strcmp(name, SortedBook[middle].name) == 0){
        printf("Book id : %d\n", SortedBook[middle].id);
        printf("Book quantity : %d\n", SortedBook[middle].quantity);
        return;
    }
    else if(strcmp(name, SortedBook[middle].name) > 0){
        SearchByName(name, middle+1, high);
    }
    else{
        SearchByName(name, low, middle-1);
    }
}

void DisplaySortedByName(){
    for(int pass=1 ; pass<capacity ; pass++){ // for sorting
        for(int i=0 ; i<capacity-pass ; i++){
            if(strcmp(SortedBook[i].name, SortedBook[i+1].name) > 0){
                char temp[80];
                strcpy(temp, SortedBook[i].name);
                strcpy(SortedBook[i].name, SortedBook[i+1].name);
                strcpy(SortedBook[i+1].name, temp);
                int tempi;
                tempi = SortedBook[i].id;
                SortedBook[i].id = SortedBook[i+1].id;
                SortedBook[i+1].id = tempi;
                tempi = SortedBook[i].quantity;
                SortedBook[i].quantity = SortedBook[i+1].quantity;
                SortedBook[i+1].quantity = tempi;
            }
        }
    }
    printf("%-30s%-15s%s\n", "NAME", "ID", "QUANTITY");
    for(int i=0 ; i<capacity ; i++){
        printf("%-30s%-15d%d\n", SortedBook[i].name, SortedBook[i].id, SortedBook[i].quantity);
    }
}

void DisplayUnSorted(){
    printf("%-30s%-15s%s\n", "NAME", "ID", "QUANTITY");
    for(int i=0 ; i<capacity ; i++){
        printf("%-30s%-15d%d\n", book[i].name, book[i].id, book[i].quantity);
    }
}
