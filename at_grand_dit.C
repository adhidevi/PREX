void at_grand_dit(){
     gStyle->SetOptFit(111);
     TChain *agg = new TChain("agg");
     agg->Add("/chafs2/work1/apar/aggRootfiles/slugRootfiles/grandRootfile/summary/rootfiles/dither_grand_aggregator_4-74.root");
     TString var_list[]= {"dit_asym_atl1_mean",
                          "dit_asym_atl2_mean",
                          "dit_asym_atr1_mean",
                          "dit_asym_atr2_mean"};
     Int_t n_var = sizeof(var_list)/sizeof(*var_list);
     TString var_err, var_rms, var_rms_err;
     TCut slug_cut = "n_runs>=26&&n_runs<=54&&n_runs!=38";
     TCut hrs_cut = "hrs==0";
     TCanvas *c1 = new TCanvas("c1","c1",1000,600);
     c1->SetGridx();
     c1->SetGridy();
     Int_t npt=agg->Draw("n_runs:Entry$",slug_cut+hrs_cut,"goff");
     double* dummy = agg->GetV1();
     double* slug = new double[npt];
     for(int i=0;i<npt;i++)
         slug[i] = *(dummy+i);
     npt=agg->Draw("ihwp:Entry$",slug_cut+hrs_cut,"goff");
     dummy = agg->GetV1();
     double* ihwp_state = new double[npt];
     for(int i=0;i<npt;i++)
         ihwp_state[i] = *(dummy+i);
     npt=agg->Draw("wein:Entry$",slug_cut+hrs_cut,"goff");
     dummy = agg->GetV1();
     double* wein_state = new double[npt];
     for(int i=0;i<npt;i++)
         wein_state[i] = *(dummy+i);

     for(int plt=0;plt<n_var;plt++){
         npt=agg->Draw(Form("%s:n_runs",var_list[plt].Data()),slug_cut+hrs_cut,"goff");
         double* mean_reg = new double[npt];
         double* mean = new double[npt];
         double* ent = new double[npt];
         dummy = agg->GetV1();
       for(int i=0;i<npt;i++){
cout<<"wein:"<<wein_state[i]<<"\t"<<"ihwp:"<<ihwp_state[i]<<endl;
         mean_reg[i] = *(dummy+i);
        if((wein_state[i]==1&&ihwp_state[i]==2)||(wein_state[i]==2&&ihwp_state[i]==1))
         mean[i] = -1*mean_reg[i];//correcting sign for IHWP OUT
        else
         mean[i] = mean_reg[i];
        }
       for(int i=0;i<npt;i++)
         ent[i] = i;
         var_err = var_list[plt];
         var_err.ReplaceAll("_mean","_mean_error");
         npt=agg->Draw(Form("%s:n_runs",var_err.Data()),slug_cut+hrs_cut,"goff");
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
         npt=agg->Draw(Form("%s:n_runs",var_rms.Data()),slug_cut+hrs_cut,"goff");
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
         npt=agg->Draw(Form("%s:n_runs",var_rms_err.Data()),slug_cut+hrs_cut,"goff");
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

         gSystem->Exec("pdfunite *.pdf plots/aggretate_AT_dit_26-54.pdf");
         gSystem->Exec("rm -rf *.pdf");
}












