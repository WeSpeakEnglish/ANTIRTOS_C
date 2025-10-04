# ANTIRTOS#C
<img src="https://github.com/WeSpeakEnglish/images/blob/main/ANTIRTOS_C.png">

No RTOS need, you will C! 
Exteamly light, pure C preprocessor macros-based universal C library designed for task management in IoT and embedded applications. It is coded in a single, small file, making it incredibly easy to integrate into your projects. Enjoy the real simplicity!  
Also: [here](https://hackaday.io/project/199046-antirtosc) 

Prefer C++? It is [ANTIRTOS C++ library](https://github.com/WeSpeakEnglish/ANTIRTOS) here and also fresh new [ANTIRTOS_MODERN](https://github.com/WeSpeakEnglish/ANTIRTOS_MODERN)

## Conveyers of functions without parameters

### Usage

### 1. Initialize needed queues like global prototypes (as many as you need, here are two like example):
```c
#include <antirtos_c.h>
  fQ(Q1,8); // define the first queue (type fQ) with name Q1, 8 elements length
  fQ(Q2,8); // define the second queue (type fQ) with name Q2, 8 elements length
```
### 2. Define your tasks:
```c
void yourTaskOne(){
//put here what ever you want to execute
}

void yourTaskTwo(){
//put here what ever you want to execute
}
```
### 3. In main loop (loop(){} instead of main(){} for Arduino) just pull from the queues
```c
void main(){ // or loop{} for Arduino
  Q1_Pull(); // pull from the Q1 and execute
  Q2_Pull(); // pull from the Q2 and execute
}
```
### 4. Wherever you want, you can now push your tasks, they will be handled! (for example in some interrupts)
```c
void ISR_1(){
  Q1_Push(yourTaskOne);  // just push your task into queue!
}
void ISR_2(){
  Q2_Push(yourTaskTwo);  // just push your task into queue!
}
```
*This is it! All the interrupts are kept extreamly fast, all the task handled*

## Conveyers of functions with parameters
Imagine now you need to pass parameters to functions in queue


### 1. Define all your queues:
```c
#include <antirtos_c.h>
 fQP(Q1,8,int);  //Q1 is 8 elements length, type fQP, functions receive int
 fQP(Q2,8,char); //Q2 is 8 elements length, type fQP, functions receive char
```
### 2. Define all your tasks, for example:
```c
#include <antirtos_c.h>
void blinkLED(int led){
  switch(led){
    case 0: 
      HAL_GPIO_TogglePin(LED_YELLOW_PORT, LED_YELLOW_PIN);
      break;
    case 1:
      HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
      break;    
  }

void printSymbol(char ch){
 printf("Symbol: %c\n", ch);
}
```
### 3. In main loop (loop{} instead of main(){} for Arduino) just add:
```c
void main(){
  Q1_Pull();  // pull from the Q1 and execute
  Q2_Pull();  // pull from the Q2 and execute
}
```
### 4. Where you want just push the tasks into queues and pass them parameter (for example in some interrupts again):
```c
void ISR_1(void){
  Q1_Push(blinkLED, 0); // just push your task into Q1 queue and parameter!
  }
void ISR_2(void){ 
  Q2_Push(printSymbol, 'a'); // just push your task into Q2 queue and parameter!
  }
```
### More parameters needed?
Just wrap them into your custom structure, for example:
```c
typedef struct {
    int index;
    int logic;
} pinout;
```
Task functions will receive here type *pinout*:
```c
void myTask(pinout p){
// put here what ever you want, your task
}
```
Now you may initialize your queues, like:
```c
#include <antirtos_c.h>
fQP(Q3,8,pinout);  //Q3 is 8 elements length, type fQP, functions receive type 'pinout'
```
Push parameters now like:
```c
 Q3_Push(myTask, (pinout){0,1}); //push your task into Q3 and passing arguments 
```
 In main loop (loop{} instead of main(){} for Arduino) as usual:
```c
void main(void){
  Q3_Pull();
}
```
## Delayed conveyers of functions without parameters
Do you need just to delay some function from execution? Do not wait any more! 
### 1. Initialize:
```c
#include <antirtos_c.h>
del_fQ(Q4,8); //Q4 is 8 elements length delayed queue
```
### 2. Put where you want your tasks into queue, specifying delay (here example of 2 functions put into queue):
```c
Q4_Push_delayed(your_func_1, 1000); // function your_func_1() will be delayed for 1000 'ticks' (see calling Q4_Tick below)
Q4_Push_delayed(your_func_2, 2000); // function your_func_2() will be delayed for 2000 'ticks'
```
### 3. In the main loop (in lopp(){} for Arduino) just need to:
```c
Q4_Pull();
```
### 4. In some timer or periodic function:
```c
Q4_Tick();
```
That is it! 
### 5. If you need to revoke a function from the delayed conveyer, use revocation 
For example, you need to revoke **your_func_1**:
```c
Q4_Revoke(your_func_1);
```

## Delayed conveyers of functions with parameters
Do you need just to delay some function from execution? Do not wait any more! 
### 1. Initialize:
```c
#include <antirtos_c.h>
del_fQP(Q5,8,int); //Q4 is 8 elements length delayed queue, int is example type of parameter - it may be your structure or standard one (float, int, long, char...)
```
### 2. Put where you want your tasks into queue, specifying delay (here example of 2 different functions put into the same queue):
```c
Q5_Push_delayed(your_func_4, -15, 1000); // function your_func_4(-15) will be delayed for 1000 'ticks' (see calling Q4_Tick below), -15 here is parameter of *int* type
Q5_Push_delayed(your_func_5, -27, 2000); // function your_func_5(-27) will be delayed for 2000 'ticks', -27 here is parameter of *int* type
```
### 3. In the main loop (in lopp(){} for Arduino) just need to:
```c
Q5_Pull();
```
### 4. In some timer or periodic function:
```c
Q5_Tick();
```
That is it! 
### 5. If you need to revoke a function from the delayed conveyer, use revocation 
For example, you need to revoke **your_func_4**:
```c
Q5_Revoke(your_func_4);
```

**All functions are handled and executed automatically. Do not waste time — keep all interrupts extremely fast and never lose them! Use ANTIRTOS!**

See [the press button simulation on Thinkercad](https://www.tinkercad.com/things/dTtzrpBL7Oz-antirtoscuno)
   

