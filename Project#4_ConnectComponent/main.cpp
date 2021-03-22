#include <iostream>
#include <fstream>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

static int newLabel = 0;

class CClabel{
private:
    int numRows, numCols;
    int minVal, maxVal;

public:
    class Property{
    public:
        int label, numPixels;
        int minR, minC;
        int maxR, maxC;

        Property(){
            int label = 0;
            int numPixels = 0;
            int minR = 9999;
            int minC = 9999;
            int maxR = -1;
            int maxC = -1;
        }
        Property(int label, int numPixels, int minR, int minC, int maxR, int maxC){
            this->label = label;
            this->numPixels = numPixels;
            this->minR = minR;
            this->minC = minC;
            this->maxR = maxR;
            this->maxC = maxC;
        }
    };

    int newMin, newMax;
    int trueNumCC = 0;
    int** zeroFramedAry; // numRows + 2 by numCols + 2
    int* nonZeroNeighborAry; //size = 5

    int* EQAry;
    Property **CCproperty;



    CClabel(){}
    CClabel(int numRows, int numCols, int minVal, int maxVal, int newMin, int newMax, int trueNumCC){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->newMin = newMin;
        this->newMax = newMax;
        this->trueNumCC = trueNumCC;
        zeroFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            zeroFramedAry[i] = new int[numCols+2];
        }
        nonZeroNeighborAry = new int[5];
        for(int i=0; i<5; i++){
            nonZeroNeighborAry[i] = INT_MAX;
        }
        EQAry = new int[numRows * numCols/4];
        for(int i=0; i<numRows*numCols/4; i++){
            EQAry[i] = i;
        }
    }

    void zero2D(int** inArray, int rows, int cols){
         for(int i=0; i<rows; i++){
             for(int j=0; j<cols; j++){
                 inArray[i][j] = 0;
             }
         }
    }

    void loadImage(ifstream& inFile, int** inArray){
         for(int i=1; i<numRows+1; i++){
             for(int j=1; j<numCols+1; j++){
                  inFile >> inArray[i][j];
             }
         }
    }


    void imgReformat(int** inArray, int rows, int cols, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                if(inArray[i][j] <10){
                    outFile << "   " << inArray[i][j];
                }else if(inArray[i][j] <100){
                    outFile << "  " << inArray[i][j];
                }else if(inArray[i][j] <1000){
                    outFile << " " << inArray[i][j];
                }
            }
            outFile << endl;
        }
    }

    int connect4Pass1(int** zeroFramedAry,  int* EQAry){
        int maxLabel = 0;
          for(int i=1; i<numRows+1; i++){
              for(int j=1; j<numCols+1; j++){
                  if(zeroFramedAry[i][j] > 0){
                    maxLabel =  changeLabelPass1_connect4(i, j, zeroFramedAry,  EQAry);
                  }
              }
          }
          return maxLabel;
    }

    int connect8Pass1(int** zeroFramedAry,  int* EQAry){
        int maxLabel = 0;
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                  maxLabel =  changeLabelPass1_connect8(i, j, zeroFramedAry,  EQAry);
                }
            }
        }
        return maxLabel;
    }

    int changeLabelPass1_connect8(int i,int j,int** zeroFramedAry,int* EQAry){
        vector<int> nonZeroNeighbor;
        int a = zeroFramedAry[i-1][j-1];
        int b = zeroFramedAry[i-1][j];
        int c = zeroFramedAry[i-1][j+1];
        int d = zeroFramedAry[i][j-1];
        if(a!=0) nonZeroNeighbor.push_back(a);
        if(b!=0) nonZeroNeighbor.push_back(b);
        if(c!=0) nonZeroNeighbor.push_back(c);
        if(d!=0) nonZeroNeighbor.push_back(d);
        if(nonZeroNeighbor.empty()){
            newLabel++;
            zeroFramedAry[i][j] = newLabel;
           // cout << "newLabel: " << newLabel << endl;
        }else{
            int minVal = *min_element(nonZeroNeighbor.begin(), nonZeroNeighbor.end());
            zeroFramedAry[i][j] = minVal;
            int size = nonZeroNeighbor.size();
            for(unsigned i=0; i<size; i++){
                int neighbor = nonZeroNeighbor[i];
                if(neighbor!=minVal){
                    EQAry[neighbor] = minVal;
                }
            }
        }
        this->newMax = newLabel;
        return newLabel;
    }

    int connect8Pass2(int** zeroFramedAry,  int* EQAry){
        int maxVal = 0;
        for(int i=numRows+1; i>=0; i--){
            for(int j=numCols+1; j>=0; j--){
                if(zeroFramedAry[i][j] > 0){
                    changeLabelPass2_connect8(i, j, zeroFramedAry, EQAry);
                }
            }
        }
        for(int i=1; i<=newLabel; i++){
            maxVal = max(maxVal, EQAry[i]);
        }
        this->newMax = maxVal;
        return maxVal;
    }

    void changeLabelPass2_connect8(int i,int j,int** zeroFramedAry,int* EQAry){
        vector<int> nonZeroNeighbor;
        int e = zeroFramedAry[i-1][j-1];
        int f = zeroFramedAry[i-1][j];
        int g = zeroFramedAry[i-1][j+1];
        int h = zeroFramedAry[i][j-1];
        if(e!=0) nonZeroNeighbor.push_back(e);
        if(f!=0) nonZeroNeighbor.push_back(f);
        if(g!=0) nonZeroNeighbor.push_back(g);
        if(h!=0) nonZeroNeighbor.push_back(h);
        if(!nonZeroNeighbor.empty()){
            if(!nonZeroNeighbor.empty()){
                int minVal = *min_element(nonZeroNeighbor.begin(), nonZeroNeighbor.end());
                if(zeroFramedAry[i][j] > minVal){
                    EQAry[zeroFramedAry[i][j]] =  minVal;
                }
                zeroFramedAry[i][j] = minVal;
            }
        }
    }

    void  changeLabelPass2_connect4(int i,int j,int** zeroFramedAry,int* EQAry){
        vector<int> nonZeroNeighbor;
        int b = zeroFramedAry[i][j+1];
        int d = zeroFramedAry[i+1][j];
        if(b!=0) { nonZeroNeighbor.push_back(b); }
        if(d!=0) { nonZeroNeighbor.push_back(d); }
        if(!nonZeroNeighbor.empty()){
            int minVal = *min_element(nonZeroNeighbor.begin(), nonZeroNeighbor.end());
            if(zeroFramedAry[i][j] > minVal){
                EQAry[zeroFramedAry[i][j]] =  minVal;
            }
            zeroFramedAry[i][j] = minVal;
        }
    }

    int connect4Pass2(int** zeroFramedAry,  int* EQAry){
        int maxVal = 0;
        for(int i=numRows+1; i>=0; i--){
            for(int j=numCols+1; j>=0; j--){
                if(zeroFramedAry[i][j] > 0){
                    changeLabelPass2_connect4(i, j, zeroFramedAry, EQAry);
                }
            }
        }
        for(int i=1; i<=newLabel; i++){
            maxVal = max(maxVal, EQAry[i]);
        }
        this->newMax = maxVal;
        return maxVal;
    }

    int changeLabelPass1_connect4(int i,int j,int** zeroFramedAry,int* EQAry){
         vector<int> nonZeroNeighbor;
         int b = zeroFramedAry[i-1][j];
         int d = zeroFramedAry[i][j-1];
         if(b!=0) { nonZeroNeighbor.push_back(b); }
         if(d!=0) { nonZeroNeighbor.push_back(d); }
         if(nonZeroNeighbor.empty()){
             newLabel++;
             zeroFramedAry[i][j] = newLabel;
         }else{
             int minVal = *min_element(nonZeroNeighbor.begin(), nonZeroNeighbor.end());
             zeroFramedAry[i][j] = minVal;
             if(b * d != 0 && b!=d){ //case 3
                 if(b < d){
                     EQAry[d] = b;
                 }else{ //b > d
                     EQAry[b] = d;
                 }
             }
         }
      this->newMax = newLabel;
         return newLabel;
    }

    int manageEQAry(int* EQAry){
        int readLabel = 0;
        int index = 1;
        while(index <= newLabel){
            if(index != EQAry[index]){
                EQAry[index] = EQAry[EQAry[index]];
            }else{
                readLabel++;
                EQAry[index] = readLabel;
            }
            index++;
        }
        return readLabel;
    }

    void connectPass3(int** zeroFramedAry, int* EQAry, int maxLabel){
        CCproperty = new Property*[maxLabel + 1];
        for(int i=1; i<maxLabel+1; i++){
            Property *prop = new Property(i, 0, numRows, numCols, 0, 0);
            CCproperty[i] = prop;
        }

        int numPixel = 0, minr = 9999, minc = 9999, maxr = 0, maxC = 0;
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j]>0){
                    zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
                }
            }
        }

        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    int curPix = zeroFramedAry[i][j];
                    //cout << "curPixel = " << curPix << endl;
                    CCproperty[curPix]->numPixels++;
                    CCproperty[curPix]->minR = min(i, CCproperty[curPix]->minR);
                    CCproperty[curPix]->minC = min(j, CCproperty[curPix]->minC);
                    CCproperty[curPix]->maxR = max(i, CCproperty[curPix]->maxR);
                    CCproperty[curPix]->maxC = max(j, CCproperty[curPix]->maxC);
                }
            }
        }

        this->newMax = maxLabel;
    }

    void printCCproperty(int maxLabel, ofstream& outFile){
        outFile << endl << numRows << " " << numCols << " " << newMin << " " << maxLabel<< endl;
        outFile << maxLabel << endl;
        for(int i=1; i<=maxLabel; i++){
          outFile << "-------------------" << endl;
          outFile << CCproperty[i]->label << endl;
          outFile << CCproperty[i]->numPixels << endl;
          outFile << CCproperty[i]->minR << " " << CCproperty[i]->minC << endl;
          outFile << CCproperty[i]->maxR << " " << CCproperty[i]->maxC << endl;
        }
    }

    void drowBoxes(int** zeroFramedAry, int maxLabel){
        int index = 1;
        int minRow = 0, minCol = 0, maxRow = 0, maxCol = 0;
        while(index <= maxLabel){
            minRow = CCproperty[index]->minR + 1;
            minCol = CCproperty[index]->minC + 1;
            maxRow = CCproperty[index]->maxR + 1;
            maxCol = CCproperty[index]->maxC + 1;
            int label = CCproperty[index]->label;
            for(int c=minCol; c<=maxCol; c++){
                zeroFramedAry[minRow][c] = label;
                zeroFramedAry[maxRow][c] = label;
            }
            for(int r=minRow; r<=maxRow; r++){
                zeroFramedAry[r][minCol] = label;
                zeroFramedAry[r][maxCol] = label;
            }
            index++;
        }
    }
    void printEQAry(int* EQAry, ofstream& outFile){

        for(int i=1; i<=newLabel; i++){
            outFile << i << "   " << EQAry[i] << endl;
        }
    }
    void printEQAry(int* EQAry, ofstream& outFile, int maxLabel){

        for(int i=1; i<=maxLabel; i++){
            outFile << i << "   " << EQAry[i] << endl;
        }
    }

};

int main(int argc, const char *argv[]) {

    ifstream inFile(argv[1]);
    int connectness = stoi(argv[2]);
    ofstream RFprettyPrintFile(argv[3]);
    ofstream labelFile(argv[4]);
    ofstream propertyFile(argv[5]);

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;

    int newMin = minVal;
    int newMax = maxVal;
    int trueNumCC = 0;
    CClabel *cclabel = new CClabel(numRows, numCols, minVal, maxVal, newMin, newMax,trueNumCC);
    newMin = cclabel->newMin;
    newMax = cclabel->newMax;
    int** zeroFramedAry = cclabel->zeroFramedAry;
    int* EQAry = cclabel->EQAry;
    trueNumCC = cclabel->trueNumCC;
    cclabel->zero2D(zeroFramedAry, numRows+2, numCols+2);
    cclabel->loadImage(inFile, zeroFramedAry);
    RFprettyPrintFile << endl << endl << "Reformate Pretty Print for initial zeroFramedAry: " << endl << endl;
    cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
    int maxLabel = 0;
    if(connectness==8){
        // 8-connectness-pass1
        maxLabel = cclabel->connect8Pass1(zeroFramedAry, EQAry);
        RFprettyPrintFile << endl << endl << "Result of 8-Connected Pass 1: " << endl << endl;
        cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
        RFprettyPrintFile << endl << endl << "Equivalency Array after 8-Connected Pass 1: " << endl;
        cclabel->printEQAry(EQAry, RFprettyPrintFile,maxLabel);

        // 8-connectness-pass2
        maxLabel = cclabel->connect8Pass2(zeroFramedAry,EQAry);
        RFprettyPrintFile << endl << endl << "Result of 8-Connected Pass 2: " << endl << endl;
        cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
        RFprettyPrintFile << endl << endl << "Equivalency Array after 8-Connected Pass 2: " << endl;
        cclabel->printEQAry(EQAry, RFprettyPrintFile, maxLabel);
    }else{
        // 4-connectness-pass1
        maxLabel = cclabel->connect4Pass1(zeroFramedAry,  EQAry);
        RFprettyPrintFile << endl << endl << "Result of 4-Connected Pass 1: " << endl << endl;
        cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
        RFprettyPrintFile << endl << endl << "Equivalency Array after 4-Connected Pass 1: " << endl;
        cclabel->printEQAry(EQAry, RFprettyPrintFile, maxLabel);

        // 4-connectness-pass2
        maxLabel = cclabel->connect4Pass2(zeroFramedAry,  EQAry);
        RFprettyPrintFile << endl << endl << "Result of 4-Connected Pass 2: " << endl << endl;
        cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
        RFprettyPrintFile << endl << endl << "Equivalency Array after 4-Connected Pass 2: " << endl;
        cclabel->printEQAry(EQAry, RFprettyPrintFile, maxLabel);
    }


         RFprettyPrintFile << endl << endl << "Result of updated Equivalency Array after manage: " << endl << endl;
         trueNumCC = cclabel->manageEQAry(EQAry);
         cclabel->printEQAry(EQAry, RFprettyPrintFile,maxLabel);

         //Pass 3
          RFprettyPrintFile << endl << endl << "Result of Pass 3: " << endl << endl;
         cclabel->connectPass3(zeroFramedAry, EQAry, trueNumCC);
         cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);
         RFprettyPrintFile << endl << endl << "Equivalency Array after Pass 3: " << endl;
         cclabel->printEQAry(EQAry, RFprettyPrintFile,maxLabel);

         newMin = cclabel->newMin;
         newMax = cclabel->trueNumCC;

         labelFile << endl << numRows << " " << numCols << " " << newMin << " " << trueNumCC << endl;
         for(int i=1; i<numRows+1; i++){
             for(int j=1; j<numCols+1; j++){
                 if(zeroFramedAry[i][j] <10){
                     labelFile << "   " << zeroFramedAry[i][j];
                 }else if(zeroFramedAry[i][j] <100){
                     labelFile << "  " << zeroFramedAry[i][j];
                 }
             }
             labelFile << endl;
         }

         newMin = cclabel->newMin;
         newMax = cclabel->trueNumCC;
         propertyFile << endl << "Connected Component Property:  " << endl;
         cclabel->printCCproperty(trueNumCC, propertyFile);

         //Drawing Box
         RFprettyPrintFile << endl << endl << "Result after Drawing Box: " << endl << endl;
         cclabel->drowBoxes(zeroFramedAry, trueNumCC);
         cclabel->imgReformat(zeroFramedAry, numRows+2, numCols+2, RFprettyPrintFile);

         RFprettyPrintFile << endl << endl << "The trueNumCC is : " << trueNumCC  << endl;
         inFile.close();
         RFprettyPrintFile.close();
         labelFile.close();
         propertyFile.close();
        return 0;
}
