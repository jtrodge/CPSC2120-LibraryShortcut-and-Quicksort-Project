#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <tuple>
#include <assert.h>
#include <string>
#include <cstring>
#include <vector>
#include <set>

using namespace std;
struct Team {
  string name;
  int off, def;
};
//Sort Offenses from greatest to least
bool teamcomp (Team &team1, Team &team2) {
  return team1.off > team2.off;
}

int main() {
  string team;
  int offense;
  int defense;
  int insert = 0;
  //string, offense
  cout << "Number of teams you would like to insert: ";
  cin >> insert;
  Team N[insert];
  for (int i = 0; i < insert; i++) {
    cin >> N[i].name >> N[i].off >> N[i].def;
  }
  cout << endl;
  for (int i = 0; i < insert; i++) {
    cout << N[i].name << " " << N[i].off << " " << N[i].def << endl;
  }
  cout << endl;
  //Sort offense from greatest to least
  sort(N, N+insert, teamcomp);

  set<string> a;
  int best_def = 0;
  for (int i = 0; i < insert; i++) {
    if (best_def < N[i].def) {
      best_def = N[i].def;
      a.insert(N[i].name);
    }
  }
  for (auto it = a.begin(); it != a.end(); it++) {
    cout << (*it) << " ";
  }
  cout << endl;

  return 0;
}
