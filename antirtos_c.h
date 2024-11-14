//-8<--8<--8<--ANTIRTOS_C BEGIN--8<--8<--8<--8<---
#define fQ(q, Q_SIZE)                            \
    volatile int q##_last = 0;                   \
    int q##_first = 0;                           \
    void (*q##_Queue[Q_SIZE])(void);             \
    int q##_Push(void (*pointerQ)(void)) {       \
        if ((q##_last + 1) % Q_SIZE == q##_first)\
            return 1; /* Queue is full */        \
        q##_Queue[q##_last++] = pointerQ;        \
        q##_last %= Q_SIZE;                      \
        return 0; /* Success */                  \
    }                                            \
    int (*q##_Pull(void))(void) {                \
        if (q##_last == q##_first)               \
            return 1; /* Queue is empty */       \
        q##_Queue[q##_first++]();                \
        q##_first %= Q_SIZE;                     \
        return 0;                                \
    }
	
#define fQP(q, Q_SIZE, param_type)                              \
    void (*q##_funcs[Q_SIZE])(param_type);                      \
    param_type q##_params[Q_SIZE];                              \
    volatile int q##_last = 0;                                  \
    int q##_first = 0;                                          \
    int q##_Push(void (*func)(param_type), param_type params) { \
        if ((q##_last + 1) % Q_SIZE == q##_first)               \
            return 1; /* Queue is full */                       \
        q##_funcs[q##_last] = func;                             \
        q##_params[q##_last++] = params;                        \
        q##_last %= Q_SIZE;                                     \
        return 0; /* Success */                                 \
    }                                                           \
    int q##_Pull(void) {                                        \
        if (q##_last == q##_first)                              \
            return 1; /* Queue is empty */                      \
        q##_funcs[q##_first](q##_params[q##_first++]);          \
        q##_first %= Q_SIZE;                                    \
        return 0; /* Success */                                 \
    }
//-8<--8<--8<--ANTIRTOS_C END --8<--8<--8<--8<---
