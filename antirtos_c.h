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
        return 0;  /* Success */ 		 \
    }

#define del_fQ(q, Q_SIZE)                                           \
    int q##_time = 0;                                               \
    void (*q##_del_fQueue[Q_SIZE])(void);                           \
    int q##_execArr[Q_SIZE] = { 0 };   	                            \
    int q##_execTime[Q_SIZE];                                       \
    fQ(q, Q_SIZE)                                                   \
    int q##_Push_delayed(void (*pointerF)(void), int delayTime){    \
        int q##_fullQ = 1;                                          \
        for (int i = 0; i < Q_SIZE; i++) {                          \
            if (!q##_execArr[i]) {                                  \
                q##_del_fQueue[i] = pointerF;                       \
                q##_execArr[i] = 1;                                 \
                q##_execTime[i] = q##_time + delayTime;             \
                q##_fullQ = 0;                                      \
                break;                                              \
            }                                                       \
        }                                                           \
        return q##_fullQ;                                           \
    }                                                               \
    void q##_Tick(void){                                            \
        for (int i = 0; i < Q_SIZE; i++) {                          \
            if (q##_execTime[i] == q##_time) {                      \
                if (q##_execArr[i]) {                               \
                    q##_Push(q##_del_fQueue[i]);                    \
                    q##_execArr[i] = 0;                             \
                }                                                   \
            }                                                       \
        }                                                           \
        q##_time++; /* Increment time */                            \
    }                                                               \
    int q##_Revoke(void (*pointerF)(void)){                         \
        int result = 1;                                             \
        for (int i = 0; i < Q_SIZE; i++) {                          \
            if (q##_del_fQueue[i] == pointerF) {                    \
                q##_execArr[i] = 0;                                 \
                result = 0;                                         \
            }                                                       \
        }                                                           \
        return result;                                              \
    }
	
#define fQP(q, Q_SIZE, param_type)                                      	\
    void (*q##_funcs[Q_SIZE])(param_type);                                      \
    param_type q##_params[Q_SIZE];                                              \
    volatile int q##_last = 0;                                                  \
    int q##_first = 0;                                                          \
    int q##_Push(void (*func)(param_type), param_type params) {                 \
        if ((q##_last + 1) % Q_SIZE == q##_first)                               \
            return 1; /* Queue is full */                                       \
        q##_funcs[q##_last] = func;                                             \
        q##_params[q##_last++] = params;                                        \
        q##_last %= Q_SIZE;                                                     \
        return 0; /* Success */                                                 \
    }                                                                           \
    int q##_Pull(void) {                                                        \
        if (q##_last == q##_first)                                              \
            return 1; /* Queue is empty */                                      \
        q##_funcs[q##_first](q##_params[q##_first++]);                          \
        q##_first %= Q_SIZE;                                                    \
        return 0; /* Success */                                                 \
    }
	
#define del_fQP(q, Q_SIZE, param_type)                                                  \
    int q##_time;                                                                       \
    void (*q##_del_fQueue[Q_SIZE])(param_type);                                         \
    param_type q##_del_params[Q_SIZE];                                                  \
    int q##_execArr[Q_SIZE] = { 0 };   	                                                \
    int q##_execTime[Q_SIZE];                                                           \
    fQP(q, Q_SIZE, param_type)                                                          \
    int q##_Push_delayed(void (*func)(param_type), param_type params, int delayTime){   \
        int q##_fullQ = 1;                                                              \
        for (int i = 0; i < Q_SIZE; i++) {                                              \
            if (!q##_execArr[i]) {                                                      \
                q##_del_fQueue[i] = func;                                               \
                q##_del_params[i] = params;                                             \
                q##_execArr[i] = 1;                                                     \
                q##_execTime[i] = q##_time + delayTime;                                 \
                q##_fullQ = 0;                                                          \
                break;                                                                  \
            }                                                                           \
        }                                                                               \
        return q##_fullQ;                                                               \
    }                                                                                   \
    void q##_tick(void){                                                                \
        for (int i = 0; i < Q_SIZE; i++) {                                              \
            if ( q##_execTime[i] == q##_time){                                          \
                if (q##_execArr[i]) {                                                   \
                    q##_Push(q##_del_fQueue[i], q##_del_params[i]);                     \
                    q##_execArr[i] = 0;                                                 \
                }                                                                       \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
    int q##_revoke(void (*func)(param_type)){                                           \
        int result = 1;                                                                 \
        for (int i = 0; i < Q_SIZE; i++) {                                              \
            if (q##_del_fQueue[i] == func) {                                            \
                q##_execArr[i] = false;                                                 \
                result = 0;                                                             \
            }                                                                           \
        }                                                                               \
        return result;                                                                  \
    }
	
