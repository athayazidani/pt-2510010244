// SiNilai v0.6: v0.5 yang tangguh terhadap masukan salah. Dibangun di atas v0.5 (laporan kelas).
// Yang baru: angka dibaca dengan aman (cin.fail, clear, ignore), nama divalidasi dan dirapikan
// huruf per huruf. Perhatikan: pola pembacaan aman ditulis berulang untuk empat komponen;
// itu sengaja, supaya di Pertemuan 9 kamu merasakan sendiri gunanya fungsi.
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    // urutan: kehadiran, mingguan, uts, uas
    const double BOBOT[4] = {0.10, 0.45, 0.25, 0.20};
    const string LABEL[4] = {"Kehadiran", "Mingguan", "UTS", "UAS"};
    const string PROMPT[4] = {"Kehadiran : ", "Mingguan  : ", "UTS       : ", "UAS       : "};

    // tabel konversi nilai ke huruf mutu, diperiksa dari batas tertinggi
    const double BATAS_MUTU[6] = {80, 75, 70, 65, 60, 40};
    const string NAMA_MUTU[6] = {"A", "B+", "B", "C+", "C", "D"};
    const double BATAS_LULUS = 60;

    int total_peserta = 0;
    cout << "=== SiNilai v0.6 ===\n";
    cout << "Masukkan jumlah mahasiswa: ";
    cin >> total_peserta;
    while (cin.fail() || total_peserta < 1)
    {
        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Input tidak valid. Masukkan jumlah mahasiswa: ";
        cin >> total_peserta;
    }
    // TODO 1: do-while di atas berputar tanpa henti kalau pengguna mengetik huruf (cin gagal, isi jadi 0).
    //         Ganti dengan pola input_aman.cpp: baca sekali, lalu while (cin.fail() || jumlah_mahasiswa < 1)
    //         { cin.clear(); cin.ignore(1000, '\n'); minta lagi; }.

    cin.ignore(1000, '\n'); // buang Enter yang tersisa setelah angka, supaya getline nama tidak memungutnya

    cout << fixed << setprecision(2);

    double akumulasi = 0;
    int lolos = 0;
    int rekap[5] = {0}; // A, B, C, D, E
    double puncak = -1;
    string juara;

    for (int no = 1; no <= total_peserta; no++)
    {
        string nama;
        string npm;
        double nilai[4] = {0, 0, 0, 0};

        cout << "\n--- Mahasiswa ke-" << no << " dari " << total_peserta << " ---\n";

        cout << "Nama      : ";
        getline(cin, nama);
        // TODO 2: bungkus pembacaan nama dengan do-while seperti validasi_nama.cpp: ulangi selama nama
        //         kosong atau hanya spasi, atau mengandung angka. Periksa huruf per huruf dengan for.
        while (true)
        {
            int cacah_huruf = 0;
            int cacah_angka = 0;

            for (int p = 0; p < (int)nama.length(); p++)
            {
                if (isalpha((unsigned char)nama[p]))
                    cacah_huruf++;
                if (isdigit((unsigned char)nama[p]))
                    cacah_angka++;
            }

            if (cacah_huruf > 0 && cacah_angka == 0)
                break;

            cout << "Nama tidak valid. Masukkan nama lagi: ";
            getline(cin, nama);
        }
        // TODO 3: rapikan nama seperti rapikan_nama.cpp: huruf pertama tiap kata besar, sisanya kecil.
        for (int p = 0; p < (int)nama.length(); p++)
        {
            bool awal = (p == 0) || (nama[p - 1] == ' ');
            if (awal)
                nama[p] = (char)toupper((unsigned char)nama[p]);
            else
                nama[p] = (char)tolower((unsigned char)nama[p]);
        }

        cout << "NPM       : ";
        getline(cin, npm);

        // TODO 4: tambahkan while (cin.fail() || kehadiran < 0 || kehadiran > 100) { clear; ignore; minta lagi; }
        for (int k = 0; k < 4; k++)
        {
            cout << PROMPT[k];
            cin >> nilai[k];
            while (cin.fail() || nilai[k] < 0 || nilai[k] > 100)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << LABEL[k] << " tidak valid. Masukkan lagi: ";
                cin >> nilai[k];
            }
        }
        // TODO 5: ulangi pola TODO 4 untuk mingguan, uts, dan uas (ya, empat kali; catat rasanya).

        cin.ignore(1000, '\n'); // buang Enter setelah uas, supaya getline nama mahasiswa berikutnya bersih

        double hasil = 0;
        for (int k = 0; k < 4; k++)
        {
            hasil += nilai[k] * BOBOT[k];
        }

        string mutu = "E";
        for (int t = 0; t < 6; t++)
        {
            if (hasil >= BATAS_MUTU[t])
            {
                mutu = NAMA_MUTU[t];
                break;
            }
        }

        bool tuntas = hasil >= BATAS_LULUS;

        cout << "  " << left << setw(20) << nama << right << setw(7) << hasil
             << "  " << left << setw(3) << mutu;
        cout << (tuntas ? "Lulus" : "Belum lulus") << "\n";

        akumulasi += hasil;
        lolos += tuntas ? 1 : 0;
        rekap[mutu[0] - 'A']++;

        if (hasil > puncak)
        {
            puncak = hasil;
            juara = nama;
        }
    }

    cout << "\n=== Laporan Kelas ===\n";
    cout << "Jumlah mahasiswa : " << total_peserta << "\n";
    cout << "Lulus            : " << lolos << "\n";
    cout << "Belum lulus      : " << total_peserta - lolos << "\n";
    cout << "Rata-rata kelas  : " << akumulasi / total_peserta << "\n";
    cout << "Tertinggi        : " << juara << " (" << puncak << ")\n";

    cout << "\nSebaran huruf mutu:\n";
    for (int i = 0; i < 5; i++)
    {
        cout << (char)('A' + i) << " | " << string(rekap[i], '#') << " " << rekap[i] << "\n";
    }
    return 0;
}