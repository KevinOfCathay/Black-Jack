#pragma once
#ifndef MC_H
#define MC_H

#include <vector>
#include <iostream>
#include <map>
#include <ctime>
#include <unordered_map>

#include <boost\functional\hash.hpp>

#define umap unordered_map
#define vfloat vector<float>

using namespace std;

class BlackJack {
public:
	enum Action { Stick, Hit };
	BlackJack () { srand (time (NULL)); };
	~BlackJack () {};
	void InitMap () {
		for (int p = 12; p <= 21; p = p + 1) {
			for (int q = 1; q <= 10; q = q + 1) {
				SMap[make_pair (p, q)] = vfloat{ 0,0 };
				HMap[make_pair (p, q)] = vfloat{ 0,0 };
			}
		}
	}
	void FirstVisit (int stp) {
		InitMap (); steps = stp;
		for (int a = 0; a < stp; a = a + 1) {
			Loop ();
		}
	}
	void ShowPlain () {
		int PS = 0;
		for (int p = 12; p <= 21; p = p + 1) {
			for (int q = 1; q <= 10; q = q + 1) {
				vfloat S = SMap[make_pair (p, q)];
				vfloat H = HMap[make_pair (p, q)];
				float ss = S[0], hh = H[0];
				cout << p << " , " << q << " , " << ss << " , " << hh;
				if (ss > hh) { cout << " , Stick"; }
				if (ss < hh) { cout << " , Hit"; }
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
		vfloat temp = {};
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
		if (SMap.at (make_pair (players, dealerfc)).at (0) > HMap.at (make_pair (players, dealerfc)).at (0)) {
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
				vfloat temp = SMap.at (make_pair (p.PS, p.DFC));
				temp[0] = (temp[0] * temp[1] + Returns) / (temp[1] + 1);
				temp[1] = temp[1] + 1;
				SMap[make_pair (p.PS, p.DFC)] = temp;
				Path.pop_back (); continue;
			}
			else if (p.A == Hit) {
				vfloat temp = HMap.at (make_pair (p.PS, p.DFC));
				temp[0] = (temp[0] * temp[1] + Returns) / (temp[1] + 1);
				temp[1] = temp[1] + 1;
				HMap[make_pair (p.PS, p.DFC)] = temp;
				Path.pop_back (); continue;
			}
		}
	}
	int DealersTurn (int dealerfc) {
		int has_A = (dealerfc == 1) ? 0 : 1;
		int dealfinal = dealerfc;
		while (true) {
			int c = Get ();
			if (c == 1) { c == 11; has_A = has_A + 1; }
			if (dealfinal > 21) {
				while (has_A > 0 && dealfinal > 21) {
					has_A = has_A - 1;
					dealfinal = dealfinal - 10;
				}
				if (dealfinal <= 21) { continue; }
				return dealfinal;
			}
			if (PD.at (dealfinal) == Stick) {
				return dealfinal;
			}
			else {
				dealfinal = dealfinal + c;
			}
		}
	}
private:
	float gamma = 1; // Discount rate
	int steps;

	umap<pair<int, int>, vfloat, boost::hash<pair<int, int>>> SMap; // key: playersum, dealerfirstcard | Value: average return, step
	umap<pair<int, int>, vfloat, boost::hash<pair<int, int>>> HMap;
	vector<Route> Path = {}; //playersum, dealerfirstcard | Action

	const umap<int, Action> PD = {
	{1,Hit},{2,Hit},{3,Hit},{4,Hit},{5,Hit},{6,Hit},{7,Hit},{8,Hit},{9,Hit},{10,Hit},{11,Hit},{12,Hit},
	{13,Hit},{14,Hit},{15,Hit},{16,Hit},{17,Stick},{18,Stick},{19,Stick},{20,Stick},{21,Stick}
	};// Dealer's policy
	const umap<int, int >ValueMap = { {1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,10},{12,10},{13,10} };
};


#endif // !MC_H
