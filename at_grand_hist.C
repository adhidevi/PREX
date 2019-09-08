void at_grand(){
     gStyle->SetOptFit(111);
     TChain *agg = new TChain("agg");
     agg->Add("/chafs2/work1/apar/aggRootfiles/slugRootfiles/grandRootfile/grand_60-83.root");
     TString var_list[]= {"reg_asym_atl1_mean",
                          "reg_asym_atl2_mean",
                          "reg_asym_atr1_mean",
                          "reg_asym_atr2_mean"};
     Int_t n_var = sizeof(var_list)/sizeof(*var_list);
     TString var_err;
     TCut slug_cut = "n_runs>=76&&n_runs<=81";
     TCut ihwp_in = "ihwp==1";//ihwp IN
     TCut ihwp_out = "ihwp==2";//ihwp OUT
     TCanvas *c1 = new TCanvas("c1","c1",1000,600);
     c1->SetGridx();
     c1->SetGridy();
     Int_t npt=agg->Draw("n_runs:Entry$",slug_cut,"goff");
     double* dummy = agg->GetV1();
     double* slug = new double[npt];
     for(int i=0;i<npt;i++)
         slug[i] = *(dummy+i);
     for(int plt=0;plt<n_var;plt++){
         npt=agg->Draw(Form("%s:n_runs",var_list[plt].Data()),slug_cut,"goff");
         double* mean_reg = new double[npt];
         double* mean = new double[npt];
         double* ent = new double[npt];
         dummy = agg->GetV1();
       for(int i=0;i<npt;i++){
         mean_reg[i] = *(dummy+i);
         mean[i] = mean_reg[i];
        }
       for(int i=0;i<npt;i++)
         ent[i] = i;
         var_err = var_list[plt];
         var_err.ReplaceAll("_mean","_mean_error");
         npt=agg->Draw(Form("%s:n_runs",var_err.Data()),slug_cut,"goff");
         dummy = agg->GetV1();
         double* err = new double[npt];
         double* x_err = new double[npt];
       for(int i=0;i<npt;i++){
         err[i] = *(dummy+i);
         x_err[i] = 0.0;
         }

         TGraphErrors* aGraph = new TGraphErrors(npt,ent,mean,x_err,err);
         aGraph->SetMarkerStyle(20);
         aGraph->Draw("AP");
         aGraph->Fit("pol0");
         aGraph->SetTitle(Form("%s",var_list[plt].Data()));
         aGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
         aGraph->GetXaxis()->SetTitle("Slug");
       for(int i=0;i<npt;i++){
         aGraph->GetXaxis()->SetBinLabel(i+1,Form("%d",(int)slug[i]));
         }
         c1->SaveAs(Form("%s.pdf",var_list[plt].Data()));
    }
         gSystem->Exec("pdfunite *.pdf plots/blindedAT/aggretate_AT_blinded_60-80.pdf");
}












