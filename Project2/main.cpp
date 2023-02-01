#include <iostream>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>
#include <cmath>
#include <ctime>
#include <chrono>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fstream>


int numberOfIntegers;  
int numberOfThreads;
pid_t mainThreadId;
int max;
int min;
int range;
int mode;
double median;
int sum = 0;
double arithmeticMean;
double harmonicMean;
double standardDeviation;
double interquartileRange;

//finds the minimum value in the array
int findMin(int numberOfIntegers, int* numberArray){
    int min1 = 1000000;
    for(int i = 0; i<numberOfIntegers; i++){
        if(numberArray[i] < min1){
            min1 = numberArray[i];
        }
    }
    return min1;
}
//finds the maximum value in the array
int findMax (int numberOfIntegers, int* numberArray){
    int max1 = 0;
    for(int i = 0; i<numberOfIntegers; i++){
        if(numberArray[i] > max1){
            max1 = numberArray[i];
        }
    }
    return max1;
}
//finds the range of the array
int findRange (int numberOfIntegers, int* numberArray){
    int arr[numberOfIntegers];
    std::copy(numberArray, numberArray + numberOfIntegers, arr);  
    std::sort(arr, arr+numberOfIntegers);
    int max1 = arr[numberOfIntegers-1];
    int min1 = arr[0];
    int range1 = max1 - min1;
    return range1; 
}
//finds the mode of the array
int findMode(int numberOfIntegers, int* numberArray){
    int arr[numberOfIntegers];
    std::copy(numberArray, numberArray + numberOfIntegers, arr);  
    std::sort(arr, arr+numberOfIntegers);
    int current = arr[0];
    int count = 0;
    int mode1 = arr[0];
    int modeCount = 0;
    for(int i = 0; i < numberOfIntegers; i++){
        if(arr[i]==current){
            count ++;
        }else{
            if(count > modeCount){
                mode1 = arr[i-1];
                modeCount = count;
                
            }
            current = arr[i];
            count = 1;
        }
        if(i == numberOfIntegers-1){
            if(count > modeCount){
                mode1 = arr[i-1];
                modeCount = count;
            }
        }
    }
    return mode1;
}
//finds the median of the array
double findMedian(int numberOfIntegers, int* numberArray){
    int arr[numberOfIntegers];
    std::copy(numberArray, numberArray + numberOfIntegers, arr);  
    std::sort(arr, arr+numberOfIntegers);
    int medianIndex;
    medianIndex = numberOfIntegers/2;
    if(numberOfIntegers % 2 == 1){
        return arr[medianIndex];
    }else{
        return double(arr[medianIndex] + arr[medianIndex-1]) /2.0;
    }
}
//finds the sum of the elements in the array
int findSum(int numberOfIntegers, int* numberArray){
    int sum1 = 0;
    for(int i = 0; i < numberOfIntegers; i++){
        sum1 += numberArray[i];
    }
    return sum1;
}
//finds the arithmetic mean of the array
double findArithmeticMean(int numberOfIntegers, int* numberArray){
    int sum1 = findSum(numberOfIntegers,numberArray);
    return double(sum1)/double(numberOfIntegers);
}
//finds the harmonic mean of the array
double findHarmonicMean(int numberOfIntegers, int* numberArray){
    double sum1 = 0.0;
    for(int i = 0; i < numberOfIntegers; i++){
        sum1 += 1.0/ double(numberArray[i]);
    }
    return double(numberOfIntegers) / sum1;
}
//finds the standard deviation of the array
double findStandardDeviation(int numberOfIntegers, int* numberArray){
    double arithmeticMean1 = findArithmeticMean(numberOfIntegers,numberArray);
    double sum1 = 0.0;
    for(int i=0; i<numberOfIntegers; i++){
        double diff = fabs(numberArray[i] - arithmeticMean1);
        sum1 += pow(diff, 2);
    }
    return sqrt(sum1/double(numberOfIntegers-1));
    
}
//finds the interquartile range of the array
double findInterquartileRange(int numberOfIntegers, int* numberArray){
    int arr[numberOfIntegers];
    std::copy(numberArray, numberArray + numberOfIntegers, arr);  
    std::sort(arr, arr+numberOfIntegers);
    int medianIndex = numberOfIntegers/2;
    int arr1[medianIndex];
    int arr2[medianIndex];
    double median1;
    double median2;
    if(numberOfIntegers % 2 == 1){
        for(int i = 0; i<medianIndex; i++){
            arr1[i] = arr[i];
        }
        for(int i = 0; i<medianIndex; i++){
            arr2[i] = arr[i+medianIndex+1];
        }
        median1 = findMedian(medianIndex,arr1);
        median2 = findMedian(medianIndex,arr2);
    }else{
        for(int i = 0; i<medianIndex; i++){
            arr1[i] = arr[i];
        }
        for(int i = 0; i<medianIndex; i++){
            arr2[i] = arr[i+medianIndex];
        }
        median1 = findMedian(medianIndex,arr1);
        median2 = findMedian(medianIndex,arr2);
    }
    return median2-median1;
}




//calls the findMin function for the threads
void * findMinT(void * args){
    int * array = (int*) args;
    min = findMin(numberOfIntegers,array);
    pthread_exit(NULL);
}
//calls the findMax function for the threads
void * findMaxT(void * args){
    int * array = (int*) args;
    max = findMax(numberOfIntegers,array);
    pthread_exit(NULL);
}
//calls the findRange function for the threads
void * findRangeT(void * args){
    int * array = (int*) args;
    range = findRange(numberOfIntegers,array);
    pthread_exit(NULL);
}
//calls the findMode function for the threads
void * findModeT(void * args){
    int * numberArray = (int*) args;
    mode = findMode(numberOfIntegers,numberArray);
    if(numberOfThreads >= 5 && numberOfThreads < 9){
        range = findRange(numberOfIntegers,numberArray);
    }
    pthread_exit(NULL);
}
//calls the findMedian function for the threads
void * findMedianT(void * args){
    int * numberArray = (int*) args;
    median = findMedian(numberOfIntegers,numberArray);
    pthread_exit(NULL);
}
//calls the findSum function and proper functions according to the number of threads
void* findSumT(void* args){
    int * numberArray = (int*) args;
    sum = findSum(numberOfIntegers,numberArray);
    if(numberOfThreads >= 5 && numberOfThreads < 8){
        median = findMedian(numberOfIntegers,numberArray);
    }else if(numberOfThreads == 2){
        max = findMax(numberOfIntegers,numberArray);
        harmonicMean = findHarmonicMean(numberOfIntegers,numberArray);
        standardDeviation = findStandardDeviation(numberOfIntegers,numberArray);
        interquartileRange = findInterquartileRange(numberOfIntegers,numberArray);
    }else if(numberOfThreads==3 || numberOfThreads==4){
        mode = findMode(numberOfIntegers,numberArray);
        median = findMedian(numberOfIntegers,numberArray);
    }
    pthread_exit(NULL);
}
//calls the findArithmeticMean function and proper functions according to the number of threads
void* findArithmeticMeanT(void* args){
    int * numberArray = (int*) args;
    arithmeticMean = findArithmeticMean(numberOfIntegers,numberArray);
    if(numberOfThreads >= 5 && numberOfThreads < 10){
        min = findMin(numberOfIntegers,numberArray);
    }else if(numberOfThreads == 2){
        min = findMin(numberOfIntegers,numberArray);
        range = findRange(numberOfIntegers,numberArray);
        mode = findMode(numberOfIntegers,numberArray);
        median = findMedian(numberOfIntegers,numberArray);
    }else if(numberOfThreads==3 || numberOfThreads==4){
        min = findMin(numberOfIntegers,numberArray);
        range = findRange(numberOfIntegers,numberArray);
    }
    pthread_exit(NULL);
}
//calls the findHarmonicMean function and proper functions according to the number of threads
void*  findHarmonicMeanT(void* args){
    int * numberArray = (int*) args;
    harmonicMean = findHarmonicMean(numberOfIntegers,numberArray);
    if(numberOfThreads >= 5 && numberOfThreads < 7){
        max = findMax(numberOfIntegers,numberArray);
    }else if(numberOfThreads==3){
        max = findMax(numberOfIntegers,numberArray);
        standardDeviation = findStandardDeviation(numberOfIntegers,numberArray);
        interquartileRange = findInterquartileRange(numberOfIntegers,numberArray);
    }else if(numberOfThreads==4){
        max = findMax(numberOfIntegers,numberArray);
    }
    pthread_exit(NULL);
}
//calls the findStandardDeviation for threads
void* findStandardDeviationT(void* args){
    int * numberArray = (int*) args;
    standardDeviation = findStandardDeviation(numberOfIntegers,numberArray);
    pthread_exit(NULL);
}
//calls the findInterquartileRange function and proper functions according to the number of threads
void* findInterquartileRangeT(void* args){
    int * numberArray = (int*) args;
    interquartileRange = findInterquartileRange(numberOfIntegers,numberArray);
    if(numberOfThreads == 5 || numberOfThreads == 4){
        standardDeviation = findStandardDeviation(numberOfIntegers,numberArray);
    }
    pthread_exit(NULL);
}





int main(int argc, char* argv[]){
    //start the timer
    auto start = std::chrono::system_clock::now();

    numberOfIntegers = atoi(argv[1]);
    if(argv[2]==NULL){
        numberOfThreads = 1;
    }else{
        numberOfThreads = atoi(argv[2]);
    }
    int numberArray[numberOfIntegers];

    //creates numbers and put them into numberArray
    for(int i = 0; i < numberOfIntegers; i++){
        int number = rand() % 9001 +1000;
        numberArray[i] = number;
        //std::cout << number << ", " ;
    }
    

    //this part of the code creates sufficient number of threads according to the given number 
    if(numberOfThreads == 1){
        min = findMin(numberOfIntegers,numberArray);
        max = findMax(numberOfIntegers,numberArray);
        range = findRange(numberOfIntegers,numberArray);
        mode = findMode(numberOfIntegers,numberArray);
        median = findMedian(numberOfIntegers,numberArray);
        sum = findSum(numberOfIntegers,numberArray);
        arithmeticMean = findArithmeticMean(numberOfIntegers,numberArray);
        harmonicMean = findHarmonicMean(numberOfIntegers,numberArray);
        standardDeviation = findStandardDeviation(numberOfIntegers,numberArray);
        interquartileRange = findInterquartileRange(numberOfIntegers,numberArray);
    }else if(numberOfThreads >= 10){

        pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9, thread10;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findMinT, numberArray);
        pthread_create(&thread2, &attr, &findMaxT, numberArray);
        pthread_create(&thread3, &attr, &findRangeT, numberArray);
        pthread_create(&thread4, &attr, &findModeT, numberArray);
        pthread_create(&thread5, &attr, &findMedianT, numberArray);
        pthread_create(&thread6, &attr, &findSumT, numberArray);
        pthread_create(&thread7, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread8, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread9, &attr, &findStandardDeviationT, numberArray);
        pthread_create(&thread10, &attr, &findInterquartileRangeT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
        pthread_join(thread7, NULL);
        pthread_join(thread8, NULL);
        pthread_join(thread9, NULL);
        pthread_join(thread10, NULL);
    }else if(numberOfThreads == 5){
        pthread_t thread1, thread2, thread3, thread4, thread5;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findModeT, numberArray);
        pthread_create(&thread3, &attr, &findSumT, numberArray);
        pthread_create(&thread4, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread5, &attr, &findInterquartileRangeT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
    }else if(numberOfThreads ==2){
        pthread_t thread1, thread2;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findSumT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }else if(numberOfThreads ==3){
        pthread_t thread1, thread2, thread3;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findSumT, numberArray);
        pthread_create(&thread3, &attr, &findHarmonicMeanT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);

    }else if(numberOfThreads ==4){
        pthread_t thread1, thread2, thread3, thread4;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findSumT, numberArray);
        pthread_create(&thread3, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread4, &attr, &findInterquartileRangeT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);

    }else if(numberOfThreads ==6){
        pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findModeT, numberArray);
        pthread_create(&thread3, &attr, &findSumT, numberArray);
        pthread_create(&thread4, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread5, &attr, &findInterquartileRangeT, numberArray);
        pthread_create(&thread6, &attr, &findStandardDeviationT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
    }else if(numberOfThreads ==7){
        pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findModeT, numberArray);
        pthread_create(&thread3, &attr, &findSumT, numberArray);
        pthread_create(&thread4, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread5, &attr, &findInterquartileRangeT, numberArray);
        pthread_create(&thread6, &attr, &findStandardDeviationT, numberArray);
        pthread_create(&thread7, &attr, &findMaxT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
        pthread_join(thread7, NULL);

    }else if(numberOfThreads ==8){
        pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findModeT, numberArray);
        pthread_create(&thread3, &attr, &findSumT, numberArray);
        pthread_create(&thread4, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread5, &attr, &findInterquartileRangeT, numberArray);
        pthread_create(&thread6, &attr, &findStandardDeviationT, numberArray);
        pthread_create(&thread7, &attr, &findMaxT, numberArray);
        pthread_create(&thread8, &attr, &findMedianT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
        pthread_join(thread7, NULL);
        pthread_join(thread8, NULL);
    }else if(numberOfThreads ==9){
        pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread1, &attr, &findArithmeticMeanT, numberArray);
        pthread_create(&thread2, &attr, &findModeT, numberArray);
        pthread_create(&thread3, &attr, &findSumT, numberArray);
        pthread_create(&thread4, &attr, &findHarmonicMeanT, numberArray);
        pthread_create(&thread5, &attr, &findInterquartileRangeT, numberArray);
        pthread_create(&thread6, &attr, &findStandardDeviationT, numberArray);
        pthread_create(&thread7, &attr, &findMaxT, numberArray);
        pthread_create(&thread8, &attr, &findMedianT, numberArray);
        pthread_create(&thread9, &attr, &findRangeT, numberArray);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);
        pthread_join(thread6, NULL);
        pthread_join(thread7, NULL);
        pthread_join(thread8, NULL);
        pthread_join(thread9, NULL);
    }
    //finish the execution
    auto end = std::chrono::system_clock::now();
    //opens the file according to the number of threads
    FILE *f;
    if(numberOfThreads == 1){
        f = fopen("output1.txt","w");
    }else if(numberOfThreads == 2){
        f = fopen("output2.txt","w");
    }else if(numberOfThreads == 3){
        f = fopen("output3.txt","w");
    }else if(numberOfThreads == 4){
        f = fopen("output4.txt","w");
    }else if(numberOfThreads == 5){
        f = fopen("output5.txt","w");
    }else if(numberOfThreads == 6){
        f = fopen("output6.txt","w");
    }else if(numberOfThreads == 7){
        f = fopen("output7.txt","w");
    }else if(numberOfThreads == 8){
        f = fopen("output8.txt","w");
    }else if(numberOfThreads == 9){
        f = fopen("output9.txt","w");
    }else if(numberOfThreads == 10){
        f = fopen("output10.txt","w");
    }
    
    std::chrono::duration<double> timeSpent = end-start;
    //prints the output to the file
    fprintf(f,"%d\n%d\n%d\n%d\n%.5f\n%d\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f",min,max,range,mode,median,sum,
    arithmeticMean,harmonicMean,standardDeviation,interquartileRange,timeSpent.count());
    return 0;
}