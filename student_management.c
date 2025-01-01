#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 50

int number_of_student;

typedef struct{
    char name[25];
    int id;
    // char courses[10][10];
} Student;

typedef struct{
    int size;
    Student students[];
} Info;

void add(Student[], char[]);
void list(Student[], int);
void count();
void find(Student[]);
void find_with_id(Student[]);
void find_with_name(Student[]);
void delete_with_id(Student[], char[]);
void load_from_file(char[], Student[]);
void update_file(char[], Student[]);
void press_any_key();
void remove_from_array(Student[], int);

int main(){
    Student students[MAX];
    char command;
    char address[] = "database.txt";
    char buffer[5];
    char instruction[] = "\nSCHOOL MANAGEMENT SYSTEM\n\n\
Functions:\n\
- a: add\n\
- f: find\n\
- c: count\n\
- l: list\n\
- d: delete\n\
- x: exit\n\n\
--> Your command: ";

    load_from_file(address, students);

    while(true){
        system("clear");
        printf("%s", instruction);

        // avoid using scanf
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%c", &command);

        switch (command)
        {
            case 'x':
                exit(0);
                break;
            case 'a':
                add(students, address);
                break;
            case 'f':
                find(students);
                break;
            case 'c':
                count();
                break;
            case 'l':
                list(students, number_of_student);
                break;
            case 'd':
                delete_with_id(students, address);
                break;
            default:
                printf("\n\nWrong command.\n");
                break;
        }
        press_any_key();
    }
    return 0;
}

void press_any_key(){
    printf("\nPress ENTER to continue...");
    getchar();
    system("clear");
}

void load_from_file(char address[], Student students[]){
    int student_index = 0;

    FILE *fptr;
    
    fptr = fopen(address, "r");
    // create a file if it is not existed
    if (fptr == NULL) {
        fptr = fopen(address, "w");
        fclose(fptr);
    }

    while(fscanf(fptr, "%d;%s", &students[student_index].id, students[student_index].name) != EOF){
        student_index++;
    }
    number_of_student = student_index;

    fclose(fptr);
}

void update_file(char address[], Student students[]){
    FILE *fptr;
    fptr = fopen(address, "w");
    
    for(int i = 0; i < number_of_student; i++){
        fprintf(fptr, "%d;%s\n", students[i].id, students[i].name);
    }
    fclose(fptr);
}

void add(Student students[], char address[]){
    char name[25];

    system("clear");
    printf("ADD STUDENT\n\nStudent name: ");
    //input
    fgets(name, 25, stdin);
    name[strlen(name) - 1] = '\0';

    number_of_student += 1;
    
    Student new_student;
    strcpy(new_student.name, name);
    new_student.id = number_of_student;

    students[number_of_student - 1] = new_student;

    update_file(address, students);

    printf("\nStudent added successfully!\n");
}

void list(Student students[], int n){
    system("clear");

    printf("LIST OF STUDENTS\n\nID\tName\n");
    for(int i = 0; i < n; i++){        
        printf("%02d\t%s\n", students[i].id, students[i].name);
    }
}

void count(){
    system("clear");

    printf("THE NUMBER OF STUDENTS\n\n");
    if(number_of_student > 1){
        printf("There are %d students.\n", number_of_student);
    }else if (number_of_student == 1){
        printf("There is 1 student.\n");
    }else{
        printf("There is no student.\n");
    }
}

void find(Student students[]){
    int option;
    char buffer[5];
    system("clear");
    
    printf("FIND STUDENTS\n\n");
    printf("Find with:\n- Name (1)\n- ID (2)\n");
    printf("\n--> ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &option);

    switch (option)
    {
    case 1:
        find_with_name(students);
        break;
    case 2:
        find_with_id(students);
        break;
    default:
        printf("Invalid option.\n");
        break;
    }
}

void find_with_id(Student students[]){
    int id;
    char buffer[25];

    system("clear");

    printf("Student's ID: ");
    // avoid using scanf
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &id);

    if(id > number_of_student - 1 | id < 0){
        printf("Can't find a student with that ID.");
    }else{
        list(&students[id-1], 1);
    }
}

void find_with_name(Student students[]){
    char name[25];
    char *res = NULL;
    Student array_found[MAX];
    int found = 0;

    system("clear");

    printf("Student's name contains: ");
    fgets(name, 25, stdin);
    name[strlen(name) - 1] = '\0';

    for (int i = 0; i < number_of_student; i++)
    {
        res = strstr(students[i].name, name);
        if(res != NULL){
            array_found[found] = students[i];
            found += 1;
        }

    }
    if(found == 0){
        printf("\nNo student's name is matched.\n");
    }else{
        list(array_found, found);
    }
}

void remove_from_array(Student students[], int id){
    int index = id - 1;
    if (index < number_of_student - 1){ // check if it's not the last element of the array
        for (int i = index; i < number_of_student - 1; i++){
            students[i] = students[i + 1];
            students[i].id -= 1; //ID stays still
        }
    }
    number_of_student -= 1;
}

void delete_with_id(Student students[], char address[]){
    char buffer[5];
    int id;
    system("clear");
    
    printf("DELETE STUDENTS\n\nStudent's ID: ");

    // avoid using scanf
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &id);

    if(id <= number_of_student){
        // remove from current array
        remove_from_array(students, id);
        // remove from database
        update_file(address, students);
        
        printf("\nDelete student successfully!\nOther students'ID may be changed.\n");
    }else{
        printf("Invalid ID.\n");
    }
    
}