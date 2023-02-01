#include <iostream>
#include <string>
#include <cstring>
#include <vector>  
#include <sstream>  
#include <queue>
#include <fstream>
using namespace std;


vector<string> history;
//split function in order to split string from specified char
void split(string const &str, char c, vector <string> &output ){
    stringstream s(str);  
    string s2;  
    while (getline (s, s2, c )) {  
        output.push_back(s2);
    }  
}
//calls system ls in order to list the current directory
void listdir(){
    system("ls");
    return;
}
//calls system hostname in order to find hostname
void mycomputername(){
    system("hostname");
    return;
}
//calls system hostname -i in order to find ip
void whatsmyip(){
    system("hostname -i");
    return;
}
//cals system cp {sourcefile} {destination file} in order to copy the contents of first file to the second one
void printfiletofile(string source, string destination){
    string s = "cp "+ source +" "+destination;
    system(s.c_str());
    return;
}
//prints the file to the terminal with getline
void printfile(string filename){
    ifstream file;
    file.open(filename);
    string line;
    string in;
    while(getline(file, line)){
        cout << line;
        //in order to process line by line take a user input. In other words wait user for the press enter 
        getline(cin, in);
    }
}
//checks the history if the target command has already done
void dididothat(string target){
    for(int i = 0; i < history.size(); i++){
        //if found, print yes and return
        if(history.at(i).compare(target)== 0){
            cout << "Yes" << endl;
            return;
        }
    }
    //if cannot be found print no and return
    cout << "No" << endl;
    return;
}
//starts the default editor with system editor
void hellotext(){
    system("gedit");
}

int main(int argc, char* argv[]){
    //string to store input, vector to store splitted input, integer to size of history vector.
    string input;
    vector<string> splittedInput;
    int historycount = 0;
    while(true){
        int isValid = 0;
        //prints the name of the user
        cout << getenv("USER");
        cout << " >>> ";
        getline(cin, input);
        //checks if it is a empty input
        if(input.compare("") == 0){
            continue;
        }
        //clears the splitted input array
        splittedInput.clear();
        //splits the string from blank spaces
        split(input, ' ', splittedInput);
        //if the command is exit, break the loop
        if(splittedInput.at(0).compare("exit") == 0){
            isValid = 1;
            break;
        }
        //if the command is listdir, calls listdir function
        else if(splittedInput.at(0).compare("listdir") == 0){
            isValid = 1;
            listdir();
        }
        //if the command is mycomputername, calls mycomputername function
        else if(splittedInput.at(0).compare("mycomputername")== 0){
            isValid = 1;
            mycomputername();
        }
        //if the command is whatsmyip, calls whatsmyip function
        else if(splittedInput.at(0).compare("whatsmyip")== 0){
            isValid = 1;
            whatsmyip();
        }
        //if the command is printfile, checks the size of input. if the size is bigger than 2
        //that means there are more parameters than one parameter. so call printfiletofile function.
        else if(splittedInput.at(0).compare("printfile")== 0){
            isValid = 1;
            if(splittedInput.size() > 2){
                printfiletofile(splittedInput.at(1), splittedInput.at(3));
            }
            //this part is for only one parameter calls of printfile command. it calls printfile function.
            else{
                printfile(splittedInput.at(1));
            }
        }
        //if the command is dididothat, calls dididothat function
        else if(splittedInput.at(0).compare("dididothat")== 0){
            isValid = 1;
            string target = "";
            for (int i = 1; i<splittedInput.size(); i++){
                target += splittedInput.at(i);
                if((i+1) < splittedInput.size()){
                    target += " ";
                }
            }
            dididothat(target);
        }
        //if the command is hellotext, calls hellotext function
        else if(splittedInput.at(0).compare("hellotext")== 0){
            isValid = 1;
            hellotext();
        }

        if(splittedInput.at(0).compare("dididothat")!=0 && isValid){
            //checks if the limit of the history exceeded.
            //if it is not exceeded, current command is added to history and increment historycount one
            if(historycount < 15){
                string entry = "\""+input+ "\"";
                history.insert(history.begin(),entry);
                historycount += 1;
            }
            //if the limit is exceeded pop one command and add the current command
            else{
                string entry = "\""+input+ "\"";
                history.pop_back();
                history.insert(history.begin(),entry);
            }
            
        }
    }
    return 0;
}