#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Image{

private:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int* hisAry;  //a 1D integer array, size of maxVal + 1
                  // need to be dynamically allocated at run time
public:
    Image(){
        numRows = 0;
        numCols = 0;
        minVal = 0;
        maxVal = 0;
        hisAry = nullptr;
    }
    Image(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        hisAry = new int[maxVal + 1];
        for(int i=0; i<maxVal+1; i++){
            hisAry[i] = 0;
        }
    }

     void computeHist(ifstream& inFile, ofstream& outFile){
        outFile << this->numRows << " " << this->numCols << " " << this->minVal << " " << this->maxVal << endl;
        int row = 0;
        while(row < this->numRows){
            int col = 0, pixel = -1;
            while(col < this->numCols) {
                inFile >> pixel;
                hisAry[pixel]++;
                col = col + 1;
            }
            row = row + 1;
         }
        for(int i=0; i<maxVal+1; i++){
            outFile << i << "    "  << hisAry[i] << endl;
        }
     }

     void dispHist(ofstream& outFile){
         outFile << this->numRows << " " << this->numCols << " " << this->minVal << " " << this->maxVal << endl;
         for(int i=0; i<this->maxVal + 1; i++){
             outFile << i << "   " << " (" << hisAry[i] << ") :  ";
             if(hisAry[i]>70) {
                 hisAry[i] = 70;
             }
             for(int j=0; j<hisAry[i]; j++){
                 outFile << "+";
             }
             outFile << endl;
         }
     }
     void threshold(ifstream& inFile, ofstream& outFile3, ofstream& outFile4, int threshold){
         inFile >> this->numRows;
         inFile >> this->numCols;
         inFile >> this->minVal;
         inFile >> this->maxVal;
         this->minVal = 0, this->maxVal = 1;
         outFile3 << this->numRows << " " << this->numCols << " " << this->minVal << " " << this->maxVal << endl;
         outFile4 << this->numRows << " " << this->numCols << " " << this->minVal << " " << this->maxVal << endl;
         int pixelVal = -1, row = 0;
         while(row < numRows){
            int col = 0;
            while(col < numCols){
                inFile >> pixelVal;
                if(pixelVal >= threshold){
                    outFile3 << 1 << " ";
                    outFile4 << 1 << " ";
                }else{
                    outFile3 << 0 << " ";
                    outFile4 << "." << " ";
                }
                col = col + 1;
            }
            outFile3 << endl;
            outFile4 << endl;
            row = row + 1;
         }
     }
};

int main(int argc, const char *argv[]) {

    ifstream inFile1(argv[1]);

    ofstream outFile1(argv[3]);
    ofstream outFile2(argv[4]);


    int numRows, numCols, minVal, maxVal;

    inFile1 >> numRows;
    inFile1 >> numCols;
    inFile1 >> minVal;
    inFile1 >> maxVal;
    cout << numRows << " " << numCols << " " << minVal << " " << maxVal << " " <<  endl;

    Image *image = new Image(numRows, numCols, minVal, maxVal);
    image->computeHist(inFile1, outFile1);
    image->dispHist(outFile2);

    inFile1.close();

    ifstream inFile2(argv[1]);
    int threshold = stoi(argv[2]);
    ofstream outFile3(argv[5]);
    ofstream outFile4(argv[6]);
    image->threshold(inFile2, outFile3, outFile4, threshold);

    outFile2.close();
    outFile3.close();
    outFile4.close();
    return 0;
}
