#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>

#define PIN_LENGTH 4
#define FILENAME "users.txt"

// Function prototypes
void loadusers();
void saveusers();
void registeruser();
void getPIN(char *pin);
int login();
int userindex(const char *userid);
void withdraw();
void deposit();
void checkbalance();

// Structure
struct User {
    char userid[20];
    int pin;
    double balance;
};
typedef struct User User;

User *users = NULL;    // Dynamically allocated users array
int userCount = 0;
char currentUserid[20];

// Function to get PIN securely
void getPIN(char *pin) {
    char ch;
    int i = 0;

    printf("Enter PIN: ");
    while (i < PIN_LENGTH) {
        ch = getch();  // Read a single character without showing it
        if (ch >= '0' && ch <= '9') {
            pin[i++] = ch;
            printf("*");  // Display '*'
        } else if (ch == '\b' && i > 0) {  // Handle backspace
            i--;
            printf("\b \b");
        }
    }
    pin[PIN_LENGTH] = '\0';  // Null-terminate the string
    printf("\n");
}

int main() {
    int choice;
    strcpy(currentUserid, "");
    loadusers();

    while(1) {
        printf("\n ATM MENU");
        printf("\n1. Register\n2. Login\n3. Exit\n Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registeruser();
                break;

            case 2:
                if(login()) {
                    printf("\n Login Successful.\n");
                    do {
                        printf("\n1. Withdraw\n2. Deposit\n3. Check Balance\n4. Exit\n Enter your choice: ");
                        scanf("%d", &choice);

                        switch(choice) {
                            case 1:
                                withdraw();
                                break;

                            case 2:
                                deposit();
                                break;

                            case 3:
                                checkbalance();
                                break;

                            case 4:
                                strcpy(currentUserid, "");
                                break;

                            default:
                                printf("Invalid choice.\n");
                        }
                    } while(strlen(currentUserid) > 0);
                }
                break;

            case 3:
                free(users);
                saveusers();
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}

// Function to load users
void loadusers() {
    FILE *file = fopen(FILENAME, "rb");
    if(file) {
        fread(&userCount, sizeof(int), 1, file);
        users = (User *)malloc(userCount * sizeof(User));
        if(users == NULL) {
            printf("\n Memory allocation failed!\n");
            exit(1);
        }
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

// Function to save users
void saveusers() {
    FILE *file = fopen(FILENAME, "wb");
    if(file) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

// Function to register
void registeruser() {
    User newuser;
    User *temp;
    char pin[PIN_LENGTH + 1];

    printf("\n Enter User ID: ");
    scanf("%s", newuser.userid);

    for(int i = 0; i < userCount; i++) {
        if(strcmp(newuser.userid, users[i].userid) == 0) {
            printf("User ID already exists.\n");
            return;
        }
    }

    getPIN(pin);
    newuser.pin = atoi(pin);
    newuser.balance = 0;

    temp = (User *)realloc(users, (userCount + 1) * sizeof(User));
    if(temp == NULL) {
        printf("\n Memory reallocation failed!\n");
        exit(1);
    }
    users = temp;

    users[userCount++] = newuser;
    saveusers();
    printf("User registered successfully.\n");
}

// Function to login
int login() {
    char inputUserid[20];
    char pin[PIN_LENGTH + 1];

    printf("\n Enter User ID: ");
    scanf("%s", inputUserid);

    getPIN(pin);
    int inputpin = atoi(pin);

    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].userid, inputUserid) == 0 && users[i].pin == inputpin) {
            strcpy(currentUserid, inputUserid);
            return 1;
        }
    }
    printf("\n Invalid User ID or PIN.\n");
    return 0;
}

int userindex(const char *userid) {
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].userid, userid) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to withdraw money
void withdraw() {
    double amount;
    int index = userindex(currentUserid);
    if(index == -1) return;

    printf("Enter amount to withdraw: Rs ");
    scanf("%lf", &amount);

    if(amount > 0 && amount <= users[index].balance) {
        users[index].balance -= amount;
        saveusers();
        printf("\nWithdrawal successful. Remaining balance: Rs %.2lf\n", users[index].balance);
    } else {
        printf(amount > users[index].balance ? "\nInsufficient balance.\n" : "Invalid amount.\n");
    }
}

// Function to deposit money
void deposit() {
    double amount;
    int index = userindex(currentUserid);
    if(index == -1) return;

    printf("\nEnter amount to deposit: Rs ");
    scanf("%lf", &amount);

    if(amount > 0) {
        users[index].balance += amount;
        saveusers();
        printf("\nDeposit successful. New balance: Rs %.2lf\n", users[index].balance);
    } else {
        printf("\nInvalid amount.\n");
    }
}

// Function to check balance
void checkbalance() {
    int index = userindex(currentUserid);
    if(index == -1) return;
    printf("\nCurrent balance: Rs %.2lf\n", users[index].balance);
}
