
// Macro for DET plots
//Auther: Devi Adhikari
//To run this macro just type root-l DET_all.C in your terminal.
//Then it will ask you to enter run number(only number).
//Type in run number you want to see and hit enter.
//This macro stands on its own.
//In order to configure to panguin some modifications are needed.
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <fstream>
 
void at_vertical(){
    gStyle->SetTitleYOffset(1.5);
    gStyle->SetOptStat(0);
//    gStyle->SetOptStat(11);
    TGaxis::SetMaxDigits(3);
//    gStyle->SetStatX(0.72);
//    gStyle->SetStatY(0.35);
//    gStyle->SetStatW(0.42);
//    gStyle->SetStatH(0.15);
    int run1=0;
    char buf[300];
    const int num=12;
    TH2D *h_asym[num];

    TString asym[12]={"asym_usl",
     "reg_asym_usr",
     "reg_asym_dsl",
     "reg_asym_dsr",
     "reg_asym_us_avg",
     "reg_asym_us_dd",
     "reg_asym_ds_avg",
     "reg_asym_ds_dd",
     "reg_asym_left_avg",
     "reg_asym_left_dd",
     "reg_asym_right_avg",
     "reg_asym_right_dd"};
    TString yield[5]={"diff_bpm4aX",
     "diff_bpm4aY",
     "diff_bpm4eX",
     "diff_bpm4eY",
     "diff_bpm11X"};

    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_slugATCa.root"));


for(int i=0;i<num;i++){
    T->Draw(Form("%s.mean/ppb:Entry$>>hdu%d",asym[i].Data(),i),"","*");
    h_asym[i] =(TH2D*)gDirectory->FindObject(Form("hdu%d",i));
    h_asym[i]->SetDirectory(gROOT);
  }

    TCanvas *c1 = new TCanvas("c1","",1000,600);
for(int i=0;i<num;i++){
    TString title=Form("%s.mean/ppb",asym[i].Data());
    h_asym[i]->SetTitle(title);
    h_asym[i]->Fit("pol0");
    c1->SaveAs(Form("vertical_AT_slugATCa%s.pdf",asym[i].Data()));
}
    gSystem->Exec(Form("pdfunite *.pdf plots/vertical_AT_slugATCa.pdf"));
    gSystem->Exec(Form("rm -rf *.pdf"));
  }


