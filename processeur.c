#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#include "calculatrice.c"

#define LAC_LENGTH 400
#define MV_LENGTH 200
#define BASE_LENGTH 20
#define STR_LENGTH 3000
#define INT 1
#define ADDR 2


/******PROCESSEUR******/
typedef void (*base)(void);

/********DECLARATION********/
int LAC[LAC_LENGTH];//Table des symbole de LAC
int MV[MV_LENGTH];//Table de MV
char str[STR_LENGTH];//Array for string
int LAC_pos;
int LAC_fun_pos;//la position de la function
int MV_pos;
int MV_pos_create;
int MV_base_maxpos;
int create_func;//judge whether the function is creating
int if_num,else_num, if_count;
int COND;

char func_nom[20];
int chaine[10000];
int chaine_e,chaine_s,chaine_pos;
int chaine_etat;
//int func_nom_length;
base processeur[BASE_LENGTH];
pile *data, *type, *retour;
pile *type_func_out,*type_func_in;
pile *func_if, *func_else, *func_judge;
pile *if_in, *if_out;

void declare_base(char *func_nom, int input_num, int* input_type, int output_num, int* output_type, int base_pos);
void initial();//ajouter initialement la fonction au table du LAC et MV
void plus();
void moin();
void fois();
void point();
void lit();
void fin();
void dup();
void drop();
void swap();
void count();
void comp();
void TYPE();
void egal();
void calculator();
void cr();
int fonc_judge(char *in_info, int in_length);
int num_judge(char *type_info);
int func_run(int i_pos);
void n_run(char* input_info);
void run(char* input_util);
int quit(char* input_util);

/********FONCTION********/
void declare_base(char *func_nom, int input_num, int* input_type, int output_num, int* output_type, int base_pos)
{
    int temp_pos;
    //if(LAC_pos==0){LAC_pos++;}
    if(LAC_fun_pos!=0)
    {
        LAC[LAC_pos]=LAC_fun_pos;
    }
    LAC_pos++;
    LAC_fun_pos=LAC_pos;
    LAC[LAC_pos++]=strlen(func_nom);
    for(temp_pos=0;temp_pos<strlen(func_nom);temp_pos++)
    {
        LAC[LAC_pos++]=(int)(func_nom[temp_pos]);
    }
    LAC[LAC_pos++]=input_num;
    temp_pos=0;
    for(temp_pos=0;temp_pos<input_num;temp_pos++)
    {
        LAC[LAC_pos++]=input_type[temp_pos];
    }
    LAC[LAC_pos++]=output_num;
    for(temp_pos=0;temp_pos<output_num;temp_pos++)
    {
        LAC[LAC_pos++]=output_type[temp_pos];
    }
    LAC[LAC_pos++]=MV_pos;
    if(MV[MV_pos]!=1)
    {
        MV[MV_pos++]=0;
        MV[MV_pos++]=base_pos;
        MV_base_maxpos=MV_pos;

    }
    MV_pos_create=MV_pos;
}

void initial()
{
    //void declare_base(char *nom, int input_num, int* input_type, int output_num, int* output_type, int base_pos);
    //int i;
    int input_t[2];
    int output_t[2];
    /*addition*/
    input_t[0]=INT;input_t[1]=INT;output_t[0]=INT;output_t[1]=INT;
    declare_base("+",2,input_t,1,output_t,0);processeur[0]=&plus;
    /*soustraction*/
    input_t[0]=INT;input_t[1]=INT;output_t[0]=INT;output_t[1]=INT;
    declare_base("-",2,input_t,1,output_t,1);processeur[1]=&moin;
    /*multiplication*/
    input_t[0]=INT;input_t[1]=INT;output_t[0]=INT;output_t[1]=INT;
    declare_base("*",2,input_t,1,output_t,2);processeur[2]=&fois;
    /*print*/
    input_t[0]=INT;input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base(".",1,input_t,0,output_t,4);processeur[4]=&point;
    /*lit*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("lit",0,input_t,0,output_t,5);//processeur[4]=&point;
    /*fin*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("fin",0,input_t,0,output_t,6);//processeur[4]=&point;

    /*dup*/
    input_t[0]=INT;input_t[1]='\0';output_t[0]=INT;output_t[1]=INT;
    declare_base("dup",1,input_t,2,output_t,7);processeur[7]=&dup;
    /*drop*/
    input_t[0]=INT;input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("drop",1,input_t,0,output_t,8);processeur[8]=&drop;
    /*swap*/
    input_t[0]=INT;input_t[1]=INT;output_t[0]=INT;output_t[1]=INT;
    declare_base("swap",2,input_t,2,output_t,9);processeur[9]=&swap;
    /*count*/
    input_t[0]=ADDR;input_t[1]='\0';output_t[0]=ADDR;output_t[1]=INT;
    declare_base("count",1,input_t,2,output_t,10);processeur[10]=&count;
    /*type*/
    input_t[0]=INT;input_t[1]=ADDR;output_t[0]='\0';output_t[1]='\0';
    declare_base("type",2,input_t,0,output_t,11);processeur[11]=&TYPE;
    /*comparaison*/
    input_t[0]=INT;input_t[1]=INT;output_t[0]=INT;output_t[1]='\0';
    declare_base("=",2,input_t,1,output_t,12);processeur[12]=&comp;
    /*str*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("str",0,input_t,0,output_t,13);
    /*calculator*/
    input_t[0]=ADDR;input_t[1]='\0';output_t[0]=INT;output_t[1]='\0';
    declare_base("calculate",1,input_t,1,output_t,14);processeur[14]=&calculator;
    /*if*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("if",0,input_t,0,output_t,15);
    /*else*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("else",0,input_t,0,output_t,16);
    /*then*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("then",0,input_t,0,output_t,17);
    /*recurse*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("recurse",0,input_t,0,output_t,18);
    /*cr*/
    input_t[0]='\0';input_t[1]='\0';output_t[0]='\0';output_t[1]='\0';
    declare_base("cr",0,input_t,0,output_t,19);processeur[19]=&cr;

    printf("LAC: ");
    for(i=0;i<LAC_pos;i++) printf("%d ",LAC[i]);
    printf("\n");
    printf("MV: ");
    for(i=0;i<MV_pos;i++) printf("%d ",MV[i]);
    printf("\n");
}

void plus()
{
    int e1,e2,s;
    pop_b(&e1,data);
    pop_b(&e2,data);
    //printf("[plus] num1 is %d\n",e2);
    //printf("[plus] num2 is %d\n",e1);
    s=e1+e2;
    push_b(s,data);
    //printf("[plus] push %d\n",s);
}

void moin()
{
    int e1,e2,s;
    pop_b(&e1,data);
    pop_b(&e2,data);
    //printf("[moin] num1 is %d\n",e2);
    //printf("[moin] num2 is %d\n",e1);
    s=e2-e1;
    push_b(s,data);
    //printf("[moin] push %d\n",s);
}

void fois()
{
    int e1,e2,s;
    pop_b(&e1,data);
    pop_b(&e2,data);
    //printf("[fois] num1 is %d\n",e2);
    //printf("[fois] num2 is %d\n",e1);
    s=e1*e2;
    push_b(s,data);
    //printf("[fois] push %d\n",s);
}

void point()
{
    int s;
    pop_b(&s,data);
    //printf("[print] num is %d\n",s);
    //printf("PRINT: %d\n",s);
    printf("%d ",s);
}

void dup()
{
    int e;
    pop_b(&e,data);
    //printf("[dup] num is %d\n",e);
    push_b(e,data);
    push_b(e,data);
    //printf("[dup] push_1 %d\n",e);
    //printf("[dup] push_2 %d\n",e);
}

void drop()
{
    int e;
    pop_b(&e,data);
    //printf("[drop] num is %d\n",e);
}

void swap()
{
    int e1,e2;
    pop_b(&e1,data);
    //printf("[swap] pop_num1 is %d\n",e1);
    pop_b(&e2,data);
    //printf("[swap] pop_num2 is %d\n",e2);
    push_b(e1,data);
    push_b(e2,data);
    //printf("[swap] push_num1 is %d\n",e1);
    //printf("[swap] push_num2 is %d\n",e2);
}

void count()
{
    int ad1,ad2,s;
    pop_b(&ad1,data);
    //printf("[count] address out is %d\n",ad1);
    int i;

    for(i=0;i<=chaine[ad1];i++)
    {
        chaine[chaine_s++]=chaine[ad1+i];
    }
    ad2=chaine_e;
    push_b(ad2,data);
    chaine_e=chaine_s;
    s=chaine[ad1];
    //printf("[count] address in is %d\n",ad2);
    push_b(s,data);
    //printf("[count] num of this string is %d\n",s);
}

void TYPE()
{
    int ad,s;
    pop_b(&s,data);
    pop_b(&ad,data);
    //printf("[type] number of type is %d\n",s);
    //printf("[type] address out is %d\n",ad);
    if(s>chaine[ad]){printf("[fail]number of type is too big !\n");push_b(ad,data);push_b(s,data);}
    else
    {
        //printf("PRINT: ");
        int i;
        for(i=0;i<s;i++)
        {
            printf("%c",chaine[ad+i+1]);
        }
        //printf("\n");
    }


}

void comp()
{
    int e1,e2,s;
    pop_b(&e1,data);
    pop_b(&e2,data);
    //printf("[compare] num1 is %d\n",e2);
    //printf("[compare] num2 is %d\n",e1);
    if(e1==e2){s=1;}
    else s=0;
    push_b(s,data);
    //printf("[compare] push %d\n",s);
}

void calculator()
{
    int ad,s;
    pop_b(&ad,data);
    int l;
    char in[100];
    //printf("[test] ");
    for(l=0;l<chaine[ad];l++)
    {
        in[l]=chaine[ad+l+1];
        //printf("%c ",in[l]);
    }
    //printf("\n");
    for(l=chaine[ad];l<100;l++)
    {
        in[l]='\0';
    }

    if(calculate(&s,in))
    {
        push_b(s,data);
        //printf("[calculator] push %d\n",s);
    }
    //else{COND=1;}

    for(l=0;l<100;l++)
    {
        in[l]='\0';
    }
}

void cr()
{
    //printf("Print: \n");
    printf("\n");
}

int fonc_judge(char *in_info, int in_length)
{
    int in_pos=LAC_fun_pos;//fonction pos of LAC
    //printf("[success] The temp pos of LAC is %d!\n",in_pos);
    int i_num=0;//pos of in_info[]
    while(in_pos!=0)
    {
        int judge=1;
        if(in_length==LAC[in_pos])
        {
            for(i_num=0;i_num<in_length;i_num++)
            {
                if(in_info[i_num]!=LAC[in_pos+i_num+1]){judge=0;break;}
            }
        }
        else {judge=0;}
        if (judge==1){break;}
        else {in_pos=LAC[in_pos-1];/*printf("[success] The temp pos of LAC is %d!\n",in_pos);*/}
    }
    if(in_pos!=0){/*printf("[success] find the function in LAC_POS %d!\n",in_pos);*/return in_pos;}
    else {/*printf("[success] Not the function in LAC\n")*/return 0;}
}

int num_judge(char *type_info)
{
    int j=1;
    if(strlen(type_info)==1)
    {
        if(isdigit(type_info[0])==0)j=0;
    }
    else if(strlen(type_info)>1)
    {
        if(type_info[0]=='-'||type_info[0]=='+')
        {
            int i;
            for(i=1;i<strlen(type_info);i++)
            {
                if(isdigit(type_info[i])==0){j=0;}
            }
        }
        else
        {
            int i;
            for(i=0;i<strlen(type_info);i++)
            {
                if(isdigit(type_info[i])==0){j=0;}
            }
        }
    }
    if(j==1){return INT;}
    else return 0;
}

int func_run(int i_pos)
{
    if(MV[i_pos+1]==18){printf("[fail]no recurse directly !\n");return 0;}
    int i=i_pos;
    //printf("[success] function of MV_POS is %d!\n",i);
    if(MV[i]==0){processeur[MV[i+1]]();/*printf("[success] use the function in cfa\n");*/}
    else if(MV[i]==1)
    {
        i++;
        //printf("[success]\n");
        /*printf("MV_base_maxpos: %d\n",MV_base_maxpos);*/
        while(MV[i]!=6)//not find "fin"
        {

            //printf("[success]0\n");
            if(MV[i]==5){i++;push_b(MV[i++],data);/*printf("[success] push the num %d!\n",MV[i-1]);*/}
            else if(MV[i]==13){i++;push_b(MV[i++],data);/*printf("[success] push the addr %d!\n",MV[i-1]);*/}
            else if(MV[i]==15)
            {
                int jud;
                pop_b(&jud,data);
                if(jud==1){i=i+2;}
                else if(jud == 0)
                {
                    i=MV[i+1]+1;
                    if(MV[i-2]!=16){int temp; pop_b(&temp,type);}

                }
                else {printf("[fail]The condition is faux !\n");return 0;}
            }
            else if(MV[i]==16)
            {
                i=MV[i+1]+1;

            }
            else if(MV[i]==18)
            {
                func_run(i_pos);i++;

            }
            else if(MV[i]<MV_base_maxpos){processeur[MV[i++]]();/*printf("[success] function of MV_POS is %d!\n",i-1);*/}
            else if(MV[i]>=MV_base_maxpos){func_run(MV[i]);i++;}
            //else if(MV[i]==6){printf("[success]4\n");return;}
        }

    }
    return 1;
}


void n_run(char* input_info)//return the position of input
{
    //printf("[success] Enter the creation function1: %d\n",MV_pos_create);
    char info[20];
    int i_num=0;//position of input_info
    if(create_func==0&&input_info[0]==':')
    {
        //printf("[success] Enter the creation function2\n");
        int t;
        for(t=0;t<20;t++){func_nom[t]='\0';}
        if(input_info[1]!=' '){printf("[fail] invalid way to define a function !\n");COND=1;return;}
        else
        {
            i_num=2;
            //printf("[success] read func_name1\n");
            while (input_info[i_num]==' '){i_num++;}
            //printf("[success] read func_name2\n");
            int i=i_num;
            if(input_info[i_num]==';'){printf("[fail] invalid function name !\n");COND=1;return;}
            while (input_info[i_num]!=' '&&input_info[i_num]!='\0'){func_nom[i_num-i]=input_info[i_num];i_num++;}
            /*printf("[success] read func_name: %s\n",func_nom);*/
            if (fonc_judge(func_nom,i_num-i)!=0){printf("[fail] function is in the LAC !\n");COND=1;return;};//judge whether the function exist
            if (num_judge(func_nom)!=0){printf("[fail] function name cannot be an integer !\n");COND=1;return;}



        }
        create_func=1;
        pile_clear(type_func_in);
        pile_clear(type_func_out);
        pile_clear(if_in);
        pile_clear(if_out);
        MV_pos_create=MV_pos;
        MV[MV_pos_create++]=1;
        if_num=0;else_num=0;
        if_count=0;
    }
    while (i_num<100)
    {
        //printf("[success] Enter the creation function: %d\n",MV_pos_create);
        while(input_info[i_num]==' '){i_num++;}
        if(input_info[i_num]=='\0'){break;}
        int i1;
        int i=0;//use in info[]
        for(i1=0;i1<20;i1++){info[i1]='\0';}


        if((input_info[i_num]=='"'&&input_info[i_num+1]==' '&&chaine_etat==0)||chaine_etat==1)
        {
            if(chaine_etat==0)i_num++;
            chaine_etat=1;
            int temp_inum;
            for(temp_inum=i_num;temp_inum<strlen(input_info);temp_inum++)
            {
                if(input_info[temp_inum]=='"'){break;}
                chaine[chaine_s++]=input_info[temp_inum];
                //printf("%d ",chaine[chaine_s-1]);
            }
            i_num=temp_inum;
        }
        if(chaine_etat==1)
        {
            if(input_info[i_num]=='"')
            {
                chaine[chaine_e]=chaine_s-chaine_e-1;
                //printf("\n%d %d \n",chaine_e,chaine[chaine_e]);
                chaine_pos=chaine_e;
                //printf("pos %d\n",chaine_pos);
                //push_b(chaine_pos,data);
                //push_b(ADDR,type);
                MV[MV_pos_create++]=13;//lit dans le base
                MV[MV_pos_create++]=chaine_pos;
                if(else_num==0){push_b(ADDR,type_func_out);}
                if(if_num==0){push_b(ADDR,if_out);}


                chaine_e=chaine_s;
                chaine_etat=0;
                i_num++;
            }
        }

        while(input_info[i_num]==' '){i_num++;}
        if(input_info[i_num]=='\0'){break;}

        while(input_info[i_num]!=' '&&input_info[i_num]!='\0')
        {
            info[i]=input_info[i_num];
            i++;i_num++;
        }
        for(i1=i+1;i1<20;i1++){info[i1]='\0';}
        //printf("[success] read %s while creating the function\n",info);

        int in_pos=fonc_judge(info,i);//position of the function
        if (in_pos!=0)
        {
            //printf("[success] Enter the creation function: %d\n",MV_pos_create);
            int func_pos;//temp position which we use the function in LAC
            func_pos=in_pos+1+i;
            int in_num=LAC[func_pos++];//num of input
            //printf("[success] num of input is %d\n",in_num);
            int input_i;
            for (input_i=0;input_i<in_num;input_i++)
            {
                if(type_func_out->num>0)
                {
                    if(else_num==0)
                    {
                        int tp;
                        pop_b(&tp,type_func_out);
                        if(tp!=LAC[func_pos]){printf("[fail] faux input type\n");create_func=0;COND=1;return;}
                    }


                    if(if_num==0)
                    {
                        if(if_out->num>0)
                        {
                            int ti;
                            pop_b(&ti,if_out);
                            if(ti!=LAC[func_pos]){printf("[fail] faux input type\n");create_func=0;COND=1;return;}
                        }
                        else
                        {
                            push_b(LAC[func_pos],if_in);
                        }
                    }

                    func_pos++;
                }
                else
                {
                    if(else_num==0)
                    {
                        push_b(LAC[func_pos],type_func_in);
                    }

                    if(if_num==0)
                    {
                        if(if_out->num>0)
                        {
                            int ti;
                            pop_b(&ti,if_out);
                            if(ti!=LAC[func_pos]){printf("[fail] faux input type\n");create_func=0;COND=1;return;}
                        }
                        else
                        {
                            push_b(LAC[func_pos],if_in);
                        }
                    }

                    func_pos++;
                }
            }



            int out_num=LAC[func_pos++];//num of output
            //printf("[success] num of output is %d\n",out_num);
            if(out_num>0)
            {
                //int output_t[out_num];
                int output_i;
                for (output_i=0;output_i<out_num;output_i++)
                {
                    //printf("[success] push the data_%d type %d!\n",output_i,LAC[func_pos]);
                    if(else_num==0)
                    {
                        push_b(LAC[func_pos],type_func_out);
                    }

                    if(if_num==0)
                    {
                        push_b(LAC[func_pos],if_out);
                    }

                    func_pos++;
                }
            }
            if(MV[LAC[func_pos]]==0)
            {
                MV[MV_pos_create++]=MV[LAC[func_pos++]+1];
                /***if***/
                if(MV[MV_pos_create-1]==15)
                {
                    int if_pos=MV_pos_create++;
                    push_b(if_pos,func_if);
                    if_num=1;
                    if_count++;
                }
                else if(MV[MV_pos_create-1]==16)
                {
                    int if_temp_pos;
                    if(func_if->num>0){pop_b(&if_temp_pos,func_if);}
                    else {printf("[fail]Lack of if !\n");create_func=0;COND=1;return;}
                    int else_pos=MV_pos_create++;
                    push_b(else_pos,func_else);
                    MV[if_temp_pos]=else_pos;
                    if_num=0;
                    else_num=1;

                }
                else if(MV[MV_pos_create-1]==17)
                {
                    int then_pos=MV_pos_create-2;
                    int if_temp_pos, else_temp_pos;
                    if(func_else->num>0){pop_b(&else_temp_pos,func_else);MV[else_temp_pos]=then_pos;}
                    else if(func_if->num>0){pop_b(&if_temp_pos,func_if);MV[if_temp_pos]=then_pos;}
                    else {printf("[fail]Lack of if !\n");create_func=0;COND=1;return;}
                    MV_pos_create=then_pos+1;
                    if_num=0;
                    else_num=0;
                }

            }
            else if(MV[LAC[func_pos]]==1){MV[MV_pos_create++]=LAC[func_pos++];}
            //printf("MV_POS: %d\n",MV_pos);

        }
        else if(num_judge(info)!=0)
        {
            //printf("[success] Enter the creation function5\n");
            int nbr=atoi(info);
            //printf("[success] add the num %d in MV!\n",nbr);
            MV[MV_pos_create++]=5;//lit dans le base
            MV[MV_pos_create++]=nbr;
            if(else_num==0)
            {
                push_b(INT,type_func_out);//printf("if");
            }

            if(if_num==0){push_b(INT,if_out);/*printf("else");*/}

            //printf("MV_POS: %d\n",MV_pos);
        }
        else if(info[0]==';')
        {
            //printf("[success] Enter the creation function6\n");
            if(func_if->num!=0 || func_else->num!=0){printf("[fail]Lack of then !\n");create_func=0;COND=1;return;}
            if((if_in->num!=type_func_in->num) || (if_out->num!=type_func_out->num))
            {
                printf("[fail]The input or output of true and false is not the same !\n");
                create_func=0;
                pile_clear(if_in);pile_clear(if_out);
                pile_clear(type_func_in);pile_clear(type_func_out);
                COND=1;
                return;
            }
            int input_type[20],output_type[20];
            int temp[20];
            int inputnum,outputnum;
            inputnum=type_func_in->num;
            outputnum=type_func_out->num-if_count;
            int i_temp;
            for(i_temp=0;i_temp<inputnum;i_temp++)
            {
                int atemp;
                pop_b(&atemp,type_func_in);
                temp[i_temp]=atemp;
            }
            for(i_temp=0;i_temp<inputnum;i_temp++)
            {
                input_type[i_temp]=temp[inputnum-1-i_temp];
            }

            for(i_temp=0;i_temp<outputnum;i_temp++)
            {
                int atemp;
                pop_b(&atemp,type_func_out);
                temp[i_temp]=atemp;
            }
            for(i_temp=0;i_temp<outputnum;i_temp++)
            {
                output_type[i_temp]=temp[outputnum-1-i_temp];
            }
            MV[MV_pos_create++]=6;
            int MV_pos_t=MV_pos_create;
            declare_base(func_nom,inputnum,input_type,outputnum,output_type,-1);
            MV_pos=MV_pos_t;
            create_func=0;
            //printf("MV_POS: %d\n",MV_pos);
            if_num=0;else_num=0;if_count=0;
            pile_clear(if_in);pile_clear(if_out);
            pile_clear(type_func_in);pile_clear(type_func_out);
            break;
        }
        else {printf("[fail]Invalid input\n");create_func=0;COND=1;break;}


    }
}

void run(char* input_util)
{
    int i_num=0;
    //int l_func_num;//current function of lac
    //int l_num;//current position of lac
    char info[20];
    if((input_util[0]==':'&&create_func==0)||create_func==1){n_run(input_util);}
    else if(input_util[0]!=':'&&input_util[0]!='\0')
    {
        while(i_num<100)
        {
            //printf("pos %d\n",chaine_pos);
            //printf("e %d\n",chaine_e);
            //printf("s %d\n",chaine_s);
            while(input_util[i_num]==' '&&chaine_etat==0){i_num++;}
            if(input_util[i_num]=='\0'){break;}

            if((input_util[i_num]=='"'&&input_util[i_num+1]==' '&&chaine_etat==0)||chaine_etat==1)
            {
                if(chaine_etat==0)i_num++;
                chaine_etat=1;
                int temp_inum;
                for(temp_inum=i_num;temp_inum<strlen(input_util);temp_inum++)
                {
                    if(input_util[temp_inum]=='"'){break;}
                    chaine[chaine_s++]=input_util[temp_inum];
                    //printf("%d ",chaine[chaine_s-1]);
                }
                i_num=temp_inum;
            }
            if(chaine_etat==1)
            {
                if(input_util[i_num]=='"')
                {
                    chaine[chaine_e]=chaine_s-chaine_e-1;
                    //printf("\n%d %d \n",chaine_e,chaine[chaine_e]);
                    chaine_pos=chaine_e;
                    //rintf("pos %d\n",chaine_pos);
                    push_b(chaine_pos,data);
                    push_b(ADDR,type);
                    chaine_e=chaine_s;
                    chaine_etat=0;
                    i_num++;
                }
            }

            while(input_util[i_num]==' '){i_num++;}
            if(input_util[i_num]=='\0'){break;}

            if(chaine_etat==0){
                int i1;
                int i=0;//use in info[]
                for(i1=0;i1<20;i1++){info[i1]='\0';}
                while(input_util[i_num]!=' '&&input_util[i_num]!='\0')
                {
                    info[i]=input_util[i_num];
                    i++;i_num++;
                }

                for(i1=i+1;i1<20;i1++){info[i1]='\0';}
                //printf("[success] read %s\n",info);

                int in_pos=fonc_judge(info,i);//position of the function
                if (in_pos!=0)
                {
                    /*******input test********/
                    int judge_func=1;//judge the condition of the function is correct
                    int func_pos;//temp position which we use the function in LAC
                    func_pos=in_pos+1+i;
                    int in_num=LAC[func_pos++];//num of input
                    //printf("[success] num of input is %d\n",in_num);
                    if(data->num<in_num){judge_func=0;printf("[fail]Manquer des donnees!\n");COND=1;break;}
                    else
                    {
                        //printf("[success] les donnees sont suffisantes!\n");
                        int input_t[in_num];
                        int input_i;
                        for (input_i=0;input_i<in_num;input_i++)
                        {
                            //int te;
                            //pop_b(&te, type);
                            //printf("leixing1 %d\n",te);
                            pop_b(&input_t[input_i],type);
                            //printf("[success] Le type de donnee_%d est %d!\n",input_i,input_t[input_i]);
                            if (input_t[input_i]!=LAC[func_pos++]){judge_func=0;printf("[fail]Le type de donnee est faux!\n");COND=1;break;}
                        }
                    }
                    int out_num=LAC[func_pos++];//num of output
                    //printf("[success] num of output is %d\n",out_num);
                    if(out_num>0)
                    {
                        //int output_t[out_num];
                        int output_i;
                        for (output_i=0;output_i<out_num;output_i++)
                        {
                            //printf("[success] push the data_%d type %d!\n",output_i,LAC[func_pos]);
                            push_b(LAC[func_pos++],type);
                        }
                    }
                    if(judge_func==1)
                    {
                        int MV_temp_pos=LAC[func_pos];
                        //push_b(MV_temp_pos,retour);
                        func_run(MV_temp_pos);



                    }

                }
                else
                {
                    if(num_judge(info)!=0)
                    {
                        int input=atoi(info);
                        push_b(input,data);
                        //printf("[success] push the num %d!\n",input);
                        push_b(INT,type);
                    }
                    else
                    {
                        //int input=info;
                        printf("[fail]Invalid input \"%s\"!\n",info);COND=1;break;
                    }
                }
            }
        }
    }
}

int quit(char* input_util)
{
    int i=0, j=0;
    if(input_util[0]!='\0')
    {
        while(input_util[i]==' '){i++;}
        if(input_util[i]=='q'&&input_util[i+1]=='u'&&input_util[i+2]=='i'&&input_util[i+3]=='t'&&(input_util[i+4]=='\0'||input_util[i+4]==' ')){j=1;}
    }
    if(j==1){/*printf("[success] QUIT!");*/return 1;}
    else return 0;
}


/********MAIN********/
int main(int argc, char* argv[])
{

    data = pile_new();//donnee
    type = pile_new();//type
    retour = pile_new();
    char input[100];
    int i;
    for(i=0;i<100;i++){input[i]='\0';}
    initial();
    type_func_out = pile_new();
    type_func_in = pile_new();
    func_if = pile_new();
    func_else = pile_new();
    func_judge = pile_new();
    if_in = pile_new();
    if_out = pile_new();



    while(1)
    {
        printf("\nEDGAR_LAC>> ");
        scanf("%99[^\n]",input);
        int in_l=strlen(input);
        if(input[0]=='.'&&input[1]=='/')
        {
            COND=0;
            char fname[100];
            char fileout[1000][100];
            int file_l=0;
            if(input[in_l-4]=='.'&&input[in_l-3]=='l'&&input[in_l-2]=='a'&&input[in_l-1]=='c')
            {
                for(i=0;i<in_l-2;i++){fname[i]=input[i+2];}
                lexical(fname,fileout,&file_l);


                //printf("[success]line_num %d\n",file_l);
                //for(i=0;i<file_l;i++){printf("%s",fileout[i]);}
                for(i=0;i<100;i++){input[i]='\0';}getchar();

                int fi;
                for(fi=0;fi<file_l;fi++)
                {
                    //printf("ligne %d\n",fi);
                    for(i=strlen(fileout[fi])-1;i<100;i++){fileout[fi][i]='\0';}
                    run(fileout[fi]);
                    if(COND==1){COND=0;break;}

                    //printf("LAC: ");
                    //for(i=0;i<LAC_pos;i++) printf("%d ",LAC[i]);
                    //printf("\n");
                    //printf("MV: ");
                    //for(i=0;i<MV_pos;i++) printf("%d ",MV[i]);
                    //printf("\n");
                }

            }
            else {printf("[error]Invalid file type !\n");for(i=0;i<100;i++){input[i]='\0';}getchar();}
        }
        else
        {
            COND=0;
            //printf("[success] input %s\n",input);
            //getchar();
            if(quit(input)){break;}
            run(input);
            //printf("data_num %d\n",data->num);
            //printf("type_num %d\n",type->num);
            for(i=0;i<100;i++){input[i]='\0';}
            getchar();

        }


        //printf("LAC: ");
        //for(i=0;i<LAC_pos;i++) printf("%d ",LAC[i]);
        //printf("\n");
        //printf("MV: ");
        //for(i=0;i<MV_pos;i++) printf("%d ",MV[i]);
        //printf("\n");
        //printf("CHAINE: ");
        //for(i=0;i<chaine_e;i++) printf("%d ",chaine[i]);
        //printf("\n");
    }

}
