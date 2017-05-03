#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct no {
    char op;
    int valor;
    struct no* esq;
    struct no* dir;
};

int em_ordem(struct no*);
int pre_ordem(struct no*);
int pos_ordem(struct no*);
int print_nodo(struct no*);
int calcula_altura(struct no*);
int valida_entrada(char*);
int conta_itens(struct no*, char*);
int elementos_arv(struct no*);
struct no* monta_arvore();
char retorna_elem(int*);
float calcula_resultado(struct no*);

char pilha[255];
int top;

int main(){
    printf("Sua expressão: ");
    scanf(" %[^\n]s", pilha);

    top = strlen(pilha) - 1;

    struct no* raiz = monta_arvore();
    
    if(!valida_entrada(pilha) || !conta_itens(raiz, pilha)){
        puts("Notação incorreta.");
        return 0;
    }

    printf("Em ordem: ");
    em_ordem(raiz);
    printf("\n");
    
    printf("Pre ordem: ");
    pre_ordem(raiz);
    printf("\n");
    
    printf("Pos ordem: ");
    pos_ordem(raiz);
    printf("\n");

    printf("Altura da árvore: %d\n", calcula_altura(raiz));
    printf("Resultado da expressão: %.2f\n", calcula_resultado(raiz));

    return 0;
}

int em_ordem(struct no* r){
    if(r){
        em_ordem(r->esq);
        print_nodo(r);
        em_ordem(r->dir);
    }
    return 1;
}

int pre_ordem(struct no* r){
    if(r){
        print_nodo(r);
        pre_ordem(r->esq);
        pre_ordem(r->dir);
    }
    return 1;
}

int pos_ordem(struct no* r){
    if(r){
        pos_ordem(r->esq);
        pos_ordem(r->dir);
        print_nodo(r);
    }
    return 1;
}

int print_nodo(struct no* r){
    if(r->op)
        printf("%c ", r->op);
    else
        printf("%d ", r->valor);
    return 1;
}

int valida_entrada(char* entrada){
    for(int i = 0; entrada[i]; i++)
        if((entrada[i] < '0' || entrada[i] > '9') \
                && entrada[i] != '*' \
                && entrada[i] != '/' \
                && entrada[i] != '-' \
                && entrada[i] != '+' \
                && entrada[i] != ' ')
            return 0;
    return 1;
}

int conta_itens(struct no* r, char* p){
    int result_p = 0, result_r = 0;
    for(int i = 0; p[i]; i++){
        if(p[i] != ' '){
            result_p++;
            while(p[i] != ' ' && p[i])
                i++;
        }
    }

    result_r = elementos_arv(r) + 1;
    return (result_p != result_r) ? 0 : 1;
}

int elementos_arv(struct no* r){
    if(r){
        int esq = 1 + elementos_arv(r->esq);
        int dir = 1 + elementos_arv(r->dir);
        return esq + dir;
    }
    return -1;
}

struct no* monta_arvore(){
    char op;
    int valor, t1, t2;
    struct no* nodo = (struct no*)malloc(sizeof(struct no));
    op = retorna_elem(&valor);
    nodo->valor = valor;
    nodo->op = op;
    if(op){
        nodo->dir = monta_arvore();
        t1 = top;
        nodo->esq = monta_arvore();
        t2 = top;
        if(t1 < 0 && t2 < 0){
            printf("Notação incorreta.\n");
            exit(1);
        }
    }
    else{
        nodo->dir = NULL;
        nodo->esq = NULL;
    }
    return nodo;
}

float calcula_resultado(struct no* r){
    if(r){
        if(r->op){
            float result;
            switch(r->op){
                case '*': result = calcula_resultado(r->esq) * calcula_resultado(r->dir);
                          break;
                case '+': result = calcula_resultado(r->esq) + calcula_resultado(r->dir);
                          break;
                case '-': result = calcula_resultado(r->esq) - calcula_resultado(r->dir);
                          break;
                case '/': result = calcula_resultado(r->esq) / calcula_resultado(r->dir);
                          break;
                default: break;
            }
            return result;
        }
        return r->valor;
    }
    return 1;
}

int calcula_altura(struct no* r){
    if(r){
        int dir = 1 + calcula_altura(r->dir);
        int esq = 1 + calcula_altura(r->esq);
        return (esq > dir) ? esq : dir;
    }
    return -1;
}

char retorna_elem(int* valor){
    char elem;
    if(pilha[top] == '*' || pilha[top] == '/' || pilha[top] == '+' || pilha[top] == '-'){
        elem = pilha[top];
        *valor = 0;
        top -= 2;
    }
    else{
        int cont = 0, i = top, base = 1, acc = 0;
        while(pilha[i] != ' ' && i >= 0){
            acc += ((pilha[i] - 48) * base);
            base *= 10;
            cont++;
            i--;
        }
        elem = '\0';
        *valor = acc;
        top -= (cont+1);
    }
    return elem;
}
