#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "linked-list.h"

namespace base {

   Node* destroy(Node* list)
   {
      return NULL;
   }

   Node* append(Node* list, uint32_t nmec, char *name)
   {
      Node* new_node = (Node*) malloc(sizeof(Node));
      if (new_node == NULL) {
         fprintf(stderr, "Error: %s\n", strerror(errno));
         exit(EXIT_FAILURE);
      }
      new_node->reg.nmec = nmec;
      new_node->reg.name = strdup(name);
      new_node->next = NULL;


   if (list == NULL) {
      return new_node; // If the list is empty, the new node becomes the head
   } else {
        Node* current = list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node; // Add the new node at the end of the list
        return list;
   }
}

   void print(Node* list)
   {
      printf("List:\n");
      while (list != NULL) {
         printf("  %d: %s\n", list->reg.nmec, list->reg.name);
         list = list->next;
      }

   }

   int exists(Node* list, uint32_t nmec)
   {
      if (list == NULL) {
         return 0;
      } else {
         Node* current = list;
         while (current != NULL) {
            if (current->reg.nmec == nmec) {
               return 1;
            }
            current = current->next;
         }
         return 0;
      }
   }

   Node* remove(Node* list, uint32_t nmec)
   {
      // Verifique se a lista está vazia
    if (list == NULL) {
        return list;
    }

    // Verifique se o primeiro nó deve ser removido
    if (list->reg.nmec == nmec) {
        Node* temp = list;
        list = list->next;
        delete temp;
        return list;
    }

    // Percorra a lista e procure pelo nó a ser removido
    Node* current = list;
    while (current->next != NULL) {
        if (current->next->reg.nmec == nmec) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return list;
        }
        current = current->next;
    }

    return list; // Retorne a lista original se o nó não for encontrado


      
   }

   const char *search(Node* list, uint32_t nmec)
   {
      if (list == NULL) {
         return NULL;
      }else{
         Node* current = list;
         while(current != NULL){
            if(current->reg.nmec == nmec){
               return current-> reg.name;
            }
         }
      }
   }

  
Node* sort_by_name(Node* list)
{
   if (list == NULL || list->next == NULL) {
         return list; // Retorna a lista sem alterações se estiver vazia ou tiver apenas um nó
   } else {
         int swapped;
         Node* current;
         Node* last = NULL;
   
         do {
               swapped = 0;
               current = list;
   
               while (current->next != last) {
                  if (strcmp(current->reg.name, current->next->reg.name) > 0) {
                     // Troca os nós
                     Node* temp = current->next;
                     current->next = temp->next;
                     temp->next = current;
                     if (last == NULL) {
                           list = temp; // Atualiza o início da lista se o primeiro elemento foi trocado
                     } else {
                           last->next = temp; // Atualiza o próximo do último nó se não for o primeiro elemento
                     }
                     last = temp;
                     swapped = 1;
                  } else {
                     last = current;
                     current = current->next;
                  }
               }
         } while (swapped);
   
      return list; // Retorna a lista após a ordenação
      }
   }

   Node* sort_by_number(Node* list)
   {
      if (list == NULL || list->next == NULL) {
         return list; // Retorna a lista sem alterações se estiver vazia ou tiver apenas um nó
      } else {
         int swapped;
         Node* current;
         Node* last = NULL;
   
         do {
               swapped = 0;
               current = list;
   
               while (current->next != last) {
                  if (current->reg.nmec > current->next->reg.nmec) {
                     // Troca os nós
                     Node* temp = current->next;
                     current->next = temp->next;
                     temp->next = current;
                     if (last == NULL) {
                           list = temp; // Atualiza o início da lista se o primeiro elemento foi trocado
                     } else {
                           last->next = temp; // Atualiza o próximo do último nó se não for o primeiro elemento
                     }
                     last = temp;
                     swapped = 1;
                  } else {
                     last = current;
                     current = current->next;
                  }
               }
         } while (swapped);
   
      return list; // Retorna a lista após a ordenação
      }

   }
}

