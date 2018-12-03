#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include "scan.h"
#include <cstdlib>
using namespace std;

//extern Tk *getTokens(char *nome);
Tk *tokens;
list<simbolos *> listSimbolos;
list<Tk *> pReservadas;
list<Tk *> listId;
Tk *listTokens;
int contId;

char reservedWords[][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while"};

void analiseLexica();
void loadId();
void loadPr();

int buscaId(char *nome);
int buscaSimbolo(char *nome);
Tk *lexaId(Tk *ptr);
Tk *lexaPr(Tk *ptr, char *nome);
int verificaOperadores(Tk *ptr);
void addListSimbolo(Tk *sb, int id);
void exibListSimbolo();
int ehOp(char c);

/*********************************
 * Nome: main
 * desc:
 * returno:
 ********************************/
int main(int argc, char *argv[])
{

    char arq[50];

    char *var;
    if (argc != 2)
    {
        printf("Digite o nome do arquivo:");
        scanf("%s", arq);
    }
    else
    {
        strcpy(arq, argv[1]);
    }

    listTokens = getTokens(arq);
    exibeTk(listTokens);
    loadId();
    loadPr();
    analiseLexica();
    liberaTk(listTokens);

    system("pause");
    return 0;
}

void carregaRW()
{
    char words[][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while"};
}

void analiseLexica()
{
    Tk *ptr;
    int aux = 0, lx;

    printf("aqui 1\n");
    for (ptr = listTokens; ptr != NULL; ptr = ptr->prox)
    {


        printf("Nome: %s\n", ptr->nome);

        if (ptr->linha != aux)
        {
            aux = ptr->linha;
        }
        lx = buscaId(ptr->nome);

        printf("return busca: %d\n", lx);
        if (lx)
        {

            ptr = lexaId(ptr->prox);
        }
        // testa par saber se é um if ou while ou switch ou for
        if (strcmp("if", ptr->nome) == 0 || strcmp("while", ptr->nome) == 0 || strcmp("switch", ptr->nome) == 0 || strcmp("for", ptr->nome) == 0)
        {
            printf("\nEncontrado, %s \n", ptr->nome);
            ptr = lexaPr(ptr->prox, ptr->nome);
        }

    }

}

Tk *lexaId(Tk *ptr)
{
    // Tk *aux;
    // loop no infinito
    for (;;)
    {
        if (buscaSimbolo(ptr->nome))
        {
            printf("\nErro... linha: %d A variavel: %s ja foi declarada \n", ptr->linha, ptr->nome);
            system("pause");
            exit(1);
        }

        if (!isalpha(ptr->nome[0]))
        {
            printf("\nErro... linha: %d A variavel: %s esperado um identificador \n", ptr->linha, ptr->nome);
            system("pause");
            exit(1);
        }

        addListSimbolo(ptr, 1);
        ptr = ptr->prox;

        if (ptr->nome[0] == ';')
        {
            ptr = ptr->prox;
            exibListSimbolo();
            return ptr;
        }
        else if (ptr->nome[0] == ',')
        {
            ptr = ptr->prox;
        }
        else
        {
            printf("\nErro linha: %d, caractere: %s invalido. Esperado uma , ou ; \n", ptr->linha, ptr->nome);
            system("pause");
            exit(1);
        }
    }
}

Tk *lexaPr(Tk *ptr, char *nome)
{
    int verificador = 1;
    int contParentesesAberto = 0;
    int contParentesesFechado = 0;
    int contador = 0;
    char *tipo;
    int lx;
    for (;;)
    {
        if (contador == 0 && ptr->nome[0] != '(')
        {
            printf("\nErro linha: %d, caractere: %s invalido. Esperado um ( \n", ptr->linha, ptr->nome);
            verificador = 0;
            system("pause");
            exit(1);

        }

        if (ptr->nome[0] == '(')
        {
            verificador = 1;
            contParentesesAberto++;
        }

        //Se fechou parentese
        if (ptr->nome[0] == ')')
        {
            contParentesesFechado++;
        }


        if (strcmp("if", nome) == 0 || strcmp("while", nome) == 0)
        {
            if (ptr->nome[0] != '(' && ptr->nome[0] != ')' && ptr->nome[0] != '{')
            {
                //Eh variavel?
                if (buscaSimbolo(ptr->nome))
                {
                    verificador = 1;
                }
                else
                {
                    verificador = 0;
                }

                //Se é um operador
                if (ehOp(ptr->nome[0]))
                {
                    if (verificaOperadores(ptr))
                    {
                        verificador = 1;
                    }
                    else
                    {
                        ptr = ptr->prox;
                        verificador = 1;
                    }
                }

                //Se é uma string
                if (verificador == 0)
                {
                    if ((ptr->nome[0] == '"' || ptr->nome[0] == '\''))
                    {
                        verificador = 1;
                    }
                }

                //Se é um inteiro
                if (verificador == 0)
                {
                    if (atoi(ptr->nome))
                    {
                        verificador = 1;
                    }
                    if (atof(ptr->nome))
                    {
                        verificador = 1;
                    }
                }

                if (verificador == 0)
                {
                    printf("\nErro linha: %d. comando inesperado \n", ptr->linha);
                    system("pause");
                    exit(1);
                }
            }
        }

        //... verificaçoes if || while
        //1 - Varieveis existem
        //2 - 1 ou 0
        //3 - Se & entao &&
        //4 - Se | entao  ||
        //5 - Se começa = entao  ==
        //6 - Se > pode ser =
        //7 - Se < pode ser =

        //Verifica se acabou o if || while || for
        if (ptr->nome[0] == '{')
        {

            if (contParentesesAberto == contParentesesFechado)
            {
                verificador = 1;
            }
            else
            {
                verificador = 0;
                printf("\nErro linha: %d. Esperado fechar todas () \n", ptr->linha, ptr->nome);
            }

            //Verficador do erro e trata.
            if (verificador)
            {
                return ptr;
            }
            else
            {
                system("pause");
                exit(1);
            }
        }

        //Verifica se chegou fim, para se não continua
        if (ptr->nome[0] != '{')
        {
            ptr = ptr->prox;
            contador++;
        }
    }
}

/*********************************
 * Nome: ehOp
 * desc:
 * returno:
 ********************************/
int ehOp(char c)
{
    return (strchr("&|><=!+*/%", c) != NULL);
}

int verificaOperadores(Tk *ptr)
{
    //Verifica se é &.
    if (strcmp("&", ptr->nome) == 0)
    {
        if (strcmp("&", ptr->prox->nome) == 0)
        {
            return 0;
        }
    }
    //Verifica se é ||.
    if (strcmp("|", ptr->nome) == 0)
    {
        if (strcmp("|", ptr->prox->nome) == 0)
        {
            return 0;
        }
    }
    //Verifica se é >.
    if (strcmp(">", ptr->nome) == 0)
    {
        if (ehOp(ptr->prox->nome[0]) && strcmp("=", ptr->prox->nome) == 0)
        {
            return 0;
        }
        return 1;
    }
    //Verifica se é <
    if (strcmp("<", ptr->nome) == 0)
    {
        if (ehOp(ptr->prox->nome[0]) && strcmp("=", ptr->prox->nome) == 0)
        {
            return 0;
        }
        return 1;
    }

    if (strcmp("!", ptr->nome) == 0)
    {
        if (strcmp("=", ptr->prox->nome) == 0)
        {
            return 0;
        }
    }

    if (strcmp("==", ptr->nome) == 0)
    {
        return 1;
    }

    //Chegou aqui é erro
    printf("\n \nErro linha: %d, caractere: %s invalido. Esperado possíveis operadores &&, ||, >, >=, ==, !=,<, <= \n", ptr->linha, ptr->nome);
    system("pause");
    exit(1);
    return 0;
}

int buscaSimbolo(char *nome)
{

    list<simbolos *>::iterator it;
    for (it = listSimbolos.begin(); it != listSimbolos.end(); it++)
    {
        if (strcmp(nome, (*it)->nome) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void exibListSimbolo()
{
    list<simbolos *>::iterator it;
    char tip[][15] = {"Ponteiro", "Id", "Constante", "Vetor", "Opr", "Preservada"};
    for (it = listSimbolos.begin(); it != listSimbolos.end(); it++)
    {
        printf("[ %s, %s , %d ]\n", (*it)->nome, tip[(*it)->id], (*it)->n);
    }

    printf("\n");
}

void loadId()
{

    char nome[][10] = {"int", "char", "float", "double", "struct"};
    Tk *l;
    //Palavras reservadas
    for (int i = 0; i < 6; i++)
    {
        l = (Tk *)malloc(sizeof(Tk));
        l->nome = (char *)malloc(sizeof(char) * strlen(nome[5]));
        strcpy(l->nome, nome[i]);
        l->id = contId;
        contId++;
        listId.push_back(l);
        //pReservadas
    }
}

int buscaId(char *nome)
{
    //list <Tk*> listId;
    list<Tk *>::iterator it;
    int i = 1;
    for (it = listId.begin(); it != listId.end(); it++, i++)
    {
        if (strcmp(nome, (*it)->nome) == 0)
        {
            return i;
        }
    }

    return 0;
}

void loadPr()
{
    char nome[][10] = {"if", "for", "do", "switch", "while", "long"};
    Tk *l;
    //Palavras reservadas
    for (int i = 0; i < 6; i++)
    {
        l = (Tk *)malloc(sizeof(Tk));
        l->nome = (char *)malloc(sizeof(char) * strlen(nome[5]));
        strcpy(l->nome, nome[i]);
        l->id = contId;
        contId++;
    }
}

void addListSimbolo(Tk *sb, int id)
{
    static int scont = 0;
    scont++;
    //list <Tk*> listSimbolos;
    simbolos *ss = new simbolos();
    ss->n = scont;
    ss->id = id;
    ss->nome = sb->nome;
    ss->linha = sb->linha;
    listSimbolos.push_back(ss);
}
