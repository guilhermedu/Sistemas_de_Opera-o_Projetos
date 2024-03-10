#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#include "linked-list.h"

using namespace base;

static Node* list = NULL;

/* ******************************************** */

static void printMenu(void)
{
   printf("\n"
         "+===============================================+\n"
         "|            Manipulation functions             |\n"
         "+===============================================+\n"
         "| 0 --- quit                                    |\n"
         "| 1 --- append new student                      |\n"
         "| 2 --- print list                              |\n"
         "| 3 --- remove student, through number          |\n"
         "| 4 --- get student's name, given the number    |\n"
         "| 5 --- sort list by name                       |\n"
         "| 6 --- sort list by number                     |\n"
         "+===============================================+\n");
}

/* ******************************************** */
/* menu handling functions */
/* ******************************************** */


void menuChoiceAppend()
{
  
   uint32_t nmec;
   char name[100];
   printf("Student number: ");
   int res = scanf("%d", &nmec);
   if (res != 1)
   {
      fprintf(stderr, "Wrong input format (unsigned integer number required)\n");
      scanf("%*s"); // consume and ignore standard input
      return;
   }
   printf("Student name: ");
   res = scanf("%s", name);
   if (res != 1)
   {
      fprintf(stderr, "Wrong input format (string required)\n");
      scanf("%*s"); // consume and ignore standard input
      return;
   }

   list = append(list, nmec, name);
   printf("\n");

}

/* ******************************************** */

void menuChoicePrint()
{
   print(list);
}

/* ******************************************** */

void menuChoiceRemove()
{
      uint32_t nmec;
      printf("Número mecanográfico do aluno a ser removido: ");
      int res = scanf("%d", &nmec);

      if (res != 1)
      {
         fprintf(stderr, "Erro ao ler o número mecanográfico.\n");
         scanf("%*s"); // Consuma e ignore a entrada padrão
         return;
      }

      list = remove(list, nmec);

      if (list != NULL)
      {
         printf("Número mecanográfico %d foi removido com sucesso.\n", nmec);
      }
      else
      {
         printf("Número mecanográfico %d não foi encontrado na lista.\n", nmec);
      }
}

/* ******************************************** */

void menuChoiceSearch()
{
      uint32_t nmec;
      printf("Número mecanográfico do aluno a ser procurado: ");
      int res = scanf("%d", &nmec);

      if (res != 1)
      {
         fprintf(stderr, "Erro ao ler o número mecanográfico.\n");
         scanf("%*s"); // Consuma e ignore a entrada padrão
         return;
      }

      const char *name = search(list, nmec);

      printf("%s\n",name);

      if (list != NULL)
      {
         printf("O Número mecanográfico do %s foi encontrado com sucesso.\n", name);
      }
      else
      {
         printf("Número mecanográfico %s não foi encontrado na lista.\n", name);
      }

}

/* ******************************************** */

void menuChoiceSortByName()
{
   printf("A lista está ordenada por nome:\n");
   list=sort_by_name(list);
   print(list);
}

/* ******************************************** */

void menuChoiceSortByNumber()
{
   printf("A lista está ordenada por numero mecanográfico:\n");
   list=sort_by_number(list);
   print(list);
}

/* ******************************************** */

/* The main function */
int main()
{
   int terminate = 0;
   while (!terminate)
   {
      printMenu();
      printf("Your command: ");
      int cmd;
      int res = scanf("%d", &cmd);
      if (res != 1)
      {
         fprintf(stderr, "Wrong input format (unsigned integer number required)\n");
         scanf("%*s"); // consume and ignore standard input
      }
      else
      {
         printf("\n");
         switch(cmd) 
         {
            case 0:
               terminate = 1;
               break;
            case 1:
               menuChoiceAppend();
               break;
            case 2:
               menuChoicePrint();
               break;
            case 3:
               menuChoiceRemove();
               break;
            case 4:
               menuChoiceSearch();
               break;
            case 5:
               menuChoiceSortByName();
               break;
            case 6:
               menuChoiceSortByNumber();
               break;
            default:
               fprintf(stderr, "Wrong action required (%d)\n", cmd);
         }
      }
   }

   return 0;
}     /* end of main */

