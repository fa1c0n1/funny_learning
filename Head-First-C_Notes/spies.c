#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char *question;
    struct node *yes;
    struct node *no;
} node;

int yes_no(char *question) {
    char answer[3];
    printf("%s? (y/n): ", question);
    fgets(answer, 3, stdin);

    return answer[0] == 'y';
}

node* create(char *question) {
    node *n = malloc(sizeof(node));
    n->question = strdup(question);
    n->yes = NULL;
    n->no = NULL;

    return n;
}

void release(node *n) {
    if (n) {
       if (n->yes) {
           release(n->yes);
       }
       if (n->no) {
           release(n->no);
       }
       if (n->question) {
          free(n->question);
       }
       
       free(n);
    }
}

int main(void) {
    char question[80];
    char suspect[20];
    node *start_node = create("Does suspect have a mustache");
    start_node->yes = create("Vinny the Spoon");
    start_node->no = create("Loretta Barnsworth");

    node *current;

    do {
        current = start_node; 

        while(1) {
            if (yes_no(current->question)) {
                if (current->yes) {
                    current = current->yes;
                } else {
                    printf("SUSPECT IDENTIFIED\n");
                    break;
                }
            } else if (current->no) {
                current = current->no; 
            } else {
                printf("Who's the suspect? ");
                fgets(suspect, 20, stdin);
                node *yes_node = create(suspect);
                current->yes = yes_node;

                node *no_node = create(current->question);
                current->no = no_node;

                printf("Give me a question that is TRUE for %s but not for %s? ", suspect, current->question);
                fgets(question, 80, stdin);
                free(current->question);
                current->question = strdup(question);

                break;
            }
        }
    } while(yes_no("Run again"));

    release(start_node);
    return 0;
}

