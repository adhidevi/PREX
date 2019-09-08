void aggregate_slopes(){

    char *slug;
    cout<<"Your rootfile should be of the form prexPrompt_something.root"<<endl;
    cout<<"Please Enter the string in the root file between _ and .:";
    cin>>slug;

    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_%s.root",slug));

    ofstream outfile_slp(Form("TextFiles/aggregate_slp_%s.csv",slug));

    TString dv_list[]=
    {"asym_usl",
     "asym_usr",
     "asym_dsl",
     "asym_dsr",
     "asym_sam1",
     "asym_sam2",
     "asym_sam3",
     "asym_sam4",
     "asym_sam5",
     "asym_sam6",
     "asym_sam7",
     "asym_sam8",
     "asym_sam_15_avg",
     "asym_sam_15_dd",
     "asym_sam_26_avg",
     "asym_sam_26_dd",
     "asym_sam_37_avg",
     "asym_sam_37_dd",
     "asym_sam_48_avg",
     "asym_sam_48_dd",
     "asym_bcm_an_ds3"};

    TString iv_list[]=
    {"diff_bpm4aX",
     "diff_bpm4aY",
     "diff_bpm4eX",
     "diff_bpm4eY",
     "diff_bpm12X"};

    Int_t nIV = sizeof(iv_list)/sizeof(*iv_list);
    Int_t nDV = sizeof(dv_list)/sizeof(*dv_list);
    TString dv_name, iv_name, plot_rms, plot_mean, plot_err,print_dv;
  for(Int_t idv=0;idv<nDV;idv++){
    for(Int_t iiv=0;iiv<nIV;iiv++){
      dv_name = dv_list[idv];
      iv_name = iv_list[iiv];
      dv_name.ReplaceAll("asym_","");
      iv_name.ReplaceAll("diff_","");
      TString coeff_name = Form("slope[%d][%d]",idv,iiv);
      T->SetAlias(Form("%s_%s",dv_name.Data(),iv_name.Data()),
			 coeff_name);
      TString coeff_name_err = Form("slope_err[%d][%d]",idv,iiv);
      T->SetAlias(Form("%s_%s_err",dv_name.Data(),iv_name.Data()),
			 coeff_name_err);
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

/*  for(int plt=0;plt<nDV;plt++){
    plot_rms = dv_list[plt];
    plot_rms.ReplaceAll("asym_","reg_asym_");
    npt=T->Draw(Form("%s.rms/ppm:Entry$",plot_rms.Data()),cut,"goff");
    TGraph* fGraph = new TGraph(npt,T->GetV2(),T->GetV1());
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->Fit("pol0");
    fGraph->SetTitle(Form("%s:%s.rms/ppm",slug,plot_rms.Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
   }
//  c1->SaveAs(Form("%s.rmsvsRun_slopes.pdf",plot_rms.Data()));
   }
*/     
      double raw[nDV];
      double rawErr[nDV];
      double b4aX[nDV],b4aY[nDV],b4eX[nDV],b4eY[nDV],b12X[nDV];
      double reg[nDV];
      double regErr[nDV];
      double b4aXErr[nDV],b4aYErr[nDV],b4eXErr[nDV],b4eYErr[nDV],b12XErr[nDV];
      double Chi_sq_raw[nDV];
      double Chi_sq_reg[nDV];
      double Chi_sq_b4aX[nDV];

  for(Int_t idv=0;idv<nDV;idv++){
    dv_name = dv_list[idv];
    dv_name.ReplaceAll("asym_","");

    npt=T->Draw(Form("%s_bpm4aX/(ppb/nm):Entry$",dv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s:%s_bpm4aX/(ppb/nm)",slug,dv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
      TF1 *fitslp=sGraph->GetFunction("pol0");
      b4aX[idv] = fitslp->GetParameter(0);
      b4aXErr[idv] = fitslp->GetParError(0);
//      Chi_sq_reg[idv] = fitslp->GetChisquare()/fitslp->GetNDF();
    c1->SaveAs(Form("%s_bpm4aX_vsRun_slopes.pdf",dv_name.Data()));
   }

  for(Int_t idv=0;idv<nDV;idv++){
    dv_name = dv_list[idv];
    dv_name.ReplaceAll("asym_","");

    npt=T->Draw(Form("%s_bpm4aY/(ppb/nm):Entry$",dv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s:%s_bpm4aY/(ppb/nm)",slug,dv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
      TF1 *fitslp=sGraph->GetFunction("pol0");
      b4aY[idv] = fitslp->GetParameter(0);
      b4aYErr[idv] = fitslp->GetParError(0);
//      Chi_sq_reg[idv] = fitslp->GetChisquare()/fitslp->GetNDF();
    c1->SaveAs(Form("%s_bpm4aY_vsRun_slopes.pdf",dv_name.Data()));
   }

  for(Int_t idv=0;idv<nDV;idv++){
    dv_name = dv_list[idv];
    dv_name.ReplaceAll("asym_","");

    npt=T->Draw(Form("%s_bpm4eX/(ppb/nm):Entry$",dv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s:%s_bpm4eX/(ppb/nm)",slug,dv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
      TF1 *fitslp=sGraph->GetFunction("pol0");
      b4eX[idv] = fitslp->GetParameter(0);
      b4eXErr[idv] = fitslp->GetParError(0);
//      Chi_sq_reg[idv] = fitslp->GetChisquare()/fitslp->GetNDF();
    c1->SaveAs(Form("%s_bpm4eX_vsRun_slopes.pdf",dv_name.Data()));
   }


  for(Int_t idv=0;idv<nDV;idv++){
    dv_name = dv_list[idv];
    dv_name.ReplaceAll("asym_","");

    npt=T->Draw(Form("%s_bpm4eY/(ppb/nm):Entry$",dv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s:%s_bpm4eY/(ppb/nm)",slug,dv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
      TF1 *fitslp=sGraph->GetFunction("pol0");
      b4eY[idv] = fitslp->GetParameter(0);
      b4eYErr[idv] = fitslp->GetParError(0);
//      Chi_sq_reg[idv] = fitslp->GetChisquare()/fitslp->GetNDF();
    c1->SaveAs(Form("%s_bpm4eY_vsRun_slopes.pdf",dv_name.Data()));
   }

  for(Int_t idv=0;idv<nDV;idv++){
    dv_name = dv_list[idv];
    dv_name.ReplaceAll("asym_","");

    npt=T->Draw(Form("%s_bpm12X/(ppb/nm):Entry$",dv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s:%s_bpm12X/(ppb/nm)",slug,dv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
      TF1 *fitslp=sGraph->GetFunction("pol0");
      b12X[idv] = fitslp->GetParameter(0);
      b12XErr[idv] = fitslp->GetParError(0);
//      Chi_sq_reg[idv] = fitslp->GetChisquare()/fitslp->GetNDF();
    c1->SaveAs(Form("%s_bpm12X_vsRun_slopes.pdf",dv_name.Data()));
   }
     outfile_slp<<"device_name"<<"\t"<<"bpm4aX"<<"\t"<<"bpm4aY"<<"\t"<<"bpm4eX"<<"\t"<<"bpm4eY"<<"\t"<<"bpm12X"<<endl;
  for(int i=0;i<nDV;i++){
      outfile_slp<<dv_list[i]<<"\t"<<b4aX[i]<<"+/-"<<b4aXErr[i]<<"\t"<<b4aY[i]<<"+/-"<<b4aYErr[i]<<"\t"<<b4eX[i]<<"+/-"<<b4eXErr[i]<<"\t"<<b4eY[i]<<"+/-"<<b4eYErr[i]<<"\t"<<b12X[i]<<"+/-"<<b12XErr[i]<<endl;
  }
    gSystem->Exec(Form("pdfunite *_slopes.pdf plots/aggregate_sams_%s_slopes.pdf",slug));
    gSystem->Exec(Form("rm -rf *_slopes.pdf"));
}
