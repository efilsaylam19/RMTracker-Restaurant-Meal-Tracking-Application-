/*
Efil SAYLAM
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Food struct definition
typedef struct {
    int id;
    char name[50];
    float price;
    int preparation_time;
    char available[50];
} Food;

// Order struct definition
typedef struct {
    char id[20];
    char food_name[50];
    float price;
    time_t order_time;
    time_t preparation_time;
    char user[50];
} Order;

// User struct definition
typedef struct {
    char username[50];
    char password[50];
} User;

// Forward declarations
void writeCurrentOrdersToFile(Order orders[], int order_count);

// Function to display customer login/register screen
void displayCustomerAuthScreen() {
    printf("\n");
    printf("=========================================================\n");
    printf("                 CUSTOMER PANEL                          \n");
    printf("=========================================================\n");
    printf("|                                                       |\n");
    printf("|  1 - Login                                            |\n");
    printf("|  2 - Register                                         |\n");
    printf("|                                                       |\n");
    printf("=========================================================\n");
}

// Function to display food list beautifully
void displayFoodList(Food foods[], int food_count) {
    printf("\n");
    printf("=========================================================\n");
    printf("                    AVAILABLE FOODS                     \n");
    printf("=========================================================\n");
    printf("%-5s %-20s %-12s %-18s %-12s\n", "ID", "Food", "Price", "Preparation Time", "Available");
    printf("---------------------------------------------------------\n");
    
    for (int i = 0; i < food_count; i++) {
        if (strcmp(foods[i].available, "Available") == 0) {
            printf("%-5d %-20s %-12.2f %-18d %-12s\n", 
                   foods[i].id, 
                   foods[i].name, 
                   foods[i].price, 
                   foods[i].preparation_time, 
                   foods[i].available);
        }
    }
    printf("=========================================================\n");
}

// Function to read food list from file
void readFoodList(Food foods[], int *food_count) {
    FILE *file = fopen("foodlist.txt", "r");
    if (file == NULL) {
        printf("Food list file not found! Starting with empty menu.\n");
        *food_count = 0;
        return;
    }

    int i = 0;
    while (i < 50 && fscanf(file, "%d %49s %f %d %49s", &foods[i].id, foods[i].name, &foods[i].price, &foods[i].preparation_time, foods[i].available) == 5) {
        i++;
    }
    *food_count = i;
    
    fclose(file);
}

// Function to register a new user
int registerUser() {
    User new_user;
    printf("\n=== REGISTRATION ===\n");
    printf("Enter username: ");
    scanf(" %s", new_user.username);
    printf("Enter password: ");
    scanf(" %s", new_user.password);
    
    // Check if user already exists
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        char line[150];
        while (fgets(line, sizeof(line), file) != NULL) {
            char existing_username[50];
            sscanf(line, "%s", existing_username);
            if (strcmp(existing_username, new_user.username) == 0) {
                fclose(file);
                printf("Username already exists! Please try again.\n");
                return 0;
            }
        }
        fclose(file);
    }
    
    // Add new user to file
    file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error: Could not save user data!\n");
        return 0;
    }
    fprintf(file, "%s %s\n", new_user.username, new_user.password);
    fclose(file);
    
    printf("Registration successful! Welcome, %s!\n", new_user.username);
    return 1;
}

// Function to login user
int loginUser(char username[]) {
    char password[50];
    printf("\n=== LOGIN ===\n");
    printf("Enter username: ");
    scanf(" %s", username);
    printf("Enter password: ");
    scanf(" %s", password);
    
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No users found. Please register first.\n");
        return 0;
    }
    
    char line[150];
    while (fgets(line, sizeof(line), file) != NULL) {
        char file_username[50];
        char file_password[50];
        sscanf(line, "%s %s", file_username, file_password);
        if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0) {
            fclose(file);
            printf("Login successful! Welcome back, %s!\n", username);
            return 1;
        }
    }
    
    fclose(file);
    printf("Invalid username or password! Please try again.\n");
    return 0;
}

// Function to create new order
void createNewOrder(Order orders[], int *order_count, Food foods[], int food_count, char username[]) {
    // Display food list
    displayFoodList(foods, food_count);
    
    // Create order id
    char order_id[20];
    sprintf(order_id, "ORD010424_%03d", (*order_count) + 1);

    // Get order information
    int food_id;
    printf("\nEnter the ID of the food you want to order: ");
    scanf(" %d", &food_id);
    
    if (food_id < 1 || food_id > food_count) {
        printf("Invalid food ID!\n");
        return;
    }
    
    if (strcmp(foods[food_id - 1].available, "Available") != 0) {
        printf("This food is not available!\n");
        return;
    }

    // Get preparation time to calculate preparation time
    int preparation_time = foods[food_id - 1].preparation_time;

    // Create order
    strcpy(orders[*order_count].id, order_id);
    strcpy(orders[*order_count].food_name, foods[food_id - 1].name);
    orders[*order_count].price = foods[food_id - 1].price;
    orders[*order_count].order_time = time(NULL);
    orders[*order_count].preparation_time = orders[*order_count].order_time + (preparation_time * 60);
    strcpy(orders[*order_count].user, username);

    // Write order information to orders.txt file
    FILE *file = fopen("orders.txt", "a");
    char order_time_str[80];
    char preparation_time_str[80];
    strftime(order_time_str, sizeof(order_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[*order_count].order_time));
    strftime(preparation_time_str, sizeof(preparation_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[*order_count].preparation_time));
    fprintf(file, "id: %s , Food: %s , Price: %.2f , Order Time: %s , Preparation Time: %s , User: %s\n", 
            orders[*order_count].id, 
            orders[*order_count].food_name, 
            orders[*order_count].price, 
            order_time_str, 
            preparation_time_str, 
            orders[*order_count].user);
    fclose(file);

    (*order_count)++;
    printf("\n[SUCCESS] Your order has been received!\n");
    printf("Order ID: %s\n", order_id);
    printf("Food: %s\n", foods[food_id - 1].name);
    printf("Price: %.2f TL\n", foods[food_id - 1].price);
}

// Function to show user their active orders
void showMyOrders(Order orders[], int order_count, char username[]) {
    printf("\n");
    printf("=========================================================\n");
    printf("                  YOUR CURRENT ORDERS                    \n");
    printf("=========================================================\n");
    
    int order_exists = 0;
    for (int i = 0; i < order_count; i++) {
        if (strcmp(orders[i].user, username) == 0) {
            order_exists = 1;
            printf("\nOrder ID: %s\n", orders[i].id);
            printf("Food: %s\n", orders[i].food_name);
            printf("Price: %.2f TL\n", orders[i].price);

            // Order time
            char order_time_str[80];
            strftime(order_time_str, sizeof(order_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].order_time));
            printf("Order Time: %s\n", order_time_str);

            // Preparation time
            char preparation_time_str[80];
            strftime(preparation_time_str, sizeof(preparation_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].preparation_time));
            printf("Preparation Time: %s\n", preparation_time_str);
            printf("---------------------------------------------------------\n");
        }
    }
    if (!order_exists) {
        printf("You have no current orders.\n");
    }
    printf("=========================================================\n");
}

// Function to show user their past orders
void showPastOrders(char username[]) {
    FILE *past_file = fopen("past_orders.txt", "r");
    if (past_file == NULL) {
        printf("\nNo past orders file found.\n");
        return;
    }

    printf("\n");
    printf("=========================================================\n");
    printf("                   YOUR PAST ORDERS                      \n");
    printf("=========================================================\n");
    
    char line[300];
    int order_exists = 0;
    int order_num = 1;
    int file_is_empty = 1;
    
    while (fgets(line, sizeof(line), past_file) != NULL) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        file_is_empty = 0;
        
        if (strstr(line, username) != NULL) {
            order_exists = 1;
            
            // Parse the line
            char id[20], food_name[50], user[50], order_time_str[80], prep_time_str[80];
            float price;
            
            if (sscanf(line, "id: %s , Food: %s , Price: %f , Order Time: %s , Preparation Time: %s , User: %s",
                      id, food_name, &price, order_time_str, prep_time_str, user) == 6) {
                
                printf("\n[Order #%d]\n", order_num++);
                printf("  Order ID:      %s\n", id);
                printf("  Food:          %s\n", food_name);
                printf("  Price:         %.2f TL\n", price);
                printf("  Order Time:    %s\n", order_time_str);
                printf("  Ready Time:    %s\n", prep_time_str);
                printf("  Status:        [COMPLETED]\n");
                printf("---------------------------------------------------------\n");
            }
        }
    }
    fclose(past_file);

    if (!order_exists) {
        if (file_is_empty) {
            printf("\nYou have no past orders.\n");
        } else {
            printf("\nYou have no past orders.\n");
        }
    }
    printf("=========================================================\n");
}

// Function to move past orders from orders.txt file to past_orders file
void moveToPastOrders(Order orders[], int *order_count) {
    if (*order_count == 0) {
        return; // No orders to check
    }
    
    FILE *past_file = fopen("past_orders.txt", "a");
    if (past_file == NULL) {
        return;
    }

    time_t current_time = time(NULL);
    int moved_count = 0;
    int i = 0;
    
    while (i < *order_count) {
        // Check if preparation time has passed
        if (orders[i].preparation_time <= current_time) {
            char order_time_str[80];
            char preparation_time_str[80];
            strftime(order_time_str, sizeof(order_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].order_time));
            strftime(preparation_time_str, sizeof(preparation_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].preparation_time));

            fprintf(past_file, "id: %s , Food: %s , Price: %.2f , Order Time: %s , Preparation Time: %s , User: %s\n", 
                    orders[i].id, 
                    orders[i].food_name, 
                    orders[i].price, 
                    order_time_str, 
                    preparation_time_str, 
                    orders[i].user);
            
            // Remove order from list
            for (int j = i; j < *order_count - 1; j++) {
                orders[j] = orders[j + 1];
            }
            (*order_count)--;
            moved_count++;
        } else {
            i++;
        }
    }

    fclose(past_file);
    
    if (moved_count > 0) {
        // Update orders.txt file to reflect the changes
        writeCurrentOrdersToFile(orders, *order_count);
    }
}

// Function to load orders from file (simplified - loads orders but time parsing is basic)
void loadOrdersFromFile(Order orders[], int *order_count) {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        *order_count = 0;
        return;
    }
    
    char line[300];
    *order_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && *order_count < 100) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        
        if (strstr(line, "id:") != NULL) {
            char id[20], food_name[50], user[50];
            float price;
            int year1, month1, day1, hour1, min1, sec1;
            int year2, month2, day2, hour2, min2, sec2;
            
            // Parse the line - handle spaces in food names
            if (sscanf(line, "id: %s , Food: %s , Price: %f , Order Time: %d-%d-%d %d:%d:%d , Preparation Time: %d-%d-%d %d:%d:%d , User: %s",
                      id, food_name, &price, 
                      &year1, &month1, &day1, &hour1, &min1, &sec1,
                      &year2, &month2, &day2, &hour2, &min2, &sec2,
                      user) == 15) {
                strcpy(orders[*order_count].id, id);
                strcpy(orders[*order_count].food_name, food_name);
                orders[*order_count].price = price;
                strcpy(orders[*order_count].user, user);
                
                // Parse time manually for Windows compatibility
                struct tm tm_order = {0}, tm_prep = {0};
                tm_order.tm_year = year1 - 1900;
                tm_order.tm_mon = month1 - 1;
                tm_order.tm_mday = day1;
                tm_order.tm_hour = hour1;
                tm_order.tm_min = min1;
                tm_order.tm_sec = sec1;
                
                tm_prep.tm_year = year2 - 1900;
                tm_prep.tm_mon = month2 - 1;
                tm_prep.tm_mday = day2;
                tm_prep.tm_hour = hour2;
                tm_prep.tm_min = min2;
                tm_prep.tm_sec = sec2;
                
                orders[*order_count].order_time = mktime(&tm_order);
                orders[*order_count].preparation_time = mktime(&tm_prep);
                
                // Validate that times were parsed correctly
                if (orders[*order_count].order_time != -1 && orders[*order_count].preparation_time != -1) {
                    (*order_count)++;
                }
            }
        }
    }
    fclose(file);
}

void writeCurrentOrdersToFile(Order orders[], int order_count) {
    FILE *file = fopen("orders.txt", "w");
    if (file == NULL) {
        printf("Orders file not found!\n");
        return;
    }

    for (int i = 0; i < order_count; i++) {
        char order_time_str[80];
        char preparation_time_str[80];
        strftime(order_time_str, sizeof(order_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].order_time));
        strftime(preparation_time_str, sizeof(preparation_time_str), "%Y-%m-%d %H:%M:%S", localtime(&orders[i].preparation_time));
        fprintf(file, "id: %s , Food: %s , Price: %.2f , Order Time: %s , Preparation Time: %s , User: %s\n", 
                orders[i].id, 
                orders[i].food_name, 
                orders[i].price, 
                order_time_str, 
                preparation_time_str, 
                orders[i].user);
    }

    fclose(file);
}

// Customer panel main function
void runCustomerPanel() {
    Food foods[50];
    Order orders[100];
    int food_count = 0;
    int order_count = 0;
    char current_user[50] = "";

    // Read food list from file
    readFoodList(foods, &food_count);
    
    // Load existing orders
    loadOrdersFromFile(orders, &order_count);

    // Welcome screen and login/register
    int auth_choice;
    int logged_in = 0;
    
    while (!logged_in) {
        displayCustomerAuthScreen();
        printf("Please enter your choice: ");
        scanf(" %d", &auth_choice);
        
        switch (auth_choice) {
            case 1:
                if (loginUser(current_user)) {
                    logged_in = 1;
                }
                break;
            case 2:
                if (registerUser()) {
                    // After registration, automatically login
                    printf("Please login with your new credentials.\n");
                    if (loginUser(current_user)) {
                        logged_in = 1;
                    }
                }
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    // Welcome message with username
    printf("\n");
    printf("=========================================================\n");
    printf("        Welcome %s!                                    \n", current_user);
    printf("=========================================================\n");

    int choice;
    do {
        // Show menu
        printf("\n");
        printf("---------------------------------------------------------\n");
        printf("  1 - Create new order                                 \n");
        printf("  2 - View my current orders                            \n");
        printf("  3 - View my past orders                               \n");
        printf("  4 - Log out                                           \n");
        printf("---------------------------------------------------------\n");
        printf("Make your choice: ");
        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                createNewOrder(orders, &order_count, foods, food_count, current_user);
                writeCurrentOrdersToFile(orders, order_count);
                break;
            case 2:
                // Move expired orders to past orders before showing current orders
                moveToPastOrders(orders, &order_count);
                showMyOrders(orders, order_count, current_user);
                break;
            case 3:
                // Move expired orders to past orders before showing past orders
                moveToPastOrders(orders, &order_count);
                showPastOrders(current_user);
                break;
            case 4:
                printf("\nLogging out... Goodbye, %s!\n", current_user);
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 4);
}
