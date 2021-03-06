#include "processor.h"


int readcommands(FILE *&F,cpu *CPU)
{
    if(!CPU)return NULL_ARGUMENT;

    int command = -2;
    double argument = -2, forpush = 0, forpop = 0;
    char c = ' ',b = ' ';
    do
    {

        CPU->coms.value=(double*) realloc((&(CPU)->coms)->value,3*sizeof(double)*( (CPU)->currcommand +1));
        assert((&(CPU)->coms)->value);

        if (!F)
        {
            FILE *F2=fopen("3.txt","w");
            assert(F2);
            fprintf(F2,"CODE NOT FOUND");
            break;
        }
        if(!feof(F))fscanf(F,"%d",&command);
        else command=0;


        switch (command)
        {
        case DIV:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=DIV;
            break;
        }
        case SQRT:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=SQRT;
            break;
        }
        case JMP:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=JMP;
            fscanf(F,"%lg",&argument);
            (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=(int)argument;
            break;
        }
        case MUL:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=MUL;
            break;
        }
        case ADD:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=ADD;
            break;
        }
        case CALL:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=CALL;
            fscanf(F,"%lg",&argument);
            (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=(int)argument;
            break;
        }
        case RET:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=RET;
            break;
        }
        case JA:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=JA;
            fscanf(F,"%lg",&argument);
            (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=(int)argument;

        }
        break;
        case POP:
        {

            c=getc(F);
            if ((c=='\n')||(c==EOF))
            {
                (&(CPU)->coms)->value[3*(CPU)->currcommand]=POP;
                (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=0;
            }
            else
            {
                (&(CPU)->coms)->value[3*(CPU)->currcommand]=POP;
                (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=1;
                fscanf(F," %c",&b);
                *(char*)&(&(CPU)->coms)->value[3*(CPU)->currcommand+2]=b;
            }
            break;
        }


        case EXIT:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=EXIT;
            break;
        }

        case CRASH:
        {
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=CRASH;
            break;
        }
        case PUSH:
        {
            fscanf(F,"%lg",&argument);
            (&(CPU)->coms)->value[3*(CPU)->currcommand]=PUSH;
            (&(CPU)->coms)->value[3*(CPU)->currcommand+1]=(int)argument;
            if (argument==0)
            {

                fscanf(F,"%lg",&forpush);
                (&(CPU)->coms)->value[3*(CPU)->currcommand+2]=forpush;
            }
            if (argument==1)
            {
                fscanf(F," %c",&b);
                *(char*)&(&(CPU)->coms)->value[3*(CPU)->currcommand+2]=b;
            }

            break;
        }
        }

        (CPU)->currcommand++;
    }
    while(command!=EXIT);
}
//TODO ������ ������
//TODO ������ ��������


int CPUdocommands(cpu *CPU,FILE *&F2)
{
    if(!CPU)return NULL_ARGUMENT;
    char c=' ';
    double forpush=0,forpop=0;   //c,forpush,forpop needed for input and output
    for (int i=0; i<(CPU)->currcommand-1; i++)
    {


        switch (int(((&(CPU)->coms)->value[3*i])))
        {
        case CRASH:
            printf("CRASH AT %d LINE",i+1);
            abort();
            break;
        case ADD:
        {
            //forpush=stack_pop(&((CPU)->st));
            //forpop=stack_pop(&((CPU)->st));
            //assert(forpop==FAIL);
            //stack_push(&((CPU)->st),forpop+forpush);
            add(&((CPU)->st));
            break;
        }

        case MUL:
        {
            mul(&((CPU)->st));
            break;
        }
        case DIV:
        {

            if(div(&((CPU)->st))==1)
            {
                printf("DIVIZION BY ZERO IS FORBIDDEN!");
                abort();
            }
            break;
        }
        case SQRT:
        {
            sqrt(&((CPU)->st));
            break;
        }

        case JMP:
        {

            i=(&(CPU)->coms)->value[3*i+1]-1;
            break;
        }

        case CALL:
        {
            stack_push(&((CPU)->functst),i);
            i = CPU->coms.value[3*i+1]-1;
            break;

        }
        case RET:
        {
            i=stack_pop(&((CPU)->functst));
            break;
        }
        case POP:
        {
            if ((&(CPU)->coms)->value[3*i+1]==0)if((forpop=stack_pop(&((CPU)->st)))!=FAIL)fprintf(F2,"%lg\n",forpop);
            if ((&(CPU)->coms)->value[3*i+1]==1)
            {
                c=*(char*)&(&(CPU)->coms)->value[3*i+2];
                if (c=='a')CPU->ax=stack_pop(&((CPU)->st));
                if (c=='b')CPU->bx=stack_pop(&((CPU)->st));
                if (c=='c')CPU->cx=stack_pop(&((CPU)->st));
                if (c=='d')CPU->dx=stack_pop(&((CPU)->st));
            }
            break;
        }

        case JA:
        {


            forpush=stack_pop(&((CPU)->st));
            forpop=stack_pop(&((CPU)->st));

            if(forpush>forpop)
            {
                i=(&(CPU)->coms)->value[3*i+1]-1;
            }
            break;
        }

        case PUSH:
        {

            if ((&(CPU)->coms)->value[3*i+1]==0)stack_push(&((CPU)->st),(&(CPU)->coms)->value[3*i+2]);
            if ((&(CPU)->coms)->value[3*i+1]==1)
            {
                c=*(char*)&(&(CPU)->coms)->value[3*i+2];
                if (c=='a')stack_push(&((CPU)->st),CPU->ax);
                if (c=='b')stack_push(&((CPU)->st),CPU->bx);
                if (c=='c')stack_push(&((CPU)->st),CPU->cx);
                if (c=='d')stack_push(&((CPU)->st),CPU->dx);
            }
            break;
        }

        }
        if(i>=(CPU)->currcommand-1)
        {
            printf("JUMP TO INVALID VALUE %d!",(CPU)->currcommand-1);
            abort();
        }
    }

}

int CPU_ok(cpu *CPU)
{
    if (!CPU)return NULL_ARGUMENT;
    if (!(&CPU->st))return 1;
    if (!(&CPU->functst))return 1;
    return 0;
}

int CPU_dump(cpu *CPU)
{
    if(!CPU)return NULL_ARGUMENT;
    printf("ax=%lg bx=%lg cx=%lg dx=%lg",CPU->ax=0, CPU->bx=0, CPU->cx=0, CPU->dx=0);
    return 0;
}

int cpuinitialize(cpu *CPU)
{
    if(!CPU)return NULL_ARGUMENT;
    CPU->ax=0;
    CPU->bx=0;
    CPU->cx=0;
    CPU->dx=0;
    CPU->currcommand=0;
    stack_ctor(&(CPU->st));
    stack_ctor(&(CPU->functst));
}

int main()
{
    FILE *F2=fopen("3.txt","w");
    if(!F2)
    {
        return 1;
        printf("SOMETHINK WENT WRONG,CANNOT CREATE OUTPUT FILE");
        return 1;
    }
    FILE *F=fopen("2.txt","r");
    if (!F)
    {
        printf("SOMETHINK WENT WRONG,CANNOT OPEN INPUT FILE");
        return 1;
    }

    cpuinitialize(&CPU_);

    readcommands(F,&CPU_);

    CPUdocommands(&CPU_,F2);

    return 0;

}



