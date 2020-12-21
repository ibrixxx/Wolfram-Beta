#include <iostream>
#include <stack>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>


using namespace std;

int Prioritet(char Znak) {
  if(Znak=='s') return 3;
  else if(Znak=='*'||Znak=='/') return 2;
  else if(Znak=='+'||Znak=='-') return 1;
  else return 0;
}

void IzvrsiOperaciju(stack<string> &s,char Operacija) {

if(Operacija=='-' || Operacija=='+'  || Operacija=='/' || Operacija=='*')
{
  if(s.size()<2) throw "Neizbalansirani operatori i operandi!";
  string O1=s.top();
  double Operand1=stod(O1);
  s.pop();
  string O2=s.top();
  double Operand2=stod(O2);
  s.pop();
  double Rezultat;
  if(Operacija=='+') Rezultat=Operand2+Operand1;
  else if(Operacija=='-') Rezultat=Operand2-Operand1;
  else if(Operacija=='*') Rezultat=Operand2*Operand1;
  else Rezultat=Operand2/Operand1;
  s.push(to_string(Rezultat));
}
else{
  if(s.size()<1) throw "Neizbalansirani operatori i operandi (sin)!";
  string O1=s.top();
  double Operand1=stod(O1);
  s.pop();
  s.push(to_string(sin(Operand1)));
     }
}

double vratiRez(const string value, char var, string s) {
    for(int i(0); i<s.size(); i++)
        if(s[i] == var)
            s.replace(i,1,value);
    stack<char> Stek;
    stack<string> StekBrojeva;
    int BrojZagrada=0;
    char Prethodni='(';
    istringstream strin(s);
    try {
      while(strin.peek()!='\n') {
        if(strin.peek()>='0'&& strin.peek()<='9') {
          string Broj("");
          while(strin.peek()>='0'&& strin.peek()<='9' || strin.peek() == '.')
            Broj += strin.get();
          StekBrojeva.push(Broj);
          Prethodni='0';
        }
        else {
          char Znak=strin.get();
          while(Znak == ' ') Znak = strin.get();
          if(Znak == '\n') {strin.clear(); break;}
          if(Prioritet(Znak)>0) {
            if((Prioritet(Prethodni)>0 && Znak!='s') || ((Prethodni=='(' && Znak!='s')&&(Prethodni=='(' && Znak!='-') ))
              throw "Neocekivan operator!";
            if(Znak == '-' && Prethodni == '('){
                StekBrojeva.push("0");
                Prethodni = '0';
            }
            while(1) {
              if(Stek.empty()||Prioritet(Znak)>Prioritet(Stek.top())) {
                Stek.push(Znak); break;
              }
              IzvrsiOperaciju(StekBrojeva,Stek.top());
              Stek.pop();
            }
          // while(cin.peek()>='a' && cin.peek()<='z')cin.get();
            if(Znak=='s'){strin.get();strin.get();}

          }
          else if(Znak=='(') {
            Stek.push('('); BrojZagrada++;
          }
          else if(Znak==')') {
            BrojZagrada--;
            if(BrojZagrada<0) throw "Visak zatvorena zagrada!";
            while(Stek.top()!='(') {
              IzvrsiOperaciju(StekBrojeva,Stek.top());
              Stek.pop();
            }
            Stek.pop();
          }
           else if(Znak=='{') {
            Stek.push('{'); BrojZagrada++;
          }
          else if(Znak=='}') {
            BrojZagrada--;
            if(BrojZagrada<0) throw "Visak zatvorena zagrada!";
            while(Stek.top()!='{') {
              IzvrsiOperaciju(StekBrojeva,Stek.top());
              Stek.pop();
            }
            Stek.pop();
          }
          else break;
          Prethodni=Znak;
        }
      }
      if(BrojZagrada>0) throw "Nedostaje zatvorena zagrada!";
      while(!Stek.empty()) {
        IzvrsiOperaciju(StekBrojeva,Stek.top());
        Stek.pop();
      }
      if(StekBrojeva.size()!=1) throw "Neizbalansirani operatori i operandi!";
    }
    catch(const char poruka[]) {
        cout<<poruka;
    }
    strin.clear();
    return stod(StekBrojeva.top());
}


void crtajGrafik(string s, char var, double pocetak, double kraj){
    ofstream unos("fajl.txt");
    unos<<s<<endl;
    vector<double> vrijednosti;
    string val;
    for(double i(pocetak); i<=kraj; i++){
        if(i < 0){
            val = "(";
            val += to_string(i);
            val += ")";
        }
        else
            val = to_string(i);
        vrijednosti.push_back(vratiRez(val, var, s));
    }
    vector<double> pom(vrijednosti);
    sort(pom.begin(), pom.end());
    vector<int> red;

    int n = vrijednosti.size();
    for(auto x : vrijednosti)
        cout<<x<<' ';
    for(int i(0); i<n; i++){
       int brojac(n-1);
       for(int j(0); j<n; j++){
         if(vrijednosti[i] > pom[j])
            brojac--;
       }
       red.push_back(brojac);
    }


    vector<vector<double>> graf(n, vector<double>(n));
    for(int i(0); i<n; i++)
        if(vrijednosti[i])
            graf[red[i]][i] = vrijednosti[i];
        else //da se i nula vrijdenost vidi
            graf[red[i]][i] = vrijednosti[i]+0.000000001;

    unos<<endl;
    string abc = to_string(round(pom[n-1]));
    int velicina = abc.size();
    int v2 = 2; //razmak izmedju zvjezdica
    int pass = pom[n-1]+10;
    for(int i(1); i<n; i++)
        if(pom[i] == pom[i-1])
            pom[i] = pass;

    for(int i(0); i<n; i++){
        if(pom[n-i-1] == pass)
            continue;
        for(int j(0); j<n; j++){
            if(j == 0){
                unos<<setw(velicina)<<pom[n-i-1];
                unos<<"  ";
            }
            if(graf[i][j])
                unos<<setw(v2)<<'*';
            else
                unos<<setw(v2)<<' ';
        }
        unos<<endl;
    }
    for(int i(0); i<n; i++)
        unos<<' ';
    unos<<endl;
    unos<<setw(velicina)<<"y/x";
    unos<<"  ";
    for(double i(pocetak); i<=kraj; i++){
        unos<<setw(v2)<<i;
    }
}


int main() {
  string s;
  getline(cin, s);
  crtajGrafik(s, 'x', -4, 4);
  //cout<<vratiRez('1', 'x', s);

  return 0;
}



