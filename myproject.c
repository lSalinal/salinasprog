#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int S[32];
int Imm;
int stability[8];
void p_flag(int result) 
{
    int count = 0;
    while (result > 0)
    {
        if (result % 2 == 1)
        {
            count++;
        }
        result /= 2;
    }
    if (count % 2 == 1)
    {
        stability[0] = 1;
    }
    else
    {
        stability[0] = 0;
    }
}
void zero_flag(int result)
{
    if (result == 0)
    {
        stability[1] = 1;
    }
    else
    {
        stability[1] = 0;
    }
}
void sign_flag(int result)
{
    if (result < 0)
    {
        stability[2] = 1;
    }
    else
    {
        stability[2] = 0;
    }
}
void overflow_flag_add(int a, int b, int c)
{
    if (b > 0 && c > 0 && a < 0 || b < 0 && c < 0 && a < 0)
    {
        stability[5] = 1;
    }
    else
    {
        stability[5] = 0;
    }
}
void overflow_flag_sub(int a, int b, int c)
{
    if (b > 0 && c < 0 && a < 0 || b < 0 && c > 0 && a > 0)
    {
        stability[5] = 1;
    }
    else
    {
        stability[5] = 0;
    }
}
void overflow_flag_mull(int a, int b, int c)
{
    if (b != 0 && (a / b) != c)
    {
        stability[5] = 1;
    }
    else
    {
        stability[5] = 0;
    }
}
void ADD(int a, int b, int c)
{
    S[a] = S[b] + S[c];
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
    overflow_flag_add(S[a], S[b], S[c]);
}
void SUB(int a, int b, int c)
{
    S[a] = S[b] - S[c];
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
    overflow_flag_sub(S[a], S[b], S[c]);
}
void AND(int a, int b, int c)
{
    S[a] = S[b] & S[c];
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void XOR(int a, int b, int c)
{
    S[a] = S[b] ^ S[c];
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void OR(int a, int b, int c)
{
    S[a] = S[b] | S[c];
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void ADDI(int a, int b, int Imm)
{
    S[a] = S[b] + Imm;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
    overflow_flag_sub(S[a], S[b], Imm);
}
void SUBI(int a, int b, int Imm)
{
    S[a] = S[b] - Imm;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
    overflow_flag_sub(S[a], S[b], Imm);
}
void ANDI(int a, int b, int Imm)
{
    S[a] = S[b] & Imm;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void XORI(int a, int b, int Imm)
{
    S[a] = S[b] ^ Imm;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void ORI(int a, int b, int Imm)
{
    S[a] = S[b] | Imm;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void MOV(int c, int Imm)
{
    S[c] = Imm;
}
void SWP(int a, int b)
{
    int jbj1, jbj2;
    jbj1 = S[a];
    jbj2 = S[b];
    MOV(a, jbj2);
    MOV(b, jbj1);
}
void DUMP_REGS()
{
    for (int i = 0; i < 32; i++)
    {
        printf("%d  ", S[i]);
    }
    printf("\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%d  ", stability[i]);
    }
}
void DUMP_REGS_F()
{
    FILE *file;
    file = fopen("regs.txt", "w");
    for(int i=0 ; i<32 ; i++)
    {
        fprintf(file, "%d  ", S[i]);
    }
    fprintf(file, "\n");
    for(int i=0; i<8; i++)
    {
        fprintf(file, "%d  ", stability[i]);
    }
    fclose(file);
}
void INPUT()
{
    scanf("%d", &S[0]);
}
void OUTPUT()
{
    printf("%d ", S[0]);
}
void DIV(int a, int b)
{
    int sub = S[a] / S[b];
    int rem = S[a] % S[b];
    S[a] = sub;
    S[b] = rem;
    p_flag(S[a]);
    zero_flag(S[a]);
    sign_flag(S[a]);
}
void MULL(int a, int b)
{
    int multiply = S[a] * S[b];
    S[b] = multiply & 15;
    S[a] = multiply >> 4;
}