#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.c"

/********DECLARATION********/
void add_nombre(void);
void add_divers(int type);
void error(int e, char c);
void put_res();
int analyse_syntaxique(liste llex);

char s[100];
int i=0, j=0, etat_courant=0;//i:l'indice de parcours de s, j:le nbr de chiffres lus
liste resultat = NULL;
lexeme lu;
pilec *Pile;//put parenthese
char sortir;//temp variable

BinTree etree;
pilet *NodeStack;
pilec *OpStack;

/********FUNCTION********/
int comp_operator(char ope1, char ope2)//operator order is same(true) or not(false)
{
    if (ope1==ope2 || (ope1=='x'&&ope2=='/') || (ope1=='/'&&ope2=='x') || (ope1=='+'&&ope2=='-') || (ope1=='-'&&ope2=='+'))
        return 1;
    else
        return 0;
}

int order_operator(char ope1, char ope2)//ensure the order of the operator(ope1 prior or the same return ture)
{
    if (ope1 == '(')
        return 0;
    else if (ope2 == '(')
        return 0;
    else if (ope2 == ')')
        return 1;
    else if (comp_operator(ope1, ope2))
        return 0;
    else if ((ope1 == 'x') || (ope1 == '/'))
        return 1;
    else if ((ope2 == 'x') || (ope2 == '/'))
        return 0;
    else if ((ope1 == '+') || (ope1 == '-'))
        return 1;
    else
        return 1;
}

void add_nombre(void)
{
    if (j!=0)
    {
        lu.type = NOMBRE;
        strncpy(lu.valeur, s+i-j, j);
        lu.valeur[j]=0;
        add_fin(lu, &resultat);
        //printf("[success]%c!\n",resultat->fin->tete.valeur[0]);
        //printf("[success]add_nombre!\n");
    }
}

void add_divers(int type)
{
    add_nombre();
    j=0;
    lu.type = type;
    strncpy(lu.valeur, s+i, 1);
    lu.valeur[1]=0;
    add_fin(lu, &resultat);
}

void put_res()
{
    do
        switch(s[i])
        {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                j++;
                //printf("[success]chiffre!\n");
                break;
            case '+': case '-':
                add_divers(SIGNE_plus_moins);
                //printf("[success]+-!\n");
                break;
            case 'x': case '/':
                add_divers(SIGNE_mul_div);
                //printf("[success]*/!\n");
                break;
            case '(':
                add_divers(Par_ou);
                //printf("[success]Par_ou!\n");
                break;
            case ')':
                add_divers(Par_fe);
                //printf("[success]Par_fe!\n");
                break;
            default:
                printf("[fail]Erreur lexicale !\n");
                return;
        }
    while (s[++i] != 0);
    add_nombre();
    //printf("ok\n");
}

void error(int e, char c)
{
    printf("[fail]Erreur syntaxique a l'etat %d, caractere %d\n", e, c);
    return;
}

void build_i(int cour_e, char* val)
{
    if (cour_e == 1 || cour_e == 5 || cour_e == 7)//NOMBRE
    {
        BinTree tempstr;
        tempstr.root = build_node(val);
        push_tb(tempstr, NodeStack);
        //BinTree temp;
        //pop_tval(&temp,NodeStack);
        //int i,a,b;
        //a=strlen(temp.root->valeur);
        //printf("num: ");
        //for(i=0;i<a;i++){printf("%c",temp.root->valeur[i]);}
        //printf("\n");

    }
    else if (cour_e == 2 || cour_e == 6)
    {
        BinTree tempstr;
        tempstr.root = build_node("0");
        push_tb(tempstr, NodeStack);
        push_cb(val[0],OpStack);
    }
    else if (cour_e == 3)
    {
        push_cb(val[0],OpStack);
    }
    else if (cour_e == 4)
    {
        if(OpStack->num==0)
        {
            //printf("push\n");
            push_cb(val[0],OpStack);
        }
        else if(pop_cval(OpStack)=='(')
        {
            //printf("push\n");
            push_cb(val[0],OpStack);
        }
        else if(order_operator(val[0],pop_cval(OpStack)))
        {
            //printf("push\n");
            push_cb(val[0],OpStack);
        }
        else
        {
            while((OpStack->num!=0)&&(order_operator(pop_cval(OpStack),val[0])||comp_operator(pop_cval(OpStack),val[0])))
            {
                //printf("pop\n");
                BinTree temptree;
                char temp_c[0];
                BinTree temp_t;
                pop_cb(temp_c,OpStack);
                etree.root=build_node(temp_c);
                pop_tb(&temp_t,NodeStack);
                temptree.root=temp_t.root;
                etree.root->right_child=temptree.root;
                temptree.root=temp_t.root=NULL;
                pop_tb(&temp_t,NodeStack);
                temptree.root=temp_t.root;
                etree.root->left_child=temptree.root;
                temptree.root=temp_t.root=NULL;
                temptree.root=etree.root;
                push_tb(temptree,NodeStack);
                etree.root=NULL;
            }
            push_cb(val[0],OpStack);
        }
    }
    else if(cour_e==8)
    {
        while(pop_cval(OpStack)!='(')
        {
            BinTree temptree;
            char temp_c[0];
            BinTree temp_t;
            pop_cb(temp_c,OpStack);
            etree.root=build_node(temp_c);
            pop_tb(&temp_t,NodeStack);
            temptree.root=temp_t.root;
            etree.root->right_child=temptree.root;
            temptree.root=temp_t.root=NULL;
            pop_tb(&temp_t,NodeStack);
            temptree.root=temp_t.root;
            etree.root->left_child=temptree.root;
            temptree.root=temp_t.root=NULL;
            temptree.root=etree.root;
            push_tb(temptree,NodeStack);
        }
        char temp_c[0];
        pop_cb(temp_c,OpStack);
    }
}

void build()
{
    while (OpStack->num!=0)
    {
        BinTree temptree;
        char temp_c[0];
        BinTree temp_t;
        pop_cb(temp_c,OpStack);
        etree.root=build_node(temp_c);
        pop_tb(&temp_t,NodeStack);
        temptree.root=temp_t.root;
        etree.root->right_child=temptree.root;
        temptree.root=temp_t.root=NULL;
        pop_tb(&temp_t,NodeStack);
        temptree.root=temp_t.root;
        etree.root->left_child=temptree.root;
        temptree.root=temp_t.root=NULL;
        temptree.root=etree.root;
        push_tb(temptree,NodeStack);
        if(OpStack->num!=0)etree.root=NULL;
    }
}

/*********¼ÆËã********/
int cal(BinNode* node)
{

    if(node->left_child!=NULL && node->right_child!=NULL)
    {
        //printf("[success]\n");
        cal(node->left_child);
        cal(node->right_child);
        int e1=atoi(node->left_child->valeur);
        //printf("[test]e1 : %d\n",e1);
        int e2=atoi(node->right_child->valeur);
        //printf("[test]e2 : %d\n",e2);
        int s;
        if (node->valeur[0]=='+'){s=e1+e2;}
        else if(node->valeur[0]=='-'){s=e1-e2;}
        else if(node->valeur[0]=='x'){s=e1*e2;}
        else if(node->valeur[0]=='/')
        {
            if(e2!=0){s=e1/e2;}
            else {printf("[Error]Denominateur ne peut pas etre 0 !\n");return 0;}
        }
        //printf("[test]s : %d\n",s);
        int k=s;
        int i=0,a;
        for(a=0;a<20;a++)
        {
            node->valeur[a]='\0';
        }
        while(k>0)
        {
            k=k/10;
            i++;
        }
        sprintf(node->valeur,"%d",s);

        node->left_child=NULL;
        node->right_child=NULL;
        //printf("root: %s\n",node->valeur);
        return s;
    }
    else return 0;

}

int analyse_syntaxique(liste llex)
{
    char c;
    if (llex != NULL)
    {
        //printf("enter\n");
        switch (etat_courant)
        {
            case 0:
                //printf("enter0\n");
                switch(c = llex->tete.type)
                {
                    case NOMBRE:
                        etat_courant = 1;
                        break;
                    case SIGNE_plus_moins:
                        etat_courant = 2;
                        break;
                    case Par_ou:
                        etat_courant = 3;
                        push_cb('P',Pile);
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 1:
                //printf("enter1\n");
                switch(c = llex->tete.type)
                {
                    case SIGNE_mul_div: case SIGNE_plus_moins:
                        etat_courant = 4;
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 2:
                //printf("enter2\n");
                switch(c = llex->tete.type)
                {
                    case NOMBRE:
                        etat_courant = 7;
                        break;
                    case Par_ou:
                        etat_courant = 3;
                        push_cb('P',Pile);
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 3:
                //printf("enter3\n");
                switch(c = llex->tete.type)
                {
                    case NOMBRE:
                        etat_courant = 5;
                        break;
                    case Par_ou:
                        push_cb('P',Pile);
                        break;
                    case SIGNE_plus_moins:
                        etat_courant = 6;
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 4:
                //printf("enter4\n");
                switch(c = llex->tete.type)
                {
                    case Par_ou:
                        etat_courant = 3;
                        push_cb('P',Pile);
                        break;
                    case NOMBRE:
                        etat_courant = 7;
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 5:
                //printf("enter5\n");
                switch(c = llex->tete.type)
                {
                    case SIGNE_mul_div: case SIGNE_plus_moins:
                        etat_courant = 4;
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 6:
                //printf("enter6\n");
                switch(c = llex->tete.type)
                {
                    case NOMBRE:
                        etat_courant = 7;
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 7:
                //printf("enter7\n");
                switch(c = llex->tete.type)
                {
                    case SIGNE_mul_div: case SIGNE_plus_moins:
                        etat_courant = 4;
                        break;
                    case Par_fe:
                        etat_courant = 8;
                        pop_cb(&sortir,Pile);
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            case 8:
                //printf("enter8\n");
                switch(c = llex->tete.type)
                {
                    case SIGNE_mul_div: case SIGNE_plus_moins:
                        etat_courant = 4;
                        break;
                    case Par_fe:
                        pop_cb(&sortir,Pile);
                        break;
                    default:
                        error(etat_courant, c);
                };break;
            default:
                printf("[fail]Analyse syntaxique est faux !\n");
        }
        build_i(etat_courant,llex->tete.valeur);
        analyse_syntaxique(llex->queue);
    }

    return((etat_courant == 1)||(etat_courant == 7)||(etat_courant == 8));
}


int calculate(int *v,char* in)
{
    i=0, j=0, etat_courant=0;
    resultat = NULL;
    int t;
    for(t=0;t<100;t++){s[t]='\0';}
    for(t=0;t<100;t++){s[t]=in[t];}
    Pile = pilec_new();
    etree = BinTree_new();
    NodeStack = pilet_new();
    OpStack = pilec_new();

    put_res();
    int res=analyse_syntaxique(resultat);
    if(!(res&&pile_vide(Pile)))
    {
        printf("[fail]La syntaxe de l'expression %s est incorrecte !\n",s);
    }
    //printf("La syntaxe de l'expression %s est %s !\n",s,(res&&pile_vide(Pile))?"Correcte":"incorrecte");
    //pop_tb(&etree,NodeStack);
    //printf("test\n");


    int ss;
    if(res&&pile_vide(Pile))
    {
        build();
        ss=cal(etree.root);
        //printf("Resultat : %d\n",ss);
        *v=ss;
    }

    pile_cclear(Pile);
    etree.root=NULL;
    etree.num=0;
    pile_tclear(NodeStack);
    pile_cclear(OpStack);

    return res&&pile_vide(Pile);
}
