#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Food struct definition (same as in customer.c)
typedef struct {
    int id;
    char name[50];
    float price;
    int preparation_time;
    char available[50];
} Food;

// Restaurant struct definition
typedef struct {
    char username[50];
    char password[50];
} Restaurant;

// Function to display restaurant login/register screen
void displayRestaurantAuthScreen() {
    printf("\n");
    printf("=========================================================\n");
    printf("                 RESTAURANT PANEL                        \n");
    printf("=========================================================\n");
    printf("|                                                       |\n");
    printf("|  1 - Login                                            |\n");
    printf("|  2 - Register                                         |\n");
    printf("|                                                       |\n");
    printf("=========================================================\n");
}

// Function to register a new restaurant
int registerRestaurant() {
    Restaurant new_restaurant;
    printf("\n");
    printf("=========================================================\n");
    printf("              RESTAURANT REGISTRATION                    \n");
    printf("=========================================================\n");
    printf("Enter restaurant username: ");
    scanf(" %s", new_restaurant.username);
    printf("Enter restaurant password: ");
    scanf(" %s", new_restaurant.password);
    
    // Check if restaurant already exists
    FILE *file = fopen("restaurants.txt", "r");
    if (file != NULL) {
        char line[150];
        while (fgets(line, sizeof(line), file) != NULL) {
            char existing_username[50];
            sscanf(line, "%s", existing_username);
            if (strcmp(existing_username, new_restaurant.username) == 0) {
                fclose(file);
                printf("Restaurant username already exists! Please try again.\n");
                return 0;
            }
        }
        fclose(file);
    }
    
    // Add new restaurant to file
    file = fopen("restaurants.txt", "a");
    if (file == NULL) {
        printf("Error: Could not save restaurant data!\n");
        return 0;
    }
    fprintf(file, "%s %s\n", new_restaurant.username, new_restaurant.password);
    fclose(file);
    
    printf("\n[SUCCESS] Restaurant registration successful!\n");
    printf("Welcome, %s!\n", new_restaurant.username);
    return 1;
}

// Function to login restaurant
int loginRestaurant(char username[]) {
    char password[50];
    printf("\n");
    printf("=========================================================\n");
    printf("                 RESTAURANT LOGIN                        \n");
    printf("=========================================================\n");
    printf("Enter restaurant username: ");
    scanf(" %s", username);
    printf("Enter restaurant password: ");
    scanf(" %s", password);
    
    FILE *file = fopen("restaurants.txt", "r");
    if (file == NULL) {
        printf("No restaurants found. Please register first.\n");
        return 0;
    }
    
    char line[150];
    while (fgets(line, sizeof(line), file) != NULL) {
        char file_username[50];
        char file_password[50];
        sscanf(line, "%s %s", file_username, file_password);
        if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0) {
            fclose(file);
            printf("\n[SUCCESS] Restaurant login successful!\n");
            printf("Welcome back, %s!\n", username);
            return 1;
        }
    }
    
    fclose(file);
    printf("Invalid username or password! Please try again.\n");
    return 0;
}

// Function to read food list from file (static to avoid conflicts)
static void readFoodListRestaurant(Food foods[], int *food_count) {
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

// Function to display food list (static to avoid conflicts)
static void displayFoodListRestaurant(Food foods[], int food_count) {
    printf("\n");
    printf("=========================================================\n");
    printf("                    CURRENT FOOD MENU                     \n");
    printf("=========================================================\n");
    printf("%-5s %-20s %-12s %-18s %-12s\n", "ID", "Food", "Price", "Preparation Time", "Available");
    printf("---------------------------------------------------------\n");
    
    for (int i = 0; i < food_count; i++) {
        printf("%-5d %-20s %-12.2f %-18d %-12s\n", 
               foods[i].id, 
               foods[i].name, 
               foods[i].price, 
               foods[i].preparation_time, 
               foods[i].available);
    }
    printf("=========================================================\n");
}

// Function to add food
void addFood(Food foods[], int *food_count) {
    Food new_food;
    new_food.id = *food_count + 1;
    char input[100];
    int valid_input = 0;

    printf("\n=== ADD NEW FOOD ===\n");
    printf("Enter new food name: ");
    scanf(" %49s", new_food.name);

    // Price validation with format specification
    while (!valid_input) {
        printf("Enter new food price (format: XX.XX, e.g., 34.00): ");
        if (scanf(" %f", &new_food.price) == 1 && new_food.price > 0) {
            valid_input = 1;
        } else {
            printf("Invalid input! Price must be a positive number in format XX.XX (e.g., 34.00). Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
        }
    }

    // Preparation time validation
    valid_input = 0;
    while (!valid_input) {
        printf("Enter new food preparation time (minutes): ");
        if (scanf(" %d", &new_food.preparation_time) == 1 && new_food.preparation_time > 0) {
            valid_input = 1;
        } else {
            printf("Invalid input! Preparation time must be a positive integer. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
        }
    }

    // New food is automatically set as Available
    strcpy(new_food.available, "Available");

    foods[*food_count] = new_food;
    (*food_count)++;

    // Write to file (in append mode)
    FILE *file = fopen("foodlist.txt", "a");
    if (file == NULL) {
        printf("Food list file could not be opened!\n");
        return;
    }

    fprintf(file, "%d %s %.2f %d %s\n", new_food.id, new_food.name, new_food.price, new_food.preparation_time, new_food.available);
    fclose(file);

    printf("\n[SUCCESS] New food added!\n");
}

// Function to update food
void updateFood(Food foods[], int food_count) {
    displayFoodListRestaurant(foods, food_count);
    
    int food_id;
    printf("\n=== UPDATE FOOD ===\n");
    printf("Enter the ID of the food you want to update: ");
    scanf(" %d", &food_id);
    
    // Clear input buffer
    while (getchar() != '\n');

    if (food_id <= 0 || food_id > food_count) {
        printf("Invalid food ID!\n");
        return;
    }

    Food *food = &foods[food_id - 1];
    char input[100];
    float new_price;
    int new_prep_time;

    printf("Enter new food name (%s) [Press Enter to keep current]: ", food->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline
    if (strlen(input) > 0) {
        strncpy(food->name, input, 49);
        food->name[49] = '\0';
    }

    printf("Enter new food price (%.2f) [Press Enter to keep current] (format: XX.XX, e.g., 34.00): ", food->price);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        if (sscanf(input, "%f", &new_price) == 1 && new_price > 0) {
            food->price = new_price;
        } else {
            printf("Invalid input! Price must be a positive number in format XX.XX (e.g., 34.00). Keeping current value.\n");
        }
    }

    printf("Enter new food preparation time (%d minutes) [Press Enter to keep current]: ", food->preparation_time);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        if (sscanf(input, "%d", &new_prep_time) == 1 && new_prep_time > 0) {
            food->preparation_time = new_prep_time;
        } else {
            printf("Invalid input! Preparation time must be a positive integer. Keeping current value.\n");
        }
    }

    // Status selection with menu
    printf("\nSelect availability status (%s):\n", food->available);
    printf("  1 - Available\n");
    printf("  2 - Not Available\n");
    printf("  [Press Enter to keep current]: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        int status_choice;
        if (sscanf(input, "%d", &status_choice) == 1) {
            if (status_choice == 1) {
                strcpy(food->available, "Available");
            } else if (status_choice == 2) {
                strcpy(food->available, "Not_Available");
            } else {
                printf("Invalid choice! Keeping current value.\n");
            }
        } else {
            printf("Invalid input! Keeping current value.\n");
        }
    }

    // Update file
    FILE *file = fopen("foodlist.txt", "w");
    if (file == NULL) {
        printf("Food list file could not be opened!\n");
        return;
    }

    for (int i = 0; i < food_count; i++) {
        fprintf(file, "%d %s %.2f %d %s\n", foods[i].id, foods[i].name, foods[i].price, foods[i].preparation_time, foods[i].available);
    }
    fclose(file);

    printf("[SUCCESS] Food information updated!\n");
}

// Function to delete food
void deleteFood(Food foods[], int *food_count) {
    displayFoodListRestaurant(foods, *food_count);
    
    int food_id;
    printf("\n=== DELETE FOOD ===\n");
    printf("Enter the ID of the food you want to delete: ");
    scanf(" %d", &food_id);

    if (food_id <= 0 || food_id > *food_count) {
        printf("Invalid food ID!\n");
        return;
    }

    printf("Are you sure you want to delete '%s'? (y/n): ", foods[food_id - 1].name);
    char confirm;
    scanf(" %c", &confirm);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = food_id - 1; i < *food_count - 1; i++) {
        foods[i] = foods[i + 1];
        foods[i].id = i + 1; // Update ID
    }
    (*food_count)--;

    // Update IDs in remaining foods
    for (int i = 0; i < *food_count; i++) {
        foods[i].id = i + 1;
    }

    // Update file
    FILE *file = fopen("foodlist.txt", "w");
    if (file == NULL) {
        printf("Food list file could not be opened!\n");
        return;
    }

    for (int i = 0; i < *food_count; i++) {
        fprintf(file, "%d %s %.2f %d %s\n", foods[i].id, foods[i].name, foods[i].price, foods[i].preparation_time, foods[i].available);
    }
    fclose(file);

    printf("[SUCCESS] Food deleted!\n");
}

// Order struct definition (for restaurant to view orders)
typedef struct {
    char id[20];
    char food_name[50];
    float price;
    time_t order_time;
    time_t preparation_time;
    char user[50];
} Order;

// Function to load current orders from file (simplified - just reads and displays)
void loadCurrentOrdersRestaurant() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("\n");
        printf("=========================================================\n");
        printf("                  CURRENT ORDERS                          \n");
        printf("=========================================================\n");
        printf("No orders file found.\n");
        printf("=========================================================\n");
        return;
    }
    
    printf("\n");
    printf("=========================================================\n");
    printf("                  CURRENT ORDERS                          \n");
    printf("=========================================================\n");
    
    char line[300];
    int order_count = 0;
    int order_num = 1;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strstr(line, "id:") != NULL) {
            char id[20], food_name[50], user[50], order_time_str[80], prep_time_str[80];
            float price;
            
            // Manual parsing to handle spaces in time strings
            char *token;
            char line_copy[300];
            strcpy(line_copy, line);
            
            // Parse id
            token = strstr(line_copy, "id: ");
            if (token) {
                sscanf(token + 4, "%s", id);
            }
            
            // Parse Food
            token = strstr(line_copy, "Food: ");
            if (token) {
                sscanf(token + 6, "%s", food_name);
            }
            
            // Parse Price
            token = strstr(line_copy, "Price: ");
            if (token) {
                sscanf(token + 7, "%f", &price);
            }
            
            // Parse Order Time (handle spaces)
            token = strstr(line_copy, "Order Time: ");
            if (token) {
                char *end = strstr(token, " , Preparation Time:");
                if (end) {
                    int len = end - (token + 12);
                    if (len > 0 && len < 80) {
                        strncpy(order_time_str, token + 12, len);
                        order_time_str[len] = '\0';
                        // Trim spaces
                        while (order_time_str[0] == ' ') {
                            memmove(order_time_str, order_time_str + 1, strlen(order_time_str));
                        }
                    }
                }
            }
            
            // Parse Preparation Time (handle spaces)
            token = strstr(line_copy, "Preparation Time: ");
            if (token) {
                char *end = strstr(token, " , User:");
                if (end) {
                    int len = end - (token + 18);
                    if (len > 0 && len < 80) {
                        strncpy(prep_time_str, token + 18, len);
                        prep_time_str[len] = '\0';
                        // Trim spaces
                        while (prep_time_str[0] == ' ') {
                            memmove(prep_time_str, prep_time_str + 1, strlen(prep_time_str));
                        }
                    }
                }
            }
            
            // Parse User
            token = strstr(line_copy, "User: ");
            if (token) {
                sscanf(token + 6, "%s", user);
            }
            
            // Check if we got all required fields
            if (strlen(id) > 0 && strlen(food_name) > 0 && strlen(order_time_str) > 0 && strlen(prep_time_str) > 0 && strlen(user) > 0) {
                order_count++;
                
                // Parse time to check if ready
                int year, month, day, hour, min, sec;
                struct tm tm_prep = {0};
                if (sscanf(prep_time_str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) == 6) {
                    tm_prep.tm_year = year - 1900;
                    tm_prep.tm_mon = month - 1;
                    tm_prep.tm_mday = day;
                    tm_prep.tm_hour = hour;
                    tm_prep.tm_min = min;
                    tm_prep.tm_sec = sec;
                    time_t prep_time = mktime(&tm_prep);
                    time_t current_time = time(NULL);
                    
                    char status[20];
                    if (prep_time <= current_time) {
                        strcpy(status, "[READY]");
                    } else {
                        strcpy(status, "[PREPARING]");
                    }
                    
                    printf("\n[Order #%d]\n", order_num++);
                    printf("  Order ID:      %s\n", id);
                    printf("  Food:          %s\n", food_name);
                    printf("  Price:         %.2f TL\n", price);
                    printf("  Customer:      %s\n", user);
                    printf("  Order Time:    %s\n", order_time_str);
                    printf("  Ready Time:    %s\n", prep_time_str);
                    printf("  Status:        %s\n", status);
                    printf("---------------------------------------------------------\n");
                } else {
                    // If time parsing fails, just show without status
                    printf("\n[Order #%d]\n", order_num++);
                    printf("  Order ID:      %s\n", id);
                    printf("  Food:          %s\n", food_name);
                    printf("  Price:         %.2f TL\n", price);
                    printf("  Customer:      %s\n", user);
                    printf("  Order Time:    %s\n", order_time_str);
                    printf("  Ready Time:    %s\n", prep_time_str);
                    printf("  Status:        [PREPARING]\n");
                    printf("---------------------------------------------------------\n");
                }
            }
        }
    }
    fclose(file);
    
    if (order_count == 0) {
        printf("No current orders found.\n");
    }
    printf("=========================================================\n");
}

// Function to show current orders (all orders for restaurant)
void showCurrentOrdersRestaurant() {
    loadCurrentOrdersRestaurant();
}

// Function to show past orders (all past orders for restaurant)
void showPastOrdersRestaurant() {
    FILE *past_file = fopen("past_orders.txt", "r");
    if (past_file == NULL) {
        printf("\n");
        printf("=========================================================\n");
        printf("                   PAST ORDERS                           \n");
        printf("=========================================================\n");
        printf("No past orders file found.\n");
        printf("=========================================================\n");
        return;
    }

    printf("\n");
    printf("=========================================================\n");
    printf("                   PAST ORDERS                            \n");
    printf("=========================================================\n");
    
    char line[300];
    int order_exists = 0;
    int order_num = 1;
    int file_is_empty = 1;
    
    while (fgets(line, sizeof(line), past_file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        file_is_empty = 0;
        
        char id[20], food_name[50], user[50], order_time_str[80], prep_time_str[80];
        float price;
        
        // Manual parsing to handle spaces in time strings
        char line_copy[300];
        strcpy(line_copy, line);
        char *token;
        
        // Parse id
        token = strstr(line_copy, "id: ");
        if (token) {
            sscanf(token + 4, "%s", id);
        }
        
        // Parse Food
        token = strstr(line_copy, "Food: ");
        if (token) {
            sscanf(token + 6, "%s", food_name);
        }
        
        // Parse Price
        token = strstr(line_copy, "Price: ");
        if (token) {
            sscanf(token + 7, "%f", &price);
        }
        
        // Parse Order Time (handle spaces)
        token = strstr(line_copy, "Order Time: ");
        if (token) {
            char *end = strstr(token, " , Preparation Time:");
            if (end) {
                int len = end - (token + 12);
                if (len > 0 && len < 80) {
                    strncpy(order_time_str, token + 12, len);
                    order_time_str[len] = '\0';
                    // Trim spaces
                    while (order_time_str[0] == ' ') {
                        memmove(order_time_str, order_time_str + 1, strlen(order_time_str));
                    }
                }
            }
        }
        
        // Parse Preparation Time (handle spaces)
        token = strstr(line_copy, "Preparation Time: ");
        if (token) {
            char *end = strstr(token, " , User:");
            if (end) {
                int len = end - (token + 18);
                if (len > 0 && len < 80) {
                    strncpy(prep_time_str, token + 18, len);
                    prep_time_str[len] = '\0';
                    // Trim spaces
                    while (prep_time_str[0] == ' ') {
                        memmove(prep_time_str, prep_time_str + 1, strlen(prep_time_str));
                    }
                }
            }
        }
        
        // Parse User
        token = strstr(line_copy, "User: ");
        if (token) {
            sscanf(token + 6, "%s", user);
        }
        
        // Check if we got all required fields
        if (strlen(id) > 0 && strlen(food_name) > 0 && strlen(order_time_str) > 0 && strlen(prep_time_str) > 0 && strlen(user) > 0) {
            order_exists = 1;
            
            printf("\n[Order #%d]\n", order_num++);
            printf("  Order ID:      %s\n", id);
            printf("  Food:          %s\n", food_name);
            printf("  Price:         %.2f TL\n", price);
            printf("  Customer:      %s\n", user);
            printf("  Order Time:    %s\n", order_time_str);
            printf("  Ready Time:    %s\n", prep_time_str);
            printf("  Status:        [COMPLETED]\n");
            printf("---------------------------------------------------------\n");
        }
    }
    fclose(past_file);

    if (!order_exists) {
        printf("\nNo past orders found.\n");
    }
    printf("=========================================================\n");
}

// Restaurant panel main function
void runRestaurantPanel() {
    Food foods[50];
    int food_count = 0;
    char current_restaurant[50] = "";

    // Read food list from file
    readFoodListRestaurant(foods, &food_count);

    // Welcome screen and login/register
    int auth_choice;
    int logged_in = 0;
    
    while (!logged_in) {
        displayRestaurantAuthScreen();
        printf("Please enter your choice: ");
        scanf(" %d", &auth_choice);
        
        switch (auth_choice) {
            case 1:
                if (loginRestaurant(current_restaurant)) {
                    logged_in = 1;
                }
                break;
            case 2:
                if (registerRestaurant()) {
                    // After registration, automatically login
                    printf("Please login with your new credentials.\n");
                    if (loginRestaurant(current_restaurant)) {
                        logged_in = 1;
                    }
                }
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    // Welcome message with restaurant name
    printf("\n");
    printf("=========================================================\n");
    printf("        Welcome %s!                                    \n", current_restaurant);
    printf("=========================================================\n");

    int choice;
    do {
        // Show menu
        printf("\n");
        printf("---------------------------------------------------------\n");
        printf("  1 - Add Food                                           \n");
        printf("  2 - Update Food                                        \n");
        printf("  3 - Delete Food                                        \n");
        printf("  4 - View Food Menu                                     \n");
        printf("  5 - View Current Orders                                 \n");
        printf("  6 - View Past Orders                                   \n");
        printf("  7 - Log out                                            \n");
        printf("---------------------------------------------------------\n");
        printf("Please enter your choice: ");
        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                addFood(foods, &food_count);
                break;
            case 2:
                updateFood(foods, food_count);
                break;
            case 3:
                deleteFood(foods, &food_count);
                break;
            case 4:
                displayFoodListRestaurant(foods, food_count);
                break;
            case 5:
                showCurrentOrdersRestaurant();
                break;
            case 6:
                showPastOrdersRestaurant();
                break;
            case 7:
                printf("\nLogging out... Goodbye, %s!\n", current_restaurant);
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 7);
}
