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

void intersect(TGraphErrors* g1,TGraphErrors* g2,double r1, double r2){
    g1->Fit("pol1","O","",r1,r2);
    g2->Fit("pol1","O","",r1,r2);
    double* fitpar1= g1->GetFunction("pol1")->GetParameters();
const double* Fiterr1= g1->GetFunction("pol1")->GetParErrors();
double* fitpar2= g2->GetFunction("pol1")->GetParameters();
const double* Fiterr2= g2->GetFunction("pol1")->GetParErrors();
std::vector<double> fiterr1;
for(int i{0};i!=g1->GetFunction("pol1")->GetNpar();++i){
  fiterr1.push_back(Fiterr1[i]*sqrt(g1->GetFunction("pol1")->GetChisquare()/g1->GetFunction("pol1")->GetNDF()));
}
std::vector<double> fiterr2;
for(int i{0};i!=g2->GetFunction("pol1")->GetNpar();++i){
  fiterr2.push_back(Fiterr2[i]*sqrt(g2->GetFunction("pol1")->GetChisquare()/g2->GetFunction("pol1")->GetNDF()));
}
double x = (fitpar2[0]-fitpar1[0])/(fitpar1[1]-fitpar2[1]);
double xerr = abs(1/(fitpar1[1]-fitpar2[1]))*fiterr1[0]+abs(1/(fitpar1[1]-fitpar2[1]))*fiterr2[0]+abs(x/(fitpar1[1]-fitpar2[1]))*(fiterr1[1]+fiterr2[1]);
std::cout<<"f crossover ="<<x<<"+/-"<<xerr<<"\n";
TF1 *ff1=g1->GetFunction("pol1");
ff1->SetLineColor(kRed);
ff1->SetLineStyle(2);
ff1->DrawCopy("SAME");
TF1 *ff2=g2->GetFunction("pol1");
ff2->SetLineColor(kRed);
ff2->SetLineStyle(2);
ff2->DrawCopy("SAME");

}

double calcError(std::fstream &s){
  double x;
  s>>std::ws>> x;
  double max{x},min{x};
  for(int i{0};i!=100;++i){
    s>>std::ws>> x;
    if(x>max) max=x;
    if(x<min) min=x;
    //std::cout<<"Max "<<max<<" Min "<<min;
  }
  return max-min;
}

void makegraph(){
gStyle->SetPadGridX(true);
 gStyle->SetPadGridY(true);
std::fstream in;
  //TH1F *rumore = new TH1F("rumore","Rumore",10000000,0,5);
  in.open("Generatore/rumore.txt");/*
  double x;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }*/
  
  
  double error{calcError(in)};
  in.close();
  std::cout<<error<<"\n";
  
  //TH1F *rumore1 = new TH1F("rumore1","Rumore",10000000,0,5);
  in.open("Alto/rumore.txt");/*
  x=0;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore1->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }*/
  
  double error1{calcError(in)};
  in.close();
  std::cout<<error1<<"\n";
  
  //TH1F *rumore2 = new TH1F("rumore2","Rumore",10000000,0,5);
  in.open("Medio/rumore.txt");/*
  x=0;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore2->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }*/
  
  double error2{calcError(in)};
  in.close();
  std::cout<<error2<<"\n";
  
  
  //TH1F *rumore3 = new TH1F("rumore3","Rumore",1000000,3.5,3.52);
  in.open("Basso/rumore.txt");/*
  x=0;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore3->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }*/
    
  double error3{calcError(in)};
  in.close();
  std::cout<<error3<<"\n";
  

TGraphErrors* data[4];

    data[0]= new TGraphErrors("Generatore/data.txt","%lg%lg%lg%lg","");
    data[1]= new TGraphErrors("Alto/data.txt","%lg%lg%lg%lg","");
    data[2]= new TGraphErrors("Medio/data.txt","%lg%lg%lg%lg","");
    data[3]= new TGraphErrors("Basso/data.txt","%lg%lg%lg%lg","");

    for(int i{0};i<data[0]->GetN();++i){
    data[0]->SetPointError(i,0,error);}
    for(int i{0};i<data[1]->GetN();++i){
    data[1]->SetPointError(i,0,error1);}
    for(int i{0};i<data[2]->GetN();++i){
    data[2]->SetPointError(i,0,error2);}
    for(int i{0};i<data[3]->GetN();++i){
    data[3]->SetPointError(i,0,error3);
    }
  data[0]->SetTitle("Tensione in frequenza ;Frequenza [Hz];Tensione [V]");
  data[0]->GetYaxis()->SetTitleOffset(1.4);
  data[0]->SetMarkerStyle(7);
  data[0]->SetMarkerColor(kBlack);
  data[0]->SetLineColor(kBlack);

  for(int j{1};j<4;++j){
  data[j]->SetTitle("Tensione in frequenza ;Frequenza [Hz];Tensione [V]");
  data[j]->GetYaxis()->SetTitleOffset(1.4);
  data[j]->SetMarkerStyle(7);
  data[j]->SetMarkerColor(kBlue+200*(j-1));
  data[j]->SetLineColor(kBlue+200*(j-1));
  }

  data[3]->SetMaximum(4);
  data[3]->Draw("APEL");
  data[2]->Draw("PEL,SAME");
  data[1]->Draw("PEL,SAME");
  data[0]->Draw("PEL,SAME");

std::cout<<"MAX passa banda:";

data[2]->Fit("pol2","O","",550,850);

double* result= data[2]->GetFunction("pol2")->GetParameters();
const double* Errors= data[2]->GetFunction("pol2")->GetParErrors();
std::vector<double> errors;
for(int i{0};i!=data[2]->GetFunction("pol2")->GetNpar();++i){
  errors.push_back(Errors[i]*sqrt(data[2]->GetFunction("pol2")->GetChisquare()/data[2]->GetFunction("pol2")->GetNDF()));
}
std::cout<<"fmax="<<-1*result[1]/(2*result[2])<<"+/-"<<errors[1]/(abs(2*result[2]))+errors[2]*abs(result[1])/(2*pow(result[2],2))<<"\n";
TF1 *ff=data[2]->GetFunction("pol2");
ff->SetLineColor(kRed);
ff->SetLineStyle(2);
ff->DrawCopy("SAME");

std::cout<<"CROSSOVER ALTO BASSO:";
intersect(data[1],data[3],600,750);

std::cout<<"CROSSOVER ALTO MEDIO:";
intersect(data[2],data[1],950,1100);

std::cout<<"CROSSOVER BASSP MEDIO:";
intersect(data[3],data[2],400,550);

  TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
leg->AddEntry(data[1],"Tweeter","lep");
leg->AddEntry(data[2],"Midrange","lep");
leg->AddEntry(data[3],"Woofer","lep");
leg->AddEntry(data[0],"Generatore di tensione","lep");
leg->Draw();
//rumore->Draw("HISTO");
}