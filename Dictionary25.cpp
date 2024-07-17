#include <stdio.h>
#include <string.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include<conio.h>
#include<time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#pragma warning ( disable : 4996 )

struct person {
    char name[100];
    char familyname[100];
    char username[100];
    char password[100];
    int phonenumber;
    char email[100];
};
struct translate {
    char word[50];
    char meaning[100];
};
//for search function
struct forsearch {
    char word[50];
    char meaning[100];
} similarWords[1000];
int similarCount = 0;
//for search function
int calculateDifference(char* word1, char* word2) {
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    int diff = abs(len1 - len2);
    for (int i = 0; i < (len1 < len2 ? len1 : len2); i++) {
        if (word1[i] != word2[i]) {
            diff++;
        }
    }

    return diff;
}

//for search function
int readDictionary(struct translate dictionary[]) {
    FILE* fp = fopen("Dictionary.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int count = 0;
    while (fread(&dictionary[count], sizeof(struct translate), 1, fp) == 1) {
        count++;
    }

    fclose(fp);
    return count;
}
//for search function
void writeDictionary(struct translate dictionary[], int count) {
    FILE* fp = fopen("Dictionary.bin", "wb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fwrite(&dictionary[i], sizeof(struct translate), 1, fp);
    }

    fclose(fp);
}
struct Node {
    struct translate data;
    struct Node* next;
    struct Node* prev;
};
struct Node* head = NULL;
struct Node* tail = NULL;

void write_linkedlist() {
    struct Node* temp1 = head;
    FILE* fp;
    fp = fopen("Dictionary.bin", "wb");
    if (fp == NULL) {
        printf("ERROR\n");
        exit(1);
    }
    while (temp1 != NULL) {
        fwrite(&temp1->data, sizeof(struct translate), 1, fp);
        temp1 = temp1->next;
    }
    fclose(fp);
}

void read_linkedlist() {
    struct translate data;
    FILE* fp;
    fp = fopen("Dictionary.bin", "rb");
    if (fp == NULL) {
        printf("File not found!\n");
        return;
    }
    while (fread(&data, sizeof(struct translate), 1, fp) == 1) {
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        temp->data = data;
        temp->next = NULL;
        temp->prev = NULL;
        if (head == NULL) {
            head = temp;
            tail = temp;
        }
        else {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
    }
    fclose(fp);
}

void Add_Word() {
    system("cls");
    char word2[100], meaning2[100];
    printf("Enter a Word:\n");
    scanf("%s", word2);
    printf("Enter the meaning : \n");
    getchar();
    fgets(meaning2, 99, stdin);
    size_t len = strlen(meaning2);
    if (len > 0 && meaning2[len - 1] == '\n') {
        meaning2[len - 1] = '\0';
    }

    struct Node* temp2 = head;
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    strcpy(temp->data.word, word2);
    strcpy(temp->data.meaning, meaning2);
    temp->prev = NULL;
    temp->next = NULL;

    if (head == NULL) {
        head = temp;
        tail = temp;
        write_linkedlist();
        return;
    }
    else {
        while (temp2 != NULL) {
            if (strcmp(temp2->data.word, word2) == 0) {
                printf("This word exists\n");
                free(temp);
                return;
            }
            else if (strcmp(temp2->data.word, word2) > 0) {
                if (temp2 == head) {
                    temp->next = temp2;
                    head = temp;
                    temp2->prev = temp;
                }
                else {
                    temp->next = temp2;
                    temp->prev = temp2->prev;
                    temp2->prev->next = temp;
                    temp2->prev = temp;
                }
                write_linkedlist();
                return;
            }
            else {
                temp2 = temp2->next;
            }
        }
        tail->next = temp;
        temp->prev = tail;
        tail = temp;
    }
    write_linkedlist();
}

void Edit_Word() {
    system("cls");
    struct Node* temp1 = head;
    bool flag = false;
    char word1[100], meaning1[150];
    printf("enter a word:\n");
    scanf("%s", word1);

    while (temp1 != NULL) {
        if (strcmp(temp1->data.word, word1) == 0) {
            flag = true;
            break;
        }
        else {
            temp1 = temp1->next;
        }
    }
    if (flag == false)
        printf("Word not found\n");
    if (flag == true) {
        int n;
        printf("1.delet meaning\n");
        printf("2.add new meaning\n");
        printf("3.edit meaning\n");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            strcpy(temp1->data.meaning, "");
            break;
        case 2:
            printf("enter the meaning:\n");
            getchar();
            fgets(meaning1, 149, stdin);
            strcat(temp1->data.meaning, "-");
            strcat(temp1->data.meaning, meaning1);
            break;
        case 3:
            printf("Enter the meaning : \n");
            getchar();
            fgets(meaning1, 149, stdin);
            strcpy(temp1->data.meaning, meaning1);
            break;
        default:
            printf("please type 1 to 3");
            break;
        }

    }
    write_linkedlist();
}

void Remove_Word() {
    system("cls");
    bool flag = false;
    char word[100];
    printf("Enter a word:\n");
    scanf("%s", word);
    struct Node* temp1 = head;
    if (head == NULL) {
        printf("dictionary is empty");
        return;
    }
    else {
        while (temp1 != NULL)
        {
            if (strcmp(temp1->data.word, word) == 0) {
                flag = true;
                break;
            }
            temp1 = temp1->next;

        }
        if (flag == false) {
            printf("Word not found\n");
        }
        if (flag == true) {
            if (temp1 == head) {
                head = temp1->next;
                temp1->next->prev = NULL;
                printf("Word remove successfully\n");

            }
            else if (temp1->next == NULL) {
                tail = temp1->prev;
                temp1->prev->next = NULL;
                printf("Word remove successfully\n");

            }
            else {
                temp1->prev->next = temp1->next;
                temp1->next->prev = temp1->prev;
                printf("Word remove successfully\n");
            }

        }

    }
    write_linkedlist();
}


void Read_From_file() {
    system("cls");
    char path[300];
    printf("Enter the file path:\n");
    scanf("%s", path);
    char line[200];
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("ERROR\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
      

        if (strncmp(line, "EOF", 3) == 0) {
            break;
        }

        char new_word[50], new_mean[100];
        if (sscanf(line, "%*d- word: %[^,], translation: %[^\n]", new_word, new_mean) == 2) {
          

            struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
            strcpy(temp->data.word, new_word);
            strcpy(temp->data.meaning, new_mean);
            temp->next = NULL;

            if (head == NULL) {
                head = temp;
                tail = temp;
                temp->prev = NULL;
            }
            else {
                struct Node* temp2 = head;
                bool flag = false;

                while (temp2 != NULL) {
                    if (strcmp(temp2->data.word, new_word) == 0) {
                        flag = true;
                        break;
                    }
                    else {
                        temp2 = temp2->next;
                    }
                }
                if (!flag) {
                    temp2 = tail;
                    temp->prev = temp2;
                    tail = temp;
                    temp2->next = temp;
                }
                else {
                    free(temp); 
                }
            }
        }

    }
    write_linkedlist();
    printf("Word added successfully\n");
    fclose(fp);
}
void Suggested_Words(char searching[]) {
    system("cls");
    printf("1. Accept\n");
    printf("2. Reject\n");
    printf("Enter the number:\n");
    int a;
    scanf("%d", &a);
    getchar();

    if (a == 1) {
        read_linkedlist();

        struct Node* temp2 = head;
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        strcpy(temp->data.word, searching);
        printf("Enter the meaning:\n");
        char mean2[100];
        fgets(mean2, sizeof(mean2), stdin);
        mean2[strcspn(mean2, "\n")] = 0;
        strcpy(temp->data.meaning, mean2);
        temp->prev = NULL;
        temp->next = NULL;

        if (head == NULL) {
            head = temp;
            tail = temp;
        }
        else {
            while (temp2 != NULL) {
                if (strcmp(temp2->data.word, searching) > 0) {
                    if (temp2 == head) {
                        temp->next = temp2;
                        head = temp;
                        temp2->prev = temp;
                    }
                    else {
                        temp->next = temp2;
                        temp->prev = temp2->prev;
                        temp2->prev->next = temp;
                        temp2->prev = temp;
                    }
                    write_linkedlist();
                    return;
                }
                else {
                    temp2 = temp2->next;
                }
            }
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        write_linkedlist();
    }
}



void Show_All_Words() {
    system("cls");
    struct Node* temp1 = head;
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    FILE* fp = fopen("show_words.txt", "w");
    if (fp == NULL) {
        printf("ERROR opening file!\n");
        return;
    }

    for (char cc = 'a'; cc <= 'z'; cc++) {
        bool found = false;
        fprintf(fp, "\n%c:\n", cc);
        printf("\n%c:\n", cc);

        temp1 = head; 
        while (temp1 != NULL) {
            if (temp1->data.word[0] == cc) {
                fprintf(fp, "Word: %s, Translation: %s\n", temp1->data.word, temp1->data.meaning);
                printf("Word: %s, Translation: %s\n", temp1->data.word, temp1->data.meaning);
                found = true;
            }
            temp1 = temp1->next;
        }

        if (!found) {
            fprintf(fp, "No words found for %c\n", cc);
        }
    }
    fclose(fp);
}

//Admin account
void Admin_menu() {
    printf("1 : Add Word\n");
    printf("2 : Edit Word\n");
    printf("3 : Remove Word\n");
    printf("4 : Read From File\n");
    printf("5 : Suggested Words\n");
    printf("6 : Show All Words\n");
    printf("7 : Back\n");
}


void forAdmin1() {
    char searching[100];
    int a;
    read_linkedlist();
    while (1) {
        // system("cls");
        Admin_menu();
        printf("Enter a number :\n");
        scanf("%d", &a);
        switch (a) {
        case 1:
            Add_Word();

            break;
        case 2:
            Edit_Word();
            break;
        case 3:
            Remove_Word();

            break;
        case 4:

            Read_From_file();
            break;
        case 5:
            Suggested_Words(searching);
            break;
        case 6:
            Show_All_Words();
            break;
        case 7:
            return;
            break;
        default:
            printf("ERROR!");


        }
    }

}

void menu3() {
    printf("1. Search\n");
    printf("2. Litner\n");
    printf("3. Test\n");
    printf("4. Show Information\n");
    printf("5. Edit information\n");
    printf("6. Back\n");
}
void Search() {
    system("cls");
    struct translate dictionary[1000];
    struct forsearch similarWords[1000];
    int count = readDictionary(dictionary);
    int similarCount = 0;

    char searching[50];
    bool found = false;

    printf("Enter the word to search:\n");
    scanf("%s", searching);
    getchar();

    for (int i = 0; i < count; i++) {
        if (strcmp(dictionary[i].word, searching) == 0) {
            printf("Word found\nMeaning: %s\n", dictionary[i].meaning);
            found = true;

            printf("If you want to add a new meaning to this word, enter '=', otherwise enter '-':\n");
            char ccc;
            ccc = getchar();
            getchar(); 
            if (ccc == '=') {
                printf("Enter the new meaning:\n");
                char meaning1[100];
                fgets(meaning1, sizeof(meaning1), stdin);
                meaning1[strcspn(meaning1, "\n")] = 0;
                strcat(dictionary[i].meaning, "-");
                strcat(dictionary[i].meaning, meaning1);
                writeDictionary(dictionary, count);
                printf("New meaning added successfully.\n");
                return;
            }
            else {
                return;
            }
        }

        int diff = calculateDifference(dictionary[i].word, searching);
        if (diff > 0 && diff <= 1) {
            strcpy(similarWords[similarCount].word, dictionary[i].word);
            strcpy(similarWords[similarCount].meaning, dictionary[i].meaning);
            similarCount++;
        }
    }

    if (!found) {
        printf("Word not found in dictionary.\n");
        if (similarCount > 0) {
            printf("Similar words:\n");
            for (int j = 0; j < similarCount; j++) {
                printf("Word: %s\nMeaning: %s\n", similarWords[j].word, similarWords[j].meaning);
            }
        }
        else {
            printf("No similar words found.\n");
        }
        similarCount = 0;

        printf("If you want to suggest this word to the admin, enter '+', otherwise, enter '-':\n");
        char s;
        s = getchar();
        getchar();  
        if (s == '+') {
            Suggested_Words(searching);
        }
     
    }
}


void Show_Information(char user[], char pass[], struct person* account, int count) {
    system("cls");
    for (int i = 0; i < count; i++) {
        if (strcmp(account[i].username, user) == 0 && strcmp(account[i].password, pass) == 0) {
            printf("name:%s\nfamilyname:%s\nusername:%s\npassword:%s\nphonenumber:%d\nemail:%s\n", account[i].name, account[i].familyname, account[i].username, account[i].password, account[i].phonenumber, account[i].email);
        }
    }
}


void Edit_information(char user[], char pass[], struct person* account, int count) {
    system("cls");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(account[i].username, user) == 0 && strcmp(account[i].password, pass) == 0) {
            found = 1;
            int n, b;
            char edit[100];
            while (1) {
                printf("1:Edit name\n2:Edit familyname\n3:Edit username\n4:Edit password\n5:Edit phonenumber\n6:Edit email\n7: exit\nEnter a number:\n");
                scanf("%d", &n);
                switch (n) {
                case 1:
                    printf("Enter new name:\n");
                    scanf("%s", edit);
                    strcpy(account[i].name, edit);
                    system("cls");
                    break;
                case 2:
                    printf("Enter new familyname:\n");
                    scanf("%s", edit);
                    strcpy(account[i].familyname, edit);
                    system("cls");
                    break;
                case 3:
                    printf("Enter new username:\n");
                    scanf("%s", edit);
                    strcpy(account[i].username, edit);
                    system("cls");
                    break;
                case 4:
                    printf("Enter new password:\n");
                    scanf("%s", edit);
                    strcpy(account[i].password, edit);
                    system("cls");
                    break;
                case 5:
                    printf("Enter new phonenumber:\n");
                    scanf("%d", &b);
                    account[i].phonenumber = b;
                    system("cls");
                    break;
                case 6:
                    printf("Enter new email:\n");
                    scanf("%s", edit);
                    strcpy(account[i].email, edit);
                    system("cls");
                    break;
                case 7:
                    printf("Edit is succsefull\n");
                    goto save_and_exit;
                default:
                    printf("Enter a numbe between 1 to 7\n");
                    break;
                }
            }
            printf("Information updated successfully.\n");
            break;


        }
    }
    if (!found) {
        printf("User not found!\n");
    }
save_and_exit:
    if (found) {
        FILE* f = fopen("account.bin", "r+b");
        if (f == NULL) {
            printf("Error opening file!\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fseek(f, i * sizeof(struct person), SEEK_SET);
            fwrite(&account[i], sizeof(struct person), 1, f);
        }
        fclose(f);
        printf("Information updated successfully.\n");
    }

}



int formembers(char user[], char pass[], struct person* account, int count) {
    int a;
    while (1) {
        menu3();
        printf("Enter a number :\n");
        scanf("%d", &a);
        switch (a) {
        case 1:
            Search();
            break;
        case 2:
            //  Litner();
            break;
        case 3:
            // Test();
            break;
        case 4:
            Show_Information(user, pass, account, count);
            break;
        case 5:
            Edit_information(user, pass, account, count);
            break;

        case 6:
            return 0;
        default:
            printf("Error");
            break;
        }

    }




}


void menu() {
    printf("1 : Sign Up\n");
    printf("2 : Sign In\n");
    printf("3 : Forget Password\n");
    printf("4 : Exit\n");

}


int sign_up(struct person* account, int count) {
    system("cls");
    int flag, i = 0;
    char c;
    printf("Enter your name :\n");
    scanf("%s", account[count].name);
    printf("Enter your familyname :\n");
    scanf("%s", account[count].familyname);
    char new_username[100];
    printf("Enter your username :\n");
    scanf("%s", new_username);
    do {
        flag = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(new_username, account[j].username) == 0) {
                flag = 1;
                printf("This username is already taken . \n");
                break;
            }
        }
        if (flag == 1) {
            scanf("%s", new_username);
        }
    } while (flag == 1);
    strcpy(account[count].username, new_username);
    printf("Enter your password :\n");
    while (i < 20) {
        c = _getch();
        if (c == '\r')
            break;
        printf("*");
        account[count].password[i] = c;
        i++;
    }
    account[count].password[i] = '\0';
    printf("\nEnter your phonenumber :\n");
    scanf("%d", &account[count].phonenumber);
    printf("Enter your email :\n");
    scanf("%s", account[count].email);
    FILE* f;
    f = fopen("account.bin", "ab");
    if (f == NULL) {
        printf("Error opening file!\n");
        return count;
    }
    fwrite(&account[count], sizeof(struct person), 1, f);
    fclose(f);

    return count + 1;

}
void sign_in(struct person* account, int count) {
    int j, i = 0;
    int flag1 = 0;
    char c;
    system("cls");
    char user[100];
    char pass[100];
    printf("Username :\n");
    scanf("%s", user);
    printf("Password : \n");
    while (i < 99) {
        c = _getch();
        if (c == '\r')
            break;
        printf("*");
        pass[i] = c;
        i++;
    }
    pass[i] = '\0';
    printf("\n");
    for (j = 0; j < count; j++) {
        if (strcmp(user, account[j].username) == 0 && strcmp(pass, account[j].password) == 0) {
            flag1 = 1;
            break;
        }
    }
    if (flag1 == 0) {
        printf("Your info is not correct!\n");
    }
    if (flag1 == 1) {
        if (strcmp(account[j].username, "Admin") == 0 && strcmp(account[j].password, "Admin") == 0) {
            forAdmin1();
        }
        else {
            formembers(user, pass, account, count);
        }
    }

}

void forget_password(struct person* account, int count) {
    system("cls");
    int j;
    int flag1 = 0;
    char user[100];
    printf("Enter your username :\n");
    scanf("%s", user);
    int phone;
    printf("Enter your phonenumber : \n");
    scanf("%d", &phone);
    for (j = 0; j <= count; j++) {
        if ((strcmp(user, account[j].username) == 0) && (phone == account[j].phonenumber)) {
            flag1 = 1;
            printf("Enter your new password :\n");
            scanf("%s", account[j].password);
            break;
        }
    }
    if (flag1 == 0) {
        printf("Your info is not correct!\n");
        return;
    }

    FILE* f;
    f = fopen("account.bin", "r+b");
    fseek(f, j * sizeof(struct person), SEEK_CUR);
    fwrite(&account[j], sizeof(struct person), 1, f);
    fclose(f);


}


int main() {
    int i;
    struct person account[100];
    FILE* f;
    f = fopen("account.bin", "rb");
    if (f) {
        for (i = 0; !feof(f); i++) {
            fread(&account[i], sizeof(struct person), 1, f);
        }
        fclose(f);
    }
    else {

        i = 0;
    }
    int c;
    while (1) {
        system("cls");
        menu();
        printf("Enter a number : \n");
        c = getch();
        switch (c) {
        case '1':
            i = sign_up(account, i);
            break;
        case '2':
            sign_in(account, i);
            break;
        case '3':
            forget_password(account, i);
            break;
        case '4':
            f = fopen("account.bin", "rb");
            if (f) {
                for (int j = 0; j <= i; j++) {
                    fwrite(&account[j], sizeof(struct person), 1, f);
                }
                fclose(f);
            }
            else {

                return 0;
            }
            return 0;
            break;
        default:
            printf("ERROR");
            break;
        }

        Sleep(2000);
    }
}
