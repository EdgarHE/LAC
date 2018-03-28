#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********STRUCTURE********/
/******PILE_INT******/
typedef struct pile_n pile_n;
struct pile_n
{
    int valeur;
    pile_n *next;
};

typedef struct pile pile;
struct pile
{
    pile_n *top;
    int num;//num de valeur dans le pile
};

pile* pile_new()
{
    pile *p = (pile*)malloc(sizeof(pile));
    p->top = NULL;
    p->num = 0;
    return p;
}

int push_b(int num_p, pile* pile_a)
{
    pile_n *p = (pile_n*)malloc(sizeof(pile_n));
    p->valeur = num_p;
    p->next = pile_a->top;
    pile_a->top = p;
    (pile_a->num)++;
    return 1;
}

int pop_b(int *num_p, pile* pile_a)
{
    if(pile_a->num > 0)
    {
        pile_n *temp = pile_a->top;
        *num_p = temp->valeur;
        pile_a->top = pile_a->top->next;
        (pile_a->num)--;
        free(temp);
        return 1;
    }
    else return 0;
}

int pop_num(int* num_p, pile* pile_a)
{
    if(pile_a->num > 0)
    {
        *num_p = pile_a->top->valeur;
        return 1;
    }
    else return 0;
}

void pile_clear(pile* pile_a)
{
    while(pile_a->num>0)
    {
        pile_n *temp = pile_a->top;
        pile_a->top = temp->next;
        (pile_a->num)--;
        free(temp);
    }
}


/******PILE_CHAR******/
typedef struct pile_cn pile_cn;
struct pile_cn
{
    char valeur;
    pile_cn *next;
};

typedef struct pilec pilec;
struct pilec
{
    pile_cn *top;
    int num;//num de valeur dans le pile
};

pilec* pilec_new()
{
    pilec *p = (pilec*)malloc(sizeof(pilec));
    p->top = NULL;
    p->num = 0;
    return p;
}

int push_cb(char val, pilec* pile_a)
{
    pile_cn *p = (pile_cn*)malloc(sizeof(pile_cn));
    p->valeur = val;
    p->next = pile_a->top;
    pile_a->top = p;
    (pile_a->num)++;
    return 1;
}

void pop_cb(char *val, pilec* pile_a)
{
    if(pile_a->num > 0)
    {
        pile_cn *temp = pile_a->top;
        *val = temp->valeur;
        pile_a->top = pile_a->top->next;
        (pile_a->num)--;
        free(temp);
        //return 1;
    }
    else
    {
        //printf("Pile vide !\n");
        exit(1);
    }
}

char pop_cval(pilec* pile_a)
{
    if(pile_a->num > 0)
    {
        char num_p = pile_a->top->valeur;
        return num_p;
    }
    else
        return 0;
}

int pile_vide(pilec* pile_a)
{
    return (pile_a->num == 0);
}

void pile_cclear(pilec* pile_a)
{
    while(pile_a->num>0)
    {
        pile_cn *temp = pile_a->top;
        pile_a->top = temp->next;
        (pile_a->num)--;
        free(temp);
    }
}

/******Lexeme******/
typedef enum lexeme_t
{
    NOMBRE,
    Par_ou, // Parenth¨¨se ouvrante
    Par_fe, // Parenth¨¨se fermante
    SIGNE_mul_div, // Op¨¦rateurs (x,/)
    SIGNE_plus_moins // signe(+,-)
} lexeme_t;

typedef struct lexeme
{
    lexeme_t type;
    char valeur[20];
} lexeme;

typedef struct cellule *liste;
struct cellule
{
    lexeme tete;
    liste queue;
    liste fin;
};

void add_fin(lexeme e, liste *l)
{
    liste element = malloc(sizeof(struct cellule));
    element->tete = e;
    element->queue = NULL;
    element->fin = element;
    if((*l)==NULL) *l = element;
    else if((*l)->queue == NULL)
    {
        (*l)->queue = element;
        (*l)->fin = element;
    }
    else
    {
        (*l)->fin->queue = element;
        (*l)->fin = element;
    }
}

/******ARBRE_BINAIRE******/
typedef struct BinNode BinNode;
struct BinNode
{
    char valeur[20];
    BinNode *left_child;
    BinNode *right_child;
};

typedef struct BinTree BinTree;
struct BinTree
{
    BinNode *root;
    int num;//层数
};

BinTree BinTree_new()
{
    BinTree *p = (BinTree*)malloc(sizeof(BinTree));
    p->root=NULL;
    p->num=0;
    return *p;
}

BinNode *build_node(char *x)
{
    BinNode *new_node = (BinNode*)malloc(sizeof(BinNode));
    int a;
    int l=strlen(x);
    //printf("%d\n",l);
    for(a=0;a<l;a++)
    {
        new_node->valeur[a]=x[a];
    }
    for(a=l;a<20;a++)
    {
        new_node->valeur[a]='\0';
    }
    new_node->left_child=NULL;
    new_node->right_child=NULL;
    //l=strlen(new_node->valeur);
    //printf("new %d\n",l);
    return (new_node);
}


/******PILE_TREE******/
typedef struct pile_tn pile_tn;
struct pile_tn
{
    BinTree valeur;
    pile_tn *next;
};

typedef struct pilet pilet;
struct pilet
{
    pile_tn *top;
    int num;//num de valeur dans le pile
};

pilet* pilet_new()
{
    pilet *p = (pilet*)malloc(sizeof(pilet));
    p->top = NULL;
    p->num = 0;
    return p;
}

int push_tb(BinTree val, pilet* pile_a)
{
    pile_tn *p = (pile_tn*)malloc(sizeof(pile_tn));
    p->valeur = val;
    p->next = pile_a->top;
    pile_a->top = p;
    (pile_a->num)++;
    return 1;
}

void pop_tb(BinTree *val, pilet* pile_a)
{
    if(pile_a->num > 0)
    {
        pile_tn *temp = pile_a->top;
        *val = temp->valeur;
        pile_a->top = pile_a->top->next;
        (pile_a->num)--;
        free(temp);
        //return 1;
    }
    else
    {
        //printf("Pile vide !\n");
        exit(1);
    }
}

int pop_tval(BinTree* num_p, pilet* pile_a)
{
    if(pile_a->num > 0)
    {
        *num_p = pile_a->top->valeur;
        return 1;
    }
    else return 0;
}

int pile_tvide(pilet* pile_a)
{
    return (pile_a->num == 0);
}

void pile_tclear(pilet* pile_a)
{
    while(pile_a->num>0)
    {
        pile_tn *temp = pile_a->top;
        pile_a->top = temp->next;
        (pile_a->num)--;
        free(temp);
    }
}

/******PILE_INT[2]******/
typedef struct pile_in pile_in;
struct pile_in
{
    int valeur[2];
    pile_in *next;
};

typedef struct pilei pilei;
struct pilei
{
    pile_in *top;
    int num;//num de valeur dans le pile
};

pilei* pile_inew()
{
    pilei *p = (pilei*)malloc(sizeof(pilei));
    p->top = NULL;
    p->num = 0;
    return p;
}

int push_ib(int num_p[2], pilei* pile_a)
{
    pile_in *p = (pile_in*)malloc(sizeof(pile_in));
    p->valeur[0] = num_p[0];
    p->valeur[1] = num_p[1];
    p->next = pile_a->top;
    pile_a->top = p;
    (pile_a->num)++;
    return 1;
}

int pop_ib(int num_p[2], pilei* pile_a)
{
    if(pile_a->num > 0)
    {
        pile_in *temp = pile_a->top;
        num_p[0] = temp->valeur[0];
        num_p[1] = temp->valeur[1];
        pile_a->top = pile_a->top->next;
        (pile_a->num)--;
        free(temp);
        return 1;
    }
    else return 0;
}


void pile_iclear(pilei* pile_a)
{
    while(pile_a->num>0)
    {
        pile_in *temp = pile_a->top;
        pile_a->top = temp->next;
        (pile_a->num)--;
        free(temp);
    }
}

