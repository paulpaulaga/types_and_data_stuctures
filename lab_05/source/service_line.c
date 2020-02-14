#include "service_line.h"

#include <time.h>

static void stub()
{
    return;
}

int aqueue_line()
{
    clock_t a1, a2; 
    clock_t full_time;
    double a1_time = generate_time(0, 6), a2_time = generate_time(1, 8); // a1 from 0 to 6, a2 from 1 to 8
    aqueue q1, q2;
    // srand(time(NULL)); // TODO когда заработает
    int err = SUCCESS;
    int q1_request, q2_request;
    int a1_handling = 1, a2_handling = 0; // флаг работы обрабатывающих автоматов


    int q1_size = 99, q2_size = 0;
    double q1_mid_size = 99, q2_mid_size = 0;
    int second_requests = 0;
    int q1_updates = 0, q2_updates = 2;

    aqueue_line_start(&q1, &q2);

    puts("Array queue test");
    puts("Source queue");
    aqueue_print(q1);
    puts("\n\nSTART\n\n");


    aqueue_pop(&q1, &q1_request);
    a1 = full_time = clock();
    
    while ((second_requests < SECOND_REQUEST_COUNT) && (a1_handling || a2_handling))
    {
        // проверка времени выполняется только в случае работы автомата
        // обработка действий первой очереди и автомата
        if (a1_handling)
        {
            if ((double)(clock() - a1) / CLOCKS_PER_SEC > a1_time)
            {
                // printf("----Handle %d; time = %lf\n", q1_request, (double)(clock() - a1) / CLOCKS_PER_SEC);
                // aqueue_print(q1);
                // aqueue_print(q2);

                // отправка запроса после его обработки автоматом
                switch (direction_choice(0.3, 0.7)) // выбор направления движения заявки
                {
                case 1:
                    // puts("2");

                    a1_handling = 0; // прекращение работы автомата
                    
                    aqueue_push(&q2, q1_request);

                    ++q2_size;
                    q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                    ++q2_updates;

                    if (!a2_handling && q2_size == 1) // если заявка добавилась в пустую очередь
                    {
                        aqueue_pop(&q2, &q2_request);

                        --q2_size;
                        q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                        ++q2_updates;

                        a2_handling = 1;
                        a2_time = generate_time(1, 8);
                        a2 = clock();
                    }
                    break;
                case 2:
                    // puts("1");

                    a1_handling = 0; // прекращение работы автомата

                    aqueue_push(&q1, q1_request);

                    ++q1_size;
                    q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                    ++q1_updates;

                    break; 
                }
                
                // направление заявки из первой очереди в автомат
                if (q1_size > 0)
                {
                    a1_time = generate_time(0, 6);
                    aqueue_pop(&q1, &q1_request);

                    --q1_size;
                    q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                    ++q1_updates;

                    a1_handling = 1;
                    a1_time = generate_time(0, 6);
                    a1 = clock();
                }
            }
        }


        // обработка действий второй очереди и автомата
        if (a2_handling)
        {
            if ((double)(clock() - a2) / CLOCKS_PER_SEC > a2_time)
            {
                // printf("++++Handle %d (second auto); time = %lf\n", q2_request, (double)(clock() - a2) / CLOCKS_PER_SEC);
                // aqueue_print(q1);
                // aqueue_print(q2);

                a2_handling = 0; // прекращение работы автомата
                ++second_requests; 

                // направление обработанной заявки в первую очередь
                aqueue_push(&q1, q2_request);

                ++q1_size;
                q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                ++q1_updates;

                if (second_requests % SECOND_REQUEST_STEP == 0)
                {
                    printf("%d requests passed through second automat\n", second_requests);
                    printf("First queue size = %d\nSecond queue size = %d\n", q1_size, q2_size);
                    printf("First queue mean size: %.5lf\n", q1_mid_size);
                    printf("Second queue mean size: %.5lf\n", q2_mid_size);
                    // aqueue_print(q1);
                    aqueue_print(q2);
                    puts("\n\n");
                }


                // направление заявки из второй очереди в автомат
                if (q2_size > 0)
                {
                    --q2_size;
                    q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                    ++q2_updates;

                    aqueue_pop(&q2, &q2_request);
                    a2_time = generate_time(1, 8);
                    a2_handling = 1;
                    a2 = clock();
                }
            }
        }
    }
    printf("Full time: %lf\n\n", (double) (clock() - full_time) / CLOCKS_PER_SEC);
    return err;
}


int lqueue_line()
{
    clock_t a1, a2; 
    clock_t full_time;
    clock_t current_a2_downtime;

    double a2_downtime = 0;
    double a1_time = generate_time(0, 6), a2_time = generate_time(1, 8); // a1 from 0 to 6, a2 from 1 to 8

    lqueue q1, q2;

    // srand(time(NULL)); // TODO когда заработает

    int err = SUCCESS;
    int q1_request, q2_request;
    int a1_handling = 1, a2_handling = 0; // флаг работы обрабатывающих автоматов

    int q1_size = 99, q2_size = 0;
    double q1_mid_size = 99, q2_mid_size = 0;
    int first_requests = 0, second_requests = 0;
    int q1_updates = 0, q2_updates = 2;

    lqueue_line_start(&q1, &q2);

    puts("Array queue");
    puts("\n\nSTART\n\n");

    lqueue_pop(&q1, &q1_request);
    a1 = full_time = current_a2_downtime = clock();
    while ((second_requests < SECOND_REQUEST_COUNT) && (a1_handling || a2_handling))
    {
        // проверка времени выполняется только в случае работы автомата
        // обработка действий первой очереди и автомата
        if (a1_handling)
        {
            if ((double)(clock() - a1) / CLOCKS_PER_SEC > a1_time)
            {
                // printf("----Handle %d; time = %lf\n", q1_request, (double)(clock() - a1) / CLOCKS_PER_SEC);
                // lqueue_print(q1);
                // lqueue_print(q2);
                ++first_requests;

                // отправка запроса после его обработки автоматом
                switch (direction_choice(0.3, 0.7)) // выбор направления движения заявки
                {
                case 1:
                    // puts("2");

                    a1_handling = 0; // прекращение работы автомата
                    
                    lqueue_push(&q2, q1_request);

                    ++q2_size;
                    q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                    ++q2_updates;

                    if (!a2_handling && q2_size == 1) // если заявка добавилась в пустую очередь
                    {
                        lqueue_pop(&q2, &q2_request);

                        --q2_size;
                        q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                        ++q2_updates;

                        a2_handling = 1;
                        a2_time = generate_time(1, 8);
                        a2 = clock();

                        a2_downtime += (double)(clock() - current_a2_downtime) / CLOCKS_PER_SEC; // обновление времени простоя 
                        // второго автомата
                    }
                    break;
                case 2:
                    // puts("1");

                    a1_handling = 0; // прекращение работы автомата

                    lqueue_push(&q1, q1_request);

                    ++q1_size;
                    q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                    ++q1_updates;

                    break; 
                }
                
                // направление заявки из первой очереди в автомат
                if (q1_size > 0)
                {
                    a1_time = generate_time(0, 6);
                    lqueue_pop(&q1, &q1_request);

                    --q1_size;
                    q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                    ++q1_updates;

                    a1_handling = 1;
                    a1_time = generate_time(0, 6);
                    a1 = clock();
                }
            }
        }


        // обработка действий второй очереди и автомата
        if (a2_handling)
        {
            if ((double)(clock() - a2) / CLOCKS_PER_SEC > a2_time)
            {
                // printf("++++Handle %d (second auto); time = %lf\n", q2_request, (double)(clock() - a2) / CLOCKS_PER_SEC);
                // lqueue_print(q1);
                // lqueue_print(q2);

                a2_handling = 0; // прекращение работы автомата
                ++second_requests; 

                // направление обработанной заявки в первую очередь
                lqueue_push(&q1, q2_request);

                ++q1_size;
                q1_mid_size = (q1_mid_size * q1_updates + q1_size) / (q1_updates + 1);
                ++q1_updates;

                if (second_requests % SECOND_REQUEST_STEP == 0)
                {
                    printf("%d requests passed through second automat\n", second_requests);
                    printf("First queue size = %d\nSecond queue size = %d\n", q1_size, q2_size);
                    printf("First queue mean size: %.2lf\n", q1_mid_size);
                    printf("Second queue mean size: %.2lf\n", q2_mid_size);
                    // lqueue_print(q1);
                    // lqueue_print(q2);
                    puts("\n\n");
                }


                // направление заявки из второй очереди в автомат
                if (q2_size > 0)
                {
                    --q2_size;
                    q2_mid_size = (q2_mid_size * q2_updates + q2_size) / (q2_updates + 1);
                    ++q2_updates;

                    lqueue_pop(&q2, &q2_request);
                    a2_time = generate_time(1, 8);
                    a2_handling = 1;
                    a2 = clock();

                    a2_downtime += (double)(clock() - current_a2_downtime) / CLOCKS_PER_SEC; // обновление времнеи простоя
                    // второго автомата
                }
                else if (!a2_handling)
                    current_a2_downtime = clock(); // начало времени простоя вторго автомата
            }
        }
    }
    
    // Вывод итоговой информации работы системы обслуживания
    printf("Full time: %lf\n", (double) (clock() - full_time) / CLOCKS_PER_SEC);
    printf("Second automat downtime: %lf\n", a2_downtime);
    printf("First automat was activated %d times\n\n", first_requests);

    lqueue_delete(&q1);
    lqueue_delete(&q2);
    return err;
}

int main_line()
{
    clock_t work_time;


    clock_t a1, a2;

    lqueue lq1, lq2;
    aqueue aq1, aq2;

    double a1_time = generate_time(0, 6), a2_time = generate_time(1, 8);
    int second_requests = 0;
    int a1_handling = 1, a2_handling = 0;
    int q1_size = REQUEST_COUNT - 1, q2_size = 0;
    int q1_request, q2_request;


    lqueue_line_start(&lq1, &lq2);
    aqueue_line_start(&aq1, &aq2);

    aqueue_print(aq1);
    lqueue_print(lq1);
    aqueue_print(aq2);
    lqueue_print(lq2);

    work_time = clock();

    aqueue_pop(&aq1, &q1_request);
    lqueue_pop(&lq1, &q1_request);
    a1 = clock();

    while ((second_requests < SECOND_REQUEST_COUNT) && (a1_handling || a2_handling))
    {
        if (a1_handling && (double)(clock() - a1) / CLOCKS_PER_SEC > a1_time)
        {
            a1_handling = 0;

            switch(direction_choice(0.3, 0.7))
            {
                case 1:
                    lqueue_push(&lq2, q1_request);
                    aqueue_push(&aq1, q1_request);
                    ++q2_size;

                    if (!a2_handling && q2_size == 1) // CHECK
                    {
                        lqueue_pop(&lq2, &q2_request);
                        aqueue_pop(&aq2, &q2_request);
                        --q2_size;

                        a2_handling = 1; 
                        a2_time = generate_time(1, 8);
                        a2 = clock();
                    }

                    break;
                case 2:
                    lqueue_push(&lq1, q1_request);
                    aqueue_push(&aq1, q1_request);
                    ++q1_size;

                    break;
            }

            if (q1_size > 0)
            {
                a1_time = generate_time(0, 6);

                lqueue_pop(&lq1, &q1_request);
                aqueue_pop(&aq1, &q1_request);
                --q1_size;

                a1_handling = 1;
                a1 = clock();
            }
        }

        if (a2_handling && (double)(clock() - a2) / CLOCKS_PER_SEC > a2_time)
        {
            a2_handling = 0;
            ++second_requests;

            lqueue_push(&lq1, q2_request);
            aqueue_push(&aq1, q2_request);
            ++q1_size;

            if (second_requests % SECOND_REQUEST_STEP == 0)
            {
                aqueue_print(aq1);
                lqueue_print(lq1);
                aqueue_print(aq2);
                lqueue_print(lq2);

                print_info(q2_request, q1_size, 0, q2_size, 0);
            }

            if (q2_size > 0)
            {
                a2_time = generate_time(0, 6);

                lqueue_pop(&lq1, &q1_request);
                aqueue_pop(&aq1, &q1_request);
                --q1_size;

                a2_handling = 1;
                a2 = clock();
            }
        }
    }

    work_time = clock() - work_time;

    print_end(work_time, 0, 0, 0);

    return 1;
}

int lqueue_line_start(lqueue_t q1, lqueue_t q2)
{
    lqueue_init(q1);
    lqueue_init(q2);
    for (int i = 0; i < REQUEST_COUNT; ++i)
        lqueue_push(q1, i + 100);
    return REQUEST_COUNT;
}

int aqueue_line_start(aqueue_t q1, aqueue_t q2)
{
    aqueue_init(q1);
    aqueue_init(q2);
    for (int i = 0; i < REQUEST_COUNT; ++i)
        aqueue_push(q1, i + 100);
    return REQUEST_COUNT;
}


int direction_choice(double right_p, double down_p)
{
    double elem = (double)rand() / 100;
    elem = elem - (long)elem;
    if (elem < right_p)
        return 1;
    else
        return 2;
}

double generate_time(int start_units, int end_units)
{
    return (double)((rand() % (end_units - start_units + 1)) + start_units) / TIME_UNIT;
}

void lqueue_expected_time()
{
    puts("Linked list queue expected work time is"); // TODO 
    
}

void aqueue_expected_time()
{
    puts("Array queue expected work time is"); // TODO
}

void print_info(int requests, int q1_size, double q1_mid_size, int q2_size, double q2_mid_size)
{
    printf("%d requests passed out of second queue\n", requests);
    printf("First queue size: %d\n", q1_size);
    printf("First queue mean size: %lf\n", q1_mid_size);
    printf("Second queue size: %d\n", q2_size);
    printf("Second queue mean size: %lf\n\n", q2_mid_size);
}

void print_end(double work_time, double down_time, double mid_q1_size, double mid_q2_size)
{
    stub();
    printf("Real modelling time: %lf\n", work_time);
    printf("Down time: %lf\n", down_time);
    printf("Mean first queue size: %lf\n", mid_q1_size);
    printf("Mean second queue size: %lf\n", mid_q2_size);
}
