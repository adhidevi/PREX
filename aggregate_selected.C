void aggregate_selected(){

    TChain *agg = new TChain("agg");
    agg->Add(Form("/chafs2/work1/apar/aggRootfiles/slugRootfiles/grandRootfile/archive/grand_aggregator_1-30.root"));

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(111);
    TString iv_list_mean[]=
    {"diff_bpm4aX_mean",
     "diff_bpm4aY_mean",
     "diff_bpm4eX_mean",
     "diff_bpm4eY_mean",
     "diff_bpm11X_mean"};

    TString iv_list_err[]=
    {"diff_bpm4aX_mean_error",
     "diff_bpm4aY_mean_error",
     "diff_bpm4eX_mean_error",
     "diff_bpm4eY_mean_error",
     "diff_bpm11X_mean_error"};


    TString iv_list_rms[]=
    {"diff_bpm4aX_rms",
     "diff_bpm4aY_rms",
     "diff_bpm4eX_rms",
     "diff_bpm4eY_rms",
     "diff_bpm11X_rms"};


    TString iv_list_rms_err[]=
    {"diff_bpm4aX_rms_error",
     "diff_bpm4aY_rms_error",
     "diff_bpm4eX_rms_error",
     "diff_bpm4eY_rms_error",
     "diff_bpm11X_rms_error"};

    Int_t nIV = sizeof(iv_list_err)/sizeof(*iv_list_err);
    TCut cut_in_right;
    TCut cut_in_left;
    TCut cut_out_right;
    TCut cut_out_left;
  for(int i=0;i<nIV;i++){
      cut_in_right = Form("%s!=0&&ihwp==1&&wein==1",iv_list_mean[i].Data());
      cut_in_left = Form("%s!=0&&ihwp==1&&wein==2",iv_list_mean[i].Data());
      cut_out_right = Form("%s!=0&&ihwp==2&&wein==1",iv_list_mean[i].Data());
      cut_out_left = Form("%s!=0&&ihwp==2&&wein==2",iv_list_mean[i].Data());
   }
//    TCut cut = "";	
    TCanvas *c2 = new TCanvas("c2","c2",1200,600);

    Int_t npt = agg->Draw("n_runs:Entry$",cut_in_right,"goff");
    double* dummy = agg->GetV1();
    double* slug =new double[npt];
  for(int plt=0;plt<npt;plt++)
    slug[plt] = *(dummy+plt);

  for(int plt=0;plt<nIV;plt++){
      npt=agg->Draw(Form("%s:Entry$",iv_list_mean[plt].Data()),cut_in_right,"goff");
      double* mean_raw = new double[npt];
      double* ent_raw = new double[npt];
      dummy = agg->GetV1();

   for(int i=0;i<npt;i++)
	mean_raw[i] = *(dummy+i);

   for(int i=0;i<npt;i++)
      ent_raw[i] = i;

      npt=agg->Draw(Form("%s:Entry$",iv_list_err[plt].Data()),cut_in_right,"goff");
      dummy = agg->GetV1();
      double* err_raw = new double[npt];
      double* x_err_raw = new double[npt];

   for(int i=0;i<npt;i++){
	err_raw[i] = *(dummy+i);
	x_err_raw[i] = 0.0;
      }

      TGraphErrors* rawGraph = new TGraphErrors(npt,ent_raw,mean_raw,x_err_raw,err_raw);
      rawGraph->SetMarkerStyle(20);
      rawGraph->Draw("AP");
    
      rawGraph->Fit("pol0","Q");
      rawGraph->SetTitle(Form("grand_1-30:%s",iv_list_mean[plt].Data()));
      rawGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
      rawGraph->GetXaxis()->SetTitle("Slug");

  for(int i=0;i<npt;i++){
    rawGraph->GetXaxis()->SetBinLabel(i+1, Form("%d",(int)slug[i]));
   }
//      TF1 *fitraw=rawGraph->GetFunction("pol0");
      c2->SaveAs(Form("%s.pdf",iv_list_mean[plt].Data()));
}
    TCanvas *c1 = new TCanvas("c1","c1",1200,600);
  for(int plt=0;plt<nIV;plt++){
    npt=agg->Draw(Form("%s:Entry$",iv_list_rms[plt].Data()),cut_in_right,"goff");
    TGraph* fGraph = new TGraph(npt,agg->GetV2(),agg->GetV1());
    fGraph->SetMarkerStyle(20);
    fGraph->Draw("AP");
    fGraph->Fit("pol0");
    fGraph->SetTitle(Form("grand_1-30:%s",iv_list_rms[plt].Data()));
    fGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
    fGraph->GetXaxis()->SetTitle("Slugs");
  for(int i=0;i<npt;i++){
    fGraph->GetXaxis()->SetBinLabel(i+1, Form("%d",(int)slug[i]));
   }
  c1->SaveAs(Form("%s.pdf",iv_list_rms[plt].Data()));
   }
    gSystem->Exec(Form("pdfunite *.pdf plots/grand_1-30.pdf"));
    gSystem->Exec(Form("rm -rf *.pdf"));
}
