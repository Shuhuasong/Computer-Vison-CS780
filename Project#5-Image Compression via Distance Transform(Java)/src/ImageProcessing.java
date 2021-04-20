import java.io.*;
import java.util.PriorityQueue;
import java.util.Scanner;

public class ImageProcessing {
    int numRows, numCols;
    int minVal, maxVal;
    int newMinVal, newMaxVal;

    public  int[][] zeroFramedAry;
    public  int[][] skeletonAry;

    public ImageProcessing(){}
    public ImageProcessing(int numRows, int numCols, int minVal, int maxVal, int newMinVal, int newMaxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.newMinVal = newMinVal;
        this.newMaxVal = newMaxVal;

        zeroFramedAry = new int[numRows+2][numCols+2];
        skeletonAry = new int[numRows+2][numCols+2];

    }
    public void setZero(int[][] inArray){
        for(int i=0; i<inArray.length; i++){
            for(int j=0; j<inArray[0].length; j++){
                inArray[i][j] = 0;
            }
        }
    }

    public void loadImage(Scanner inFile, int[][] zeroFrameAry){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                zeroFrameAry[i][j] = inFile.nextInt();
            }
        }
    }

    public void firstPass_8Distance(int[][] inArray, BufferedWriter outFile) throws IOException {
        outFile.write("\nFirst Pass for 8-Connected Distance \n\n");
         for(int i=1; i<inArray.length-1; i++){
             for(int j=1; j<inArray[0].length-1; j++){
                 if(inArray[i][j] > 0){
                     int val = getCellVal_pass1(inArray, i, j);
                     inArray[i][j] = val + 1;
                     newMinVal = Math.min(newMinVal, inArray[i][j]);
                     newMaxVal = Math.max(newMaxVal, inArray[i][j]);
                 }
             }
         }
        System.out.println("firstPass_8Distance: " + newMinVal + " " + newMaxVal);
    }

    public int getCellVal_pass1(int[][] inArray, int i, int j){
        int a = inArray[i-1][j-1];
        int b = inArray[i-1][j];
        int c = inArray[i-1][j+1];
        int d = inArray[i][j-1];
        PriorityQueue<Integer> pq = new PriorityQueue<>();
        pq.add(a); pq.add(b);
        pq.add(c); pq.add(d);
        return pq.peek();
    }

    public void secondPass_8Distance(int[][] inArray, BufferedWriter outFile) throws IOException {
        newMaxVal = newMinVal;
        outFile.write("\n\nSeconde Pass for 8-Connected Distance\n");
         for(int i=inArray.length-2; i>=1; i--){
             for(int j=inArray[0].length-2; j>=1; j--){
                 int val = getCellVal_pass2(inArray, i, j);
                 inArray[i][j] = val;
                 newMinVal = Math.min(newMinVal, inArray[i][j]);
                 newMaxVal = Math.max(newMaxVal, inArray[i][j]);
             }
         }
        System.out.println("secondPass_8Distance: " + newMinVal + " " + newMaxVal);
    }

    public int getCellVal_pass2(int[][] inArray, int i, int j){
        int e = inArray[i][j+1]+1;
        int f = inArray[i+1][j-1]+1;
        int g = inArray[i+1][j]+1;
        int h = inArray[i+1][j+1]+1;
        PriorityQueue<Integer> pq = new PriorityQueue<>();
        pq.add(e); pq.add(f);
        pq.add(g); pq.add(h);
        pq.add(inArray[i][j]);
        return pq.peek();
    }

    public void compute8Distance(int[][] zeroFramedAry, BufferedWriter outFile) throws IOException {
        firstPass_8Distance(zeroFramedAry, outFile);
        reformatPrettyPrint(zeroFramedAry, outFile);
        secondPass_8Distance(zeroFramedAry, outFile);
        reformatPrettyPrint(zeroFramedAry, outFile);
    }


   public void skeletonExtraction(int[][] zeroFramedAry, int[][] skeletonAry, BufferedWriter skeletonFile, BufferedWriter outFile) throws IOException {
        outFile.write("\n Skeleton Extraction\n");
        computeLocalMaxima(zeroFramedAry, skeletonAry);
        reformatPrettyPrint(skeletonAry, outFile);
        extractLocalMaxima(skeletonAry, skeletonFile);
        skeletonFile.close();
   }

   public void computeLocalMaxima(int[][] zeroFramedAry, int[][] skeletonAry){
       newMaxVal = newMinVal;
          int rows = zeroFramedAry.length;
          int cols = zeroFramedAry[0].length;
          for(int i=1; i<rows-1; i++){
              for(int j=1; j<cols-1; j++){
                  int localMax = getLocalMax(zeroFramedAry, i, j);
                  //System.out.println();
                  if(zeroFramedAry[i][j] >= localMax){
                      skeletonAry[i][j] = zeroFramedAry[i][j];
                  }else{
                      skeletonAry[i][j] = 0;
                  }
                  newMinVal = Math.min(newMinVal, skeletonAry[i][j]);
                  newMaxVal = Math.max(newMaxVal, skeletonAry[i][j]);
              }
          }
       System.out.println("compute Local Maxima: " + newMinVal + " " + newMaxVal);
   }
   public int getLocalMax(int[][] zeroFramedAry, int i, int j){
        PriorityQueue<Integer> pq = new PriorityQueue<>((a, b)->b-a);
        for(int x=i-1; x<=i+1; x++){
            for(int y=j-1; y<=j+1; y++){
                if(x==i && y==j) continue;
                pq.add(zeroFramedAry[x][y]);
            }
        }
       /* pq.add(zeroFramedAry[i-1][j-1]);
        pq.add(zeroFramedAry[i-1][j]);
        pq.add(zeroFramedAry[i-1][j+1]);
        pq.add(zeroFramedAry[i][j-1]);
        pq.add(zeroFramedAry[i][j+1]);
        pq.add(zeroFramedAry[i+1][j-1]);
        pq.add(zeroFramedAry[i+1][j]);
        pq.add(zeroFramedAry[i+1][j+1]); */
        return pq.peek();
   }

   public void extractLocalMaxima(int[][] skeletonAry, BufferedWriter skeletonFile) throws IOException {
         for(int i=0; i<skeletonAry.length; i++){
             for(int j=0; j<skeletonAry[0].length; j++){
                 if(skeletonAry[i][j] > 0){
                     skeletonFile.write(i + "  " + j + "  " + skeletonAry[i][j] + "\n");
                 }
             }
         }
   }
   public void skeletonExpansion(int[][] zeroFramedAry,  BufferedWriter outFile, String skeletonFileName) throws IOException {
       Scanner skeletonFile = new Scanner(new FileReader(skeletonFileName));
       setZero(zeroFramedAry);
       loadSkeleton(skeletonFile, zeroFramedAry);
       firstPass_Expension(zeroFramedAry, outFile);
       reformatPrettyPrint(zeroFramedAry, outFile);
       secondPass_Expension(zeroFramedAry, outFile);
       reformatPrettyPrint(zeroFramedAry, outFile);
   }

   public void loadSkeleton(Scanner skeletonFile, int[][] zeroFramedAry){
        int i, j, value;
        while(skeletonFile.hasNextInt()){
            i = skeletonFile.nextInt();
            j = skeletonFile.nextInt();
            value = skeletonFile.nextInt();
            //System.out.println(i + " " + j + " " + value);
            zeroFramedAry[i][j] = value;
        }
   }

   public void firstPass_Expension(int[][] zeroFramedAry,  BufferedWriter outFile) throws IOException {
         outFile.write("\n" + "First Pass Expansion for 8-Connectness"  + "\n");
         int maxValue = 0;
         for(int i=1; i<zeroFramedAry.length-1; i++){
             for(int j=1; j<zeroFramedAry[0].length-1; j++){
                 if(zeroFramedAry[i][j]==0){
                     maxValue  = getLocalMax_firstExpansion(zeroFramedAry, i, j);
                     if(zeroFramedAry[i][j] < maxValue){
                         zeroFramedAry[i][j] = maxValue;
                         newMinVal = Math.min(newMinVal, zeroFramedAry[i][j]);
                         newMaxVal = Math.max(newMaxVal, zeroFramedAry[i][j]);
                     }
                 }
             }
         }
       System.out.println("firstPass_Expension: " + newMinVal + " " + newMaxVal);
   }

   public int getLocalMax_firstExpansion(int[][] zeroFramedAry, int i, int j){
       PriorityQueue<Integer> pq = new PriorityQueue<>((a, b)->b-a);
       for(int x=i-1; x<=i+1; x++){
           for(int y=j-1; y<=j+1; y++){
               if(x==i && y==j) continue;
               pq.add(zeroFramedAry[x][y]);
           }
       }
      /* pq.add(zeroFramedAry[i-1][j-1]-1);
       pq.add(zeroFramedAry[i-1][j]-1);
       pq.add(zeroFramedAry[i-1][j+1]-1);
       pq.add(zeroFramedAry[i][j-1]-1);
       pq.add(zeroFramedAry[i][j+1]-1);
       pq.add(zeroFramedAry[i+1][j-1]-1);
       pq.add(zeroFramedAry[i+1][j]-1);
       pq.add(zeroFramedAry[i+1][j+1]-1); */
       return pq.peek();
   }

   public void secondPass_Expension(int[][] zeroFramedAry, BufferedWriter outFile) throws IOException {
       newMaxVal = newMinVal;
        outFile.write("\n" + "Second Pass Expansion for 8-Connectness"  + "\n");
         int maxValue = 0;
        for(int i=zeroFramedAry.length-2; i>=1; i--){
             for(int j=zeroFramedAry[0].length-2; j>=1; j--){
                 maxValue = getLocalMax_secondExpansion(zeroFramedAry, i, j);
                 if(zeroFramedAry[i][j] < maxValue){
                     zeroFramedAry[i][j] = maxValue - 1;
                     newMinVal = Math.min(newMinVal, zeroFramedAry[i][j]);
                     newMaxVal = Math.max(newMaxVal, zeroFramedAry[i][j]);
                 }
             }
         }
       System.out.println("secondPass_Expension: " + newMinVal + " " + newMaxVal);
   }

   public int getLocalMax_secondExpansion(int[][] zeroFramedAry, int i, int j){
       PriorityQueue<Integer> pq = new PriorityQueue<>((a, b)->b-a);
       for(int x=i-1; x<=i+1; x++){
           for(int y=j-1; y<=j+1; y++){
               if(x==i && y==j) continue;
               pq.add(zeroFramedAry[x][y]);
           }
       }
       /*pq.add(zeroFramedAry[i-1][j-1]);
       pq.add(zeroFramedAry[i-1][j]);
       pq.add(zeroFramedAry[i-1][j+1]);
       pq.add(zeroFramedAry[i][j-1]);
       pq.add(zeroFramedAry[i][j+1]);
       pq.add(zeroFramedAry[i+1][j-1]);
       pq.add(zeroFramedAry[i+1][j]);
       pq.add(zeroFramedAry[i+1][j+1]); */
       return pq.peek();
   }

   public void decompressFile(int[][] zeroFramedAry, BufferedWriter outFile) throws IOException {
        outFile.write("\n Decompressed File \n");
        outFile.write(numRows + "  " + numCols + "  " + minVal + "  " + maxVal + "\n");
        for(int i=1; i<zeroFramedAry.length; i++){
            for(int j=1; j<zeroFramedAry[0].length; j++){
                if(zeroFramedAry[i][j] >= 1){
                    outFile.write(1 + " ");
                }else{
                    outFile.write(0 + " ");
                }
            }
            outFile.write("\n");
        }
   }
  //Pretty Print
   public void reformatPrettyPrint(int[][] inArray, BufferedWriter outFile) throws IOException {
        //System.out.println("PrettyPrint Test");
       outFile.write(numRows + " " + numCols + " " + newMinVal + " " + newMaxVal + "\n");
        for(int i=1; i<inArray.length-1; i++){
            for(int j=1; j<inArray[0].length-1; j++){
                if(inArray[i][j]==0) {
                    outFile.write("." + " ");
                }else{
                    outFile.write(inArray[i][j] + " ");
                }
            }
            outFile.write("\n");
        }
    }

    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new FileReader(args[0]));
        String infileName = args[0];
        BufferedWriter outFile1 = new BufferedWriter(new FileWriter(new File(args[1])));
        BufferedWriter outFile2 = new BufferedWriter(new FileWriter(new File(args[2])));
        int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
        numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal  = inFile.nextInt();
        maxVal  = inFile.nextInt();
        newMinVal = minVal;
        newMaxVal = maxVal;
        //Define Custom File Name
        System.out.println(numRows + " " + numCols + " " + minVal + " " + maxVal);
        String skeletonFileName = infileName.substring(0,infileName.length()-4) + "_skeleton.txt";
        String decompressFileName = infileName.substring(0,infileName.length()-4) + "_decompressed.txt";
        BufferedWriter skeletonFile = new BufferedWriter(new FileWriter(new File(skeletonFileName)));
        BufferedWriter decompressFile = new BufferedWriter(new FileWriter(new File(decompressFileName)));

        ImageProcessing imageProcess = new ImageProcessing(numRows, numCols, minVal, maxVal, newMinVal, newMaxVal);
        int[][] zeroFramedAry = imageProcess.zeroFramedAry;
        int[][] skeletonAry = imageProcess.skeletonAry;
        imageProcess.setZero(zeroFramedAry);
        imageProcess.setZero(skeletonAry);
        //Load Original Image
        imageProcess.loadImage(inFile, zeroFramedAry);
        outFile1.write("\n" + "Print of input File \n");
        imageProcess.reformatPrettyPrint(zeroFramedAry, outFile1);
        //Compute 8 Distance Transfer
        outFile1.write(infileName + "  Printing \n\n");
        imageProcess.compute8Distance(zeroFramedAry, outFile1);
        //Extract Skeleton
        imageProcess.skeletonExtraction(zeroFramedAry, skeletonAry, skeletonFile, outFile1);
        //Expand Skeleton
        imageProcess.skeletonExpansion(zeroFramedAry,  outFile2, skeletonFileName);
        //Decompress File
        imageProcess.decompressFile(zeroFramedAry, decompressFile);
        inFile.close();
        outFile1.close();
        outFile2.close();
        //skeletonFile.close();
        decompressFile.close();
    }
}
