#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <climits> 
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <fstream>
#include <cmath>

using namespace std;
#define numOfRounds 8

int generateRandomInteger(int max) {
    srand((unsigned) time(0));
    int x;
    for(int i = 0; i < 100; i++)
        x = (rand() % max + 1);
    return x;
}

string getFromFile(string filename="input.txt") {
    string x;
    char tmp;
    ifstream infile(filename.c_str());
    while(!infile.eof()) {
        infile.get(tmp);
        x+=tmp;
    }
    return x.substr(0,x.size()-2);
}

void saveToFile(string data, string filename) {
    ofstream outfile(filename.c_str());
    outfile << data + "";
    outfile.close();
}

class fiestel {
private:
    string encrypted;
public:
    string dataBlock;
    string keyBlock;
    string result;
    string finalKeys[numOfRounds];

    fiestel() {
    }

    fiestel(string dataBlock, string keyBlock) {
        this->dataBlock = dataBlock;
        this->keyBlock = keyBlock;
        //generate round keys
        this->generateKeys();
    }

    void execute(string value, string key, bool modeToUse) {
        this->dataBlock = value;
        this->keyBlock = key;
        this->generateKeys();
        this->execute(modeToUse);
    }

    //boolean =1 for encrypt , zero for decrypt

    void execute(bool b) {
        string LEnext, REnext, LEcurr, REcurr, tmp;
        LEcurr = this->dataBlock.substr(0, 4);
        REcurr = this->dataBlock.substr(4, 4);
        if(b) {
            for(int i = 0; i < numOfRounds; i++) {
                LEnext = REcurr;
                tmp = functionF(REcurr, this->finalKeys[i]);
                for(int ii = 0; ii < 4; ii++) {
                    REnext.append(1, tmp[ii] ^ LEcurr[ii]);
                }
                //clear all that will be used in order to use them for the other loops
                tmp = "";
                LEcurr = LEnext;
                LEnext = "";
                REcurr = REnext;
                REnext = "";
            }
            //swap LE and RE after rounds finish
            tmp = LEcurr;
            LEcurr = REcurr;
            REcurr = tmp;
        }
        else if(!b) {
            for(int i = 7; i >= 0; i--) {
                LEnext = REcurr;
                tmp = functionF(REcurr, this->finalKeys[i]);
                for(int ii = 0; ii < 4; ii++) {
                    REnext.append(1, tmp[ii] ^ LEcurr[ii]);
                }
                //clear all that will be used in order to use them for the other loops
                tmp = "";
                LEcurr = LEnext;
                LEnext = "";
                REcurr = REnext;
                REnext = "";
            }
            //swap after the last round
            tmp = LEcurr;
            LEcurr = REcurr;
            REcurr = tmp;
        }
        this->result = LEcurr + REcurr;
        cout << "RESULT is " + this->result << endl;
    }

    string functionF(string RE, string keyy) {
        string s = "";
        for(int i = 0; i < 4; i++) {
            s.append(1, RE[i]^keyy[i]);
        }
        return s;
    }
    //random keys generator

    void generateKeys() {
        string s = this->keyBlock;
        string keys[numOfRounds];
        keys[0] = this->keyBlock;
        int numberOfSwapTimes, numberOfSwapPlaces;
        //number of keys = no of rounds =8
        for(int i = 1; i < numOfRounds; i++) {
            //delay as the current time is my seeder
            for(int v = 0; v < 710000000; v++) {
            }
            numberOfSwapTimes = generateRandomInteger(128);
            //repeating number of swapping times
            for(int j = 0; j < numberOfSwapTimes; j++) {
                //delay as the current time is my seeder
                for(int v = 0; v < 29000000; v++) {
                }
                numberOfSwapPlaces = generateRandomInteger(8);
                //repeating swaps according to number of swaps , and also defining swap positions.
                for(int k = numberOfSwapPlaces; k < s.size(); k++) {
                    swap(s[k - numberOfSwapPlaces], s[k]);
                }
            }
            keys[i] = s;
        }
        for(int i = 0; i < numOfRounds; i++)
            this->finalKeys[i] = keys[i].substr(2, 4);
    }
};

int main() {
 
    //get file content without new line
    string rawFileData = getFromFile();    
    int remaining = rawFileData.size()%8 ;
    remaining = remaining*8 -rawFileData.size();
    //in case the remainder is negative
    while(remaining<0)
        remaining+=8;
    for(int i=0;i<remaining;i++){
        rawFileData.append(1,'.');
    }
    //divided it's content into blocks ( 8 chars each )
    int sizee = rawFileData.size()/8;
    string inputs[sizee];
    fiestel agents[sizee];
    //creating cipher blocks for each data block
    for(int i=0;i<sizee;i++){
        inputs[i]=rawFileData.substr(i*8,8);
        fiestel tmp(inputs[i],"abcdefgh");
        agents[i] = tmp;
    }
    
    string encryptedFileContent="";
    
    for(int i =0;i<sizee;i++){
        agents[i].execute(true);
        encryptedFileContent+=agents[i].result;
    }
    
    saveToFile(encryptedFileContent,"firstTrial.txt");
    
    
    // preview on Decryption
    string encryptedCode = getFromFile("firstTrial.txt"); 
    int remaining2 = encryptedCode.size()%8 ;
    remaining2 = remaining2*8 -encryptedCode.size();
    //in case the remainder is negative
    while(remaining2<0)
        remaining2+=8;
    for(int i=0;i<remaining2;i++){
        encryptedCode.append(1,'.');
    }
    //divided it's content into blocks ( 8 chars each )
    int sizee2 = encryptedCode.size()/8;
    sizee2-=1;
    cout<<sizee2<<endl;
    string inputss[sizee2];
    
    // this time i will use the same agents in order to have the keys 
    // from them as i use time as a seed which will never be repeatable 
    
    for(int i=0;i<sizee2;i++){
        inputss[i]=encryptedCode.substr(i*8,8);
        agents[i].dataBlock=inputss[i];
        agents[i].result="";
    }
    
    string decryptedFileContent ="";
    
    for(int i=0;i<sizee2;i++){
        agents[i].execute(false);
        decryptedFileContent+=agents[i].result;
    }
    
    saveToFile(decryptedFileContent,"finalOutput.txt");
    return 0;
}