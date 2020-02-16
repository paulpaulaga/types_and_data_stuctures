#include "../include/taskqueue.h"

#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void task_init(tqueue_t *t)
{
    clock_t timer, time;
    int buf;

    lqueue_init(&t->lq);
    aqueue_init(&t->aq);

    t->first_passed = 0;

    // TIME INIT
    if (t->sys.state == 'a')
    {
        time = 0;
        for (int i = 0; i < AQUEUE_LEN; ++i)
        {
            timer = clock();
            aqueue_push(&t->aq, i);
            timer = clock() - timer;
            time += timer;
        }
        t->mean_push_time = ((double)time) / AQUEUE_LEN;

        time = 0;
        for (int i = 0; i < AQUEUE_LEN; ++i)
        {
            timer = clock();
            aqueue_pop(&t->aq, &buf);
            timer = clock() - timer;
            time += timer;
        }
        t->mean_pop_time = ((double)time) / AQUEUE_LEN;
    }

    if (t->sys.state == 'a')
    {
        time = 0;
        for (int i = 0; i < AQUEUE_LEN; ++i)
        {
            timer = clock();
            lqueue_push(&t->lq, i);
            timer = clock() - timer;
            time += timer;
        }
        t->mean_push_time = ((double)time) / AQUEUE_LEN;

        time = 0;
        for (int i = 0; i < AQUEUE_LEN; ++i)
        {
            timer = clock();
            lqueue_pop(&t->lq, &buf);
            timer = clock() - timer;
            time += timer;
        }
        t->mean_pop_time = ((double)time) / AQUEUE_LEN;
    }
    // MEMORY INIT
    
    t->lq_size = (int)(sizeof(lqueue_t) + sizeof(struct list_node_t) * QUEUE_SIZE);
    t->aq_size = (int)(sizeof(aqueue_t));
    // PREDICTION
    t->predicted_time = TIME_UNIT * SECOND_PASS * 10;

    t->first_passed = 0;
}

int go_to_second_prob()
{
    return (random() % 100 + 1) <= SECOND_PROB;
}

clock_t wait_time(int start, int end)
{
    return TIME_UNIT * (random() % (end - start + 1) + start);
}

void system_init(system_t *sys)
{
    sys->second_passed = 0;

    sys->first_working = 0;
    sys->second_working = 0;

    sys->second_passed = 0;

    sys->fa_buf = -1;
    sys->sa_buf = -1;

    if (sys->state == 'a')
    {
        aqueue_init(&sys->faq);
        aqueue_init(&sys->saq);
    }
    else
    {
        lqueue_init(&sys->flq);
        lqueue_init(&sys->slq);
    }
}

int task_task(tqueue_t *t)
{
    srand(time(NULL));
    system_t *s = &t->sys;
    task_init(t);
    system_init(s);
    task_fill_first(t);
    aqueue_print(&s->faq);

    t->main_time = t->sys.current_downtime = clock();

    task_first_to_a(t);
    while (t->sys.second_passed < SECOND_PASS)
    {
        if (t->sys.first_working && (clock() - t->sys.fstart) > t->sys.ftime)
        {

            if (go_to_second_prob())
                task_first_to_second(t);
            else
                task_first_to_first(t);
        }
        if (t->sys.second_working && (clock() - t->sys.sstart) > t->sys.stime)
        {
            task_second_to_first(t);
            if (t->sys.second_passed % SECOND_STEP == 0)
                task_print_info(t);
        }
    }
    t->main_time = clock() - t->main_time;
    task_print_result(t);
    return 1;
}

void task_first_to_first(tqueue_t *t)
{
    if (t->sys.state == 'a')
        aqueue_push(&t->sys.faq, t->sys.fa_buf);
    else
        lqueue_push(&t->sys.flq, t->sys.fa_buf);
    
    // printf("Из первого автомата в первую очередь направляется %d\n", t->sys.fa_buf);
    //aqueue_print(&t->sys.faq);


    ++(t->sys.first_size);
    t->f_meansize = (double)((t->f_meansize * t->f_meansize_m) + t->sys.first_size) / (++t->f_meansize_m);

    t->sys.first_working = 0;

    t->sys.fa_buf = -1; // debug

    if (t->sys.first_size)
        task_first_to_a(t);
    return;
}

void task_first_to_second(tqueue_t *t)
{
    if (t->sys.state == 'a')
        aqueue_push(&t->sys.saq, t->sys.fa_buf);
    else
        lqueue_push(&t->sys.slq, t->sys.fa_buf);

    // printf("Из первого автомата во вторую очередь направляется %d\n", t->sys.fa_buf);
    // aqueue_print(&t->sys.saq);

    ++(t->sys.second_size);
    t->s_meansize = (double)((t->s_meansize * t->s_meansize_m) + t->sys.second_size) / (++t->s_meansize_m);

    t->sys.first_working = 0;

    t->sys.fa_buf = -1; // debug

    if (t->sys.first_size)
        task_first_to_a(t);
    if (t->sys.second_size == 1 && !t->sys.second_working)
        task_second_to_a(t);
}

void task_second_to_first(tqueue_t *t)
{
    if (t->sys.state == 'a')
        aqueue_push(&t->sys.faq, t->sys.sa_buf);
    else
        lqueue_push(&t->sys.flq, t->sys.sa_buf);
    
    // printf("Из второго автомата в первую очередь направляется %d\n", t->sys.sa_buf);
    // aqueue_print(&t->sys.faq);

    ++(t->sys.first_size);
    t->f_meansize = (double)((t->f_meansize * t->f_meansize_m) + t->sys.first_size) / (++t->f_meansize_m);

    t->sys.second_working = 0;

    t->sys.sa_buf = -1; // debug;

    if (!t->sys.second_size)
        t->sys.current_downtime = clock();

    if (t->sys.first_size == 1)
        task_first_to_a(t);
    if (t->sys.second_size)
        task_second_to_a(t);

    ++(t->sys.second_passed);
    return;
}

void task_first_to_a(tqueue_t *t)
{
    if (t->sys.state == 'a')
        aqueue_pop(&t->sys.faq, &t->sys.fa_buf);
    else
        lqueue_pop(&t->sys.flq, &t->sys.fa_buf);
    
    // printf("%d отправляется из первой очереди в автомат\n", t->sys.fa_buf);
    // aqueue_print(&t->sys.faq);

    --(t->sys.first_size);
    t->f_meansize = (double)((t->f_meansize * t->f_meansize_m) + t->sys.first_size) / (++t->f_meansize_m);

    t->sys.ftime = wait_time(0, 6);
    t->sys.first_working = 1;

    ++(t->first_passed);

    t->sys.fstart = clock();
}

void task_second_to_a(tqueue_t *t)
{
    if (t->sys.state == 'a')
        aqueue_pop(&t->sys.saq, &t->sys.sa_buf);
    else
        lqueue_pop(&t->sys.slq, &t->sys.sa_buf);
    t->downtime += clock() - t->sys.current_downtime;


    // printf("%d отправляется из второй очереди в автомат\n", t->sys.sa_buf);
    // aqueue_print(&t->sys.saq);


    --(t->sys.second_size);
    t->s_meansize = (double)((t->s_meansize * t->s_meansize_m) + t->sys.second_size) / (++t->s_meansize_m);

    t->sys.second_working = 1;
    t->sys.stime = wait_time(1, 8);

    t->sys.sstart = clock();
    return;
}

void task_print_info(tqueue_t *t)
{
    printf("Через второй автомат прошло %d заявок\n", t->sys.second_passed);
    printf("Текущий размер первой очереди: %d\nТекущий размер второй очереди: %d\n",
           t->sys.first_size, t->sys.second_size);

    printf("Средний размер первой очереди: %.3lf\n", t->f_meansize);
    printf("Средний размер второй очереди: %.3lf\n" , t->s_meansize);

    /*
    printf("Первая очередь: ");
    aqueue_print(&t->sys.faq);
    printf("Вторая очередь: ");
    aqueue_print(&t->sys.saq);
    printf("Элементы в автоматах: %d %d\n", t->sys.fa_buf, t->sys.sa_buf);
    */
   printf("\n");
}

void task_fill_first(tqueue_t *t)
{
    if (t->sys.state == 'a')
        for (int i = 0; i < QUEUE_SIZE; ++i)
            aqueue_push(&t->sys.faq, i + 1);
    else
        for (int i = 0; i < QUEUE_SIZE; ++i)
            lqueue_push(&t->sys.flq, i + 1);

    // aqueue_print(&t->sys.faq);
    // aqueue_print(&t->sys.saq);

    t->sys.first_size = QUEUE_SIZE;
    t->f_meansize = QUEUE_SIZE;
    t->s_meansize = 0;
    t->f_meansize_m = 1;
    t->s_meansize_m = 0;
    t->sys.second_size = 0;
}

void task_print_result(tqueue_t *t)
{
    printf("Ожидаемое время моделирования: %.3lf сек.\n", (double)t->predicted_time / CLOCKS_PER_SEC);
    printf("Время моделирования: %.3lf сек.\n", (double)t->main_time / CLOCKS_PER_SEC);
    printf("Погрешность: %.2lf%%\n", 100 * fabs(t->predicted_time - t->main_time) / t->predicted_time);
    printf("Средний размер первой очереди: %.3lf\n", t->f_meansize);
    printf("Средний размер второй очереди: %.3lf\n" , t->s_meansize);
    printf("Через первый автомат прошло %d заявок\n", t->first_passed);

    printf("Очередь, реализованная массивом занимает %d байт памяти\n", t->aq_size);
    printf("Очередь, реализованная списком занимает %d байт памяти\n", t->lq_size);
}

