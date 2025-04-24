#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <omp.h>

// Estrutura para um nó da lista encadeada
typedef struct Node {
    char *filename;
    struct Node *next;
} Node;

// Função para criar um novo nó
Node* createNode(const char *filename) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    
    newNode->filename = strdup(filename);
    if (newNode->filename == NULL) {
        perror("Erro ao alocar memória para o nome do arquivo");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    
    newNode->next = NULL;
    return newNode;
}

// Função para adicionar um nó no final da lista
void appendNode(Node **head, const char *filename) {
    Node *newNode = createNode(filename);
    
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Função para liberar a memória da lista
void freeList(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp->filename);
        free(temp);
    }
}

// Função para processar um nó (imprime o nome do arquivo e a thread)
void processNode(Node *node) {
    printf("Arquivo: %s - Processado pela thread %d\n", 
           node->filename, omp_get_thread_num());
}

int main() {
    Node *head = NULL;
    
    // Adiciona alguns arquivos fictícios à lista
    appendNode(&head, "documento1.txt");
    appendNode(&head, "imagem.jpg");
    appendNode(&head, "relatorio.pdf");
    appendNode(&head, "planilha.xlsx");
    appendNode(&head, "apresentacao.ppt");
    
    // Inicia a região paralela
    #pragma omp parallel
    {
        // Apenas uma thread cria as tarefas
        #pragma omp single nowait
        {
            Node *current = head;
            while (current != NULL) {
                // Cria uma tarefa para cada nó
                #pragma omp task firstprivate(current)
                {
                    processNode(current);
                }
                current = current->next;
            }
        }
        // Todas as threads esperam as tarefas serem concluídas
        #pragma omp taskwait
    }
    
    // Libera a memória da lista
    freeList(head);
    
    return 0;
}