#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>  
#include <queue>
#include <pthread.h>
#include<unistd.h>   
#include <chrono>
#include <thread>
#include <cstring>

//locks for machines
pthread_mutex_t machineLocks[10];
//lokcs for companies (kevin bob stuart otto dave)
pthread_mutex_t companyLocks[5];
//lock for writing to the file
pthread_mutex_t writeLock;
pthread_t vendingThreads[10];
FILE *f;

//array that checks whether a new customer has come to the machine
int customerCheckArray[10];
int machineIds[10];

//company
int Kevin = 0;
int Bob = 0;
int Stuart = 0;
int Otto = 0;
int Dave= 0;

//customer object that holds customer's information
class Customer{
    public:
        int id;
        int sleepTime;
        int machineId;
        std::string companyName;
        int paymentAmount;
        int companyId;
};

//split function splits the given string from specified char
void split(std::string const &str, char c, std::vector <std::string> &output ){
    std::stringstream s(str);  
    std::string s2;  
    while (getline (s, s2, c )) {  
        output.push_back(s2);
    }  
}

//check variable for exiting vending machine thread
int check = 0;
Customer customerArrayVending[10];
//vending machine function for vening machine threads. 
//this function makes payment to the companies and 
//writes the payment information to the output file
void* vendingMachine(void* args){
    int machineId = *(int*)(args);
    //checks if the thread should exit
    while(check == 0){
        //checks if a new customer has arrived
        if(customerCheckArray[machineId] == 1){
            Customer customer = customerArrayVending[machineId];
            int companyId = customer.companyId;
            pthread_mutex_lock(&companyLocks[companyId]);
            pthread_mutex_lock(&writeLock);
            fprintf(f,"[VTM%d]: Customer%d,%dTL,%s\n",customer.machineId,customer.id,customer.paymentAmount,customer.companyName.c_str());
            pthread_mutex_unlock(&writeLock);
            int paymentAmount = customer.paymentAmount;
            //make payment to proper company
            if(companyId == 0){
                Kevin += paymentAmount;
            }else if(companyId == 1){
                Bob += paymentAmount;
            }else if(companyId == 2){
                Stuart += paymentAmount;
            }else if(companyId == 3){
                Otto += paymentAmount;
            }else if(companyId == 4){
                Dave += paymentAmount;
            }
            //set the check variable to 0 and wait new customers
            customerCheckArray[machineId] = 0;
            pthread_mutex_unlock(&companyLocks[companyId]);
        }else{
            continue;
        } 
    }
    pthread_exit(NULL);
}

//function for customer threads
void* customerT(void* args){
    Customer* customer = (Customer*) args;
    int sleepTime = customer->sleepTime;
    //sleep for specified amount
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    int machineId = customer -> machineId;
    //lock the machine so no one can access the machine during the operation
    pthread_mutex_lock(&machineLocks[machineId-1]);
    customerArrayVending[machineId-1] = *customer;
    //set the variable to 1 in order to notify the specified machine thread
    customerCheckArray[machineId-1] = 1;
    //wait until thread finishes its job
    while(customerCheckArray[machineId-1] == 1);
    //release the lock
    pthread_mutex_unlock(&machineLocks[machineId-1]);    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    std::string inputfileName = argv[1];
    std::ifstream inputfile;
    inputfile.open(inputfileName);
    std::string line;
    std::vector<std::string> splittedLine;
    std::vector<Customer> customerArray;
    std::getline(inputfile,line);  
    int numberOfCustomers = stoi(line);
    pthread_t customerThreads[numberOfCustomers];
    std::vector<std::string> fileName;
    split(inputfileName,'.',fileName);
    std::string outputFileName = fileName.at(0)+"_log.txt";
    f = fopen(outputFileName.c_str(),"w");
    //read the input file and create the customer objects
    for(int i = 0; i < numberOfCustomers; i++){
        std::getline(inputfile,line);
        split(line, ',' ,splittedLine);  
        Customer custom;
        custom.id = i+1;    
        custom.sleepTime = stoi(splittedLine.at(0));    
        custom.machineId = stoi(splittedLine.at(1));
        custom.companyName = splittedLine.at(2);
        if(splittedLine.at(2).compare("Kevin")==0){
            custom.companyId=0;
        }else if(splittedLine.at(2).compare("Bob")==0){
            custom.companyId=1;
        }else if(splittedLine.at(2).compare("Stuart")==0){
            custom.companyId=2;
        }else if(splittedLine.at(2).compare("Otto")==0){
            custom.companyId=3;
        }else if(splittedLine.at(2).compare("Dave")==0){
            custom.companyId=4;
        }
        custom.paymentAmount = stoi(splittedLine.at(3));
        customerArray.push_back(custom);
        splittedLine.clear();
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // initialize the check array to zero before starting the operations
    for(int i = 0; i<10; i++){
        customerCheckArray[i] = 0;
    }
    //create the machine threads
    for (int i = 0; i<10; i++){
        machineIds[i] = i;
        pthread_create(&vendingThreads[i], &attr, &vendingMachine, &machineIds[i]);
    }
        
    //create the customer threads
    for(int i = 0; i<numberOfCustomers; i++){
        Customer customer = customerArray.at(i);
        pthread_create(&customerThreads[i], &attr, &customerT,&customerArray.at(i));
    }
    //join the customer threads
    for(int i = 0; i<numberOfCustomers; i++){
        pthread_join(customerThreads[i], NULL);
    }
    //set check to 1 so the machine threads can finish their execution
    check = 1;
    //join the machine threads
    for (int i = 0; i<10; i++){
        pthread_join(vendingThreads[i], NULL);
    }
    //print the results to the output file
    fprintf(f,"[Main]: All payments are completed\n");
    fprintf(f, "[Main]: Kevin: %d\n[Main]: Bob: %d\n[Main]: Stuart: %d\n[Main]: Otto: %d\n[Main]: Dave: %d\n",Kevin,Bob,Stuart,Otto,Dave);


}