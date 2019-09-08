#include <TGaxis.h>
#include <TCanvas.h>
#include <TPaveLabel.h>
#include <TGraphErrors.h>


void chargeperslug(){
	gStyle->SetOptStat(0);
	ifstream infile ("outfiles/goodcharge_run.txt");
	int slug = 0;
	if(slug<=0)
	cout<<"Please Enter slug number:";
	cin>>slug;
	ifstream inlist (Form("slug%d.list",slug));
	Int_t run,sl_run;
	double charge;
	int count = 0;
	int sl_run_ct = 0;
	double tot_sum = 0;
	const int points = 500;
	double my_run[points],my_charge[points],my_sl_run[points];
	double total_ch = 0;
   for (int i=0;i<points;i++){
	inlist>>sl_run;
	if(sl_run!=0){
	sl_run_ct++;
	my_sl_run[sl_run_ct] = sl_run;
	}
   }
	cout<<"first_run:"<<my_sl_run[1]<<"\t"<<"last_run:"<<my_sl_run[sl_run_ct]<<endl;
   for (int i=0;i<points;i++){
	infile>>run>>charge;
	if(run>=my_sl_run[1]&&run<=my_sl_run[sl_run_ct]){
	count++;
	my_run[count] = run;
        my_charge[count] = charge;
	total_ch+=charge;
   }
}
	cout<<"Total Charge:"<<total_ch<<endl;
	infile.close();
	const int my_num = count;

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
        TCanvas *c2 = new TCanvas("c2","c2", 1000, 600);

        TH1F* hr1 = new TH1F("hr1", "Good charge vs run", 1400, 3100+0.5, 4500+0.5);
	for(int i=1;i<=my_num;i++){
	cout<<my_sl_run[i]<<"\t"<<my_charge[i]<<endl;
	hr1->Fill(my_sl_run[i],my_charge[i]);
   }
	hr1->SetTitle(Form("Good Charge per Run (slug%d);Run;Charge (C)",slug));
	hr1->SetMarkerStyle(20);
        hr1->GetXaxis()->SetRangeUser(my_sl_run[1], my_sl_run[sl_run_ct]+1);
        hr1->SetFillStyle(3001);
        hr1->SetFillColor(kGreen);
	hr1->Draw("HIST");
	TString label = Form("Good Charge in slug%d:: %.2fC",slug,total_ch);
	TPaveLabel *pl = new TPaveLabel(0.17,0.18,0.48,0.22,label,"NDC");
	pl->SetBorderSize(0);
	pl->SetTextColor(kRed);
	pl->SetTextSize(1.0);
	pl->SetFillColor(kSpring-9);
	pl->Draw("text");
	TString label1 = Form("Good Runs in slug%d:: %d",slug,my_num);
	TPaveLabel *pl1 = new TPaveLabel(0.20,0.14,0.45,0.18,label1,"NDC");
	pl1->SetBorderSize(0);
	pl1->SetTextColor(kRed);
	pl1->SetTextSize(1.0);
	pl1->SetFillColor(kSpring-9);
	pl1->Draw("text");


        c2->SaveAs(Form("plots/charge_slug%d.pdf",slug));
}

