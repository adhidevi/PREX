#include <TGaxis.h>
#include <TCanvas.h>
#include <TPaveLabel.h>
#include <TPaveText.h>
#include "TGraphErrors.h"

void plotCharge(){
  //Define parameters common to all analysis configurations
  const Int_t points = ;

   gStyle->SetPalette(1);
   gStyle->SetPadGridX(kTRUE);
   gStyle->SetPadGridY(kTRUE);
   TGaxis::SetMaxDigits(4);
   gStyle->SetOptStat(kFALSE);


   ifstream infile("goodcharge_run.txt");

   Int_t Run;
   float charge;
   float my_run[points], my_charge[points];
   for(int i=0;i<points;i++) {
   infile >> Run >> charge;
   my_run[i] = Run;
   my_charge[i] = charge;
   cout<<my_run[i]<<"\t"<<my_charge[i]<< endl;
  }

   infile.close();

   TCanvas *c = new TCanvas("c","Good charge",0,0,700,500);

   TGraph *gr = new TGraph(points,my_run, my_charge);
   
   gr->SetTitle("Good Charge for slug; Run; Charge (C)");
   TExec *ex = new TExec("ex","getPoints();");
   gr->GetListOfFunctions()->Add(ex);
   gr->Draw("AP");
   gr->GetXaxis()->CenterTitle();
   gr->GetYaxis()->CenterTitle();
   gPad->Modified();
   c->SaveAs("goodChager_slug.png");

   }

void getPoints(){
   Int_t i,points,j;
   Double_t x,y;

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   points = g->GetN();
   TMarker *m;
   for (i=0; i<points; i++) {
   g->GetPoint(i,x,y);
   m = new TMarker(x,y,29);
   m->SetMarkerColor(i+1);	
   m->Paint();
   }
   }


