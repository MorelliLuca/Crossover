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

void makegraph(){
  TGraph * noise[4];
  TF1 * fitScarti[4];
   gStyle->SetPadGridX(true);
 gStyle->SetPadGridY(true);
TCanvas *canvas = new TCanvas("canvas","Data",700,700);
TPad *pad1 = new TPad("pad1","Graph",0,.2,1,1);
TPad *pad2 = new TPad("pad2","Scarti",0,0,1,.2);
double error[4];
std::fstream in;
  TH1F *rumore0 = new TH1F("rumore","Rumore",1000000,-10,10);
  in.open("alto/rumore.txt");
  double x1;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x1;
    rumore0->Fill(x1);
  }
  in.close();
  
  error[0]={rumore0->GetStdDev()};
  TH1F *rumore1 = new TH1F("rumore","Rumore",1000000,-10,10);
  in.open("medio/rumore.txt");
  double x0;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x0;
    rumore1->Fill(x0);
  }
  in.close();
  
  error[1]={rumore1->GetStdDev()};

  TH1F *rumore2 = new TH1F("rumore","Rumore",1000000,-10,10);
  in.open("basso/rumore.txt");
  double x2;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x2;
    rumore2->Fill(x2);
  }
  in.close();
  
  error[2]={rumore2->GetStdDev()};

  TH1F *rumore3 = new TH1F("rumore","Rumore",1000000,-10,10);
  in.open("gen/rumore.txt");
  double x;
  for(int i{0};i<100;++i)
  {
    in>>std::ws>> x;
    rumore3->Fill(x);
    std::cout<<i<<':'<<x<<"\n";
  }
  in.close();
  
  error[3]={rumore3->GetStdDev()};
  
  

TGraphErrors* data[4];

    data[0]= new TGraphErrors("alto/data.txt","%lg%lg%lg%lg","");
    data[1]= new TGraphErrors("medio/data.txt","%lg%lg%lg%lg","");
    data[2]= new TGraphErrors("basso/data.txt","%lg%lg%lg%lg","");
    data[3]= new TGraphErrors("gen/data.txt","%lg%lg%lg%lg","");

TF1* fitFun[4];
TF1* fitRes[4];
//pad2->cd();
for(int j{0};j<4;++j){
    for(int i{0};i<data[j]->GetN();++i){
      
    data[j]->SetPointError(i,0,error[j]);
  }
  data[j]->SetTitle("Grafico tensione tempo a 760Hz ;Tempo [s];Tensione [V]");
  data[j]->GetYaxis()->SetTitleOffset(1.4);
  data[j]->SetMarkerStyle(7);
  data[j]->SetMarkerColor(kBlue+200*j);
  fitFun[j] = new TF1("fitFun","[0]*sin(x*2*pi*[1] + [2]) + [3]");
  fitFun[j]->SetParameters(10,750,0,0);
  data[j]->Fit("fitFun","O");
  fitRes[j]=data[j]->GetFunction("fitFun");
  fitRes[j]->SetLineColor(kBlue+200*j);
  double *X{data[j]->GetX()},*Y{data[j]->GetY()};
double meanY;

int N{data[j]->GetN()};
std::vector<double> delta;

for(int i{0};i!=N ; ++i){
  delta.push_back(Y[i]-fitRes[j]->Eval(X[i])); //removed +1
  meanY+=Y[i];
}
meanY/=N;
double sumDen{0}, sumNum{0};
for(int i{0};i!=N ; ++i){
  sumDen+=((Y[i]-meanY)*(Y[i]-meanY));
  sumNum+=(delta[i]*delta[i]);
}
double *d = &delta[0];

noise[j] = new TGraph(data[j]->GetN(),data[j]->GetX(),d);
noise[j]->SetTitle(";Tempo [s];Tensione [V]");
noise[j]->GetYaxis()->SetLabelSize(.1);
noise[j]->GetYaxis()->SetNdivisions(5);
noise[j]->GetXaxis()->SetLabelSize(.1);
noise[j]->GetXaxis()->SetTitleSize(.1);
noise[j]->GetYaxis()->SetTitleSize(.1);
noise[j]->GetXaxis()->SetTitleOffset(.8);
noise[j]->GetYaxis()->SetTitleOffset(.5);


//noise[j]->Draw("APL");

TF1 *lin = new TF1("lin","[0]");

noise[j]->Fit(lin,"0Q");

fitScarti[j]=noise[j]->GetFunction("lin");
if(j==3){fitScarti[j]->SetLineColor(kBlack);}
else{fitScarti[j]->SetLineColor(kBlue+200*j);}
if(j==3){noise[j]->SetLineColor(kBlack);}
else{noise[j]->SetLineColor(kBlue+200*j);}
fitScarti[j]->SetLineStyle(2);
//fitScarti[j]->Draw("SAME");

  }
data[3]->SetMarkerColor(kBlack);
fitRes[3]->SetLineColor(kBlack);
pad1->cd();

  data[3]->Draw("APE");
  data[1]->Draw("PE,SAME");
  data[2]->Draw("PE,SAME");
  data[0]->Draw("PE,SAME");

  fitRes[0]->Draw("SAME");
  fitRes[1]->Draw("SAME");
  fitRes[2]->Draw("SAME");
  fitRes[3]->Draw("SAME");



  TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
leg->AddEntry(fitRes[0],"Tweeter","lep");
leg->AddEntry(fitRes[1],"Midrange","lep");
leg->AddEntry(fitRes[2],"Woofer","lep");
leg->AddEntry(fitRes[3],"Generatore","lep");
leg->Draw();
pad2->cd();  
noise[1]->Draw("APL");
noise[3]->Draw("PL,SAME");
noise[2]->Draw("PL,SAME");
noise[0]->Draw("PL,SAME");
fitScarti[0]->Draw("SAME");
fitScarti[1]->Draw("SAME");
fitScarti[2]->Draw("SAME");
fitScarti[3]->Draw("SAME");

canvas->cd();
pad1->Draw();
pad2->Draw();
double w{fitRes[3]->GetParameter("1")*(2*TMath::Pi())},
VH{fitRes[0]->GetParameter("0")},
VM{fitRes[1]->GetParameter("0")},
VL{fitRes[2]->GetParameter("0")},
VG{fitRes[3]->GetParameter("0")},
FH{fitRes[0]->GetParameter("2")},
FM{fitRes[1]->GetParameter("2")},
FL{fitRes[2]->GetParameter("2")},
FG{fitRes[3]->GetParameter("2")},
RH{149.96},
RM{150.15},
RL{149.86},
CH{1e-6},
CM{0.99e-6},
LM{39.45e-3},
LL{45.22e-3};
double we{fitRes[3]->GetParError(1)*(2*TMath::Pi())*sqrt(fitRes[3]->GetChisquare()/fitRes[3]->GetNDF())},
VHe{fitRes[0]->GetParError(0)*sqrt(fitRes[0]->GetChisquare()/fitRes[0]->GetNDF())},
VMe{fitRes[1]->GetParError(0)*sqrt(fitRes[1]->GetChisquare()/fitRes[1]->GetNDF())},
VLe{fitRes[2]->GetParError(0)*sqrt(fitRes[2]->GetChisquare()/fitRes[2]->GetNDF())},
VGe{fitRes[3]->GetParError(0)*sqrt(fitRes[3]->GetChisquare()/fitRes[3]->GetNDF())},
FHe{fitRes[0]->GetParError(2)*sqrt(fitRes[0]->GetChisquare()/fitRes[0]->GetNDF())},
FMe{fitRes[1]->GetParError(2)*sqrt(fitRes[1]->GetChisquare()/fitRes[1]->GetNDF())},
FLe{fitRes[2]->GetParError(2)*sqrt(fitRes[2]->GetChisquare()/fitRes[2]->GetNDF())},
FGe{fitRes[3]->GetParError(2)*sqrt(fitRes[3]->GetChisquare()/fitRes[3]->GetNDF())},
Re{0.17},
CHe{CH*0.01},
CMe{CM*0.01},
LMe{0.0010550},
LLe{0.0012350};

double VMa=VG/(sqrt(1+pow(w*LM/RM-1/(w*RM*CM),2)));
double VHa=VG/(sqrt(1+pow(1/(w*RH*CH),2)));
double VLa=VG/(sqrt(1+pow(w*LL/RL,2)));
double VMae=VMa/VG*VGe+VG/pow(1+pow(w*LM/RM-1/(w*RM*CM),2),1.5)*abs(w*LM/RM-1/(w*RM*CM))*((LM/RM+1/(w*w*RM*CM))*we+w/RM*LMe+CMe/(w*RM*CM*CM)+Re/RM*abs(w*LM-1/(w*CM)));
double VHae=VHa/VG*VGe+VG/pow(1+pow(1/(w*RH*CH),2),1.5)*abs(1/(w*RH*CH))*((1/(w*w*RH*CH))*we+CHe/(w*RH*CH*CH)+Re/RH*abs(1/(w*CH)));
double VLae=VLa/VG*VGe+VG/pow(1+pow(w*LL/RL,2),1.5)*abs(w*LL/RL)*((LL/RL)*we+w/RL*LLe+Re/RL*abs(w*LL));

std::cout<<"V Alto misurato:"<<VH<<"+/-"<<VHe<<" Atteso:"<<VHa<<"+/-"<<VHae<<"\n"<<
"V Medio misurato:"<<VM<<"+/-"<<VMe<<" Atteso:"<<VMa<<"+/-"<<VMae<<"\n"<<
"V basso misurato:"<<VL<<"+/-"<<VLe<<" Atteso:"<<VLa<<"+/-"<<VLae<<"\n";

double FMa=atan((1-w*w*LM*CM)/(w*RM*CM));
double FLa=atan(-1*w*RL/LL);
double FHa=atan(1/(w*RH*CH));

double FHae=pow(cos(1/(w*RH*CH)),2)/(w*RH*CH)*(we/w+Re/RH+CHe/CH);
double FLae=pow(cos(1*w*RL/LL),2)/*(1*w*RL/LL)*/*(we/w+Re/RL+LLe/LL);
double FMae=pow(cos((1-w*w*LM*CM)/(w*RM*CM)),2)*abs((1-w*w*LM*CM)/(w*RM*CM))*(abs((w*w*CM*LM+1)/(w*w*RM*CM))*we+abs((w*w*CM*LM-1)/(RM*w*RM*CM))*Re+(1/(w*RM*CM*CM))*CMe+w/RM*LMe);

std::cout<<"Phi Alto Misurato: "<<FH-FG<<"+/-"<<FHe+FHe<< "Atteso: "<<FHa<<"+/-"<<FHae<<"\n";
std::cout<<"Phi Medio Misurato: "<<FM-FG<<"+/-"<<FMe+FHe<< "Atteso: "<<FMa<<"+/-"<<FMae<<"\n";
std::cout<<"Phi Basso Misurato: "<<FL-FG<<"+/-"<<FLe+FHe<< "Atteso: "<<FLa<<"+/-"<<FLae<<"\n";
std::cout<<"Chi/ndf Gen:"<<fitRes[3]->GetChisquare()/fitRes[3]->GetNDF()<<"\n";
std::cout<<"Chi/ndf Low:"<<fitRes[2]->GetChisquare()/fitRes[2]->GetNDF()<<"\n";
std::cout<<"Chi/ndf Medium:"<<fitRes[1]->GetChisquare()/fitRes[1]->GetNDF()<<"\n";
std::cout<<"Chi/ndf High:"<<fitRes[0]->GetChisquare()/fitRes[0]->GetNDF()<<"\n";
}