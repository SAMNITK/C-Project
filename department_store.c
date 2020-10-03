#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void insert(FILE *fp);
void display(FILE *fp);
void displayCategory(FILE *fp);
void displayAll(FILE *fp);
void delete(FILE *fp);
void deleteCategory(FILE *fp);
void deleteAll(FILE *fp);
void update(FILE *fp);
void lowInventory(FILE *fp);
void itemsSold(FILE *fp);
int search(FILE *fp, char *name);

struct items {
  char itemName[30], itemCode[10], itemCategory[20];
  int quantity;
  float price;
}item;

struct customers {
  char date[30], billNumber[30], name[30], itemName[10][30], itemCode[10][10], itemCategory[10][20];
  int quantity[10];
  unsigned long mobile;
  float price[10], total;
}customer;

int main() {

  FILE *fp;
  int choice;

  fp = fopen("items.txt", "rb+" );

  if(fp == NULL) {
    fp = fopen("items.txt", "wb+");

    if(fp == NULL) {
      printf("Error while opening file.\n");
    }
  }

  while(1) {

    time_t t;
    time(&t);
    printf("%s", ctime(&t));

    printf("\n1. insert item(single)\n");
    printf("2. update item(single)\n");
    printf("3. delete item(single)\n");
    printf("4. delete item(category)\n");
    printf("5. delete items(all)\n");
    printf("6. display item(single)\n");
    printf("7. display items(category)\n");
    printf("8. display items(all)\n");
    printf("9. low inventory\n");
    printf("10. Print bill\n");
    printf("11. exit\n");

    printf("Enter your choice : ");
    scanf("%d",&choice);

    switch(choice) {

      case 1:
        insert(fp);
        break;
      case 2:
        update(fp);
        break;
      case 3:
        delete(fp);
        fp = fopen("items.txt", "rb+");
        break;
      case 4:
        deleteCategory(fp);
        fp = fopen("items.txt", "rb+");
        break;
      case 5:
        deleteAll(fp);
        fp = fopen("items.txt", "rb+");
        break;
      case 6:
        display(fp);
        break;
      case 7:
        displayCategory(fp);
        break;
      case 8:
        displayAll(fp);
        break;
      case 9:
        lowInventory(fp);
        break;
      case 10:
        itemsSold(fp);
        break;
      case 11:
        fclose(fp);
        exit(1);
        break;
      default :
        printf("\nWrong input entered.\n\n");
    }

  }

  return 0;
}

void insert(FILE *fp) {

  int ch = 1;
  char code[10], name[30];

  while(ch) {
      scanf("%*c");

      dupCode: {

        printf("\nEnter the code of the item : ");
        scanf("%[^\n]%*c", code);

        for(int i = 0; name[i] != '\0'; i++)
            {
              code[i] = toupper(code[i]);
            }

         rewind(fp);

         while(fread(&item, sizeof(item), 1, fp) == 1) {

           if(strcmp(code, item.itemCode) == 0) {
              printf("Item with this code already exist.\n\n");
              goto dupCode;
           }
        }
      }

        dupName: {

          printf("\nEnter the name of the item : ");
          scanf("%[^\n]%*c", name);

          for(int i = 0; name[i] != '\0'; i++)
              {
                name[i] = toupper(name[i]);
              }

          rewind(fp);

          while(fread(&item, sizeof(item), 1, fp) == 1) {

            if(strcmp(name, item.itemName) == 0) {
                printf("Item with this name already exist.\n\n");
                goto dupName;
            }
          }
        }

          printf("\nEnter the category of the item : ");
          scanf("%[^\n]%*c", item.itemCategory);

          for(int i = 0; item.itemCategory[i] != '\0'; i++) {
                item.itemCategory[i] = toupper(item.itemCategory[i]);
              }

          printf("\nEnter the quantity of the item : ");
          scanf("%d", &item.quantity);
          printf("\nEnter the price of the item : ");
          scanf("%f", &item.price);

          fseek(fp, 0, 2);

          strcpy(item.itemCode, code);
          strcpy(item.itemName, name);
          fwrite(&item, sizeof(item), 1, fp);
          printf("\nItem added successfully.\n\n");

      printf("Press 1 to add more item or press 0 to exit.\n");
      scanf("%d", &ch);
   }
}

void display(FILE *fp) {

  char name[30];
  long size = sizeof(item);
  printf("Enter the name or code of item you want to search\n");
  scanf("%*c");
  scanf("%[^\n]%*c", name);

  for(int i=0; name[i]!='\0'; i++)
      {
        name[i]=toupper(name[i]);
      }

  printf("\n-----------------------------------------------------------------------------------------------");
  printf("\nCODE\t\tNAME\t\t\t\tCATEGORY\t\tQUANTITY\tPRICE\n");
  printf("\n-----------------------------------------------------------------------------------------------\n");

  if(search(fp, name) == 1) {
    fseek(fp, -size, 1);

    fread(&item, sizeof(item), 1, fp);
    printf("%-16s%-32s%-24s%-8d\t%-8.2f\n", item.itemCode, item.itemName, item.itemCategory, item.quantity, item.price);
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("\n");
}
  else {
    printf("\nNo item available of this name or code.\n");
    printf("-----------------------------------------------------------------------------------------------\n");
  }
  printf("\n");
}

void displayCategory(FILE *fp) {

  int flag = 0;
  char name[30];
  long size = sizeof(item);

  printf("Enter the category of item you want to search\n");
  scanf("%*c");
  scanf("%[^\n]%*c", name);

  for(int i=0; name[i]!='\0'; i++)
      {
        name[i]=toupper(name[i]);
      }

  rewind(fp);

  printf("\n-----------------------------------------------------------------------------------------------");
  printf("\nCODE\t\tNAME\t\t\t\tCATEGORY\t\tQUANTITY\tPRICE\n");
  printf("\n-----------------------------------------------------------------------------------------------\n");

  while(fread(&item, sizeof(item), 1, fp) == 1) {

      if(strcmp(name, item.itemCategory) == 0) {

        flag = 1;

        printf("%-16s%-32s%-24s%-8d\t%-8.2f\n", item.itemCode, item.itemName, item.itemCategory, item.quantity, item.price);
        printf("-----------------------------------------------------------------------------------------------\n");
        printf("\n");

      }
  }
  if(flag == 0) {
    printf("\nNo item available of this category.\n");
    printf("-----------------------------------------------------------------------------------------------\n");
  }
  printf("\n");
}

void displayAll(FILE *fp) {

  rewind(fp);
  printf("\n-----------------------------------------------------------------------------------------------");
  printf("\nCODE\t\tNAME\t\t\t\tCATEGORY\t\tQUANTITY\tPRICE\n");
  printf("\n-----------------------------------------------------------------------------------------------\n");

  while(fread(&item, sizeof(item), 1, fp) == 1) {
  printf("%-16s%-32s%-24s%-8d\t%-8.2f\n", item.itemCode, item.itemName, item.itemCategory, item.quantity, item.price);
  printf("-----------------------------------------------------------------------------------------------\n");
}
printf("\n");
}

void update(FILE *fp) {

  int ch;
  char name[30];
  long size = sizeof(item);

  printf("Enter the name or code of item you want to update.\n");
  scanf("%*c");
  scanf("%[^\n]%*c", name);

  for(int i=0; name[i]!='\0'; i++)
      {
        name[i]=toupper(name[i]);
      }

  if(search(fp, name) == 0) {
    printf("\nItem not found.\n\n");
    return;
  }

  fseek(fp, -size, 1);

  updateLabel : {

    printf("\n1. Update the code of the item.\n");
    printf("2. Update the name of the item.\n");
    printf("3. Update the category of the item.\n");
    printf("4. Update the quantity of the item.\n");
    printf("5. Update the price of the item.\n");
    printf("6. Exit\n");
    scanf("%d", &ch);

    if(ch == 1) {

      printf("\nEnter the code of the item : ");
      scanf("%*c");
      scanf("%s%*c", item.itemCode);

      for(int i=0; item.itemCode[i]!='\0'; i++)
          {
            item.itemCode[i]=toupper(item.itemCode[i]);
          }
      printf("\n\nItem code successfully modified.\n\n");
      goto updateLabel;

    }

    else if(ch == 2) {

      printf("\nEnter the name of the item : ");
      scanf("%*c");
      scanf("%[^\n]%*c", item.itemName);

      for(int i=0; item.itemName[i]!='\0'; i++)
          {
            item.itemName[i]=toupper(item.itemName[i]);
          }
      printf("\n\nItem name successfully modified.\n\n");
      goto updateLabel;

    }

    else if(ch == 3) {

      printf("\nEnter the category of the item : ");
      scanf("%*c");
      scanf("%[^\n]%*c", item.itemCategory);

      for(int i=0; item.itemCategory[i]!='\0'; i++)
          {
            item.itemCategory[i]=toupper(item.itemCategory[i]);
          }
      printf("\n\nItem category successfully modified.\n\n");
      goto updateLabel;

    }

    if(ch == 4) {

      printf("\nEnter the quantity of the item : ");
      scanf("%d", &item.quantity);
      printf("\n\nItem quantity successfully modified.\n\n");
      goto updateLabel;

    }

    else if(ch == 5) {

      printf("\nEnter the price of the item : ");
      scanf("%f", &item.price);
      printf("\n\nItem price successfully modified.\n\n");
      goto updateLabel;

    }

    else if(ch == 6) {

      printf("\nExiting.\n\n");

    }

    else {

      printf("\nWrong choice\n\n");
      goto updateLabel;

    }
  }

  fwrite(&item, sizeof(item), 1, fp);

}

void delete(FILE *fp) {

  char name[30];
  FILE *fptr;

  printf("Enter the name or code of items you want to delete.\n");
  scanf("%*c");
  scanf("%[^\n]%*c", name);

  for(int i = 0; name[i] != '\0'; i++)
      {
        name[i] = toupper(name[i]);
      }

  if(search(fp, name) == 0) {
    printf("Item not found.\n");
    return;
  }

  fptr = fopen("temp.txt", "wb");
  rewind(fp);

  if(fptr == NULL) {
    printf("Error in opening file");
    exit(1);
  }

  while(fread(&item, sizeof(item), 1, fp) == 1) {

    if(strcmp(name, item.itemName) != 0 && strcmp(name, item.itemCode) != 0) {
      fwrite(&item, sizeof(item), 1, fptr);
    }

  }

  fclose(fp);
  fclose(fptr);
  remove("items.txt");
  rename("temp.txt", "items.txt");

  printf("\nRecord successfully deleted.\n\n");

}

void deleteCategory(FILE *fp) {

  char name[30];
  FILE *fptr;

  printf("Enter the category of items you want to delete.\n");
  scanf("%*c");
  scanf("%[^\n]%*c", name);

  for(int i = 0; name[i] != '\0'; i++)
      {
        name[i] = toupper(name[i]);
      }

  fptr = fopen("temp.txt", "wb");
  rewind(fp);

  if(fptr == NULL) {
    printf("Error in opening file");
    exit(1);
  }

  while(fread(&item, sizeof(item), 1, fp) == 1) {

    if(strcmp(name, item.itemCategory) != 0) {

      fwrite(&item, sizeof(item), 1, fptr);

    }

 }

  fclose(fp);
  fclose(fptr);
  remove("items.txt");
  rename("temp.txt", "items.txt");

  printf("\nRecord successfully deleted.\n\n");

}

void deleteAll(FILE *fp) {

  FILE *fptr;

  fptr = fopen("temp.txt", "wb");

  if(fptr == NULL) {
    printf("Error in opening file");
    exit(1);
  }

  fclose(fp);
  fclose(fptr);
  remove("items.txt");
  rename("temp.txt", "items.txt");

  printf("\nAll records successfully deleted.\n\n");

}

void lowInventory(FILE *fp) {

  int flag = 0;
  rewind(fp);

  printf("\n-----------------------------------------------------------------------------------------------");
  printf("\nCODE\t\tNAME\t\t\t\tCATEGORY\t\tQUANTITY\tPRICE\n");
  printf("\n-----------------------------------------------------------------------------------------------\n");

  while(fread(&item, sizeof(item), 1, fp) == 1) {

    if(item.quantity <= 5) {
      flag = 1;
      printf("%-16s%-32s%-24s%-8d\t%-8.2f\n", item.itemCode, item.itemName, item.itemCategory, item.quantity, item.price);
      printf("-----------------------------------------------------------------------------------------------\n");
      printf("\n");
    }
  }
  if(flag == 0) {
    printf("\nNo item having low inventory.\n");
    printf("-----------------------------------------------------------------------------------------------\n");
  }
  printf("\n");
}

int search(FILE *fp, char *name) {
  int flag = 0;

  rewind(fp);

  while(fread(&item, sizeof(item), 1, fp) == 1) {

    if(strcmp(name, item.itemName) == 0 || strcmp(name, item.itemCode) == 0) {
      flag = 1;
      break;
    }
  }
  return flag;
}


void itemsSold(FILE *fp) {

  FILE *fptr;
  long size = sizeof(item);
  char name[30];
  unsigned long mobile;
  char list[30];
  int quantity, flag = 0, i = 0, j = 0, k = 0, l = 0, m = 0;
  float price, total = 0;

  fptr = fopen("customers.txt", "r+" );

  if(fptr == NULL) {
    fptr = fopen("customers.txt", "w+");

    if(fptr == NULL) {
      printf("Error while opening file.\n");
    }
  }

  time_t t;
  time(&t);
  strcpy(customer.date, ctime(&t));

  printf("Enter customer's name : \n");
  scanf("%*c");
  scanf("%[^\n]%*c", customer.name);

  printf("Enter customer's mobile number : \n");
  scanf("%lu", &customer.mobile);

  int p, len, z = 0;
  len  = strlen(customer.date);
  char billNumber[30] = {0};

  for(p = 0; p < len; p++) {
    if(customer.date[p] >= 48 && customer.date[p] <= 57)
    billNumber[z++] = customer.date[p];
  }

  strcpy(customer.billNumber, billNumber);

  for(int i = 0; name[i] != '\0'; i++)
      {
        name[i] = toupper(name[i]);
      }

  while(1) {

    printf("Enter product's name or code : \n");
    scanf("%*c");
    scanf("%[^\n]%*c", list);

    for(int i = 0; list[i] != '\0'; i++)
        {
          list[i] = toupper(list[i]);
        }

    printf("Enter product's quantity : \n");
    scanf("%d", &quantity);

    if(search(fp, list) == 1) {

      fseek(fp, -size, 1);

      if((item.quantity - quantity) < 0) {
        int flag = 0;
        printf("Only %d quantity is in stock\n", item.quantity);

        printf("Press 1 to add this item or 0 to exit.\n");
        scanf("%d", &flag);

        if(flag == 1) {
           scanf("%d", &quantity);
        }

        else {
          goto more;
          break;
        }

      }

      item.quantity -= quantity;
      price = item.price * quantity;
      total += price;

      strcpy(customer.itemName[i++], item.itemName);
      strcpy(customer.itemCode[j++], item.itemCode);
      strcpy(customer.itemCategory[k++], item.itemCategory);
      customer.price[l++] = price;
      customer.quantity[m++] = quantity;
      customer.total = total;

      fwrite(&item, sizeof(item), 1, fp);

    }

    else {
      printf("Item not available.\n");
    }

    more : {
      printf("If you want to add another item press 1 else press 0 to print bill.\n");
      scanf("%d", &flag);
    }

    if(flag == 0) {

      fseek(fptr, 0, 2);
      fwrite(&customer, sizeof(customer), 1, fptr);
      fclose(fptr);

      fptr = fopen("customers.txt", "r+" );

        if(fptr == NULL) {
          printf("Error while opening file.\n");
        }

      size = sizeof(customer);

      fseek(fptr, -size, 1);

      fwrite(&customer, sizeof(customer), 1, fptr);

      printf("\n%s\n\n", customer.date);
      printf("Bill Number : ");
      printf("%s     ", customer.billNumber);
      printf("Customer's Name : ");
      printf("%s     ", customer.name);
      printf("Customer's Mobile : ");
      printf("%lu\n", customer.mobile);
      printf("\n-----------------------------------------------------------------------------------------------");
      printf("\nCODE\t\tNAME\t\t\t\tCATEGORY\t\tQUANTITY\tPRICE\n");
      printf("\n-----------------------------------------------------------------------------------------------\n");

      for(int n = 0; n < i; n++)
      printf("\n%-16s%-32s%-24s   %-8d\t%-8.2f\n", customer.itemCode[n], customer.itemName[n], customer.itemCategory[n], customer.quantity[n], customer.price[n]);

      printf("\n-----------------------------------------------------------------------------------------------\n");
      printf("\n");
      printf("Total                                                                                   ");
      printf("%.2f\n", customer.total);
      printf("-----------------------------------------------------------------------------------------------\n");
      printf("\n");

      fclose(fptr);

      break;
    }

  }

}
