void aggregate_maindet_AT_2(){

    char *date_shift;
//    Int_t date = 0;
//  if(date<=0){
    cout<<"Your rootfile should be of the form prexPrompt_something.root"<<endl;
    cout<<"Please Enter the string in the root file between _ and .:";
    cin>>date_shift;
//    }
    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_%s.root",date_shift));
    TString dv_list[]=
    {"asym_usl",
     "asym_usr",
     "asym_dsl",
     "asym_dsr",
     "asym_us_avg",
     "asym_us_dd",
     "asym_ds_avg",
     "asym_ds_dd",
     "asym_left_avg",
     "asym_left_dd",
     "asym_right_avg",
     "asym_right_dd",
     "asym_bcm_an_us"};

    TString iv_list[]=
    {"diff_bpm4aX",
     "diff_bpm4aY",
     "diff_bpm4eX",
     "diff_bpm4eY",
     "diff_bpm11X"};

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

  for(int plt=0;plt<nDV;plt++){
    plot_rms = dv_list[plt];
    plot_rms.ReplaceAll("asym_","reg_asym_");
    npt=T->Draw(Form("%s.rms/ppm:Entry$",plot_rms.Data()),cut,"goff");
    TGraph* fGraph = new TGraph(npt,T->GetV2(),T->GetV1());
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
  c1->SaveAs(Form("%s.rmsvsRun.pdf",plot_rms.Data()));
   }
  for(int plt=0;plt<nDV;plt++){
      npt=T->Draw(Form("%s.mean/ppb:Entry$",dv_list[plt].Data()),cut,"goff");
      double* mean_raw = new double[npt];
      double* mymean = new double[npt];
      double* ent_raw = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++){
	mean_raw[plt] = *(dummy+plt);
//for Ca 6%
      if(run[plt]>=4126&&run[plt]<=4127){
//for Pb/Diamond
//      if(run[plt]==4128){
//for C 1%
//      if(run[plt]>=4131&&run[plt]<=4132){
	mymean[plt] = -1*mean_raw[plt];
cout<<"IHWP OUT:"<<run[plt]<<endl;
      }else{
	mymean[plt] = 1*mean_raw[plt];
cout<<"IHWP IN:"<<run[plt]<<endl;
      }
   }

   for(int plt=0;plt<npt;plt++)
      ent_raw[plt] = plt;
      npt=T->Draw(Form("%s.err/ppb:Entry$",dv_list[plt].Data()),cut,"goff");
      dummy = T->GetV1();
      double* err_raw = new double[npt];
      double* x_err_raw = new double[npt];
   for(int plt=0;plt<npt;plt++){
	err_raw[plt] = *(dummy+plt);
	x_err_raw[plt] = 0.0;
      }

      TGraphErrors* rawGraph = new TGraphErrors(npt,ent_raw,mymean,x_err_raw,err_raw);
      rawGraph->SetMarkerStyle(20);
      rawGraph->Draw("AP");
      rawGraph->Fit("pol0");
      rawGraph->SetTitle(Form("%s.mean/ppb",dv_list[plt].Data()));
      rawGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      rawGraph->GetXaxis()->SetTitle("Run.Mini");
   for(int plt=0;plt<npt;plt++){
	rawGraph->GetXaxis()->SetBinLabel(plt+1,
					Form("%d.%d",(int)run[plt],(int)mini[plt]));
      }
  c1->SaveAs(Form("%s.meanvsRun.pdf",dv_list[plt].Data()));
  }
  for(int plt=0;plt<nDV;plt++){
      plot_mean = dv_list[plt];
      plot_mean.ReplaceAll("asym_","reg_asym_");
      npt=T->Draw(Form("%s.mean/ppb:Entry$",plot_mean.Data()),cut,"goff");
      double* mean_reg = new double[npt];
      double* mean = new double[npt];
      double* ent = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++){
	mean_reg[plt] = *(dummy+plt);
//for Ca 6%
      if(run[plt]>=4126&&run[plt]<=4127){
//for Pb/Diamond
//      if(run[plt]==4128){
//for C 1%
//      if(run[plt]>=4131&&run[plt]<=4132){
	mean[plt] = -1*mean_reg[plt];
      }else{
	mean[plt] = 1*mean_reg[plt];
      }
   }
   for(int plt=0;plt<npt;plt++)
      ent[plt] = plt;
      plot_err = dv_list[plt];
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
  c1->SaveAs(Form("%s.meanvsRun.pdf",plot_mean.Data()));
  }
/*  for(int iiv=0;iiv<nIV;iiv++){
    npt=T->Draw(Form("%s.mean/nm:Entry$",iv_list[iiv].Data()),cut,"goff");
    TGraph* fGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->SetTitle(Form("%s.mean/nm",iv_list[iiv].Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
  }
    c1->SaveAs(Form("%s.meanvsRun.pdf",iv_list[iiv].Data()));
  }

  for(int iiv=0;iiv<nIV;iiv++){
    npt=T->Draw(Form("%s.rms/um:Entry$",iv_list[iiv].Data()),cut,"goff");
    TGraph* fGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->SetTitle(Form("%s.rms/um",iv_list[iiv].Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
  }
    c1->SaveAs(Form("%s.rmsvsRun.pdf",iv_list[iiv].Data()));
  }

    TCanvas *c2 = new TCanvas("c2","c2",1000,600);
  for(int plt=0;plt<nDV;plt++){
    plot_rms = dv_list[plt];
    plot_rms.ReplaceAll("asym_","reg_asym_");
    T->Draw(Form("%s.rms/ppm",plot_rms.Data()),cut);
    c2->SaveAs(Form("%s.rms.pdf",plot_rms.Data()));
  }
*/
    gSystem->Exec(Form("pdfunite *.pdf plots/aggregate_maindet_%s.pdf",date_shift));
    gSystem->Exec(Form("rm -rf *.pdf"));
}
