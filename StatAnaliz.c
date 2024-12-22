#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 500
#define RESET   "\033[0m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[1;37m"

void PrintHistogram(double w_h[], double h, int k);
void ConfidenceInterval(int mas[]);
double MathExpectation(int mas[]);
void GenerationData(int mas[]);
double Dispersion(int mas[]);
double Deviation(int mas[]);
double ArithmeticMean(int mas[]);
int Median(int mas[]);
int Scope(int mas[]);
void Sort(int mas[]);
double RoundDouble(double n);
int NormalDistribution();
void Histogram(int mas[]);

int main()
{
    int mas[MAX];
    GenerationData(mas);
    Sort(mas);
    
    printf("Мат ожидание: %f\n", MathExpectation(mas));
    printf("Дисперсия: %f\n", Dispersion(mas));
    printf("Среднее квадратическое отклонение: %f\n", Deviation(mas));
    printf("Медиана: %d\n", Median(mas));
    printf("Размах: %d\n", Scope(mas));
    printf("Среднее арифметическое: %f\n", ArithmeticMean(mas));
    ConfidenceInterval(mas);
    printf("Гистограмма относительных частот:\n");
    Histogram(mas);

    return 0;
}

//--------------Математическое ожидание--------------
double MathExpectation(int mas[])
{
    double res = 0;

    for (int i = 0; i < MAX; i++)
    {
        res += mas[i] * ((double)1 / MAX);
    }

    return RoundDouble(res);
}

//--------------Дисперсия--------------
double Dispersion(int mas[])
{
    int mas2[MAX];
    for (int i = 0; i < MAX; i++)
    {
        mas2[i] = mas[i] * mas[i];
    }

    double res = MathExpectation(mas2) - (MathExpectation(mas) * MathExpectation(mas));

    return RoundDouble(res);
}

//--------------Среднее квадратическое отклонение--------------
double Deviation(int mas[])
{
    double dispesion = Dispersion(mas);
    double res = sqrtf(dispesion);
    return RoundDouble(res);
}

//--------------Медиана--------------
int Median(int mas[])
{
    double res = 0;

    if (MAX % 2 == 0)
    {
        res = (mas[MAX / 2] + mas[MAX / 2 - 1]) / 2;
    }
    else
    {
        res = mas[MAX / 2];
    }

    return RoundDouble(res);
}

//--------------Размах--------------
int Scope(int mas[])
{
    return mas[MAX - 1] - mas[0];
}

//--------------Среднее арифметическое--------------
double ArithmeticMean(int mas[])
{
    double res = 0;

    for (int i = 0; i < MAX; i++)
    {
        res += mas[i];
    }

    return RoundDouble(res / MAX);
}

//--------------Доверительный интервал--------------
void ConfidenceInterval(int mas[])
{
    double conf_factor = 1.96; // коэффициент доверия при вероятности 0,95
    double dispersion = Dispersion(mas);
    double mean = ArithmeticMean(mas);

    double accuracy = (conf_factor * dispersion) / (sqrt(MAX)); // точность оценки

    printf("Доверительный интервал: (%.3f ; %.3f)\n", mean - accuracy, mean + accuracy);
}

//--------------Гистограмма относительных частот--------------
void Histogram(int mas[])
{
    int k = 1 + 3.322 * log(MAX); // количество интервалов
    double h = (mas[MAX - 1] - mas[0]) / (double)k;

    double *x = malloc(k * sizeof(double));
    int *n = malloc(k * sizeof(int));

    double start_interval = mas[0];
    for (int i = 0; i < k; i++)
    {
        x[i] = (start_interval * 2 + h) / 2;
        start_interval += h;
    }

    for (int i = 0; i < MAX; i++)
    {
        int j = 0;
        for (double start_interval = mas[0]; j < k - 1 && (double)mas[i] > start_interval + h; start_interval += h, j++)
            ;
        n[j]++;
    }

    double *w_h = malloc(k * sizeof(double)); // плотность относительных частот
    for (int i = 0; i < k; i++)
    {
        w_h[i] = (n[i] / (double)MAX) / h;
    }

    PrintHistogram(w_h, h, k);
}

void PrintHistogram(double w_h[], double h, int k)
{
    printf("\n");
    double max = w_h[0];
    for (int i = 1; i < k; i++)
    {
        if (w_h[i] > max)
        {
            max = w_h[i];
        }
    }

    for (double i = max; i > 0; i -= 0.01)
    {
        printf("%5.2f| ", i);
        for (int j = 0; j < k; j++)
        {
            if (w_h[j] > i)
            {
                printf(" %sxxxxx%s ",WHITE, RESET);
            }
            else
            {
                printf("       ");
            }
        }
        printf("\n");
    }

    for(int i=0;i<k;i++) {
        printf("--------");
    }
    printf("\n       ");
    double h0 = 0 + h;
    for (int i = 0; i < k; i++)
    {
        printf(" %5.2f ", h0);
        h0 += h;
    }
    printf("\n");
}

void Sort(int mas[])
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX - i - 1; j++)
        {
            if (mas[j] > mas[j + 1])
            {
                int t = mas[j];
                mas[j] = mas[j + 1];
                mas[j + 1] = t;
            }
        }
    }
}

double RoundDouble(double n)
{
    char str[MAX];
    sprintf(str, "%.2f", n);
    // printf("n = %f | str = %s | Round = %f\n", n, str, atof(str));
    return atof(str);
}

void GenerationData(int mas[])
{
    srand(time(NULL));

    for (int i = 0; i < MAX; i++)
    {
        int random = NormalDistribution();
        switch (random)
        {
        case 1:
            mas[i] = 191 + rand() % (200 - 191 + 1);
            break;
        case 2:
            mas[i] = 186 + rand() % (190 - 186 + 1);
            break;
        case 3:
            mas[i] = 181 + rand() % (185 - 181 + 1);
            break;
        case 4:
            mas[i] = 170 + rand() % (180 - 170 + 1);
            break;
        case -3:
            mas[i] = 165 + rand() % (169 - 165 + 1);
            break;
        case -2:
            mas[i] = 160 + rand() % (164 - 160 + 1);
            break;
        case -1:
            mas[i] = 150 + rand() % (159 - 150 + 1);
            break;
        default:
            mas[i] = 176;
            break;
        }
    }
}

int NormalDistribution()
{
    int res = 0;
    int random = rand() % 100;

    if (random < 5)
        res = 1;
    else if (random < 20)
        res = 2;
    else if (random < 50)
        res = 3;
    else
        res = 4;

    if (random != 4 && rand() % 2 == 0)
    {
        res *= -1;
    }

    return res;
}