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
    string finalKeys[8];

    fiestel() {

    }

    fiestel(string dataBlock, string keyBlock) {
        this->dataBlock = dataBlock;
        this->keyBlock = keyBlock;
    }

    void execute() {
        this->generateKeys();
        string LEnext, REnext, LEcurr, REcurr, tmp;
        LEcurr = this->dataBlock.substr(0, 4);
        REcurr = this->dataBlock.substr(4, 4);
        cout << "---------------------------------------------------------" << endl;
        for(int i = 0; i < 8; i++) {

            LEnext = REcurr;
            cout << "value|" + LEnext + "|" << endl;
            cout << "key|" + (this->finalKeys[i]) + "|" << endl;

            tmp = functionF(REcurr, this->finalKeys[i]);


            for(int ii = 0; ii < 4; ii++) {
                REnext.append(1, tmp[ii] ^ LEcurr[ii]);
            }
            cout << "cipher|" + REnext + "|" << endl;
            cout << "---------------------------------------------------------" << endl;

            tmp = "";

            LEcurr = LEnext;
            LEnext = "";

            REcurr = REnext;
            REnext = "";
        }

        cout << "--------------------------after Encryption-------------------------------" << endl;

        tmp = LEcurr;
        LEcurr = REcurr;
        REcurr = tmp;
        cout << LEcurr + REcurr << endl;

        cout << "---------------------------------------------------------" << endl;


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

            tmp = "";

            LEcurr = LEnext;
            LEnext = "";

            REcurr = REnext;
            REnext = "";
        }


        cout << "--------------------------after decryption-------------------------------" << endl;

        tmp = LEcurr;
        LEcurr = REcurr;
        REcurr = tmp;
        cout << LEcurr + REcurr << endl;


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
        string keys[8];
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
                //repeating swaps according to number of swaps , and aslo defining swap positions.
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


    char dataEntered[9];

    cout << "enter data (max 8 chars)" << endl;
    cin.get(dataEntered, 9);
    cout << endl;
    fiestel f(dataEntered, "abcdefgh");
    f.execute();


    return 0;
}

