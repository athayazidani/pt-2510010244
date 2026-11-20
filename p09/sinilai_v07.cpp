// SiNilai v0.7 - refactoring SiNilai v0.6 menjadi sembilan fungsi.
// Catatan desain: fungsi yang menghasilkan satu nilai memakai return; fungsi yang mengisi banyak
// variabel milik pemanggil memakai parameter referensi (&); tidak ada variabel global.
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Konstanta di luar fungsi tidak melanggar aturan "tanpa variabel global" karena tidak berubah.
const double BOBOT_HADIR = 0.10;
const double BOBOT_TUGAS_MINGGUAN = 0.45;
const double BOBOT_UJIAN_TENGAH = 0.25;
const double BOBOT_UJIAN_AKHIR = 0.20;

int bacaJumlah();
string bacaNama();
double bacaKomponen(string label);
void bacaData(string& nama, string& npm, double& kehadiran, double& mingguan, double& uts, double& uas);
double hitungNilaiAkhir(double kehadiran, double mingguan, double uts, double uas);
string hurufMutu(double nilai_akhir);
void cetakBaris(string nama, double nilai_akhir, string huruf, bool lulus);
void perbaruiPenghitung(char awal, int& a, int& b, int& c, int& d, int& e);
void cetakSebaran(int a, int b, int c, int d, int e);

int main() {
    const string judul = "=== SiNilai v0.7 ===";
    cout << judul << '\n';
    int cacah_mhs = bacaJumlah();
    cout << fixed << setprecision(2);

    double nilai_terkumpul = 0;
    int cacah_lulus = 0;
    int pa = 0, pb = 0, pc = 0, pd = 0, pe = 0;
    double puncak = -1;
    string nama_puncak;

    int urutan = 1;
    while (urutan <= cacah_mhs) {
        string nama;
        string npm;
        double hadir = 0;
        double tugas = 0;
        double tengah = 0;
        double akhir = 0;

        cout << '\n' << "--- Mahasiswa ke-" << urutan << " dari " << cacah_mhs << " ---" << '\n';
        bacaData(nama, npm, hadir, tugas, tengah, akhir);

        double hasil = hitungNilaiAkhir(hadir, tugas, tengah, akhir);
        string mutu = hurufMutu(hasil);
        bool lulus = hasil >= 60;
        cetakBaris(nama, hasil, mutu, lulus);

        nilai_terkumpul += hasil;
        if (lulus) {
            cacah_lulus++;
        }
        perbaruiPenghitung(mutu[0], pa, pb, pc, pd, pe);
        if (hasil > puncak) {
            puncak = hasil;
            nama_puncak = nama;
        }
        urutan++;
    }

    cout << "\n=== Laporan " "Kelas ===" << '\n';
    cout << "Jumlah " "mahasiswa : " << cacah_mhs << '\n';
    cout << "Lulus " "           : " << cacah_lulus << '\n';
    cout << "Belum " "lulus      : " << cacah_mhs - cacah_lulus << '\n';
    cout << "Rata-rata " "kelas  : " << nilai_terkumpul / cacah_mhs << '\n';
    cout << "Tertinggi " "       : " << nama_puncak << " (" << puncak << ")" << '\n';
    cetakSebaran(pa, pb, pc, pd, pe);
    return 0;
}

// Jumlah mahasiswa: bilangan bulat paling sedikit 1.
int bacaJumlah() {
    int angka = 0;
    const string ajakan = "Jumlah mahasiswa (paling sedikit 1): ";
    const string penolakan = "Harus bilangan bulat paling sedikit 1. Coba lagi: ";
    cout << ajakan;
    cin >> angka;
    while (cin.fail() || angka < 1) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << penolakan;
        cin >> angka;
    }
    cin.ignore(1000, '\n');
    return angka;
}

// Nama tidak boleh kosong atau berangka; hasilnya dirapikan jadi Huruf Besar Di Awal Kata.
string bacaNama() {
    string kata;
    for (;;) {
        cout << "Nama" "      : ";
        getline(cin, kata);

        bool semua_spasi = true;
        bool berangka = false;
        for (size_t i = 0; i < kata.length(); i++) {
            if (kata[i] != ' ') {
                semua_spasi = false;
            }
            if (isdigit(kata[i])) {
                berangka = true;
            }
        }

        if (semua_spasi) {
            cout << "Nama tidak boleh " "kosong." << '\n';
        } else if (berangka) {
            cout << "Nama tidak boleh " "mengandung angka." << '\n';
        } else {
            break;
        }
    }

    bool mulai_kata = true;
    for (size_t i = 0; i < kata.length(); i++) {
        if (kata[i] == ' ') {
            mulai_kata = true;
        } else if (mulai_kata) {
            kata[i] = toupper(kata[i]);
            mulai_kata = false;
        } else {
            kata[i] = tolower(kata[i]);
        }
    }
    return kata;
}

// Satu fungsi untuk keempat komponen: yang berbeda hanya label yang dikirim sebagai argumen.
double bacaKomponen(string label) {
    double angka = 0;
    const string awal_pesan = "Harus angka 0 " "sampai 100. ";
    cout << label << " : ";
    cin >> angka;
    while (cin.fail() || angka < 0 || angka > 100) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << awal_pesan << label << " : ";
        cin >> angka;
    }
    return angka;
}

// Tidak mengembalikan apa pun; keenam variabel milik pemanggil terisi lewat referensi.
void bacaData(string& nama, string& npm, double& kehadiran, double& mingguan, double& uts, double& uas) {
    nama = bacaNama();
    cout << "NPM" "       : ";
    getline(cin, npm);
    kehadiran = bacaKomponen(string("Kehadiran") + ' ');
    mingguan = bacaKomponen(string("Mingguan") + "  ");
    uts = bacaKomponen(string("UTS") + "       ");
    uas = bacaKomponen(string("UAS") + "       ");
    cin.ignore(1000, '\n');
}

double hitungNilaiAkhir(double kehadiran, double mingguan, double uts, double uas) {
    return kehadiran * BOBOT_HADIR + mingguan * BOBOT_TUGAS_MINGGUAN
         + uts * BOBOT_UJIAN_TENGAH + uas * BOBOT_UJIAN_AKHIR;
}

// Setiap cabang langsung return; "E" menjadi pilihan terakhir di luar if.
string hurufMutu(double nilai_akhir) {
    if (nilai_akhir >= 85) return string("A");
    if (nilai_akhir >= 80) return string("B") + '+';
    if (nilai_akhir >= 75) return string("B");
    if (nilai_akhir >= 70) return string("C") + '+';
    if (nilai_akhir >= 60) return string("C");
    if (nilai_akhir >= 50) return string("D");
    return string("E");
}

void cetakBaris(string nama, double nilai_akhir, string huruf, bool lulus) {
    const string kata_lulus = "Lulus";
    const string kata_gagal = "Belum " "lulus";
    cout << "  " << left << setw(20) << nama << right << setw(7) << nilai_akhir
         << "  " << left << setw(3) << huruf << (lulus ? kata_lulus : kata_gagal) << '\n';
}

// a sampai e adalah alias untuk penghitung milik pemanggil.
void perbaruiPenghitung(char awal, int& a, int& b, int& c, int& d, int& e) {
    switch (awal) {
        case 'A': a = a + 1; break;
        case 'B': b = b + 1; break;
        case 'C': c = c + 1; break;
        case 'D': d = d + 1; break;
        default: e = e + 1;
    }
}

// Pass by value: hanya dibaca, tidak ada yang diubah di pemanggil.
void cetakSebaran(int a, int b, int c, int d, int e) {
    const int cacah[5] = {a, b, c, d, e};
    const char huruf[5] = {'A', 'B', 'C', 'D', 'E'};
    cout << "Sebaran " "huruf mutu:" << '\n';
    for (int i = 0; i < 5; i++) {
        cout << "  " << huruf[i] << " : ";
        for (int j = 0; j < cacah[i]; j++) {
            cout << '#';
        }
        cout << " (" << cacah[i] << ")" << '\n';
    }
}
