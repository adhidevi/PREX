void at_grand_main(){
     gStyle->SetOptFit(111);
     TChain *agg = new TChain("agg");
     agg->Add("/chafs2/work1/apar/aggRootfiles/slugRootfiles/grandRootfile/grand_60-86.root");
     TString var_list[]= {"reg_asym_atl1_mean",
                          "reg_asym_atl2_mean",
                          "reg_asym_atr1_mean",
                          "reg_asym_atr2_mean"};
     Int_t n_var = sizeof(var_list)/sizeof(*var_list);
     TString var_err, var_rms, var_rms_err;
//     TCut slug_cut = "";
     TCut slug_cut = "n_runs>=82&&n_runs<=86";
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
        if(i==1||i==3)
         mean[i] = -1*mean_reg[i];
        else
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

     for(int plt=0;plt<n_var;plt++){
         var_rms = var_list[plt];
         var_rms.ReplaceAll("_mean","_rms");
         npt=agg->Draw(Form("%s:n_runs",var_rms.Data()),slug_cut,"goff");
         double* rms_reg = new double[npt];
         double* rms = new double[npt];
         double* ent_rms = new double[npt];
         dummy = agg->GetV1();
       for(int i=0;i<npt;i++){
         rms_reg[i] = *(dummy+i);
         rms[i] = rms_reg[i];
        }
       for(int i=0;i<npt;i++)
         ent_rms[i] = i;
         var_rms_err = var_list[plt];
         var_rms_err.ReplaceAll("_mean","_rms_error");
         npt=agg->Draw(Form("%s:n_runs",var_rms_err.Data()),slug_cut,"goff");
         dummy = agg->GetV1();
         double* err_r = new double[npt];
         double* x_err_r = new double[npt];
       for(int i=0;i<npt;i++){
         err_r[i] = *(dummy+i);
         x_err_r[i] = 0.0;
         }

         TGraphErrors* rGraph = new TGraphErrors(npt,ent_rms,rms,x_err_r,err_r);
         rGraph->SetMarkerStyle(20);
         rGraph->Draw("AP");
         rGraph->Fit("pol0");
         rGraph->SetTitle(Form("%s",var_rms.Data()));
         rGraph->GetXaxis()->Set(npt,-0.5,npt-0.5);
         rGraph->GetXaxis()->SetTitle("Slug");
       for(int i=0;i<npt;i++){
         rGraph->GetXaxis()->SetBinLabel(i+1,Form("%d",(int)slug[i]));
         }
         c1->SaveAs(Form("%s.pdf",var_rms.Data()));
    }

         gSystem->Exec("pdfunite *.pdf plots/blindedAT/aggretate_AT_blinded_82-86.pdf");
}












