#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class kCurvature;
class Image{
    friend class kCurvature;
public:
    int numRows, numCols;
    int minVal, maxVal;
    int label; //
    int** imgAry;
    Image(){ }
    Image(int numRows, int numCols, int minVal, int maxVal, int label){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->label = label;
        imgAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            imgAry[i] = new int[numCols];
            for(int j=0; j<numCols; j++){
                imgAry[i][j] = 0;
            }
        }
    }

    //void plotPt2Img(int** imgAry){ }

    void reformatPrettyPrint(int** inArray, ofstream& outFile){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(inArray[i][j] != 0){
                    outFile << inArray[i][j] << " ";
                }else{
                    outFile << "." << " ";
                }
            }
            outFile << endl;
        }
    }

    int countPts(ifstream& inFile){
        int numPts = 0;
        while(!inFile.eof()){
            int x = -1, y = -1;
            inFile >> x >> y;
            if(x!=-1 && y!=-1){
                numPts++;
            }
        }
        return numPts;
    }

    void print(int** imgAry, ofstream& outFile){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                outFile << imgAry[i][j] << " ";
            }
            outFile << endl;
        }
    }
};

class BoundaryPt{
    friend class kCurvature;
public:
    int x, y;
    double curvature;
    int localMax;
    int corner; // 1 = non-corner,  or 9 = corner
    BoundaryPt(int x, int y, double curvature, int localMax, int corner){
        this->x = x;
        this->y = y;
        this->curvature = curvature;
        this->localMax = localMax;
        this->corner = corner;
        //cout << "Point created" << endl;
    }
};

class kCurvature{
public:
    int K, numPts;
    int Q, P, R;
    BoundaryPt** PtAry;
    int** boundAry;
    kCurvature(int K, int numPts){
        this->K = K;
        this->numPts = numPts;
        Q = 0; P = K; R = 2 * K;
        PtAry = new BoundaryPt*[numPts];
        boundAry = new int*[numPts];
        for(int i=0; i<numPts; i++){
            boundAry[i] = new int[2];
        }

    }
    kCurvature(int K, int numPts, int Q, int P, int R){
        this->K = K;
        this->numPts = numPts;
        this->Q = Q;
        this->P = P;
        this->R;
    }
    void initialization(ifstream& inFile, ofstream& outFile){
        int index = 0;
        while(!inFile.eof() && index < numPts){
            int  x = -1, y = -1;
            inFile >> x >> y;
            if(x!=-1 && y!=-1){
                boundAry[index][0] = x;
                boundAry[index][1] = y;
            }
            index++;
        }
        storePt(outFile);
    }
    void storePt(ofstream& outFile){
        outFile << "Print Boundary Points Array: " << endl;
        for(int i=0; i<numPts; i++){
            int curX = boundAry[i][0];
            int curY = boundAry[i][1];
            //cout << "get: " << curX << " " << curY << endl;
            PtAry[i] = new BoundaryPt(curX, curY, 0.0, -1,-1);
            outFile <<  PtAry[i]->x << " " << PtAry[i]->y << endl;
            //cout << "Array : " <<  PtAry[i]->x << " " << PtAry[i]->y << endl;
        }
        /*
        for(int i=0; i<numPts; i++) {
            cout << "Array : " << PtAry[i]->x << " " << PtAry[i]->y << endl;
        }*/
    }

    void cornerDetection(BoundaryPt** PtAry, ofstream& outFile){
        int Q = 0, P = K, R = 2 * K;
        //cout << "Origin: " << Q << " " << P << " " << R << endl;
        int index = P;
        double curva = 0.0;
        int i = 0;
        outFile << "\nrow " << "col " << "curvature" << endl;
        while(i<numPts){
            curva = computeCurvature(PtAry, Q, P, R);
            PtAry[index%numPts]->curvature = curva;
            //cout << " index = " << index << ", i =  " << i  << " Origin: " << Q << " " << P << " " << R << endl;
            //cout << "PtAry[" << (index%numPts) << "].curvature = " <<  PtAry[index%numPts]->curvature <<  endl;
           // cout << " x = " << PtAry[index%numPts]->x <<  " y = " << PtAry[index%numPts]->y << endl;
            outFile << PtAry[index%numPts]->x <<  " " << PtAry[index%numPts]->y << " " << PtAry[index%numPts]->curvature << endl;
            Q = (Q+1) % numPts;
            P = (P+1) % numPts;
            R = (R+1) % numPts;
            index++;
            i++;
        }
    }
    //double computeCurvature(int Q, int P, int R);
    double computeCurvature(BoundaryPt** PtAry, int Q, int P, int R){
        double result = 0.0, slop1 = 0.0, slop2 = 0.0;
        double r1 =-1, c1 = -1, r2 = -1, c2 = -1, r3 = -1, c3 = -1;
        if(P < numPts && Q < numPts && R < numPts){
            //Q
            r1 = PtAry[Q]->x;
            c1 = PtAry[Q]->y;
            //P
            r2 = PtAry[P]->x;
            c2 = PtAry[P]->y;
            //R
            r3 = PtAry[R]->x;
            c3 = PtAry[R]->y;
            // cout << "\nData: " << r1 <<  " " << c1 << " " << r2 << " " << c2 << " " << r3 << " " << c3 << endl;
            if(r1-r2==0){
                slop1 = (c1-c2)/0.5;
            }else{
                slop1 = (c1-c2)/(r1-r2);
            }
            if(r2-r3==0){
                slop2 = (c2-c3)/0.5;
            }else{
                slop2 = (c2-c3)/(r2-r3);
            }
            if(slop1 > slop2){
                result = slop1 - slop2;
            }else{
                result = slop2 - slop1;
            }
            //cout << "result[" << P << "] = " << result << endl;
        }
        return result;
    }

    int computeLocalMaxima(BoundaryPt** PtAry, int i){
            int neib1 = (i-2) < 0 ? (numPts-2) : (i-2);
            int neib2 = (i-1) < 0 ? (numPts-1) : (i-1);
            int neib3 = (i+1) >= numPts ? (i+1)%numPts : (i+1);
            int neib4 = (i+2) >= numPts ? (i+2)%numPts : (i+2);
            if(PtAry[i]->curvature > PtAry[neib1]->curvature &&
               PtAry[i]->curvature > PtAry[neib2]->curvature &&
               PtAry[i]->curvature > PtAry[neib3]->curvature &&
               PtAry[i]->curvature > PtAry[neib4]->curvature){
                return 1;
            }
            return 0;
    }

    void markCorner(BoundaryPt** PtAry){
        int isLocalMax = 0, preMax = 0, nextMax = 0;
        int pre2Max = 0, next2Max = 0;
        int index = 0;
        for(int i=0; i<numPts; i++){
            //index = i;
            isLocalMax = computeLocalMaxima(PtAry, i);
            //index = (i-1) < 0 ? (numPts-1) : (i-1);
            if(i-1 < 0){
                preMax = computeLocalMaxima(PtAry, numPts-1);
            }else{
                preMax = computeLocalMaxima(PtAry, i-1);
            }
            if(i+1 >= numPts){
                nextMax = computeLocalMaxima(PtAry, (i+1)%numPts);
            }else{
                nextMax = computeLocalMaxima(PtAry, i+1);
            }

            if(i-2 < 0){
                pre2Max = computeLocalMaxima(PtAry, numPts-2);
            }else{
                pre2Max = computeLocalMaxima(PtAry, i-2);
            }
            if(i+2 >= numPts){
                next2Max = computeLocalMaxima(PtAry, (i+2)%numPts);
            }else{
                next2Max = computeLocalMaxima(PtAry, i+2);
            }

            if(isLocalMax==1){
                if( preMax==1 || nextMax==1 || pre2Max==0 && next2Max==0){
                    PtAry[i]->corner = 9;
                }
            }else{
                PtAry[i]->corner = 1;
            }
        }
    }

    void printBoundary(BoundaryPt** PtAry, ofstream& outFile){
         for(int i=0; i<numPts; i++){
             outFile << PtAry[i]->x << " " << PtAry[i]->y << " " << PtAry[i]->corner << endl;
         }
    }

    void plotPt2Img(BoundaryPt** PtAry, int** imgAry){
        for(int i=0; i<numPts; i++){
            int x = PtAry[i]->x;
            int y = PtAry[i]->y;
            int corner = PtAry[i]->corner;
            imgAry[x][y] = corner;
        }
    }

};

int main(int argc, const char *argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile1(argv[2]); //The result of the K-curvature of the object boundary points.
    ofstream outFile2(argv[3]); //Pretty print (displaying)of the result of the K-curvature corner detection
    ofstream outFile3(argv[4]); //all debugging output
    int K = stoi(argv[5]);
    int numRows = 0, numCols = 0, minVal = 0, maxVal = 0, label = 0;
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    inFile >> label;
    cout << K << endl;
    cout << numRows << " " << numCols << " " << minVal << " "<< maxVal << endl;
    cout <<  label << endl;
    cout << "Points: " << endl;
    outFile1 << numRows << " " << numCols << " " << minVal << " "<< maxVal << endl;
    outFile1 << label << endl;
    Image  *image  = new Image(numRows, numCols, minVal, maxVal, label);
    int** imgAry = image->imgAry;
    int numPts = image->countPts(inFile);
    cout << "numPts = " << numPts << endl;
    outFile1 << numPts << endl;
    outFile2 << numRows << " " << numCols << " " << minVal << " "<< maxVal << endl;
    outFile2 << label << endl;
    inFile.close();
    //inFile(argv[1]);
    inFile.open(argv[1]);
    int numRows1 = 0, numCols1 = 0, minVal1 = 0, maxVal1 = 0, label1 = 0;

    inFile >> numRows1;
    inFile >> numCols1;
    inFile >> minVal1;
    inFile >> maxVal1;
    inFile >> label1;
    cout << "label1 = " << label1 << endl;
    //cout << numRows1 << " " << numCols1 << " " << minVal1 << " "<< maxVal1 << " " << label1 << endl;
    kCurvature  *kCur = new kCurvature(K, numPts);
    BoundaryPt** PtAry = kCur->PtAry;
    kCur->initialization(inFile, outFile3);
   /* for(int i=0; i<numPts; i++) {
        cout << "PtAry-" << i << " : " << PtAry[i]->x << " " << PtAry[i]->y << endl;
    }*/
    kCur->cornerDetection(PtAry,outFile3);
    kCur->markCorner(PtAry);
    kCur->printBoundary(PtAry, outFile1);
    kCur->plotPt2Img(PtAry, imgAry);
    //image->print(imgAry, outFile2);
    image->reformatPrettyPrint(imgAry, outFile2);

    inFile.close();
    outFile1.close();
    outFile2.close();
    outFile3.close();
    return 0;
}
