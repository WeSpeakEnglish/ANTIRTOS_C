# ANTIRTOS_C
No RTOS need, you will C!
See also: [here](https://hackaday.io/project/199046-antirtosc)
## Conveyers of function pointers without parameters
Usage
### 1. Define your tasks:
```c
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

### 3. In main loop just pull from the queues
```c
void main(){ // or loop{} for Arduino
  Q1_Pull();
  Q2_Pull();
}
```
### 4. Where ever you want now you may push your tasks, they will be handled! (for example in some interrupts)
```c
void ISR_1{
  Q1_Push(yourTaskOne);  // just push your task into queue!
}
void ISR_2{
  Q2_Push(yourTaskTwo);  // just push your task into queue!
}
```
*This is it! All the interrupts are kept extreamly fast, all the task handled*

