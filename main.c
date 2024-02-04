#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void clearConsole() {
    system("cls"); // For Windows
}

struct subscriber {
    char phonenumber[20];
    char name[50];
    float amount;
} s;

void addrecords();
void listrecords();
void modifyrecords();
void searchrecords();
void payment();
void displayPaymentHistory(char *phonenumber);
void generateReport();
int validatePhoneNumber(const char *phone);

int main() {
    int choice;

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n**************************************************************");
    printf("\n\t\t------WELCOME TO TELECOM BILLING MANAGEMENT SYSTEM---");
    printf("\n\t\t****************************************************************");

    while (1) {

        printf("\n Enter\n 1 : for adding new records.\n 2 : for list of records");
        printf("\n 3 : for modifying records.\n 4 : for payment");
        printf("\n 5 : for searching records.\n 6 : for exit");
        printf("\n 7 : for display payment history\n 8 : for generate report\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addrecords();
                break;
            case 2:
                listrecords();
                break;
            case 3:
                modifyrecords();
                break;
            case 4:
                payment();
                break;
            case 5:
                searchrecords();
                break;
            case 6:
                printf("\n\n\t\t\t\tTHANK YOU FOR USING OUR SERVICE");
                exit(0);
            case 7:
                displayPaymentHistory(s.phonenumber);
                break;
            case 8:
                generateReport();
                break;
            default:
                printf("Incorrect Input\n");
        }
    }
}

int validatePhoneNumber(const char *phone) {
    int len = strlen(phone);
    return (len == 10) && (strspn(phone, "0123456789") == len);
}

void addrecords() {
    FILE *f;
    int test = 1;
    f = fopen("file.ojs", "ab+");
    if (f == NULL) {
        f = fopen("file.ojs", "wb+");
        printf("Please wait while we configure your computer\n");
    }

    while (test != 27) {
        printf("\n Enter phone number (10 digits): ");
        scanf("%s", s.phonenumber);

        if (!validatePhoneNumber(s.phonenumber)) {
            printf("Invalid phone number. Please enter a 10-digit phone number.\n");
            continue;
        }

        printf("\n Enter name: ");
        scanf(" %49[^\n]", s.name);
        printf("\n Enter amount: ");
        scanf("%f", &s.amount);
        fwrite(&s, sizeof(s), 1, f);
        printf("1 record successfully added\n");
        printf("Press 27 to exit, any other key to add another record: ");
        scanf("%d", &test);
        if (test == 27)
            break;
    }

    fclose(f);
}

void listrecords() {
    FILE *f;
    if ((f = fopen("file.ojs", "rb")) == NULL) {
        printf("Error opening file.ojs for reading.\n");
        exit(1);
    }
    system("clear");

    printf("Phone Number\t\tUser Name\t\t\tAmount\n");
    for (int i = 0; i < 79; i++)
        printf("-");

    struct subscriber records[100]; // Assuming a maximum of 100 records
    int numRecords = 0;

    while (fread(&records[numRecords], sizeof(struct subscriber), 1, f) == 1) {
        numRecords++;
    }

    // Sort records by phone number (ascending order)
    for (int i = 0; i < numRecords - 1; i++) {
        for (int j = i + 1; j < numRecords; j++) {
            if (strcmp(records[i].phonenumber, records[j].phonenumber) > 0) {
                // Swap records[i] and records[j]
                struct subscriber temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }

    for (int i = 0; i < numRecords; i++) {
        printf("\n%-10s\t\t%-20s\t\tRs. %.2f /-", records[i].phonenumber, records[i].name, records[i].amount);
    }

    printf("\n");
    for (int i = 0; i < 79; i++)
        printf("-");

    fclose(f);
}

void searchrecords() {
    FILE *f;
    char phonenumber[20];
    int flag = 1;
    f = fopen("file.ojs", "rb+");

    if (f == NULL) {
        printf("Error opening file.ojs for reading.\n");
        exit(1);
    }

    fflush(stdin);
    printf("Enter Phone Number to search in our database: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            printf("Record Found\n");
            printf("Phonenumber: %s\nName: %s\nAmount: Rs. %.2f\n", s.phonenumber, s.name, s.amount);
            flag = 0;
            break;
        }
    }

    if (flag == 1) {
        printf("Requested Phone Number Not found in our database\n");
    }

    fclose(f);
}

void modifyrecords() {
    FILE *f;
    char phonenumber[20];
    long int size = sizeof(s);

    if ((f = fopen("file.ojs", "rb+")) == NULL) {
        printf("Error opening file.ojs for reading and writing.\n");
        exit(1);
    }

    printf("Enter phone number of the subscriber to modify: ");
    scanf("%s", phonenumber);
    fflush(stdin);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            printf("\n Enter phone number: ");
            scanf("%s", s.phonenumber);
            printf("\n Enter name: ");
            fflush(stdin);
            scanf(" %49[^\n]", s.name);
            printf("\n Enter amount: ");
            scanf("%f", &s.amount);
            fseek(f, -size, SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);
            break;
        }
    }

    fclose(f);
}

void payment() {
    FILE *f;
    char phonenumber[20];
    long int size = sizeof(s);
    float amt;
    int i;

    if ((f = fopen("file.ojs", "rb+")) == NULL) {
        printf("Error opening file.ojs for reading and writing.\n");
        exit(1);
    }

    printf("Enter phone number of the subscriber for payment: ");
    scanf("%s", phonenumber);
    fflush(stdin);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            printf("\nPhone No.: %s\n", s.phonenumber);
            printf("Name: %s\n", s.name);
            printf("Current amount: %.2f\n", s.amount);

            for (i = 0; i < 79; i++)
                printf("-");

            printf("\nEnter amount of payment: ");
            fflush(stdin);
            scanf("%f", &amt);
            s.amount = s.amount - amt;
            fseek(f, -size, SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);

            // Update payment history
            FILE *paymentHistoryFile = fopen("payment_history.ojs", "ab+");
            if (paymentHistoryFile != NULL) {
                fprintf(paymentHistoryFile, "Subscriber: %s, Paid: Rs. %.2f\n", s.phonenumber, amt);
                fclose(paymentHistoryFile);
            }

            break;
        }
    }

    printf("THANK YOU %s FOR YOUR TIMELY PAYMENTS\n", s.name);
    fclose(f);
}

void displayPaymentHistory(char *phonenumber) {
    FILE *paymentHistoryFile = fopen("payment_history.ojs", "r");
    if (paymentHistoryFile != NULL) {
        char line[100];
        printf("\nPayment History for Subscriber %s:\n", phonenumber);
        while (fgets(line, sizeof(line), paymentHistoryFile)) {
            printf("%s", line);
        }
        fclose(paymentHistoryFile);
    } else {
        printf("No payment history found for subscriber %s.\n", phonenumber);
    }
}

void generateReport() {
    FILE *f;
    if ((f = fopen("file.ojs", "rb")) == NULL) {
        printf("Error opening file.ojs for reading.\n");
        exit(1);
    }

    struct subscriber records[100]; // Assuming a maximum of 100 records
    int numRecords = 0;

    while (fread(&records[numRecords], sizeof(struct subscriber), 1, f) == 1) {
        numRecords++;
    }

    fclose(f);

    // Sort records by phone number (ascending order)
    for (int i = 0; i < numRecords - 1; i++) {
        for (int j = i + 1; j < numRecords; j++) {
            if (strcmp(records[i].phonenumber, records[j].phonenumber) > 0) {
                // Swap records[i] and records[j]
                struct subscriber temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }

    FILE *reportFile = fopen("report.ojs", "w");
    if (reportFile != NULL) {
        fprintf(reportFile, "Phone Number\t\tUser Name\t\t\tAmount\n");
        for (int i = 0; i < 79; i++)
            fprintf(reportFile, "-");
        fprintf(reportFile, "\n");

        for (int i = 0; i < numRecords; i++) {
            fprintf(reportFile, "%-10s\t\t%-20s\t\tRs. %.2f /-\n", records[i].phonenumber, records[i].name, records[i].amount);
        }

        fclose(reportFile);
        printf("Report generated successfully (report.ojs).\n");
    } else {
        printf("Error creating the report file.\n");
    }
}



