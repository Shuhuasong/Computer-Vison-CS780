import java.io.*;
import java.util.Scanner;



public class Morphology {

     public static int ROWs = 0, COLS = 0;

     int numImgRows, numImgCols;
     int imgMin, imgMax;

     int numStructRows, numStructCols;
     int structMin, structMax;
     int rowOrigin, colOrigin;

    int rowFrameSize;  // numStructRows / 2
    int colFrameSize; //  set to numStructCols / 2

    int extraRows; // = rowFrameSize / 2
    int extraCols; // = colFrameSize / 2

    public int[][] zeroFrameAry;
    public int[][] morphAry;
    public int[][] tempAry;
    public int[][] structAry;
    
    public  Morphology(){};
    public Morphology(int numImgRows,int numImgCols,int imgMin,int imgMax,
                      int numStructRows, int numStructCols, int structMin, int structMax,
                      int rowOrigin,int  colOrigin){
        this.numImgRows = numImgRows;
        this.numImgCols = numImgCols;
        this.imgMin = imgMin;
        this.imgMax = imgMax;
        this.numStructRows = numStructRows;
        this.numStructCols = numStructCols;
        this.structMin = structMin;
        this.structMax = structMax;
        this.rowOrigin = rowOrigin;
        this.colOrigin = colOrigin;
        this.rowFrameSize = numStructRows/2;
        this.colFrameSize = numStructCols/2;

        this.extraRows = rowFrameSize * 2;
        this.extraCols = colFrameSize * 2;
        zeroFrameAry = new int[numImgRows + extraRows][numImgCols + extraCols];
        morphAry     = new int[numImgRows + extraRows][numImgCols + extraCols];
        tempAry      = new int[numImgRows + extraRows][numImgCols + extraCols];
        structAry = new int[numStructRows][numStructCols];
    }
     public void zero2DAry(int[][] inArray){
        for(int i=0; i<inArray.length; i++){
            for(int j=0; j<inArray[0].length; j++){
                inArray[i][j] = 0;
           }
        }
     }
    public void loadImg(Scanner inFile, int[][] zeroFrameAry){
        for(int i=rowOrigin; i<numImgRows+rowOrigin; i++){
            for(int j=colOrigin; j<numImgCols+colOrigin; j++){
                zeroFrameAry[i][j] = inFile.nextInt();
            }
        }
    }
    public void loadstruct(Scanner inFile, int[][] structAry){
        for(int i=0; i<numStructRows; i++){
            for(int j=0; j<numStructCols; j++){
                structAry[i][j] = inFile.nextInt();
            }
        }
    }
    public void computeDilation(int[][] inArray, int[][] outArray, int[][] structAry){
       int i = rowFrameSize;
       while(i<numImgRows+rowFrameSize){
           int j = colFrameSize;
           while(j<numImgCols+colFrameSize){
               if(inArray[i][j] == 1){
                   //System.out.println(i + " " + j + " " + inArray[i][j]);
                   dilation(i, j, inArray, outArray, structAry, rowFrameSize, colFrameSize);
               }
                 j++;
            }
           i++;
        }
    }

    public void dilation(int i, int j, int[][] inArray, int[][] outArray, int[][] structAry, int rowFrameSize, int colFrameSize){
        for(int r=i-rowFrameSize, x=0; r<i+rowFrameSize && x<structAry.length; r++, x++) {
              for (int c=j-colFrameSize, y=0; c<j+colFrameSize && y<structAry[0].length; c++, y++) {
                  if (structAry[x][y] == 1) {
                      outArray[r][c] = 1;
                  }
              }
          }
    }
    public void computeErosion(int[][] inArray, int[][] outArray, int[][] structAry){
        int i = rowFrameSize;
        while(i<numImgRows+rowFrameSize){
            int j = colFrameSize;
            while(j<numImgCols+colFrameSize){
                if(inArray[i][j] == 1){
                   // System.out.println(i + " " + j + " " + inArray[i][j]);
                    erosion(i, j, inArray, outArray, structAry, rowFrameSize, colFrameSize);
                }
                j++;
            }
            i++;
        }
    }
    public void erosion(int i, int j, int[][] inArray, int[][] outArray, int[][] structAry, int rowFrameSize, int colFrameSize){
        boolean flag = true;
        for(int r=i-rowFrameSize, x=0; r<=i+rowFrameSize && x<structAry.length; r++, x++) {
            for (int c=j-colFrameSize, y=0; c <=j+colFrameSize && y<structAry[0].length; c++, y++) {
                //System.out.println(structAry[x][y] + "--- " + inArray[r][c]);
                if (structAry[x][y] == 1 && inArray[r][c]==0) {
                   flag = false;
                   break;
                }
            }
        }
        if(flag){
            outArray[i][j] = 1;
        }
    }
    //Opening
    public void computeOpening(int[][] zeroFrameAry, int[][] morphAry, int[][] tempAry, int[][] structAry){
        computeErosion(zeroFrameAry, tempAry, structAry);
        computeDilation(tempAry, morphAry, structAry);
    }

    //Closing
    public void computeClosing(int[][]zeroFrameAry, int[][] morphAry, int[][] tempAry, int[][] structAry){
        computeDilation(zeroFrameAry, tempAry, structAry);
        computeErosion(tempAry, morphAry, structAry);
    }

    public void aryToFile(int[][] inArray, BufferedWriter outFile) throws IOException {
       outFile.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n\n");
       for(int i=rowOrigin; i<inArray.length-rowFrameSize-1; i++) {
           for (int j = colOrigin; j < inArray[0].length-colFrameSize-1; j++) {
               outFile.write(inArray[i][j] + " ");
           }
           outFile.write("\n");
       }
    }

    public void prettyPrint(int[][] inArray, BufferedWriter outFile) throws IOException {

         for(int i=0; i<inArray.length; i++){
            for(int j=0; j<inArray[0].length; j++){
                if(inArray[i][j]==0) {
                    outFile.write("." + " ");
                }else{
                    outFile.write(inArray[i][j] + " ");
                }
            }
            outFile.write("\n");
         }
    }

    public void print(int[][] inArray, BufferedWriter outFile) throws IOException {
          for(int i=0; i<inArray.length; i++){
              for(int j=0; j<inArray[0].length; j++){
                 outFile.write(inArray[i][j] + " ");
              }
             outFile.write("\n");
          }
           outFile.write("\n\n");
    }

    public static void main(String[] args) throws IOException {

        Scanner inFile1 = new Scanner(new FileReader(args[0]));
        Scanner inFile2 = new Scanner(new FileReader(args[1]));
        BufferedWriter dilateOutFile = new BufferedWriter(new FileWriter(new File(args[2])));
        BufferedWriter erodeOutFile = new BufferedWriter(new FileWriter(new File(args[3])));
        BufferedWriter closingOutFile = new BufferedWriter(new FileWriter(new File(args[4])));
        BufferedWriter openingOutFile =  new BufferedWriter(new FileWriter(new File(args[5])));
        BufferedWriter prettyPrintFile = new BufferedWriter(new FileWriter(new File(args[6])));
        BufferedWriter originPretty = new BufferedWriter(new FileWriter(new File(args[7])));

        int numImgRows, numImgCols, imgMin, imgMax;
        int numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
        numImgRows = inFile1.nextInt();
        numImgCols = inFile1.nextInt();
        imgMin = inFile1.nextInt();
        imgMax = inFile1.nextInt();
        numStructRows = inFile2.nextInt();
        numStructCols = inFile2.nextInt();
        structMin = inFile2.nextInt();
        structMax = inFile2.nextInt();
        rowOrigin = inFile2.nextInt();
        colOrigin = inFile2.nextInt();
        Morphology morphology = new Morphology(numImgRows, numImgCols, imgMin, imgMax,
                                     numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin);
        System.out.println(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax);

        //dilateOutFile.write("This String would be written" + " to the specified File");
        int[][] zeroFrameAry = morphology.zeroFrameAry;
        int[][] structAry = morphology.structAry;
        int[][] morphAry = morphology.morphAry;
        int[][] tempAry = morphology.tempAry;
        morphology.zero2DAry(zeroFrameAry);
        morphology.loadImg(inFile1, zeroFrameAry);
        originPretty.write("Origin ZeroFrameAry with Frame \n\n");
        originPretty.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n");
        morphology.print(zeroFrameAry, originPretty);
        prettyPrintFile.write("ZeroFramedArray's Pretty Print\n\n");
        morphology.prettyPrint(zeroFrameAry, prettyPrintFile);
        prettyPrintFile.write("\n\n");

        morphology.zero2DAry(structAry);
        morphology.loadstruct(inFile2, structAry);
        originPretty.write("Struct Element Ary  \n\n");
        originPretty.write(numStructRows + " " + numStructCols + " " + structMin + " " + structMax + "\n");
        originPretty.write(rowOrigin + " " + colOrigin + "\n");
        morphology.print(structAry, originPretty);
        prettyPrintFile.write("StructAry's Pretty Print\n\n");
        morphology.prettyPrint(structAry, prettyPrintFile);
        prettyPrintFile.write("\n\n");
        //Dilation
        morphology.zero2DAry(morphAry);
        morphology.computeDilation(zeroFrameAry, morphAry,structAry);
        morphology.aryToFile(morphAry, dilateOutFile);
        prettyPrintFile.write("Dilation's Pretty Print\n\n");
        morphology.prettyPrint(morphAry, prettyPrintFile);
        //Erosion
        morphology.zero2DAry(morphAry);
        morphology.computeErosion(zeroFrameAry, morphAry, structAry);
        morphology.aryToFile(morphAry, erodeOutFile);
        prettyPrintFile.write("Erosion's Pretty Print\n\n");
        morphology.prettyPrint(morphAry, prettyPrintFile);
        //Opening
        morphology.zero2DAry(morphAry);
        morphology.computeOpening(zeroFrameAry, morphAry, tempAry, structAry);
        morphology.aryToFile(morphAry, openingOutFile);
        prettyPrintFile.write("Opening's Pretty Print\n\n");
        morphology.prettyPrint(morphAry, prettyPrintFile);
        //Closing
        morphology.zero2DAry(morphAry);
        morphology.computeClosing(zeroFrameAry, morphAry, tempAry, structAry);
        morphology.aryToFile(morphAry, closingOutFile);
        prettyPrintFile.write("Closing's Pretty Print\n\n");
        morphology.prettyPrint(morphAry, prettyPrintFile);

        inFile1.close();
        inFile2.close();
        dilateOutFile.close();
        erodeOutFile.close();
        closingOutFile.close();
        openingOutFile.close();
        prettyPrintFile.close();
        originPretty.close();
    }
}
