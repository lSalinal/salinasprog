#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int S[32];
int Imm;
int stability[8];
int stack[64];
int lines;

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
    for (int i = 0; i < 32; i++)
    {
        fprintf(file, "%d  ", S[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < 8; i++)
    {
        fprintf(file, "%d  ", stability[i]);
    }
    fclose(file);
}
void INPUT()
{
    scanf("%d ", &S[0]);
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
void PUSH(int a)
{
    for (int i = 0; i < 64; i++)
    {
        stack[i + 1] = stack[i];
    }
    stack[0] = S[a];
}
void POP(int a)
{
    stack[a] = stack[0];
    for (int i = 0; i < 64; i++)
    {
        stack[i] = stack[i + 1];
    }
}

int main(int argc, char *argv[])
{
    int first, second, third;
    int count = 0;
    char temp[1000];
    char file_name[32];
    FILE *inputs;
    if (argc < 1)
    {
        inputs = fopen("in.txt", "r");
    }
    else
    {
        inputs = fopen(argv[1], "r");
    }
    while (fscanf(inputs, "%[^\n]\ns", temp) != -1)
    {
        count++;
        char check[16];

        for (int i = 0; i < sizeof(temp); i++)
        {
            temp[i] = toupper(temp[i]);
        }

        for (int j = 0; temp[j] != ' '; j++)
        {
            check[j] = temp[j];
            check[j + 1] = '\0';
        }
        if (strcmp(check, "EXIT") == 0)
        {
            exit(0);
        }
        else if (strcmp(check, "JMP") == 0)
        {
            printf("ssssssssssssss");
            sscanf(temp, "JMP %d", &first);
            lines = 0;
            fseek(inputs, lines, SEEK_SET);
            while (lines + 1 < first)
            {
                char chars;
                chars = fgetc(inputs);
                if (chars == '\n')
                {
                    lines++;
                }
            }
        }
        else if (strcmp(check, "ADD") == 0)
        {
            sscanf(temp, "ADD S%d, S%d, S%d", &first, &second, &third);
            ADD(first, second, third);
        }
        else if (strcmp(check, "SUB") == 0)
        {
            sscanf(temp, "SUB S%d, S%d, S%d", &first, &second, &third);
            SUB(first, second, third);
        }
        else if (strcmp(check, "AND") == 0)
        {
            sscanf(temp, "AND S%d, S%d, S%d", &first, &second, &third);
            AND(first, second, third);
        }
        else if (strcmp(check, "XOR") == 0)
        {
            sscanf(temp, "XOR S%d, S%d, S%d", &first, &second, &third);
            XOR(first, second, third);
        }
        else if (strcmp(check, "OR") == 0)
        {
            sscanf(temp, "OR S%d, S%d, S%d", &first, &second, &third);
            OR(first, second, third);
        }
        else if (strcmp(check, "ADDI") == 0)
        {
            sscanf(temp, "ADDI S%d, S%d, %d", &first, &second, &third);
            ADDI(first, second, third);
        }
        else if (strcmp(check, "SUBI") == 0)
        {
            sscanf(temp, "SUBI S%d, S%d, %d", &first, &second, &third);
            SUBI(first, second, third);
        }
        else if (strcmp(check, "ANDI") == 0)
        {
            sscanf(temp, "ANDI S%d, S%d, %d", &first, &second, &third);
            ADDI(first, second, third);
        }
        else if (strcmp(check, "XORI") == 0)
        {
            sscanf(temp, "XORI S%d, S%d, %d", &first, &second, &third);
            XORI(first, second, third);
        }
        else if (strcmp(check, "ORI") == 0)
        {
            sscanf(temp, "ORI S%d, S%d, %d", &first, &second, &third);
            ORI(first, second, third);
        }
        else if (strcmp(check, "MOV") == 0)
        {
            if (temp[8] == 'S' || temp[9] == 'S')
            {
                sscanf(temp, "MOV S%d, S%d", &first, &second);
                MOV(first, S[second]);
            }
            else
            {
                sscanf(temp, "MOV S%d, %d", &first, &second);
                MOV(first, second);
            }
        }
        else if (strcmp(check, "SWP") == 0)
        {
            sscanf(temp, "SWP S%d, S%d", &first, &second);
            SWP(first, second);
        }
        else if (strcmp(check, "DUMP_REGS") == 0)
        {
            DUMP_REGS();
        }
        else if (strcmp(check, "DUMP_REGS_F") == 0)
        {
            DUMP_REGS_F();
        }
        else if (strcmp(check, "INPUT") == 0)
        {
            INPUT();
        }
        else if (strcmp(check, "OUTPUT") == 0)
        {
            OUTPUT();
        }
        else if (strcmp(check, "MULL") == 0)
        {
            sscanf(temp, "MULL S%d, S%d", &first, &second);
            MULL(first, second);
        }
        else if (strcmp(check, "DIV") == 0)
        {
            sscanf(temp, "DIV S%d, S%d", &first, &second);
            DIV(first, second);
        }
        else if (strcmp(check, "POP") == 0)
        {
            sscanf(temp, "POP S%d", &first);
            POP(first);
        }
        else if (strcmp(check, "PUSH") == 0)
        {
            sscanf(temp, "PUSH S%d", &first);
            PUSH(first);
        }
        else
        {
            printf("The instruction not supported! ");
        }
    }
    fclose(inputs);
}