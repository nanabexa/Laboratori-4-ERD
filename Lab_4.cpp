#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lnode {
    int value;
    struct lnode *next;
};

struct list {
  struct lnode *head;
  struct lnode *tail;
};


int list_add(struct list *lst, int value)
{
    /* Append the value to the end of the linked list. */
    struct lnode *new_node = (struct lnode *)malloc(sizeof(struct lnode));
    if (new_node == NULL) {
        return -1; // Memory allocation failed
    }

    new_node->value = value;
    new_node->next = NULL;

    // If list is empty, set both head and tail to new node
    if (lst->head == NULL) {
        lst->head = new_node;
        lst->tail = new_node;
    } else {
        // Link the new node to the end of the list
        lst->tail->next = new_node;
        lst->tail = new_node;
    }

    return 0;
}

struct lnode *list_find(struct list *lst, int value)
{
    struct lnode *current = lst->head;

    // Traverse the list looking for the value
    while (current != NULL) {
        if (current->value == value) {
            return current; // Found the value
        }
        current = current->next;
    }

    return NULL; // Value not found
}


int list_dump(struct list *lst)
{
    struct lnode *cur;
    printf("\nDump:\n");
    for(cur=lst->head; cur != NULL; cur=cur->next) {
        printf("  %d\n", cur->value);
    }
    return 0;
}

int list_size(struct list *lst) {
    struct lnode *cur;
    int size = 0;
    for(cur=lst->head; cur != NULL; cur=cur->next) {
        size++;
    }
    return size;
}

int list_is_empty(struct list *lst)
{
    if(lst->head == NULL) {
        return 1;
    }
    return 0;
}

int list_get_first(struct list *lst)
{
    struct lnode *cur;
    if(list_is_empty(lst) == 1) {
        return -1;
    }
    cur = lst->head;
    return cur->value;
}

int list_get_last(struct list *lst)
{
    struct lnode *cur;
    if(list_is_empty(lst) == 1) {
        return -1;
    }
    cur = lst->tail;
    return cur->value;
}

int list_remove_first(struct list *lst)
{
    struct lnode *cur;
    if(list_is_empty(lst) == 1) {
        return -1;
    }
    cur = lst->head;
    lst->head = lst->head->next;
    free(cur);
    return 0;
}

int list_remove_last(struct list *lst)
{
    struct lnode *cur;
    if(list_is_empty(lst) == 1) {
        return -1;
    }
    struct lnode *penult = lst->head;
    while (penult->next->next != NULL) {
        penult = penult->next;
    }
    free(penult->next);
    penult->next = NULL;
    lst->tail = penult;
    return 0;
}

int list_remove_value(struct list *lst, int value) {
    struct lnode *cur = lst->head;
    struct lnode *penult = NULL;
    if (cur == NULL) {
        return -1;
    }
    int pos = 1;
    while (cur->value != value) {
        penult = cur;
        cur = cur->next;
        pos++;
    }

    if (cur == lst->head) {
        list_remove_first(lst);
        return 0;
    }

    if (cur == lst->tail) {
        list_remove_last(lst);
        return 0;
    }

    if (cur != NULL) {
        penult->next = cur->next;
        free(cur);
    }

    return 0;
}
// **Problema 3.1: list_insert_at
int list_insert_at(struct list *lst, int index, int value) {
    // Validar que el índice no sea negativo
    if (index < 0) {
        return -1;
    }

    // Si el índice es 0, insertar al inicio
    if (index == 0) {
        struct lnode *new_node = (struct lnode *)malloc(sizeof(struct lnode));
        if (new_node == NULL) {
            return -1; // Error de memoria
        }

        new_node->value = value;
        new_node->next = lst->head;
        lst->head = new_node;

        // Si la lista estaba vacía, también actualizar tail
        if (lst->tail == NULL) {
            lst->tail = new_node;
        }

        return 0;
    }

    // Encontrar la posición donde insertar
    struct lnode *current = lst->head;
    int current_index = 0;

    // Avanzar hasta la posición anterior al índice deseado
    while (current != NULL && current_index < index - 1) {
        current = current->next;
        current_index++;
    }

    // Si el índice está fuera de rango
    if (current == NULL) {
        return -1;
    }

    // Crear el nuevo nodo
    struct lnode *new_node = (struct lnode *)malloc(sizeof(struct lnode));
    if (new_node == NULL) {
        return -1; // Error de memoria
    }

    new_node->value = value;
    new_node->next = current->next;
    current->next = new_node;

    // Si insertamos al final, actualizar tail
    if (new_node->next == NULL) {
        lst->tail = new_node;
    }

    return 0;
}

// **Problema 3.2: list_reverse**
void list_reverse(struct list *lst) {
    // Si la lista está vacía o tiene un solo elemento, no hay nada que hacer
    if (lst->head == NULL || lst->head->next == NULL) {
        return;
    }

    struct lnode *prev = NULL;
    struct lnode *current = lst->head;
    struct lnode *next = NULL;

    // Guardar el tail original (que será el nuevo head)
    lst->tail = lst->head;

    // Invertir los enlaces
    while (current != NULL) {
        next = current->next;    // Guardar el siguiente nodo
        current->next = prev;    // Invertir el enlace
        prev = current;          // Mover prev hacia adelante
        current = next;          // Mover current hacia adelante
    }

    // Actualizar head al último nodo procesado
    lst->head = prev;
}

// **Problema 4.1: list_clear**
void list_clear(struct list *lst) {
    struct lnode *current = lst->head;
    struct lnode *next;

    // Recorrer la lista liberando cada nodo
    while (current != NULL) {
        next = current->next;    // Guardar el siguiente antes de liberar
        free(current);           // Liberar el nodo actual
        current = next;          // Mover al siguiente
    }

    // Resetear la lista
    lst->head = NULL;
    lst->tail = NULL;
}

// **Problema 4.2: list_copy**
struct list* list_copy(struct list *lst) {
    // Crear una nueva lista
    struct list *new_list = (struct list *)malloc(sizeof(struct list));
    if (new_list == NULL) {
        return NULL; // Error de memoria
    }

    // Inicializar la nueva lista
    new_list->head = NULL;
    new_list->tail = NULL;

    // Si la lista original está vacía, retornar la lista vacía
    if (lst->head == NULL) {
        return new_list;
    }

    struct lnode *current = lst->head;

    // Copiar cada nodo
    while (current != NULL) {
        // Intentar agregar el valor a la nueva lista
        if (list_add(new_list, current->value) == -1) {
            // Si falla, limpiar la memoria y retornar NULL
            list_clear(new_list);
            free(new_list);
            return NULL;
        }
        current = current->next;
    }

    return new_list;
}

int main()
{
    struct list mylist;
    struct lnode *mynode;

    mylist.head = NULL;
    mylist.tail = NULL;

    list_add(&mylist, 10);
    list_add(&mylist, 20);
    list_add(&mylist, 30);
    list_add(&mylist, 40);
    list_add(&mylist, 50);
    list_dump(&mylist);
    printf("tamaño de la lista: %d\n",list_size(&mylist));
    printf("la lista está vacía? %d\n",list_is_empty(&mylist));
    printf("primer valor de la lista: %d\n",list_get_first(&mylist));
    printf("último valor de la lista: %d\n",list_get_last(&mylist));
    list_remove_first(&mylist);
    list_remove_last(&mylist);
    list_remove_value(&mylist, 30);
    list_dump(&mylist);

    mynode = list_find(&mylist, 42);
    if ( mynode == NULL ) {
        printf("Did not find 42\n");
    } else {
        printf("Looked for 42, found %d\n", mynode->value);
    }

    mynode = list_find(&mylist, 30);
    if ( mynode == NULL || mynode->value != 30) {
        printf("Did not find 30\n");
    } else {
        printf("Found 30\n");
    }

    list_add(&mylist, 40);
    list_dump(&mylist);
    return 0;

}
