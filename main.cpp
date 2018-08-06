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

using namespace std;
#define numOfRounds 8

int generateRandomInteger(int max) {
    srand((unsigned) time(0));
    int x;
    for(int i = 0; i < 100; i++)
        x = (rand() % max + 1);
    return x;
}

class fiestel {
private:
    string encrypted;
public:
    string dataBlock;
    string keyBlock;
    string result;
    string finalKeys[numOfRounds];

    fiestel() {}

    fiestel(string dataBlock, string keyBlock) {
        this->dataBlock = dataBlock;
        this->keyBlock = keyBlock;
        cout << "-------------Process Started-------------------" << endl;
        //generate round keys
        cout << "-------------Keys generation Started-------------------" << endl;
        this->generateKeys();
        cout << "-------------Keys generation Ended-------------------" << endl;
    }

    void execute(string value,string key , bool modeToUse){
        this->dataBlock = value;
        this->keyBlock=key;
        this->generateKeys();
        this->execute(modeToUse);
    }
    
    //boolean =1 for encrypt , zero for decrypt
    void execute(bool b) {
        string LEnext, REnext, LEcurr, REcurr, tmp;
        LEcurr = this->dataBlock.substr(0, 4);
        REcurr = this->dataBlock.substr(4, 4);

        if(b) {
            cout << "-------------Start Encryption-------------------------" << endl;
            for(int i = 0; i < numOfRounds; i++) {
                LEnext = REcurr;
                cout << "LEnext|" + LEnext + "|" << endl;
                cout << "key|" + (this->finalKeys[i]) + "|" << endl;
                tmp = functionF(REcurr, this->finalKeys[i]);
                for(int ii = 0; ii < 4; ii++) {
                    REnext.append(1, tmp[ii] ^ LEcurr[ii]);
                }
                cout << "REnext|" + REnext + "|" << endl;
                cout << "---------------------------------------------------------" << endl;
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
            cout << "--------------------------End Encryption-------------------------------" << endl;
            cout << "cipher is " + (LEcurr + REcurr) << endl;
            cout << "---------------------------------------------------------" << endl;        
        }


        else if(!b) {
            cout << "-------------Start Decryption-------------------------" << endl;
            for(int i = 7; i >= 0; i--) {
                LEnext = REcurr;
                cout << "LEnext|" + LEnext + "|" << endl;
                cout << "key|" + (this->finalKeys[i]) + "|" << endl;
                tmp = functionF(REcurr, this->finalKeys[i]);
                for(int ii = 0; ii < 4; ii++) {
                    REnext.append(1, tmp[ii] ^ LEcurr[ii]);
                }
                cout << "REnext|" + REnext + "|" << endl;
                cout << "---------------------------------------------------------" << endl;
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
            cout << "--------------------------End Decryption-------------------------------" << endl;
            cout << "Original is " + (LEcurr + REcurr) << endl;
            cout << "---------------------------------------------------------" << endl;
        }
        this->result = LEcurr+REcurr;
        cout<<"RESULT is "+this->result<<endl;
    }

    string functionF(string RE, string keyy) {
        string s = "";
        for(int i = 0; i < 4; i++) {
            s.append(1, RE[i]^keyy[i]);
        }
        cout << "stringInF|" + (s) + "|" << endl;
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


    char dataEntered[9], keyEntered[9];

    cout << "enter data (max 8 chars)" << endl;
    cin.get(dataEntered, 9);
    
    cin.get();
    
    cout << "enter key (max 8 chars)" << endl;
    cin.get(keyEntered, 9);
    
    
    fiestel f(dataEntered, keyEntered);
    
    return 0;
}

