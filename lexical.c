#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include "structure.c"

pilei* p_o, *p_f;


//char* expr1="^[\\\\\\s]|[\\s\\\\\\s]";
char* expr1="^(\\\\ )|( \\\\ )";
void lexical(char *fname, char texte_in[1000][100],int* file_l)
{
    int chaine_n,chaine_i;
    p_o=pile_inew();
    p_f=pile_inew();
    FILE *fp = fopen(fname, "rb");
    if(fp==NULL){printf("[error]No such file!\n");}
    else
    {
        char texte[1000][100];
        int texte_l=0;//ligne de texte
        int temp_i;
        while((fgets(texte[texte_l],100,fp))!=NULL)
        {
            //printf("aaaaa%d!\n",texte_l);
            texte_l++;
        }
        *file_l=texte_l;

        int po[2];
        for(temp_i=0;temp_i<texte_l;temp_i++)
        {
            regex_t myregexp1;
            regcomp(&myregexp1,expr1,REG_EXTENDED|REG_NEWLINE);
            regmatch_t pmatch1[100];
            int res1 = 0, debut1;//, fin1, offset1 = 0;
            res1 = regexec(&myregexp1,texte[temp_i],1,pmatch1,0);
            debut1 = pmatch1[0].rm_so;
            //fin1 = pmatch1[0].rm_eo;
            int i=0;
            if (res1 != 0);
            else
            {
                if(debut1==0)
                {
                    for (i=1;i<100;i++){texte[temp_i][i]='\0';} texte[temp_i][debut1]='\n';
                }
                else
                {
                    for (i=99;i>debut1;i--){texte[temp_i][i]='\0';}
                    texte[temp_i][debut1]='\n';
                }
            }
            regfree(&myregexp1);

            po[0]=temp_i;
            for(i=0;i<100;i++)
            {
                chaine_i=0;
                po[1]=i;
                if(i==0)
                {
                    if(texte[temp_i][i]=='"'&&texte[temp_i][i+1]==' '&&chaine_i==0)
                    {
                        if(chaine_n==0)chaine_n++;
                        else chaine_n--;
                        chaine_i=1;
                    }
                    else if(texte[temp_i][i]=='"'&&chaine_i==0){chaine_n--;chaine_i=1;}
                    else if(texte[temp_i][i]=='('&&texte[temp_i][i+1]==' '&&chaine_n%2==0){push_ib(po, p_o);/*printf("push%d\n",p_o->num);*/}
                }
                else if(i<99&&i>0)
                {
                    if(texte[temp_i][i-1]==' '&&texte[temp_i][i]=='"'&&texte[temp_i][i+1]==' '&&chaine_i==0)
                    {
                        if(chaine_n==0)chaine_n++;
                        else chaine_n--;
                        chaine_i=1;
                    }
                    else if(texte[temp_i][i]=='"'&&chaine_i==0){chaine_n--;chaine_i=1;}
                    else if(texte[temp_i][i-1]==' '&&texte[temp_i][i]=='('&&texte[temp_i][i+1]==' '&&chaine_n%2==0){push_ib(po, p_o);/*printf("push%d\n",p_o->num);*/}
                }

                if(texte[temp_i][i]==')'&&chaine_n%2==0)
                {
                    //printf("find pf\n");
                    //printf("yinhao %d\n",chaine_n);
                    if(p_o->num>0)
                    {
                        //printf("find pf\n");
                        int temp_p[2];
                        pop_ib(temp_p,p_o);
                        //printf("pos%d %d\n",temp_p[0],temp_p[1]);
                        int d_l,d_c;
                        if(temp_p[0]==temp_i)
                        {
                            //printf("enter0\n");
                            for(d_c=temp_p[1];d_c<=i;d_c++)
                            {
                                texte[temp_i][d_c]=' ';
                                if(d_c==99){texte[temp_i][d_c]='\n';}
                            }
                        }
                        else
                        {
                            for (d_l=temp_p[0];d_l<=temp_i;d_l++)
                            {
                                //printf("enter1\n");
                                if(d_l==temp_p[0])
                                {
                                    {texte[d_l][temp_p[1]]='\n';}
                                    for (d_c=temp_p[1]+1;d_c<=99;d_c++){ texte[d_l][d_c]='\0';}

                                }
                                else if(d_l==temp_i)
                                {
                                    for (d_c=temp_p[1];d_c<=i;d_c++){texte[d_l][d_c]=' ';}
                                    if(d_c==99){texte[d_l][d_c]='\n';}
                                }
                                else
                                {
                                    for (d_c=1;d_c<=99;d_c++){texte[d_l][d_c]='\0';}
                                    texte[d_l][0]='\n';
                                }
                            }
                        }
                    }
                    else {printf("[error]Lack of left parenthesis !\n");return;}
                }

            }

        }

        if(p_o->num!=0){printf("[error]Lack of right parenthesis !\n");return;}
        if(chaine_n%2!=0){printf("[error]Lack of quotation mark !\n");return;}




        int temp_j;
        for(temp_i=0;temp_i<1000;temp_i++)
            for (temp_j=0;temp_j<100;temp_j++)
                texte_in[temp_i][temp_j]=texte[temp_i][temp_j];
        //for(temp_i=0;temp_i<texte_l;temp_i++){printf("%s",texte[temp_i]);}
    }
    pile_iclear(p_o);pile_iclear(p_f);chaine_n=0;chaine_i=0;
    //printf("quotation: %d\n",chaine_n);

}
