#include <stdio.h>
#include <string.h>
#include <math.h>

int IP_Table[64] = {
        58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
int E_Table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
int P_Table[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
int IPR_Table[64] = {
        40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
int PC1_Table[56] = {
        57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
int PC2_Table[48] = {
        14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
int S_Box[8][4][16] = {
        // S1
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
        // S2
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
        // S3
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
        // S4
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
        // S5
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
        // S6
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
        // S7
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
        // S8
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

void init(char string[], int array[])
{
    for (int i = 0; i < 64; i++)
    {
        array[i] = (string[i / 8] >> (7 - i % 8)) & 1;
    }
}

void copy(int dst[], int src[], int n)
{
    for (int i = 0; i < n; i++)
    {
        dst[i] = src[i];
    }
}

void ip(int block[])
{
    int temp[64];
    copy(temp, block, 64);
    for (int i = 0; i < 64; i++)
    {
        block[i] = temp[IP_Table[i]-1];
    }
}

void extend(int R[], int E[])
{
    for (int i = 0; i < 48; i++)
    {
        E[i] = R[E_Table[i]-1];
    }
}

void xor(int DST[], int SRC1[], int SRC2[], int n)
{
    for (int i = 0; i < n; i++)
    {
        DST[i] = (SRC1[i]!=SRC2[i]);
    }
}

void sbox(int X[], int S[])
{
    int row, column, value, count;
    for (int i = 0; i < 48; i = i + 6)
    {
        row = X[i]*2+X[i+5];
        column = X[i+1]*8+X[i+2]*4+X[i+3]*2+X[i+4];
        value = S_Box[i/6][row][column];
        count = (i/6)*4;
        for (int j = count; j < count + 4; j++)
        {
            S[j] = (value >> (3 - j%4)) & 1;
        }
    }
}

void pbox(int S[], int P[])
{
    int temp[32];
    copy(temp, S, 32);
    for (int i = 0; i < 32; i++)
    {
        P[i] = S[P_Table[i]-1];
    }
}

void inv_ip(int block[], int R[], int L[])
{
    int temp[64];
    copy(temp, R, 32);
    copy(&temp[32], L, 32);
    for (int i = 0; i < 64; i++)
    {
        block[i] = temp[IPR_Table[i]-1];
        if (i % 8 == 0 && i != 0)
        {
            printf(" ");
        }
        printf("%d", block[i]);
    }
}

void pc1(int K[], int key_block[])
{
    for (int i = 0; i < 56; i++)
    {
        K[i] = key_block[PC1_Table[i]-1];
    }
}

void shift(int K[], int n)
{
    int temp;
    for (int i = 0; i < n; i++)
    {
        temp = K[0];
        for (int i = 0; i < 27; i++)
        {
            K[i] = K[i+1];
        }
        K[27] = temp;
    }
}

void pc2(int Ki[], int K[])
{
    for (int i = 0; i < 48; i++)
    {
        Ki[i] = K[PC2_Table[i]-1];
    }
}

void generate(int K[][48], char key[])
{
    int key_block[64];
    int KY[56];
    int C[28];
    int D[28];
    init(key, key_block);
    pc1(KY, key_block);
    copy(C, KY, 28);
    copy(D, &KY[28], 28);
    for (int i = 0; i < 16; i++)
    {
        if ((i==0)||(i==1)||(i==8)||(i==15))
        {
            shift(C, 1);
            shift(D, 1);
        }
        else
        {
            shift(C, 2);
            shift(D, 2);
        }
        copy(KY, C, 28);
        copy(&KY[28], D, 28);
        pc2(K[i], KY);
    }
}

void print(int array[])
{
    for (int i = 0; i < 32; i++)
    {
        if (i % 8 == 0)
        {
            printf(" ");
        }
        printf("%d", array[i]);
    }
    printf("\n");
}

void transfer(int block[], char cipher[])
{
    int i, j;
    for (i = 0; i < 64; i = i + 8)
    {
        cipher[i/8] = 0;
        for (j = 1; j < 8; j++)
        {
            if (block[i] == 0)
            {
                cipher[i/8] = cipher[i/8] + block[i + j] * pow(2, 7-j);
            }
            else
            {
                cipher[i/8] = cipher[i/8] - block[i + j] * pow(2, 7-j);
            }
        }
    }
    cipher[i/8] = 0;
}

void DES(char data[], char cipher[], int K[][48])
{
    int l;
    char string[8];
    int block[64];
    int L[32];
    int TEMP_L[32];
    int R[32];
    int TEMP_R[32];
    int E[48];
    int X[48];
    int S[32];
    int P[32];
    int F[32];
    for (int i = 0; i < strlen(data); i = i + 8)
    {
        if ((l = strlen(data) - i) < 8)
        {
            memmove(string, &data[i], l);
            for (int j = l; j < 8; j++)
            {
                string[j] = 0;
            }
        }
        else
        {
            memmove(string, &data[i], 8);
        }
        init(string, block);
        ip(block);
        copy(L, block, 32);
        copy(R, &block[32], 32);
        for (int k = 0; k < 16; k++)
        {
            copy(TEMP_L, L, 32);
            copy(TEMP_R, R, 32);
            copy(L, TEMP_R, 32);
            extend(TEMP_R, E);
            xor(X, E, K[k], 48);
            sbox(X, S);
            pbox(S, P);
            xor(F, TEMP_L, P, 32);
            copy(R, F, 32);
            printf("L%d:", k);
            print(L);
            printf("R%d:", k);
            print(R);
        }
        inv_ip(block, R, L);
    }
    transfer(block, cipher);
}

void reverse(int K[][48])
{
    int temp[48];
    for (int i = 0; i < 8; i++)
    {
        copy(temp, K[i], 48);
        copy(K[i], K[15-i], 48);
        copy(K[15-i], temp, 48);
    }
}

int main(int argc, char const *argv[])
{
    char cipher[9];
    int K[16][48];

    char *plain = "12345678";
    char *key = "12345678";
    generate(K, key);
    DES(plain, cipher, K);

    return 0;
}