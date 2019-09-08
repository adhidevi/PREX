#include "TString.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TH1.h"
#include <fstream>
#include <cstdio>

void test(){
//	TTree * MyTree = new TTree("MyTree", "MyTree");
//	MyTree->ReadFile("goodcharge_run.txt","run:charge");
//	MyTree->Draw("charge:run","","*");

	const char *file_in = "goodcharge_run.txt";
	const char * file_out = "goodcharge_run.root";
	if((!file_in)||(!(file_in[0]))) return;
	if((!file_out)||(!(file_out[0]))) return;
	ifstream in;

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);

	#if 1
	in.open(file_in);
	#else
	TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentmacroName());
	dir.ReplaceAll("test.C","");
	dir.ReplaceAll("/./","/");
	in.open(Form("%s%s",dir.Data(),file_in));
	#endif
	
	int run;
	Double_t charge;
	Int_t nlines = 0;
	TFile *f = new TFile(file_out,"RECREATE");
	TNtuple *T = new TNtuple("T","data from txt file","run:charge");

	while(1){
	in>>run>>charge;
	if(in.fail()) break;
	printf("run = %d\t charge = %f\n",run,charge);
	T->Fill(run,charge);
	nlines++;
   }
	printf("found %d points\n", nlines);
	in.close();

//	Int_t xbins = 100;
//	Double_t xmin = T->GetMinimum("run");
//	Double_t xmax = T->GetMaximum("run");
//	xmax += ((xmax-xmin)/xbins)*0.001;
//	TH1F *h1 = new TH1F("h1","charge distribution",xbins,xmin,xmax);

	TCanvas *c1=new TCanvas("c1","charge",600,400);

	T->Draw("charge:run","run>=3692&&run<3712","*");
	c1->SaveAs(Form("plots/charge_slug.pdf"));
	f->Write();
}
