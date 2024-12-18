# ANTIRTOS#C
<img src="https://github.com/WeSpeakEnglish/images/blob/main/ANTIRTOS_C.png">

No RTOS need, you will C! 
Exteamly light, pure C preprocessor macros-based universal C library designed for task management in IoT and embedded applications. It is coded in a single, small file, making it incredibly easy to integrate into your projects. Enjoy the real simplicity!  
Also: [here](https://hackaday.io/project/199046-antirtosc)
## Conveyers of function pointers without parameters
Usage
### 1. Define your tasks:
```c
#include <antirtos_c.h>
void yourTaskOne(){
//put here what ever you want to execute
}

void yourTaskTwo(){
//put here what ever you want to execute
}
```
### 2. Initialize needed queues like global prototypes (as many as you need, here are two like example):
```c
  fQ(Q1,8); // define first queue (type fQ) with name Q1, 8 elements length
  fQ(Q2,8);   // define second queue (type fQ) with name Q2, 8 elements length
```

### 3. In main loop (loop{} instead of main(){} for Arduino) just pull from the queues
```c
void main(){ // or loop{} for Arduino
  Q1_Pull(); // pull from the Q1 and execute
  Q2_Pull(); // pull from the Q2 and execute
}
```
### 4. Wherever you want, you can now push your tasks, they will be handled! (for example in some interrupts)
```c
void ISR_1{
  Q1_Push(yourTaskOne);  // just push your task into queue!
}
void ISR_2{
  Q2_Push(yourTaskTwo);  // just push your task into queue!
}
```
*This is it! All the interrupts are kept extreamly fast, all the task handled*

## Conveyers of pointers of functions with parameters
Imagine now you need to pass parameters to functions in queue

### 1. Define all your tasks, for example:
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

### 2. Define all your queues:
```c
 fQP(Q1,8,int);  //Q1 is 8 elements length, type fQP, functions receive int
 fQP(Q2,8,char); //Q2 is 8 elements length, type fQP, functions receive char
```

### 3. In main loop (loop{} instead of main(){} for Arduino) just add:
```c
void main(){
  Q1_Pull();  // pull from the Q1 and execute
  Q2_Pull();  // pull from the Q2 and execute
}
```
### 4. Where you want just push the tasks into queues and pass them parameters (for example in some interrupts again):
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




  
