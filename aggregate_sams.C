void aggregate_sams(){

    char *slug;
    cout<<"Your rootfile should be of the form prexPrompt_something.root"<<endl;
    cout<<"Please Enter the string in the root file between _ and .:";
    cin>>slug;

    TChain *T = new TChain("T");
    T->Add(Form("rootfiles/prexPrompt_%s.root",slug));

    ofstream outfile(Form("TextFiles/aggregate_sams_%s.csv",slug));

    TString dv_list[]=
    {"asym_sam1",
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
    fGraph->SetTitle(Form("%s.rms/ppm",plot_rms.Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    fGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
   }
  c1->SaveAs(Form("%s.rmsvsRun.pdf",plot_rms.Data()));
   }
     
      double raw[nDV];
      double rawErr[nDV];
      double reg[nDV];
      double regErr[nDV];
      double Chi_sq_raw[nDV];
      double Chi_sq_reg[nDV];
  for(int plt=0;plt<nDV;plt++){
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
      rawGraph->SetTitle(Form("%s.mean/ppb",dv_list[plt].Data()));
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
  c1->SaveAs(Form("%s.meanvsRun.pdf",dv_list[plt].Data()));
  }
  for(int plt=0;plt<nDV;plt++){
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
      regGraph->SetTitle(Form("%s.mean/ppb",plot_mean.Data()));
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
  c1->SaveAs(Form("%s.meanvsRun.pdf",plot_mean.Data()));
  }

      outfile<<"det_var"<<"\t"<<"p0(ppb)"<<"\t"<<"Chi_sq"<<"\t"<<"det_var"<<"\t"<<"p0(ppb)"<<"\t"<<"Chi_sq"<<endl;
   for(int i=0;i<nDV;i++){
      print_dv = dv_list[i];
      print_dv.ReplaceAll("asym_","reg_asym_");
      outfile<<dv_list[i]<<"\t"<<raw[i]<<"+/-"<<rawErr[i]<<"\t"<<Chi_sq_raw[i]<<"\t"<<print_dv<<"\t"<<reg[i]<<"+/-"<<regErr[i]<<"\t"<<Chi_sq_reg[i]<<endl;
   }

  for(Int_t idv=0;idv<nDV;idv++){
    for(int iiv=0;iiv<nIV;iiv++){
    dv_name = dv_list[idv];
    iv_name = iv_list[iiv];
    dv_name.ReplaceAll("asym_","");
    iv_name.ReplaceAll("diff_","");

//    npt=T->Draw(Form("%s_%s/(ppm/nm):Entry$",dv_name.Data(),iv_name.Data()),cut,"goff");
//    double* mean = new double[idv][iiv];
//    double* ent = new double[idv][iiv];
//    dummy = T->GetV1();
//   for(int plt=0;plt<npt;plt++)
//    mean[idv][iiv] = *(dummy+plt);
//   for(int plt=0;plt<npt;plt++)
//    ent[idv][iiv] = plt;
//    plot_err = dv_list[plt];
//    plot_err.ReplaceAll("asym_","reg_asym_");
//    npt=T->Draw(Form("%s.err/ppb:Entry$",plot_err.Data()),cut,"goff");
//    dummy = T->GetV1();
//    double* err = new double[npt];
//    double* x_err = new double[npt];
//   for(int plt=0;plt<npt;plt++){
//    err[plt] = *(dummy+plt);
//    x_err[plt] = 0.0;
//   }
    npt=T->Draw(Form("%s_%s/(ppm/nm):Entry$",dv_name.Data(),iv_name.Data()),cut,"goff");
    Double_t *ent = new Double_t[npt];
  for(int ipt=0;ipt<npt;ipt++)
    ent[ipt] = ipt;
    TGraph* sGraph = new TGraph(npt,ent,T->GetV1());
    sGraph->SetMarkerStyle(20);
    sGraph->Draw("AP");
    sGraph->Fit("pol0");
    sGraph->SetTitle(Form("%s_%s/(ppm/nm)",dv_name.Data(),iv_name.Data()));
    sGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    sGraph->GetXaxis()->SetTitle("Run.Mini");
  for(int plt=0;plt<npt;plt++){
    sGraph->GetXaxis()->SetBinLabel(plt+1,
				    Form("%d.%d",(int)run[plt],(int)mini[plt]));
    }
    c1->SaveAs(Form("%s_%s_vsRun.pdf",dv_name.Data(),iv_name.Data()));
   }
  }


/*    TCanvas *c2 = new TCanvas("c2","c2",1000,600);
  for(int plt=0;plt<nDV;plt++){
    plot_rms = dv_list[plt];
    plot_rms.ReplaceAll("asym_","reg_asym_");
    T->Draw(Form("%s.rms/ppm",plot_rms.Data()),cut);
    c2->SaveAs(Form("%s.rms.pdf",plot_rms.Data()));
  }
*/
    gSystem->Exec(Form("pdfunite *.pdf plots/aggregate_sams_%s.pdf",slug));
    gSystem->Exec(Form("rm -rf *.pdf"));
}
