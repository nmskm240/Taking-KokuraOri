import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Wav2PNG_20201025 extends PApplet {

//\u53c2\u8003\u306b\u3057\u305f\u30b5\u30a4\u30c8 https://mell0w-5phere.net/jaded5phere/2017/12/26/display-waveform/

//******* \u30e6\u30fc\u30b6\u30fc\u8a2d\u5b9a ********************
String filename = null;

String fileFormat ="png";              // "png" , "bmp" , "tiff"
int barcodeHight = 1000;
int darkLevel=0;
int brigtLevel=255;
int dataStep=4;
//****************************************

PrintWriter outfile;
byte wavByte[];
int bytePos;

//wave file chunk
float fmtCode;
int ch;
int sampleRate;
int bytePerSec;
int blockSize;
int sampleByte;
int sampleNum;
float waveData[][];

//draw barcode
float maxWaveData=-1;
float minWaveData=1;
int waveDataNum;
float waveLchNorData[];


public void setup() {
  for(String s : args)
  {
    String[] str = s.split("=");
    if(str[0].trim().equals("filename"))
    {
      filename = str[1];
    }
  }
  
  wavByte = loadBytes(filename+".wav");
  outfile = createWriter(filename+".csv");      //\u30d5\u30a1\u30a4\u30eb\u540d
  Read();

  waveLchNorData = new float [sampleNum];
  outfile.println("filename,"+filename+".wave");
  outfile.println("fmtCode,"+fmtCode);
  outfile.println("ch,"+ch);
  outfile.println("sampleRate,"+sampleRate);
  outfile.println("bytePerSec,"+bytePerSec);
  outfile.println("blockSize,"+blockSize);
  outfile.println("sampleByte,"+sampleByte);
  outfile.println("sampleNum,"+sampleNum);
 // outfile.println("waveDataNum,"+waveDataNum);
  outfile.println("max,"+ maxWaveData);
  outfile.println("min,"+ minWaveData);

  for (int i=0; i<sampleNum; i++) {
    waveLchNorData[i]=map(waveData[0][i], minWaveData, maxWaveData, 0, 1);
    outfile.println(waveLchNorData[i]);
  }
  outfile.flush(); //\u6b8b\u308a\u3092\u51fa\u529b\u3059\u308b
  outfile.close(); // \u30d5\u30a1\u30a4\u30eb\u3092\u9589\u3058\u308b
  println(String.format("fmtCode:%f/ch:%d/sampleRate:%d,bytePerSec:%d,blockSize:%d,sampleByte:%d,sampleNum:%d", fmtCode, ch, sampleRate, bytePerSec, blockSize, sampleByte, sampleNum));


  //\u63cf\u753b\uff08\u5b9f\u969b\u306b\u306f\u66f8\u304b\u306a\u3044\u304c\uff09
  size(sampleNum/dataStep, barcodeHight);  
  noSmooth();
  int c1 = color(brigtLevel, brigtLevel, brigtLevel);
  int c2 = color(darkLevel, darkLevel, darkLevel );
  println("now drawing");
  for (int i=0; i<sampleNum; i+=dataStep) {
    int c = lerpColor(c2, c1, waveLchNorData[i]);
    stroke(c);  
    line(i/dataStep, height, i/dataStep, 0);
    //println(i);
  }
  // \u4fdd\u5b58
  String path = String.format("%s.%s", filename, fileFormat);
  save(path);
  println("screen saved." + path);

  size(320, 320);
}

public void draw() {
  background(0);
  fill(255);
  textSize(16);
  textAlign(LEFT, TOP);
  /*
  text(String.format("%7.3f sec. %9d/%d samples",
   (float)timer/1000,timePos,sampleNum),5,5);
   textAlign(RIGHT,TOP);
   text(String.format("%dbit, %dHz  %.1f FPS",
   sampleByte*8,sampleRate,frameRate),width-5,5); //...(10)
   */


  text("filename:", 10, 0);
  text("fmtCode:", 10, 20);
  text("ch:", 10, 40);
  text("sampleRate:", 10, 60);
  text("bytePerSec:", 10, 80);  
  text("bytePerSec:", 10, 80);
  text("blockSize:", 10, 100);
  text("sampleByte:", 10, 120);
  text("sampleNum:", 10, 140);

  text(filename+".wave", 150, 0);
  text(fmtCode, 150, 20);
  text(ch, 150, 40);
  text(sampleRate, 150, 60);
  text(bytePerSec, 150, 80);  
  text(bytePerSec, 150, 80);
  text(blockSize, 150, 100);
  text(sampleByte, 150, 120);
  text(sampleNum, 150, 140);

  
  text("Barcode file",10,230);
  text(String.format("%s_%d_%d_%d_%d.%s SAVED", filename, barcodeHight, darkLevel, brigtLevel, dataStep, fileFormat),10,250);
}

public int ReadLittleEnd(int bytec, boolean signed) {
  byte[] subByte=new byte[bytec];
  System.arraycopy(wavByte, bytePos, subByte, 0, bytec);  //System.arraycopy(\u30b3\u30d4\u30fc\u5143\u914d\u5217, \u30b3\u30d4\u30fc\u5143\u914d\u5217\u306e\u30b3\u30d4\u30fc\u958b\u59cb\u4f4d\u7f6e, \u30b3\u30d4\u30fc\u5148\u914d\u5217, \u30b3\u30d4\u30fc\u5148\u914d\u5217\u306e\u958b\u59cb\u4f4d\u7f6e, \u30b3\u30d4\u30fc\u306e\u500b\u6570)
  bytePos+=bytec;
  int v=0;

  for (int i=0; i<bytec; i++) {
    int s=(subByte[i]&0xff)<<(i*8); //...(1)
    v=v|s;
  }
  if (signed)return BitsToInt(v, bytec);
  else return v;
}
public int BitsToInt(int n, int bytec) {
  if (bytec>4)return n;
  int mask=1<<(8*bytec-1);

  if ((n&mask)==mask) {
    n=-((~n&(mask-1))+1); //...(2)
  }
  return n;
}

public float ReadLittleEnd() {
  byte[] subByte=new byte[4];
  System.arraycopy(wavByte, bytePos, subByte, 0, 4);
  bytePos+=4;
  int vi=0;

  for (int i=0; i<4; i++) {
    int s=(subByte[i]&0xff)<<(i*8);
    vi=vi|s;
  }
  return Float.intBitsToFloat(vi);
}

public String ReadId() {
  String id="";

  for (int i=0; i<4; i++) {
    char c=(char)wavByte[bytePos++];
    id+=c;
  }
  return id;
}

public void Read() {
  if (!ReadId().equals("RIFF")) {
    return;
  }
  int size=ReadLittleEnd(4, false);
  println(String.format("RIFF Size:%d", size));

  if (!ReadId().equals("WAVE")) {
    return;
  }

  while (bytePos<size-8) {
    String id=ReadId();
    int chunkSize=ReadLittleEnd(4, false); //...(3)
    println(String.format("id:%s/size:%d ", id, chunkSize));
    if (id.equals("fmt ")) {
      if (chunkSize!=16) {
        return;
      }
      fmtCode=ReadLittleEnd(2, false);
      ch=ReadLittleEnd(2, false);
      sampleRate=ReadLittleEnd(4, false);
      bytePerSec=ReadLittleEnd(4, false);
      blockSize=ReadLittleEnd(2, false);
      sampleByte=ReadLittleEnd(2, false)/8;
    } else if (id.equals("data")) {
      sampleNum=chunkSize/(ch*sampleByte);
      waveData=new float[ch][sampleNum];
      if (fmtCode==1) {
        int maxLv=(1<<sampleByte*8-1)-1;
        for (int i=0; i<sampleNum; i++) {
          for (int c=0; c<ch; c++) {
            int v=ReadLittleEnd(sampleByte, true);
            waveData[c][i]=(float)v/maxLv; //...(5)
            if (c==0) {  
              if (waveData[c][i]>maxWaveData)  maxWaveData=waveData[c][i];
              if (waveData[c][i]<minWaveData)  minWaveData=waveData[c][i];
            }
          }
        }
      } else if (fmtCode==3) {
        for (int i=0; i<sampleNum; i++) {
          for (int c=0; c<ch; c++) {
            waveData[c][i]=ReadLittleEnd(); //...(6)
            if (c==0) {  
              if (waveData[c][i]>maxWaveData)  maxWaveData=waveData[c][i];
              if (waveData[c][i]<minWaveData)  minWaveData=waveData[c][i];
            }
          }
        }
      }
    } else {
      bytePos+=chunkSize; //...(4)
    }
  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Wav2PNG_20201025" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
