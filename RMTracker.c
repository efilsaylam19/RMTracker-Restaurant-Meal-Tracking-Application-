/*
Efil SAYLAM
RMTracker - Main Application
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void runCustomerPanel();

void runRestaurantPanel();

void displayMainWelcomeScreen() {
    printf("\n");
    printf("*********************************************************\n");
    printf("*            Welcome to RMTracker!                      *\n");
    printf("*********************************************************\n");
    printf("*                                                       *\n");
    printf("*  Please select a panel:                               *\n");
    printf("*                                                       *\n");
    printf("*  1 - Customer Panel                                   *\n");
    printf("*  2 - Restaurant Panel                                 *\n");
    printf("*  3 - Exit                                             *\n");
    printf("*                                                       *\n");
    printf("*********************************************************\n");
}

int main() {
    int choice;
    
    do {
        displayMainWelcomeScreen();
        printf("Please enter your choice: ");
        scanf(" %d", &choice);
        
        switch (choice) {
            case 1:
                runCustomerPanel();
                break;
            case 2:
                runRestaurantPanel();
                break;
            case 3:
                printf("\nThank you for using RMTracker! Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
    } while (choice != 3);
    
    return 0;
}

