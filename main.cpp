#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include "Port.h"

using namespace std;

template <class Container>
void split(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}

int main(int argc, char* argv[]){
    if(argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << std::endl;
        return 1;
    }

    ifstream infile(argv[1]);
    string line;
    // process first line
    getline(infile,line);
    vector<string> firstLine;
    split(line,firstLine);
    // number of passengers
    int P = stoi(firstLine[0]);
    // number of check-in counters
    int L = stoi(firstLine[1]);
    // number of security gates
    int S = stoi(firstLine[2]);
    vector<Passenger> passengers;
    passengers.reserve(P);
    // adding passengers to the vector from input file
    for(int i=0;i<P;i++){
        getline(infile,line);
        //cout<< line << endl;
        vector<string> word ;
        split(line,word);
        Passenger newPassenger(stoi(word[0]),stoi(word[1]),stoi(word[2]),stoi(word[3]),word[4],word[5]);
        passengers.push_back(newPassenger);
    }
    infile.close();     // end of reading from file

    Port myPort(L,S);
    myPort.runAllCases(passengers);

    freopen(argv[2], "w", stdout);

    double cs1 = myPort.getTotalWaitingTimeCase1() / (double)P ;
    double cs2 = myPort.getTotalWaitingTimeCase2() / (double)P ;
    double cs3 = myPort.getTotalWaitingTimeCase3() / (double)P ;
    double cs4 = myPort.getTotalWaitingTimeCase4() / (double)P ;
    double cs5 = myPort.getTotalWaitingTimeCase5() / (double)P ;
    double cs6 = myPort.getTotalWaitingTimeCase6() / (double)P ;
    double cs7 = myPort.getTotalWaitingTimeCase7() / (double)P ;
    double cs8 = myPort.getTotalWaitingTimeCase8() / (double)P ;
   
    int mcs1 = myPort.getMissFlightCase1();
    int mcs2 = myPort.getMissFlightCase2();
    int mcs3 = myPort.getMissFlightCase3();
    int mcs4 = myPort.getMissFlightCase4();
    int mcs5 = myPort.getMissFlightCase5();
    int mcs6 = myPort.getMissFlightCase6();
    int mcs7 = myPort.getMissFlightCase7();
    int mcs8 = myPort.getMissFlightCase8();



    printf("%.2lf\t", cs1);
    printf("%d\n",mcs1 );
    printf("%.2lf\t", cs2);
    printf("%d\n",mcs2 );
    printf("%.2lf\t", cs3);
    printf("%d\n",mcs3 );
    printf("%.2lf\t", cs4);
    printf("%d\n",mcs4 );
    printf("%.2lf\t", cs5);
    printf("%d\n",mcs5 );
    printf("%.2lf\t", cs6);
    printf("%d\n",mcs6 );
    printf("%.2lf\t", cs7);
    printf("%d\n",mcs7 );
    printf("%.2lf\t", cs8);
    printf("%d\n",mcs8 );


    fclose(stdout);



/*
    ofstream myfile;  // writing to the file
    myfile.open (argv[2]);

    myfile << cs1 << " " << mcs1 << endl ;
    myfile << cs2 << " " << mcs2 << endl ;
    myfile << cs3 << " " << mcs3 << endl ;
    myfile << cs4 << " " << mcs4 << endl ;
    myfile << cs5 << " " << mcs5 << endl ;
    myfile << cs6 << " " << mcs6 << endl ;
    myfile << cs7 << " " << mcs7 << endl ;
    myfile << cs8 << " " << mcs8 << endl ;

    myfile.close();

    cout << cs1 << " " << mcs1 << endl ;
    cout << cs2 << " " << mcs2 << endl ;
    cout << cs3 << " " << mcs3 << endl ;
    cout << cs4 << " " << mcs4 << endl ;
    cout << cs5 << " " << mcs5 << endl ;
    cout << cs6 << " " << mcs6 << endl ;
    cout << cs7 << " " << mcs7 << endl ;
    cout << cs8 << " " << mcs8 << endl ;
*/



    
    return 0;


}