#pragma once
#ifndef MC_H
#define MC_H

#include <vector>
#include <iostream>
#include <map>
#include <ctime>

using namespace std;

class BlackJack {
public:
	enum Action { Stick, Hit };
	BlackJack () { srand (time (NULL)); };
	~BlackJack () {};
	void InitMap () {
		for (int p = 12; p <= 21; p = p + 1) {
			for (int q = 1; q <= 10; q = q + 1) {
				SMap[{ p, q }] = vector<float>{ 0,0 };
				HMap[{ p, q }] = vector<float>{ 0,0 };
			}
		}
	}
	void FirstVisit (int stp) {
		InitMap (); steps=stp;
		for (int a = 0; a < stp; a = a + 1) {
			Loop ();
		}
	}
	void ShowPlain() {
		int PS = 0;
		for (int p = 12; p <= 21; p = p + 1) {
			for (int q = 1; q <= 10; q = q + 1) {
				vector<float> S = SMap[vector<int>{p, q}];
				vector<float> H = HMap[vector<int>{p, q}];
				float ss = S[0], hh = H[0];
				cout<<p<<" , "<<q<<" , "<<ss<<" , "<<hh ;
				if(ss > hh){cout <<" , Stick"; }
				if(ss < hh) { cout << " , Hit"; }
				if (ss == hh) { cout << " , S/H"; }
				cout << endl;
			}
		}
	}
protected:
	struct Route {
		int PS, DFC; Action A;
	};
	int Get () { return ValueMap.at ((rand () % 13) + 1); }
	void InitState (Route& R) {
		R.PS = rand () % 10 + 12;
		R.DFC = rand () % 10 + 1;
		R.A = (rand () % 2 == 0) ? Stick : Hit;
	}
	void Loop () {
		vector<float> temp = {};
		Route r; int Dealerfinal; InitState (r);
		Path.push_back (r);
		Nxt (r.PS, r.A);
		while (true) {
			if (r.PS > 21) { break; }
			if (r.A == Stick) { break; }
			r.A = NxtA (r.PS, r.DFC);
			Path.push_back (r);
			Nxt (r.PS, r.A);
		}
		Dealerfinal = DealersTurn (r.DFC);
		Update (Rt (r.PS, Dealerfinal));
	}
	void Nxt (int& cardvalue, Action a) {
		if (a == Hit) { cardvalue = cardvalue + Get (); }
	}
	Action NxtA (int players, int  dealerfc) {
		if (SMap[vector<int>{players, dealerfc}].at(0) > HMap[vector<int>{players, dealerfc}].at(0)) {
			return Stick;
		}
		else {
			return Hit;
		}
	}
	int Rt (int vplayer, int vdealer) {
		if (vplayer > 21) {
			if (vdealer > 21) { return 0; }
			else { return -1; }
		}
		else if (vplayer <= 21) {
			if (vdealer > 21) { return 1; }
			if (vplayer > vdealer) { return 1; }
			else if (vplayer < vdealer) { return -1; }
			else { return 0; }
		}
	}
	void Update (int Returns) {
		while (!Path.empty ()) {
			Route p = Path.back ();
			if (p.A == Stick) {
				vector<float> temp = SMap[vector<int>{p.PS, p.DFC}];
				temp[0] = (temp[0] * temp[1] + Returns) / (temp[1] + 1);
				temp[1] = temp[1] + 1;
				SMap[vector<int>{p.PS, p.DFC}] = temp;
				Path.pop_back (); continue;
			}
			else if (p.A == Hit) {
				vector<float> temp = HMap[vector<int>{p.PS, p.DFC}];
				temp[0] = (temp[0] * temp[1] + Returns) / (temp[1] + 1);
				temp[1] = temp[1] + 1;
				HMap[vector<int>{p.PS, p.DFC}] = temp;
				Path.pop_back (); continue;
			}
		}
	}
	int DealersTurn (int dealerfc) {
		int dealfinal = dealerfc;
		while (true) {
			if (dealfinal > 21) { return dealfinal; }
			if (PD.at (dealfinal) == Stick) {
				return dealfinal;
			}
			else {
				dealfinal = dealfinal + Get ();
			}
		}
	}
private:
	float gamma = 1; // Discount rate
	int steps;

	map<vector<int>, vector<float>> SMap; //playersum, dealerfirstcard | average return, step
	map<vector<int>, vector<float>> HMap;
	vector<Route> Path = {}; //playersum, dealerfirstcard | Action

	const map<int, Action> PD = {
	{1,Hit},{2,Hit},{3,Hit},{4,Hit},{5,Hit},{6,Hit},{7,Hit},{8,Hit},{9,Hit},{10,Hit},{11,Hit},{12,Hit},
	{13,Hit},{14,Hit},{15,Hit},{16,Hit},{17,Stick},{18,Stick},{19,Stick},{20,Stick},{21,Stick}
	};// Dealer's policy
	const map<int, int >ValueMap = { {1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,10},{12,10},{13,10} };
};


#endif // !MC_H
