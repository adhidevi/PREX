#include "device_list.h"

void aggregate_maindet_AT_color(){

    char *date_shift;

    cout<<"Your rootfile should be of the form prexPrompt_something.root"<<endl;
    cout<<"Please Enter the string in the root file between _ and .:";
    cin>>date_shift;

    Int_t out_list[]={4108,4110,4111,4112,4113,4114,4126,4127,4128,4131,4132};
    Int_t nOUT = sizeof(out_list)/sizeof(*out_list);
    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_%s.root",date_shift));

    Int_t nIV = sizeof(iv_list)/sizeof(*iv_list);
    Int_t nDV = sizeof(dv_list)/sizeof(*dv_list);
    TString dv_name, iv_name, plot_rms, plot_mean, plot_err;
  for(Int_t idv=0;idv<nDV;idv++){
    for(Int_t iiv=0;iiv<nIV;iiv++){
      dv_name = dv_list[idv];
      iv_name = iv_list[iiv];
      dv_name.ReplaceAll("asym_","");
      iv_name.ReplaceAll("diff_","");
      TString coeff_name = Form("slope[%d][%d]",idv,iiv);
      T->SetAlias(Form("%s_%s",dv_name.Data(),iv_name.Data()),
			 coeff_name);
    }
  }

    TCut cut = "";

    TCanvas *c1 = new TCanvas("c1","c1",1000,600);
    c1->SetGridx();
    c1->SetGridy();
    Int_t npt = T->Draw("run:Entry$",cut,"goff");
    double* dummy = T->GetV1();
    double* run =new double[npt];
  for(int plt=0;plt<npt;plt++)
    run[plt] = *(dummy+plt);

    T->Draw("mini:Entry$",cut,"goff");
    double* mini = new double[npt];
    dummy = T->GetV1();
  for(int plt=0;plt<npt;plt++)
    mini[plt] = *(dummy+plt);

  for(int idv=0;idv<nDV-1;idv++){
    plot_rms = dv_list[idv];
    plot_rms.ReplaceAll("asym_","reg_asym_");
    npt=T->Draw(Form("%s.rms/ppm:Entry$",plot_rms.Data()),cut,"goff");
    TGraph* fGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    TExec *ex0 = new TExec("ex0","DrawCol();");
    fGraph->GetListOfFunctions()->Add(ex0);
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->Fit("pol0");
    fGraph->SetTitle(Form("%s.rms/ppm",plot_rms.Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
   }
  c1->SaveAs(Form("%s.rmsvsRun_color.pdf",plot_rms.Data()));
   }
  for(int idv=0;idv<nDV;idv++){
      npt=T->Draw(Form("%s.mean/ppb:Entry$",dv_list[idv].Data()),cut,"goff");
      double* mean_raw = new double[npt];
      double* mymean = new double[npt];
      double* ent_raw = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++){
	mean_raw[plt] = *(dummy+plt);
//Correcting sign for IHWP OUT
//for Ca 6%
//      if(run[plt]>=4126&&run[plt]<=4127){
//for Pb/Diamond
//      if((run[plt]>=4110&&run[plt]<=4114)||run[plt]==4128){
//for C 1%
//      if(run[plt]==4108||(run[plt]>=4131&&run[plt]<=4132)){
      if(run[plt]==4108||(run[plt]>=4110&&run[plt]<=4114)||(run[plt]>=4126&&run[plt]<=4128)||(run[plt]>=4131&&run[plt]<=4132)){
	mymean[plt] = -1*mean_raw[plt];
      }else{
	mymean[plt] = 1*mean_raw[plt];
      }
   }
   for(int plt=0;plt<npt;plt++)
      ent_raw[plt] = plt;
      npt=T->Draw(Form("%s.err/ppb:Entry$",dv_list[idv].Data()),cut,"goff");
      dummy = T->GetV1();
      double* err_raw = new double[npt];
      double* x_err_raw = new double[npt];
   for(int plt=0;plt<npt;plt++){
	err_raw[plt] = *(dummy+plt);
	x_err_raw[plt] = 0.0;
      }

      TGraphErrors* rawGraph = new TGraphErrors(npt,ent_raw,mymean,x_err_raw,err_raw);
      rawGraph->SetMarkerStyle(20);
      TExec *ex1 = new TExec("ex1","DrawCol();");
      rawGraph->GetListOfFunctions()->Add(ex1);
      rawGraph->Draw("AP");
      rawGraph->Fit("pol0");
      rawGraph->SetTitle(Form("%s.mean/ppb",dv_list[idv].Data()));
      rawGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      rawGraph->GetXaxis()->SetTitle("Run.Mini");
   for(int plt=0;plt<npt;plt++){
	rawGraph->GetXaxis()->SetBinLabel(plt+1,
					Form("%d.%d",(int)run[plt],(int)mini[plt]));
      }
  c1->SaveAs(Form("%s.meanvsRun_color.pdf",dv_list[idv].Data()));
  }

  for(int idv=0;idv<nDV-1;idv++){
      plot_mean = dv_list[idv];
      plot_mean.ReplaceAll("asym_","reg_asym_");
      npt=T->Draw(Form("%s.mean/ppb:Entry$",plot_mean.Data()),cut,"goff");
      double* mean_reg = new double[npt];
      double* mean = new double[npt];
      double* ent = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++){
	mean_reg[plt] = *(dummy+plt);
//for Ca 6%
//      if(run[plt]>=4126&&run[plt]<=4127){
//for Pb/Diamond
//      if((run[plt]>=4110&&run[plt]<=4114)||run[plt]==4128){
//for C 1%
//      if(run[plt]==4108||(run[plt]>=4131&&run[plt]<=4132)){
//Correcting sign for IHWP OUT
      if(run[plt]==4108||(run[plt]>=4110&&run[plt]<=4114)||(run[plt]>=4126&&run[plt]<=4128)||(run[plt]>=4131&&run[plt]<=4132)){
	mean[plt] = -1*mean_reg[plt];
      }else{
	mean[plt] = 1*mean_reg[plt];
      }
   }
   for(int plt=0;plt<npt;plt++)
      ent[plt] = plt;
      plot_err = dv_list[idv];
      plot_err.ReplaceAll("asym_","reg_asym_");
      npt=T->Draw(Form("%s.err/ppb:Entry$",plot_err.Data()),cut,"goff");
      dummy = T->GetV1();
      double* err = new double[npt];
      double* x_err = new double[npt];
   for(int plt=0;plt<npt;plt++){
	err[plt] = *(dummy+plt);
	x_err[plt] = 0.0;
      }

      TGraphErrors* eGraph = new TGraphErrors(npt,ent,mean,x_err,err);
      TExec *ex2 = new TExec("ex2","DrawCol();");
      eGraph->GetListOfFunctions()->Add(ex2);
      eGraph->SetMarkerStyle(20);
      eGraph->Draw("AP");
      eGraph->Fit("pol0");
      eGraph->SetTitle(Form("%s.mean/ppb",plot_mean.Data()));
      eGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      eGraph->GetXaxis()->SetTitle("Run.Mini");
   for(int plt=0;plt<npt;plt++){
	eGraph->GetXaxis()->SetBinLabel(plt+1,
					Form("%d.%d",(int)run[plt],(int)mini[plt]));
      }
  c1->SaveAs(Form("%s.meanvsRun_color.pdf",plot_mean.Data()));
  }
  for(int iiv=0;iiv<nIV;iiv++){
      npt=T->Draw(Form("%s.mean/nm:Entry$",iv_list[iiv].Data()),cut,"goff");
      double* mean_diff = new double[npt];
      double* mean_bpm = new double[npt];
      double* ent_bpm = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++){
	mean_diff[plt] = *(dummy+plt);
//for Ca 6%
//      if(run[plt]>=4126&&run[plt]<=4127){
//for Pb/Diamond
//      if((run[plt]>=4110&&run[plt]<=4114)||run[plt]==4128){
//for C 1%
//      if(run[plt]==4108||(run[plt]>=4131&&run[plt]<=4132)){
//Correcting sign for IHWP OUT
      if(run[plt]==4108||(run[plt]>=4110&&run[plt]<=4114)||(run[plt]>=4126&&run[plt]<=4128)||(run[plt]>=4131&&run[plt]<=4132)){
	mean_bpm[plt] = -1*mean_diff[plt];
      }else{
	mean_bpm[plt] = 1*mean_diff[plt];
      }
   }
   for(int plt=0;plt<npt;plt++)
      ent_bpm[plt] = plt;
      npt=T->Draw(Form("%s.err/nm:Entry$",iv_list[iiv].Data()),cut,"goff");
      dummy = T->GetV1();
      double* err_bpm = new double[npt];
      double* x_err_bpm = new double[npt];
   for(int plt=0;plt<npt;plt++){
	err_bpm[plt] = *(dummy+plt);
	x_err_bpm[plt] = 0.0;
      }

    TGraphErrors* fGraph = new TGraphErrors(npt,ent_bpm,mean_bpm,x_err_bpm,err_bpm);
    TExec *ex3 = new TExec("ex3","DrawCol();");
    fGraph->GetListOfFunctions()->Add(ex3);
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->Fit("pol0");
    fGraph->SetTitle(Form("%s.mean/nm",iv_list[iiv].Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
  }
    c1->SaveAs(Form("%s.meanvsRun_color.pdf",iv_list[iiv].Data()));
  }

  for(int iiv=0;iiv<nIV;iiv++){
    npt=T->Draw(Form("%s.rms/um:Entry$",iv_list[iiv].Data()),cut,"goff");
    TGraph* feGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    TExec *ex4 = new TExec("ex4","DrawCol();");
    feGraph->GetListOfFunctions()->Add(ex4);
    feGraph->SetMarkerStyle(20);
    feGraph->Draw("AP");
    feGraph->Fit("pol0");
    feGraph->SetTitle(Form("%s.rms/um",iv_list[iiv].Data()));
    feGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    feGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    feGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
  }
    c1->SaveAs(Form("%s.rmsvsRun_color.pdf",iv_list[iiv].Data()));
  }

  for(Int_t idv=0;idv<nDV;idv++){
    for(Int_t iiv=0;iiv<nIV;iiv++){
      dv_name = dv_list[idv];
      iv_name = iv_list[iiv];
      dv_name.ReplaceAll("asym_","");
      iv_name.ReplaceAll("diff_","");

    npt=T->Draw(Form("%s_%s/(ppb/nm):Entry$",dv_name.Data(),iv_name.Data()),cut,"goff");
    TGraph* sGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    TExec *ex5 = new TExec("ex5","DrawCol();");
    sGraph->GetListOfFunctions()->Add(ex5);
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s_%s/(ppb/nm)",dv_name.Data(),iv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
  }
    c1->SaveAs(Form("%s_%svsRun_color.pdf",dv_name.Data(),iv_name.Data()));
  }
  }

    gSystem->Exec(Form("pdfunite *.pdf plots/atALLCOMBO/aggregate_maindet_%s_color.pdf",date_shift));
    gSystem->Exec(Form("rm -rf *.pdf"));
}

void DrawCol(){
   Int_t i,n;
   Double_t x,y;
   TLatex *l;

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   TMarker *m;
   for(i=0; i<n; i++){
      g->GetPoint(i,x,y);
//Ca 6%
      if(i>=13&&i<=18){
//C 1%
//    if(i>=6&&i<=16){
//Pb/Diamond
//      if((i>=0&&i<=22)||(i>=44&&i<=49)){
      m = new TMarker(x,y,20);
      m->SetMarkerColor(2);
    }else{
      m = new TMarker(x,y,21);
      m->SetMarkerColor(3);
    }
      m->Paint();
   }
}
