#include <fstream>

void getAsym(){
int runNo;
cout<<"Please Enter runNo:";
cin>>runNo;
ifstream ifile(Form("/chafs2/work1/apar/postpan-outputs/prexPrompt_%d_000_postpan_summary.txt",runNo));
ofstream ofile("outfile.ods");
if(ifile.is_open()){
   string line;
   int i = 0;
   ofile<< "Detector" << "\t" << "Raw Mean(ppm)" << "\t" << "Err(ppm)" << "\t" << "Reg Mean(ppm)" << "\t" << "Err(ppm)" << endl;
   while(getline(ifile, line)){
      i++;
      if((i>=1&&i<=2)||(i>=11&&i<=14)||(i>=29&&i<=32)){
//         ofile<<line.c_str()<<endl;
//         cout<<line.c_str()<<endl;
//      }

      stringstream ss(line);
      TString det, Raw, Reg;
      float RMSraw, RMSreg, Corr, Coherent;
     if(ss >> det >> Raw >> RMSraw >> Reg >> RMSreg >> Corr >> Coherent){
        Raw.ReplaceAll("+/-","\t");
        Reg.ReplaceAll("+/-","\t");
        cout<< det << "\t" << Raw << "\t" << Reg << endl;
        ofile<< det << "\t" << Raw << "\t" << Reg << endl;
        }  
      }
    }
   ifile.close();
   ofile.close();
  }
}
