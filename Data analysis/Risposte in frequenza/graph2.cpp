#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "TROOT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TFile.h"

void intersect(TGraphErrors* g1,double r1, double r2,double a,double aErr){
    g1->Fit("pol1","O","",r1,r2);
    double* fitpar1= g1->GetFunction("pol1")->GetParameters();
const double* fiterr1= g1->GetFunction("pol1")->GetParErrors();

double x = (a-fitpar1[0])/fitpar1[1];
double xerr = (fitpar1[0]*aErr+fiterr1[0]*a+x*fiterr1[1])/fitpar1[1];
std::cout<<"f Taglio ="<<x<<"+/-"<<xerr<<"\n";
TF1 *ff1=g1->GetFunction("pol1");
ff1->SetLineColor(kRed);
ff1->SetLineStyle(2);
ff1->DrawCopy("SAME");

}

void makegraph2(){

std::fstream in;
  TH1F *rumore = new TH1F("rumore","Rumore",10000,3.5,3.52);
  in.open("rumore.txt");
  double x;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }
  in.close();
  
  double error{rumore->GetStdDev()};
  std::cout<<error<<"\n";
  

TGraphErrors* data[3];

    data[0]= new TGraphErrors("Alto/data.txt","%lg%lg%lg%lg","");
    data[1]= new TGraphErrors("Medio/data.txt","%lg%lg%lg%lg","");
    data[2]= new TGraphErrors("Basso/data.txt","%lg%lg%lg%lg","");

for(int j{0};j<3;++j){
    for(int i{0};i<data[j]->GetN();++i){
    data[j]->SetPointError(i,0,error);
  }
  data[j]->SetTitle("Risposta in frequenza ;Frequenza [Hz];Tensione [V]");
  data[j]->GetYaxis()->SetTitleOffset(1.4);
  data[j]->SetMarkerStyle(7);
  data[j]->SetMarkerColor(kBlue+200*j);
  data[j]->SetLineColor(kBlue+200*j);
  }

  data[2]->Draw("APEL");
  data[1]->Draw("PEL,SAME");
  data[0]->Draw("PEL,SAME");




std::cout<<"Taglio ALTO:";
intersect(data[0],1100,1200,2.62198,8.5746e-6);
/*
std::cout<<"Taglio MEDIO:";
intersect(data[1],900,1050);
*/
std::cout<<"Taglio BASSo:";
intersect(data[2],300,400,2.62198,8.5746e-6);

  TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
leg->AddEntry(data[0],"FIltro passa alto","lep");
leg->AddEntry(data[1],"FIltro passa banda","lep");
leg->AddEntry(data[2],"FIltro passa basso","lep");
leg->Draw();
//rumore->Draw("HISTO");
}