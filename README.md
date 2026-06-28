![ANTIRTOS_C](https://github.com/WeSpeakEnglish/images/raw/main/ANTIRTOS_C.png)

# ANTIRTOS_C

### *No RTOS needed, you will C!*

ANTIRTOS_C is an ultra-lightweight, robust, and efficient pure C library for task management in IoT and embedded applications. Implemented as a single header file of preprocessor macros, it requires no dynamic memory allocation, no C++ compiler, and no RTOS — just drop in `antirtos_c.h` and go.

Four macro variants cover every common need: **fQ** is the basic no-parameter queue, **fQP** adds a typed argument to queued functions, **del_fQ** adds a delay before execution, and **del_fQP** adds both.

Each queue expands to a statically allocated array of function pointers and a pair of generated functions: `_Push` adds a task to the back, `_Pull` executes the next task and removes it. Several queues can coexist, and all execution happens outside interrupts in a fully non-blocking way.

Prefer C++? See [ANTIRTOS](https://github.com/WeSpeakEnglish/ANTIRTOS) and [ANTIRTOS_MODERN](https://github.com/WeSpeakEnglish/ANTIRTOS_MODERN).  
Also on [Hackaday.io](https://hackaday.io/project/199046-antirtoscuno).

### Benefits

1. Interrupts kept fast and controllable — push a pointer and return immediately, no logic executes in the ISR.
2. Easy to debug and understand — no hidden scheduler, no task stacks, no magic.
3. Simple multitasking without critical sections, mutexes, or semaphores.
4. No dummy waiting or blocking. Wait by doing!
5. No heap allocation — all state is statically declared by the macro.
6. Pure C (C99) — works on AVR, PIC, STM32, RISC-V, and any toolchain that has a C compiler.

---

## Queues without parameters — `fQ`

### 1. Include and declare queues globally

```c
#include "antirtos_c.h"

fQ(Q1, 8); // queue named Q1, 8 slots (holds up to 7 tasks)
fQ(Q2, 8); // queue named Q2, 8 slots
```

> One slot is always reserved to distinguish full from empty, so a queue of size N holds N−1 tasks.

### 2. Define your tasks

```c
void yourTaskOne(void) {
    // whatever you need to do
}

void yourTaskTwo(void) {
    // whatever you need to do
}
```

### 3. Pull in the main loop

```c
int main(void) {          // or loop(){} on Arduino
    while (1) {
        Q1_Pull();
        Q2_Pull();
    }
}
```

### 4. Push from anywhere — ISRs, callbacks, other tasks

```c
void ISR_1(void) {
    Q1_Push(yourTaskOne); // ISR returns immediately; task runs in main loop
}
void ISR_2(void) {
    Q2_Push(yourTaskTwo);
}
```

*That's it. Interrupts stay fast; tasks are handled safely outside the ISR.*

---

## Queues with parameters — `fQP`

### 1. Declare queues with a parameter type

```c
#include "antirtos_c.h"

fQP(Q1, 8, int);  // Q1: 8 slots, functions receive int
fQP(Q2, 8, char); // Q2: 8 slots, functions receive char
```

### 2. Define typed tasks

```c
void blinkLED(int led) {
    switch (led) {
        case 0: HAL_GPIO_TogglePin(LED_YELLOW_PORT, LED_YELLOW_PIN); break;
        case 1: HAL_GPIO_TogglePin(LED_GREEN_PORT,  LED_GREEN_PIN);  break;
    }
}

void printSymbol(char ch) {
    printf("Symbol: %c\n", ch);
}
```

### 3. Pull in the main loop

```c
int main(void) {
    while (1) {
        Q1_Pull();
        Q2_Pull();
    }
}
```

### 4. Push with parameter from ISR

```c
void ISR_1(void) { Q1_Push(blinkLED,    0);   }
void ISR_2(void) { Q2_Push(printSymbol, 'a'); }
```

### Multiple parameters? Use a struct.

```c
typedef struct { int index; int logic; } pinout;

void myTask(pinout p) { /* ... */ }

fQP(Q3, 8, pinout); // queue with struct type

// push:
Q3_Push(myTask, (pinout){0, 1});

// pull in main loop:
Q3_Pull();
```

---

## Delayed queues without parameters — `del_fQ`

Do you need to delay a function from execution? Do not wait any more!

### 1. Declare

```c
del_fQ(Q4, 8); // 8-slot delayed queue
```

### 2. Push with a delay (in ticks)

```c
Q4_Push_delayed(your_func_1, 1000); // your_func_1() executes after 1000 ticks
Q4_Push_delayed(your_func_2, 2000); // your_func_2() executes after 2000 ticks
```

### 3. Pull in the main loop

```c
Q4_Pull();
```

### 4. Tick from a timer ISR

```c
void TimerISR(void) {
    Q4_Tick(); // call once per tick period (e.g. every 1 ms)
}
```

### 5. Revoke a pending function

```c
Q4_Revoke(your_func_1); // cancels all pending your_func_1 entries
```

---

## Delayed queues with parameters — `del_fQP`

### 1. Declare

```c
del_fQP(Q5, 8, int); // 8-slot delayed queue, functions receive int
```

### 2. Push with parameter and delay

```c
Q5_Push_delayed(your_func_4, -15, 1000); // your_func_4(-15) after 1000 ticks
Q5_Push_delayed(your_func_5, -27, 2000); // your_func_5(-27) after 2000 ticks
```

### 3. Pull in the main loop

```c
Q5_Pull();
```

### 4. Tick from a timer ISR

```c
void TimerISR(void) {
    Q5_Tick();
}
```

### 5. Revoke a pending function

```c
Q5_Revoke(your_func_4);
```

---

## Wait by doing

Instead of blocking with `delay()` or a busy loop, keep pulling tasks while you wait:

```c
void waitOnQ1(unsigned int ms) {
    unsigned int target = millis() + ms;
    while (millis() < target)
        Q1_Pull();
}

// usage:
digitalWrite(13, HIGH);
waitOnQ1(1000);     // 1 second passes — tasks still run
val = analogRead(3);
```

Or waiting for a condition:

```c
while (!neededFlag) {
    Q1_Pull();
    Q2_Pull();
}
```

---

## Tips

- **Use queue sizes that are powers of 2** (4, 8, 16, 32, 64, 128) — the compiler replaces `% N` with a fast bitwise AND.

- **One queue per interrupt** if interrupt priorities are uncertain. Mixing multiple ISRs into one queue is safe only when pushes cannot preempt each other.

- **Group tasks by execution time** for better responsiveness:
  1. Split functions into fast, medium, and slow groups, and give each group its own queue.
  2. Pull fast tasks while waiting inside medium ones.
  3. Pull medium and fast tasks while waiting inside slow ones.

- **Maximum Q_SIZE is 256**, holding up to 255 tasks. Queue indices are `volatile unsigned char` — a single-instruction atomic read/write on both 8-bit (AVR, PIC) and 32-bit (Cortex-M, RISC-V) MCUs.

---

Try it: [Tinkercad simulation (Arduino/AVR)](https://www.tinkercad.com/things/dTtzrpBL7Oz-antirtoscuno)

**Keep all interrupts extremely fast and never lose them. Use ANTIRTOS_C!**

