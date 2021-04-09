//参考にしたサイト https://mell0w-5phere.net/jaded5phere/2017/12/26/display-waveform/

//******* ユーザー設定 ********************
String filename = "Iloveyou";

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


void setup() {
  for(String s : args)
  {
    String[] str = s.split("=");
    if(str[0].trim().equals("filename"))
    {
      filename = str[1];
    }
    else
    {
      return;
    }
  }
  
  wavByte = loadBytes(filename+".wav");
  outfile = createWriter(filename+".csv");      //ファイル名
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
  outfile.flush(); //残りを出力する
  outfile.close(); // ファイルを閉じる
  println(String.format("fmtCode:%f/ch:%d/sampleRate:%d,bytePerSec:%d,blockSize:%d,sampleByte:%d,sampleNum:%d", fmtCode, ch, sampleRate, bytePerSec, blockSize, sampleByte, sampleNum));


  //描画（実際には書かないが）
  size(sampleNum/dataStep, barcodeHight);  
  noSmooth();
  color c1 = color(brigtLevel, brigtLevel, brigtLevel);
  color c2 = color(darkLevel, darkLevel, darkLevel );
  println("now drawing");
  for (int i=0; i<sampleNum; i+=dataStep) {
    color c = lerpColor(c2, c1, waveLchNorData[i]);
    stroke(c);  
    line(i/dataStep, height, i/dataStep, 0);
    //println(i);
  }
  // 保存
  String path = String.format("%s_%d_%d_%d_%d.%s", filename, barcodeHight, darkLevel, brigtLevel, dataStep, fileFormat);
  save(path);
  println("screen saved." + path);

  size(320, 320);
}

void draw() {
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

int ReadLittleEnd(int bytec, boolean signed) {
  byte[] subByte=new byte[bytec];
  System.arraycopy(wavByte, bytePos, subByte, 0, bytec);  //System.arraycopy(コピー元配列, コピー元配列のコピー開始位置, コピー先配列, コピー先配列の開始位置, コピーの個数)
  bytePos+=bytec;
  int v=0;

  for (int i=0; i<bytec; i++) {
    int s=(subByte[i]&0xff)<<(i*8); //...(1)
    v=v|s;
  }
  if (signed)return BitsToInt(v, bytec);
  else return v;
}
int BitsToInt(int n, int bytec) {
  if (bytec>4)return n;
  int mask=1<<(8*bytec-1);

  if ((n&mask)==mask) {
    n=-((~n&(mask-1))+1); //...(2)
  }
  return n;
}

float ReadLittleEnd() {
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

String ReadId() {
  String id="";

  for (int i=0; i<4; i++) {
    char c=(char)wavByte[bytePos++];
    id+=c;
  }
  return id;
}

void Read() {
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
