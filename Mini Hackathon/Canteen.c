    #include <stdio.h>
    #include <string.h>
    #include <time.h>

#define MAX 50

typedef struct {
    int id;
    char name[30];
    char category[20];
    float price;
    int stock;
} Item;

typedef struct {
    int id;
    char name[30];
    float price;
    int qty;
    float total;
} CartItem;

Item menu[8] = {
    {101, "Veg Grilled Sandwich", "Snacks",       60.0, 15},
    {102, "Crispy Paneer Burger", "Fast Food",    80.0, 12},
    {103, "Masala Dosa",          "South Indian", 70.0, 20},
    {104, "Cold Coffee",          "Beverages",    40.0,  4},
    {105, "Fresh Mango Juice",    "Beverages",    50.0,  0},
    {106, "Veg Hakka Noodles",    "Chinese",      90.0, 18},
    {107, "Chocolate Brownie",    "Dessert",      55.0, 10},
    {108, "Chilli Paneer",    "Chinese",          80.0, 16}
};
int menu_count = 8;

float daily_sales = 0.0;
int total_orders = 0;
long bill_counter = 1001;

void print_divider(char ch, int len) {
    for (int i = 0; i < len; i++) putchar(ch);
    putchar('\n');
}

void display_menu(void) {
    printf("\n");
    print_divider('=', 72);
    printf("                      >>> CAMPUS CANTEEN MENU <<<                       \n");
    print_divider('=', 72);

    printf("%-6s | %-24s | %-14s | %-8s | %-12s\n", 
           "ID", "Item Name", "Category", "Price", "Availability");
    print_divider('-', 72);

    for (int i = 0; i < menu_count; i++) {
        char status[32];
        if (menu[i].stock == 0) {
            snprintf(status, sizeof(status), "[SOLD OUT]");
        } else if (menu[i].stock <= 5) {
            snprintf(status, sizeof(status), "[LOW: %2d]", menu[i].stock);
        } else {
            snprintf(status, sizeof(status), "%2d Left", menu[i].stock);
        }

        printf("%-6d | %-24s | %-14s | Rs.%-5.2f | %-12s\n",
               menu[i].id, menu[i].name, menu[i].category, menu[i].price, status);
    }
    print_divider('-', 72);
}

void customer_order(void) {
    CartItem cart[MAX];
    int cart_count = 0;
    float subtotal = 0.0;

    display_menu();

    while (1) {
        int id, qty, found = -1;
        printf("\nEnter Item ID to order (or 0 to checkout): ");
        if (scanf("%d", &id) != 1 || id == 0) break;

        for (int i = 0; i < menu_count; i++) {
            if (menu[i].id == id) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Item ID not found!\n");
            continue;
        }
        if (menu[found].stock <= 0) {
            printf("Sorry, '%s' is SOLD OUT!\n", menu[found].name);
            continue;
        }

        printf("Enter Quantity (Available: %d): ", menu[found].stock);
        if (scanf("%d", &qty) != 1 || qty <= 0 || qty > menu[found].stock) {
            printf("Invalid quantity!\n");
            continue;
        }

        cart[cart_count].id = menu[found].id;
        strcpy(cart[cart_count].name, menu[found].name);
        cart[cart_count].price = menu[found].price;
        cart[cart_count].qty = qty;
        cart[cart_count].total = qty * menu[found].price;
        subtotal += cart[cart_count].total;

        menu[found].stock -= qty;
        cart_count++;
        printf("Added %d x %s to your cart!\n", qty, menu[found].name);
    }

    if (cart_count == 0) {
        printf("No items ordered.\n");
        return;
    }

    float discount = (subtotal >= 150.0) ? 20.0 : 0.0;
    float gst = (subtotal - discount) * 0.05;
    float grand_total = (subtotal - discount) + gst;

    daily_sales += grand_total;
    total_orders++;

    time_t now = time(NULL);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%d-%b-%Y %H:%M", localtime(&now));

    printf("\n");
    print_divider('*', 55);
    printf("             CAMPUS CANTEEN TAX INVOICE            \n");
    print_divider('*', 55);
    printf("Bill No: #%ld                Date: %s\n", bill_counter++, time_str);
    print_divider('-', 55);
    printf("%-24s %-6s %-10s %-10s\n", "Item", "Qty", "Price", "Amount");
    print_divider('-', 55);

    for (int i = 0; i < cart_count; i++) {
        printf("%-24s %-6d Rs.%-7.2f Rs.%-8.2f\n",
               cart[i].name, cart[i].qty, cart[i].price, cart[i].total);
    }
    print_divider('-', 55);
    printf("Subtotal:                               Rs. %8.2f\n", subtotal);
    if (discount > 0)
        printf("Combo / Festival Discount:             -Rs. %8.2f\n", discount);
    printf("GST (5%%):                               Rs. %8.2f\n", gst);
    print_divider('=', 55);
    printf("FINAL AMOUNT PAYABLE:                   Rs. %8.2f\n", grand_total);
    print_divider('=', 55);
    printf("     Thank you! Enjoy your meal & have a great day!    \n");
    print_divider('*', 55);
}

void admin_panel(void) {
    char password[20];
    printf("Enter Admin Password: ");
    scanf("%19s", password);

    if (strcmp(password, "admin") != 0) {
        printf("Wrong password!\n");
        return;
    }

    while (1) {
        int choice;
        printf("\n");
        print_divider('=', 40);
        printf("        ADMIN MANAGEMENT PANEL          \n");
        print_divider('=', 40);
        printf("1. View Current Stock\n");
        printf("2. Restock Item\n");
        printf("3. View Daily Sales Summary\n");
        printf("4. Back to Main Menu\n");
        print_divider('-', 40);
        printf("Enter Choice (1-4): ");
        if (scanf("%d", &choice) != 1 || choice == 4) break;

        if (choice == 1) {
            display_menu();
        } else if (choice == 2) {
            int id, add_qty, found = -1;
            printf("Enter Item ID to restock: ");
            scanf("%d", &id);
            for (int i = 0; i < menu_count; i++) {
                if (menu[i].id == id) { found = i; break; }
            }
            if (found != -1) {
                printf("Enter quantity to add: ");
                scanf("%d", &add_qty);
                menu[found].stock += add_qty;
                printf("Restocked! New stock: %d\n", menu[found].stock);
            } else {
                printf("Item ID not found!\n");
            }
        } else if (choice == 3) {
            printf("\n--- DAILY SALES SUMMARY ---\n");
            printf("Total Orders Completed : %d\n", total_orders);
            printf("Total Revenue Collected: Rs. %.2f\n", daily_sales);
        }
    }
}

int main(void) {
    int choice;
    while (1) {
        printf("\n========================================\n");
        printf("   SMART CAMPUS CANTEEN POS SYSTEM      \n");
        printf("========================================\n");
        printf("  1. Customer Order Portal\n");
        printf("  2. Admin Management Dashboard\n");
        printf("  3. Exit Application\n");
        printf("----------------------------------------\n");
        printf("Select Option (1-3): ");

        if (scanf("%d", &choice) != 1 || choice == 3) {
            printf("\nThank you for using Smart Canteen POS. Goodbye!\n");
            break;
        }

        if (choice == 1) {
            customer_order();
        } else if (choice == 2) {
            admin_panel();
        }
    }
    return 0;
}