#include <bits/stdc++.h>
using namespace std;

struct Patient {
  string category;
  string name;
  int day;
};

int main() {
  int n;
  cin >> n;

  vector<Patient> antrian;
  int daynow = 1;

  for (int i = 0; i < n; i++) {
    string cmd;
    cin >> cmd;

    if (cmd == "LAYANI") {
      if (antrian.empty()) {
        cout << "ANTREAN KOSONG\n";
      } else {
        Patient p = antrian.front();
        antrian.erase(antrian.begin());
        cout << "MELAYANI " << p.name << " HARI KE-" << p.day << "\n";
        daynow = p.day + 1;
      }
    } else if (cmd == "TAMPILKAN") {
      if (antrian.empty()) {
        cout << "ANTREAN KOSONG\n";
      } else {
        for (int j = 0; j < (int)antrian.size(); j++) {
          cout << "ANTREAN " << (j + 1) << " " << antrian[j].name
            << " HARI KE-" << antrian[j].day << "\n";
        }
      }
    } else {
      string kategori = cmd;
      int daybook;
      string nama;
      cin >> daybook >> nama;

      if (daybook < daynow) {
        cout << "BOOKING GAGAL, HARI " << daybook << " SUDAH TIDAK DILAYANI\n";
        continue;
      }

      if (kategori == "MANDIRI") {
        set<int> mandiriday;
        for (auto& p : antrian) {
          if (p.category == "MANDIRI") mandiriday.insert(p.day);
        }

        int dilayaniday = daybook;
        while (mandiriday.count(dilayaniday)) {
          dilayaniday++;
        }

        set<int> mandiri = mandiriday;
        mandiri.insert(dilayaniday);
        vector<int> bpjs;

        for (int j = 0; j < (int)antrian.size(); j++) {
          if (antrian[j].category == "BPJS" && antrian[j].day >= dilayaniday) {
            bpjs.push_back(j);
          }
        }

        set<int> fixday = mandiri;
        for (int j = 0; j < (int)antrian.size(); j++) {
          if (antrian[j].category == "BPJS" && antrian[j].day < dilayaniday) {
            fixday.insert(antrian[j].day);
          }
        }

        for (int idx : bpjs) {
          int d = antrian[idx].day;
          while (fixday.count(d)) d++;
          antrian[idx].day = d;
          fixday.insert(d);
        }

        Patient newPatient = {kategori, nama, dilayaniday};
        int insertPos = (int)antrian.size();
        for (int j = 0; j < (int)antrian.size(); j++) {
          if (antrian[j].day > dilayaniday) {
            insertPos = j;
            break;
          }
        }

        antrian.insert(antrian.begin() + insertPos, newPatient);
        stable_sort(antrian.begin(), antrian.end(), [](const Patient& a, const Patient& b) {
            return a.day < b.day;
            });
      } else {
        set<int> occupiedDays;
        for (auto& p : antrian) occupiedDays.insert(p.day);

        int assignedDay = daybook;
        while (occupiedDays.count(assignedDay)) assignedDay++;

        Patient newPatient = {kategori, nama, assignedDay};
        int insertPos = (int)antrian.size();
        for (int j = 0; j < (int)antrian.size(); j++) {
          if (antrian[j].day > assignedDay) {
            insertPos = j;
            break;
          }
        }
        antrian.insert(antrian.begin() + insertPos, newPatient);
      }
    }
  }

  return 0;
}
