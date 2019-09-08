void aggregate_all(){

    char *slug;
    cout<<"Your rootfile should be of the form prexPrompt_something.root"<<endl;
    cout<<"Please Enter the string in the root file between _ and .:";
    cin>>slug;

    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_%s.root",slug));

    ofstream outfile(Form("TextFiles/aggregate_qty_%s.csv",slug));

    TString dv_list[]=
    {"asym_usl",
     "asym_usr",
     "asym_dsl",
     "asym_dsr",
     "asym_us_avg",
     "asym_us_dd",
     "asym_ds_avg",
     "asym_ds_dd",
     "asym_atr1",
     "asym_atl1",
     "asym_atr2",
     "asym_atl2",
     "asym_atl_avg",
     "asym_atl_dd",
     "asym_atr_avg",
     "asym_atr_dd",
     "asym_at1_avg",
     "asym_at1_dd",
     "asym_at2_avg",
     "asym_at2_dd",
     "asym_atl1r2_avg",
     "asym_atl1r2_dd",
     "asym_atr1l2_avg",
     "asym_atr1l2_dd",
     "asym_left_avg",
     "asym_left_dd",
     "asym_right_avg",
     "asym_right_dd",
     "asym_bcm_an_ds3",
     "asym_bcm_an_ds",
     "asym_bcm_an_us",
     "asym_bcm_dg_us",
     "asym_bcm_dg_ds",
     "bcm_an_us_ds3_dd",
     "bcm_an_us_ds_dd",
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
     "asym_sam_37_avg",
     "asym_sam_37_dd",
     "asym_sam_26_avg",
     "asym_sam_26_dd",
     "asym_sam_48_avg",
     "asym_sam_48_dd"};

TString yld_list[]=
  {"asym_usl",
     "asym_usr",
     "asym_dsl",
     "asym_dsr",
     "asym_atr1",
     "asym_atl1",
     "asym_atr2",
     "asym_atl2",
     "asym_bcm_an_ds3",
     "asym_bcm_an_ds",
     "asym_bcm_an_us",
     "asym_bcm_dg_us",
     "asym_bcm_dg_ds",
     "asym_sam1",
     "asym_sam2",
     "asym_sam3",
     "asym_sam4",
     "asym_sam5",
     "asym_sam6",
     "asym_sam7",
     "asym_sam8"};

    TString iv_list[]=
    {"diff_bpm4aX",
     "diff_bpm4aY",
     "diff_bpm4eX",
     "diff_bpm4eY",
     "diff_bpm11X"};

    Int_t nIV = sizeof(iv_list)/sizeof(*iv_list);
    Int_t nDV = sizeof(dv_list)/sizeof(*dv_list);
    Int_t yDV = sizeof(yld_list)/sizeof(*yld_list);
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

  for(int plt=0;plt<nDV;plt++){
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
  c1->SaveAs(Form("%s.rmsvsRun.pdf",plot_rms.Data()));
   }

  for(int plt=0;plt<yDV;plt++){
    plot_rms = dv_list[plt];
    plot_rms.ReplaceAll("asym_","yield_");
    npt=T->Draw(Form("%s.mean/ppm:Entry$",plot_rms.Data()),cut,"goff");
    TGraph* fGraphY = new TGraph(npt,T->GetV2(),T->GetV1());
    fGraphY->SetMarkerStyle(20);
    fGraphY->Draw("AP");
    fGraphY->Fit("pol0");
    fGraphY->SetTitle(Form("%s:%s.mean/ppm",slug,plot_rms.Data()));
    fGraphY->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraphY->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraphY->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
   }
  c1->SaveAs(Form("%s.meanvsRun.pdf",plot_rms.Data()));
   }
     
      double raw[nDV];
      double rawErr[nDV];
      double b4aX[nDV],b4aY[nDV],b4eX[nDV],b4eY[nDV],b12X[nDV];
      double reg[nDV];
      double regErr[nDV];
      double b4aXErr[nDV],b4aYErr[nDV],b4eXErr[nDV],b4eYErr[nDV],b12XErr[nDV];
      double Chi_sq_raw[nDV];
      double Chi_sq_reg[nDV];
      double Chi_sq_b4aX[nDV];

    TCanvas *c2 = new TCanvas("c2","c2",1200,600);
      c2->Clear();
      c2->Divide(2,1);
  for(int plt=0;plt<nDV;plt++){
      c2->cd(1);
      gPad->SetPad(0.,0.,0.60,1);
      npt=T->Draw(Form("%s.mean/ppb:Entry$",dv_list[plt].Data()),cut,"goff");
      double* mean_raw = new double[npt];
      double* ent_raw = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++)
	mean_raw[plt] = *(dummy+plt);

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

      TGraphErrors* rawGraph = new TGraphErrors(npt,ent_raw,mean_raw,x_err_raw,err_raw);
      rawGraph->SetMarkerStyle(20);
      rawGraph->Draw("AP");
      rawGraph->Fit("pol0");
      rawGraph->SetTitle(Form("%s:%s.mean/ppb",slug,dv_list[plt].Data()));
      rawGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      rawGraph->GetXaxis()->SetTitle("Run.Mini");
   for(int plt=0;plt<npt;plt++){
	rawGraph->GetXaxis()->SetBinLabel(plt+1,
					Form("%d.%d",(int)run[plt],(int)mini[plt]));
      }
      TF1 *fitraw=rawGraph->GetFunction("pol0");
      raw[plt] = fitraw->GetParameter(0);
      rawErr[plt] = fitraw->GetParError(0);
      Chi_sq_raw[plt] = fitraw->GetChisquare()/fitraw->GetNDF();
      c2->cd(2);
      gPad->SetPad(0.60,0,1,1);
      TH1D *hPullPlot = new TH1D(Form("hPullPlot"),"1D pull distribution",40,-8,8);
  for(int i=1;i<=npt;i++){
      double pull_val = (mean_raw[i] - fitraw->Eval(ent_raw[i])) / err_raw[i];
//cout<<pull_val<< endl;
      hPullPlot -> Fill(pull_val);
      hPullPlot->Fit("gaus","Q");
  }
  c2->SaveAs(Form("%s.meanvsRun.pdf",dv_list[plt].Data()));
  }

  for(int plt=0;plt<nDV;plt++){
      c2->cd(1);
      gPad->SetPad(0.,0.,0.60,1);
      plot_mean = dv_list[plt];
      plot_mean.ReplaceAll("asym_","reg_asym_");
      npt=T->Draw(Form("%s.mean/ppb:Entry$",plot_mean.Data()),cut,"goff");
      double* mean = new double[npt];
      double* ent = new double[npt];
      dummy = T->GetV1();
   for(int plt=0;plt<npt;plt++)
	mean[plt] = *(dummy+plt);

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

      TGraphErrors* regGraph = new TGraphErrors(npt,ent,mean,x_err,err);
      regGraph->SetMarkerStyle(20);
      regGraph->Draw("AP");
      regGraph->Fit("pol0");
      regGraph->SetTitle(Form("%s:%s.mean/ppb",slug,plot_mean.Data()));
      regGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      regGraph->GetXaxis()->SetTitle("Run.Mini");
   for(int plt=0;plt<npt;plt++){
	regGraph->GetXaxis()->SetBinLabel(plt+1,
					Form("%d.%d",(int)run[plt],(int)mini[plt]));
      }
      TF1 *fitreg=regGraph->GetFunction("pol0");
      reg[plt] = fitreg->GetParameter(0);
      regErr[plt] = fitreg->GetParError(0);
      Chi_sq_reg[plt] = fitreg->GetChisquare()/fitreg->GetNDF();

      c2->cd(2);
      gPad->SetPad(0.60,0,1,1);
      TH1D *hPullPlot = new TH1D(Form("hPullPlot"),"1D pull distribution",40,-8,8);
  for(int i=1;i<=npt;i++){
      double pull_val = (mean[i] - fitreg->Eval(ent[i])) / err[i];
//cout<<pull_val<< endl;
      hPullPlot -> Fill(pull_val);
      hPullPlot->Fit("gaus","Q");
  }
  c2->SaveAs(Form("%s.meanvsRun.pdf",plot_mean.Data()));
  }

      outfile<<"det_var"<<"\t"<<"p0(ppb)"<<"\t"<<"Chi_sq"<<"\t"<<"det_var"<<"\t"<<"p0(ppb)"<<"\t"<<"Chi_sq"<<endl;
   for(int i=0;i<nDV;i++){
      print_dv = dv_list[i];
      print_dv.ReplaceAll("asym_","reg_asym_");
      outfile<<dv_list[i]<<"\t"<<raw[i]<<"+/-"<<rawErr[i]<<"\t"<<Chi_sq_raw[i]<<"\t"<<print_dv<<"\t"<<reg[i]<<"+/-"<<regErr[i]<<"\t"<<Chi_sq_reg[i]<<endl;
   }

    gSystem->Exec(Form("pdfunite *.pdf plots/aggregate_det_sam_%s.pdf",slug));
    gSystem->Exec(Form("rm -rf *.pdf"));
}
