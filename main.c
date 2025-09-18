#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N = 288;
const int MIN_CORRECT = 10;
const int MAX_CORRECT = 50;

void filter(int *data) {
    for (int i = 0; i < N; i++) {
        // Проверяем, является ли значение аномальным
        if (data[i] < MIN_CORRECT || data[i] > MAX_CORRECT) {
            int sum = 0;
            int count = 0;

            // Берём по 1 элементу слева и справа, если они корректные
            if (i > 0 && data[i - 1] >= MIN_CORRECT && data[i - 1] <= MAX_CORRECT) {
                sum += data[i - 1];
                count++;
            }
            if (i < N - 1 && data[i + 1] >= MIN_CORRECT && data[i + 1] <= MAX_CORRECT) {
                sum += data[i + 1];
                count++;
            }

            if (count > 0) {
                data[i] = sum / count;
            } else {
                int total = 0, total_count = 0;
                for (int j = 0; j < N; j++) {
                    if (data[j] >= MIN_CORRECT && data[j] <= MAX_CORRECT) {
                        total += data[j];
                        total_count++;
                    }
                }
                if (total_count > 0)
                    data[i] = total / total_count;
            }
        }
    }
}

void visualize(int *data, int size, const char *title) {
    printf("\n=== %s ===\n", title);
    int limit = size < 50 ? size : 50;

    for (int i = 0; i < limit; i++) {
        printf("%3d: ", i);
        int len = data[i] / 2; // длина полосы для наглядности

        if (data[i] < MIN_CORRECT || data[i] > MAX_CORRECT) {
            // Аномальные значения выделяем *
            for (int j = 0; j < len; j++) printf("*");
        } else {
            // Корректные значения #
            for (int j = 0; j < len; j++) printf("#");
        }

        printf(" (%d)\n", data[i]);
    }

    // Простая диаграмма распределения (кол-во элементов по диапазону)
    int bins[6] = {0}; // 0-9, 10-19, 20-29, 30-39, 40-49, >=50
    for (int i = 0; i < size; i++) {
        if (data[i] < 10) bins[0]++;
        else if (data[i] < 20) bins[1]++;
        else if (data[i] < 30) bins[2]++;
        else if (data[i] < 40) bins[3]++;
        else if (data[i] < 50) bins[4]++;
        else bins[5]++;
    }

    printf("\nDistribution:\n");
    for (int i = 0; i < 6; i++) {
        if (i == 0) printf("  0-9: ");
        else if (i == 1) printf("10-19: ");
        else if (i == 2) printf("20-29: ");
        else if (i == 3) printf("30-39: ");
        else if (i == 4) printf("40-49: ");
        else printf("50+:  ");

        for (int j = 0; j < bins[i]; j += 2) printf("#"); // масштабируем график
        printf(" (%d)\n", bins[i]);
    }
}

void sort(int *data) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (data[j] < data[i]) {
                int tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
            }
        }
    }
}

void statistics(int *data) {
    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += data[i];
    }
    double average = sum / N;
    double median;
    if (N % 2 == 0) {
        median = (data[N/2 - 1] + data[N/2]) / 2.0;
    }
    else {
        median = data[N/2];
    }

    int minn = MAX_CORRECT;
    int maxx = MIN_CORRECT;
    for (int i = 0; i < N; i++) {
        if (data[i] >= MIN_CORRECT && data[i] <= MAX_CORRECT) {
            if (data[i] < minn) {
                minn = data[i];
            }
            if (data[i] > maxx) {
                maxx = data[i];
            }
        }
    }

    printf("Average: %.2f\n", average);
    printf("Median: %.2f\n", median);
    printf("Min: %d\n", minn);
    printf("Max: %d\n", maxx);
}

void print_array(int *data) {
    for (int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

int main() {
    int data_origin[N];
    int data[N];

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        if (rand() % 100 < 95) {
            data[i] = MIN_CORRECT + rand() % (MAX_CORRECT - MIN_CORRECT + 1);
        } else {
            if (rand() % 2 == 0) {
                data[i] = rand() % 3;
            } else {
                data[i] = 300 + rand() % 201;
            }
        }
        data_origin[i] = data[i];
    }
    sort(data);
    print_array(data);
    statistics(data);
    filter(data);
    sort(data);
    print_array(data);
    statistics(data);
    visualize(data, N, "Bubble Sort");


    return 0;
}
