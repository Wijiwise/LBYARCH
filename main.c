#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

extern void imgCvtGrayDoubleToInt(int height, int width, const double *input, uint8_t *output);

static double randomDouble(void)
{
    return (double)rand() / (double)RAND_MAX;
}

static uint8_t expectedOutput(double pixel)
{
    return (uint8_t)(pixel * 255.0 + 0.5);
}

static int verifyOutput(const double *input, const uint8_t *output, int count)
{
    for (int i = 0; i < count; i++)
    {
        uint8_t exp = expectedOutput(input[i]);
        if (output[i] != exp)
        {
            printf("Mismatch at index %d: input=%.17g expected=%u got=%u\n",
                   i, input[i], (unsigned)exp, (unsigned)output[i]);
            return 0;
        }
    }
    return 1;
}

static void printOutput(const uint8_t *output, int height, int width)
{
    int count = height * width;
    for (int i = 0; i < count; i++)
    {
        printf("%u", (unsigned)output[i]);
        if ((i + 1) % width == 0)
            printf("\n");
        else
            printf(" ");
    }
}

static double callAsm(int height, int width, const double *input, uint8_t *output)
{
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    imgCvtGrayDoubleToInt(height, width, input, output);
    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)freq.QuadPart;
}

static void fill(double *input, int count)
{
    for (int i = 0; i < count; i++)
    {
        input[i] = randomDouble();
    }
}

static int benchmark(int height, int width, int runs)
{
    int count = height * width;
    double *input = (double *)malloc((size_t)count * sizeof(double));
    uint8_t *output = (uint8_t *)malloc((size_t)count * sizeof(uint8_t));
    if (!input || !output)
    {
        fprintf(stderr, "Allocation failed for %dx%d\n", height, width);
        free(input);
        free(output);
        return 0;
    }

    double total_ms = 0.0;
    int correct = 1;
    for (int i = 0; i < runs; i++)
    {
        fill(input, count);
        total_ms += callAsm(height, width, input, output);
        if (!verifyOutput(input, output, count))
        {
            correct = 0;
        }
    }

    printf("Benchmark %dx%d (%d runs): avg = %.6f ms, correctness = %s\n",
           height, width, runs, total_ms / runs, correct ? "PASS" : "FAIL");

    free(input);
    free(output);
    return correct;
}

int main(void)
{
    srand((unsigned)time(NULL));

    int height = 0, width = 0;
    printf("Enter height and width (e.g. 3 3): ");
    if (scanf("%d %d", &height, &width) != 2 || height <= 0 || width <= 0)
    {
        fprintf(stderr, "Invalid dimensions.\n");
        return 1;
    }

    int count = height * width;
    double *input = (double *)malloc((size_t)count * sizeof(double));
    uint8_t *output = (uint8_t *)malloc((size_t)count * sizeof(uint8_t));
    if (!input || !output)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        free(input);
        free(output);
        return 1;
    }

    int choice = 0;
    printf("Input mode: 1) Enter %d pixel values  2) Generate random [0.0,1.0]: ", count);
    if (scanf("%d", &choice) != 1)
    {
        fprintf(stderr, "Invalid choice.\n");
        free(input);
        free(output);
        return 1;
    }

    if (choice == 1)
    {
        printf("Enter %d double pixel values:\n", count);
        for (int i = 0; i < count; i++)
        {
            if (scanf("%lf", &input[i]) != 1)
            {
                fprintf(stderr, "Invalid pixel value.\n");
                free(input);
                free(output);
                return 1;
            }
            if (input[i] < 0.0 || input[i] > 1.0)
            {
                fprintf(stderr, "Pixel value %.17g out of range [0.0, 1.0].\n", input[i]);
                free(input);
                free(output);
                return 1;
            }
        }
    }
    else
    {
        fill(input, count);
        printf("Generated input pixels:\n");
        for (int i = 0; i < count; i++)
        {
            printf("%.6f%s", input[i], ((i + 1) % width == 0) ? "\n" : " ");
        }
    }

    /* Time assembly function execution only (excluding I/O and memory allocation). */
    double ms = callAsm(height, width, input, output);
    printf("Converted integer pixel values:\n");
    printOutput(output, height, width);
    printf("Correctness check: %s\n", verifyOutput(input, output, count) ? "PASS" : "FAIL");
    printf("Assembly execution time (single run): %.6f ms\n", ms);

    free(input);
    free(output);

    printf("\nRunning benchmarks (30 runs each, ASM time only):\n");
    benchmark(10, 10, 30);
    benchmark(100, 100, 30);
    benchmark(1000, 1000, 30);

    return 0;
}
