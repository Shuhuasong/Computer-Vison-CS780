#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdlib.h>
using namespace std;

class ImageProcessing{

private:
    int numRows, numCols;
    int minVal, maxVal;
    int newMin, newMax;
    int thrVal; // argv[2]

public:
    int* neighborAry;
    int** mirror3by3Ary;
    int** mirror5by5Ary;
    int** avgAry;
    int** newAvgAry;
    int** thrAry;
    int** medianAry;
    int** newMedianAry;
    int** CPAry;
    int** newCPAry;
    int** CPneighbor5x5;
    int* originImage;
    int** inputImage;

    ImageProcessing(){
        numRows = 0; numCols = 0;
        minVal = 0; maxVal = 0;
        newMin = 0; newMax = 0;
        neighborAry = nullptr;
        mirror3by3Ary = nullptr;
        mirror5by5Ary = nullptr;
        avgAry = nullptr;
        newAvgAry = nullptr;
        thrAry = nullptr;
        medianAry = nullptr;
        newMedianAry = nullptr;
        CPAry = nullptr;
        newCPAry = nullptr;
        CPneighbor5x5 = nullptr;
        originImage = nullptr;
        inputImage = nullptr;
    }

    ImageProcessing(int numRows, int numCols, int minVal, int maxVal, int newMin, int newMax, int thrVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->newMin = newMin;
        this->newMax = newMax;
        this->thrVal = thrVal;
        neighborAry = new int[9];
        //loading input image
        mirror3by3Ary = new int*[numRows + 2];
        for(int i=0; i<numRows+2; i++){
            mirror3by3Ary[i] = new int[numCols + 2];
            for(int j=0; j<numCols+2; j++){
                mirror3by3Ary[i][j] = 0;
            }
        }
        //loading input image
        mirror5by5Ary = new int*[numRows + 4];
        for(int i=0; i<numRows+4; i++){
            mirror5by5Ary[i] = new int[numCols + 4];
            for(int j=0; j<numCols+2; j++){
                mirror5by5Ary[i][j] = 0;
            }
        }
        //storing the result of the 3*3 averaging
        avgAry = new int*[numRows + 2];
        for(int i=0; i<numRows+2; i++){
            avgAry[i] = new int[numCols + 2];
            for(int j=0; j<numCols+2; j++){
                avgAry[i][j] = 0;
            }
        }
        //storing the result of the 3*3 averaging without frame
        newAvgAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            newAvgAry[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                newAvgAry[i][j] = 0;
            }
        }
        //storing the result of the 3*3 averaging after threshold
        thrAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            thrAry[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                thrAry[i][j] = 0;
            }
        }
        //storing the result of the 3*3 median filter
        medianAry = new int*[numRows + 2];
        for(int i=0; i<numRows+2; i++){
            medianAry[i] = new int[numCols + 2];
            for(int j=0; j<numCols+2; j++){
                medianAry [i][j] = 0;
            }
        }

        newMedianAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            newMedianAry[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                newMedianAry[i][j] = 0;
            }
        }
        //storing the result of 5*5 corner preserving averaging
        CPAry = new int*[numRows + 4];
        for(int i=0; i<numRows+4; i++){
            CPAry[i] = new int[numCols+4];
            for(int j=0; j<numCols+4; j++){
                CPAry[i][j] = 0;
            }
        }

        newCPAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            newCPAry[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                newCPAry[i][j] = 0;
            }
        }

        CPneighbor5x5 = new int*[5];
        for(int i=0; i<5; i++){
            CPneighbor5x5[i] = new int[5];
            for(int j=0; j<5; j++){
                CPneighbor5x5[i][j] = 0;
            }
        }

        originImage = new int[numRows * numCols];
        for(int i=0; i<numRows*numCols; i++){
            originImage[i] = 0;
        }

        inputImage = new int*[numRows];
        for(int i=0; i<numRows; i++){
            inputImage[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                inputImage[i][j] = 0;
            }
        }
    }

    void loadImage(ifstream& inFile){
        int k=0, x = 0;
       for(int i=0; i<numRows && k<numRows*numCols; i++){
           for(int j=0; j<numCols; j++){
               inFile >> originImage[k];
              // cout << originImage[k] << " ";
               k++;
           }
       }
       //load data to mirror3By3Ary
       k=0;
       for(int i=1; i<numRows+1 && k<numRows*numCols; i++){
           for(int j=1; j<numCols+1; j++){
               mirror3by3Ary[i][j] = originImage[k++];
           }
       }
       k=0;
       for(int i=2; i<numRows+2 && k<numRows*numCols; i++){
           for(int j=2; j<numCols+2; j++){
               mirror5by5Ary[i][j] = originImage[k++];
               //outFile <<  mirror5by5Ary[i][j] << " ";
           }
           //outFile << endl;
       }
       k = 0;
       for(int i=0; i<numRows && k<numRows*numCols; i++){
           for(int j=0; j<numCols; j++){
               inputImage[i][j] = originImage[k++];
           }
       }
    }

    void mirrorFraming(int frameSize){
         if(frameSize==1){
             for(int j=1; j<numCols+1; j++){
                 mirror3by3Ary[0][j] = mirror3by3Ary[1][j];
                 mirror3by3Ary[numRows+1][j] = mirror3by3Ary[numRows][j];
             }
             for(int i=0; i<numRows+2; i++){
                 mirror3by3Ary[i][0] = mirror3by3Ary[i][1];
                 mirror3by3Ary[i][numCols+1] = mirror3by3Ary[i][numCols];
             }
         }else if(frameSize==2){
             for(int j=2; j<numCols+1; j++){
                 mirror5by5Ary[0][j] = mirror5by5Ary[3][j];
                 mirror5by5Ary[1][j] = mirror5by5Ary[2][j];
                 mirror5by5Ary[numRows+2][j] = mirror5by5Ary[numRows+1][j];
                 mirror5by5Ary[numRows+3][j] = mirror5by5Ary[numRows][j];
             }
             for(int i=0; i<numRows+4; i++){
                 mirror5by5Ary[i][0] = mirror5by5Ary[i][3];
                 mirror5by5Ary[i][1] = mirror5by5Ary[i][2];
                 mirror5by5Ary[i][numCols+2] = mirror5by5Ary[i][numCols+1];
                 mirror5by5Ary[i][numCols+3] = mirror5by5Ary[i][numCols];
             }
         }
    }

    void imgReformat(int **inArray, ofstream& outFile, int frameSize){
       outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
        for(int i=0; i<numRows+2*frameSize; i++){
            for(int j=0; j<numCols+2*frameSize; j++){
                //outFile << mirror5by5Ary[i][j] << "  ";
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

    void imgReformat(int **inArray, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;

        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                //outFile << mirror5by5Ary[i][j] << "  ";
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

    void computeMedian(int **inArray, int frameSize){
        newMin = 9999, newMax = 0;
        int r = 1;
        while(r < numRows + 1){
            int c = 1;
            while(c < numCols + 1){
                medianAry[r][c] = median3x3(inArray, r, c);
                if(newMin > medianAry[r][c]){
                    newMin = medianAry[r][c];
                }
                if(newMax < medianAry[r][c]){
                    newMax = medianAry[r][c];
                }
                c++;
            }
            r++;
        }
    }

    int median3x3(int ** inArray, int r, int c){
        int rows[9] = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
        int cols[9] = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
        int sum = 0;
        for(int i=0; i<9; i++){
            int x = r + rows[i];
            int y = c + cols[i];
            neighborAry[i] = inArray[x][y];
        }
        sort(neighborAry, neighborAry + 9);
        return neighborAry[4];
    }

    void computeAvg(int **inArray, int frameSize){
         newMin = 9999, newMax = 0;
        int r = 1;
        while(r < numRows + 1){
            int c = 1;
            while(c < numCols + 1){
                avgAry[r][c] = avg3x3(inArray, r, c);
                if(newMin > avgAry[r][c]){
                    newMin = avgAry[r][c];
                }
                if(newMax < avgAry[r][c]){
                    newMax = avgAry[r][c];
                }
                c++;
            }
            r++;
        }
    }

    int avg3x3(int **inArray, int r, int c){
        int rows[9] = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
        int cols[9] = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
        int sum = 0;
        for(int i=0; i<9; i++){
            int x = r + rows[i];
            int y = c + cols[i];
            neighborAry[i] = inArray[x][y];
            sum += neighborAry[i];
        }
        return sum/9;
    }

    void deleteFrame(int** avgAry, int** noFrameArray, int frameSize){
        int row = 0, col = 0;
        for(int i=frameSize; i<numRows+frameSize && row < numRows; i++){
            for(int j=frameSize; j<numCols+frameSize && col < numCols; j++){
                noFrameArray[row][col] = avgAry[i][j];
                col++;
                if(col==numCols){
                    col = 0;
                    row++;
                }
            }
        }
    }

    void threshold(int **thrAry, int ** newAvgAry, ofstream& outFile){

        newMin = 0, newMax = 1;
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(newAvgAry[i][j] >= thrVal){
                    thrAry[i][j] = 1;
                    outFile << thrAry[i][j] << " ";
                }else{
                    thrAry[i][j] = 0;
                    outFile << thrAry[i][j] << " ";
                }
            }
            outFile << endl;
        }
    }

    void computeCPfilter(int** inArray){
        newMin = 9999, newMax = 0;
        int r = 2;
        while(r < numRows + 2){
            int c = 2;
            while(c < numCols + 2){
                CPAry[r][c] = CP5x5(inArray, r, c);
                if(newMin > CPAry[r][c]){
                    newMin = CPAry[r][c];
                }
                if(newMax < CPAry[r][c]){
                    newMax = CPAry[r][c];
                }
                c++;
            }
            r++;
        }
    }

    int CP5x5(int** inArray, int r, int c){
        int rows[25] = {-2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
        int cols[25] = {-2,-1, 0, 1, 2, -2,-1, 0, 1, 2, -2,-1, 0, 1, 2, -2,-1, 0, 1, 2, -2,-1, 0, 1, 2};
        int avgArray[8];
        int diffOfX[8];
        int CPmasks[8][5][5] = {
                { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {1, 1, 1, 1, 1} },
                { {1, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 0, 0, 0}, {1, 0, 0, 0, 0} },
                { {1, 1, 1, 1, 1}, {0, 1, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} },
                { {0, 0, 0, 0, 1}, {0, 0, 0, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 0, 1} },
                { {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} },
                { {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} },
                { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1} },
                { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0} }
        };
        int  row = 0, col = 0;
        for(int i=0; i<25 && row < 5; i++){
            int x = r + rows[i];
            int y = c + cols[i];
            CPneighbor5x5[row][col] = inArray[x][y];
            col++;
            if(col==5){
                col = 0;
                row++;
            }
        }
        int l = 0;
        //cout << "(x, y) = " << inArray[r][c] << endl;
        for(int i=0; i<8 && l<8; i++){
            int sumOfMask = 0, total = 0;
            for(int j=0; j<5; j++){
                for(int k=0; k<5; k++){
                    sumOfMask += CPmasks[i][j][k];
                    total += CPneighbor5x5[j][k] * CPmasks[i][j][k];
                }
            }

            avgArray[l] = total/sumOfMask;
            //cout << avgArray[l] << " ";
            l++;
        }
        cout << endl;
        int minDiff = 9999, index = -1;
        for(int i=0; i<8; i++){
            diffOfX[i] = abs(avgArray[i]-inArray[r][c]);
            //cout << diffOfX[i] << " ";
            if(diffOfX[i] < minDiff){
                index = i;
                minDiff = diffOfX[i];
            }
        }
        cout << endl;
        //cout << avgArray[index] << endl;
        return avgArray[index];
    }

    void prettyPrint(int **inAry, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(inAry[i][j] > 0){
                    outFile << "  ";
                }else{
                    outFile <<  "." << " ";
                }
            }
            outFile << endl;
        }
    }
    void printArray(ofstream& outFile){
            for(int i=0; i<5; i++){
                for(int j=0; j<5; j++){
                    outFile << CPneighbor5x5[i][j] << " ";
                }
                outFile << endl;
            }
     }
};

int main(int argc, const char *argv[]) {
    //open input file
    ifstream inFile(argv[1]);
    int thrVal = stoi(argv[2]);
    int numRows, numCols, minVal, maxVal;
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    cout << thrVal << endl;
    int newMin = minVal;
    int newMax = maxVal;
    cout << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
    ImageProcessing *image = new ImageProcessing(numRows, numCols, minVal, maxVal, newMin, newMax, thrVal);
    //open output files
    ofstream rfImg(argv[3]);
    ofstream avgOutImg(argv[4]);
    ofstream avgThrImg(argv[5]);
    ofstream avgPrettyPrint(argv[6]);
    ofstream medianOutImg(argv[7]);
    ofstream medianThrImg(argv[8]);
    ofstream medianPrettyPrint(argv[9]);
    ofstream CPOutImg(argv[10]);
    ofstream CPThrImg(argv[11]);
    ofstream CPPrettyPrint(argv[12]);
    ofstream oriCPImg(argv[13]);
    //load image
    image->loadImage(inFile);
    int** inputImage = image->inputImage;
    image->imgReformat(inputImage, rfImg);

    //Average enchancement processing
    int** mirror3by3Ary = nullptr;
    image->mirrorFraming(1);
    mirror3by3Ary = image->mirror3by3Ary;
    image->imgReformat(mirror3by3Ary, oriCPImg, 1);//output of mirrorArray with frame
    image->computeAvg(mirror3by3Ary, 1);
    int** avgArray = image->avgAry;
    //image->imgReformat(avgArray, outFile3, 1);
    int** newAvgAry = image->newAvgAry;
    image->deleteFrame(avgArray, newAvgAry, 1);
    image->imgReformat(newAvgAry, avgOutImg);//output avgImg without frame
    int** thrAry = image->thrAry;
    image->threshold(thrAry, newAvgAry, avgThrImg);
    //image->imgReformat(thrAry, outFile3);
    image->prettyPrint(thrAry, avgPrettyPrint);

    //Median enchancement processing
    image->computeMedian(mirror3by3Ary, 1);
    int** medianAry = image->medianAry;
    //image->imgReformat(medianAry, outFile5,1);
    int** newMedianAry = image->newMedianAry;
    image->deleteFrame(medianAry, newMedianAry,1);
    image->imgReformat(newMedianAry, medianOutImg);
    image->threshold(thrAry, newMedianAry, medianThrImg);
    image->prettyPrint(thrAry, medianPrettyPrint);

    //Gaussion enchancement processing
    image->mirrorFraming(2);
    int** mirror5by5Ary = image->mirror5by5Ary;
    //image->imgReformat(mirror5by5Ary, oriCPImg, 2);
    //image->prettyPrint(mirror5by5Ary, )
    image->computeCPfilter(mirror5by5Ary);
    int** CPAry = image->CPAry;
    //image->imgReformat(CPAry, oriCPImg, 2);
    int** newCPAry = image->newCPAry;
    image->deleteFrame(CPAry, newCPAry, 2);
    image->imgReformat(newCPAry, CPOutImg);
    image->threshold(thrAry, newCPAry, CPThrImg);
    image->prettyPrint(thrAry, CPPrettyPrint);
    return 0;

}
