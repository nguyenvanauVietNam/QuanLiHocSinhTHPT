#pragma once
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
#include <iostream> // Thu vi?n cung c?p c�c d?i tu?ng nh?p/xu?t nhu cin, cout
#include <iomanip> // Add this include at the top of your file for std::setw, std::setprecision, std::fixed
#include <string>   // Thu vi?n cho ki?u d? li?u string
#include <vector>   // Thu vi?n cho ki?u d? li?u vector
#include <algorithm>
#include <fstream>   // Xu?t file b�o c�o
#include <cctype>    // isdigit, tolower
#include <limits>    // cin error handling
#include <clocale>
#include <locale>
#include <cstdio>
#include <sstream>
#include <ctime>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
// Ensure this include is at the top of your file, before any usage of tinyxml2
#include "third_party/tinyxml2/tinyxml2.h"

using std::string;
using std::vector;
// Add this line after the include to bring the tinyxml2 namespace into scope
using namespace tinyxml2;

#pragma region Defines

// Struct luu th�ng tin di?m s?
struct DiemSo {
    string loaiDiem;       // Lo?i di?m: H?nh ki?m, h?c l?c, ki?m tra,...
    float giaTri = 0;          // �i?m s? c? th?
};

// Struct luu di?m th�nh ph?n c?a 1 m�n trong 1 h?c k?
// Bao g?m: 15 ph�t, d?u gi? (HS 1), 1 ti?t (HS 2), gi?a k? (HS 2), cu?i k? (HS 3)
struct DiemMonHocHK {
    float diem15p    = 0.0f;  // Ki?m tra 15 ph�t     (h? s? 1)
    float diemDauGio = 0.0f;  // Ki?m tra d?u gi?    (h? s? 1)
    float diem1Tiet  = 0.0f;  // Ki?m tra 1 ti?t     (h? s? 2)
    float diemGiuaKy = 0.0f;  // �i?m gi?a k?        (h? s? 2)
    float diemCuoiKy = 0.0f;  // �i?m cu?i k?        (h? s? 3)

    // TB h?c k? = S(di?m � h? s?) / S(h? s?), ch? t�nh di?m > 0
    float tinhTB() const {
        float tong = 0.0f, heso = 0.0f;
        if (diem15p    > 0) { tong += diem15p    * 1; heso += 1; }
        if (diemDauGio > 0) { tong += diemDauGio * 1; heso += 1; }
        if (diem1Tiet  > 0) { tong += diem1Tiet  * 2; heso += 2; }
        if (diemGiuaKy > 0) { tong += diemGiuaKy * 2; heso += 2; }
        if (diemCuoiKy > 0) { tong += diemCuoiKy * 3; heso += 3; }
        return (heso > 0) ? tong / heso : 0.0f;
    }

    bool coNhapDiem() const {
        return (diem15p > 0 || diemDauGio > 0 || diem1Tiet > 0 || diemGiuaKy > 0 || diemCuoiKy > 0);
    }
};

// Struct luu th�ng tin m�n h?c (di?m t�ch theo HK1 / HK2)
struct MonHoc {
    string tenMonHoc;
    DiemMonHocHK hk1;   // �i?m h?c k? 1 (15p, d?u gi?, 1 ti?t, GK1, CK1)
    DiemMonHocHK hk2;   // �i?m h?c k? 2 (15p, d?u gi?, 1 ti?t, GK2, CK2)

    float tinhTBHK1()    const { return hk1.tinhTB(); }
    float tinhTBHK2()    const { return hk2.tinhTB(); }
    // TB c? nam = (TB_HK1 + TB_HK2 � 2) / 3 (Th�ng tu 22/2021)
    float tinhTBCaNam() const {
        float t1 = hk1.tinhTB(), t2 = hk2.tinhTB();
        if (t1 == 0 && t2 == 0) return 0.0f;
        if (t1 == 0) return t2;
        if (t2 == 0) return t1;
        return (t1 + t2 * 2.0f) / 3.0f;
    }
};

// Struct luu th�ng tin ngu?i co b?n
struct Nguoi {
    string IDMaDinhDanh;                   // ID nguoi
	string ho;                   // H?
	string tenDem;               // T�n d?m
	string ten;                  // T�n
	string ngaySinh;             // Ng�y/th�ng/nam sinh
    string gioiTinh;             // Gi?i t�nh
};
// Struct luu th�ng tin h?c sinh
struct HocSinh {
	Nguoi thongTin;          // Th�ng tin c� nh�n
    string thongTinPhuHuynh;     // Th�ng tin ph? huynh
    double hocPhi = 0.0;         // H?c ph�
    vector<MonHoc> danhSachMon;  // C�c m�n h?c & di?m (HK1 + HK2)
    float hanhKiemHK1 = 0.0f;   // �i?m h?nh ki?m h?c k? 1 (0�10)
    float hanhKiemHK2 = 0.0f;   // �i?m h?nh ki?m h?c k? 2 (0�10)
    float diemTBHK1   = 0.0f;   // �i?m trung b�nh h?c k? 1
    float diemTBHK2   = 0.0f;   // �i?m trung b�nh h?c k? 2
    float diemTBNam   = 0.0f;   // �i?m trung b�nh c? nam (x?p lo?i, x?p h?ng)
    string khoiHoc;              // Ban h?c: co b?n, t? nhi�n, x� h?i
};

// Struct luu th�ng tin gi�o vi�n
struct GiaoVien {
	Nguoi thongTin;          // Th�ng tin c� nh�n
    string monGiangDay;          // B? m�n gi?ng d?y
};

// Struct luu th�ng tin l?p h?c
struct LopHoc {
    string tenLop;               // T�n l?p (VD: 10A1, 11B2,...)
    vector<HocSinh> danhSachHS;  // Danh s�ch h?c sinh
    GiaoVien giaoVienCN;         // Gi�o vi�n ch? nhi?m
	//Si s? l?p h?c
    int siSo() const { 
        return (int)danhSachHS.size(); // Tr? v? s? lu?ng h?c sinh trong l?p
	}
	// T�n gi�o vi�n ch? nhi?m
    string tenGiaoVienChuNhiem() const { 
        return giaoVienCN.thongTin.ho + " " + giaoVienCN.thongTin.tenDem + " " + giaoVienCN.thongTin.ten; 
	}
};

inline string xepLoaiHocLuc(float diemTB);
inline string xepLoaiHanhKiem(float diemHK);

inline void cauHinhConsoleTiengViet() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "");
#endif
    const char* locales[] = { ".UTF-8", "vi_VN.UTF-8", "en_US.UTF-8", "" };
    for (const char* localeName : locales) {
        if (std::setlocale(LC_ALL, localeName) != nullptr) break;
    }

    try {
        std::locale nativeLocale("");
        std::locale::global(nativeLocale);
        std::cin.imbue(nativeLocale);
        std::cout.imbue(nativeLocale);
        std::cerr.imbue(nativeLocale);
    }
    catch (...) {
        // Code page UTF-8 ? tr�n v?n d? d? Windows Terminal hi?n th? ti?ng Vi?t.
    }
}

inline string hoTenNguoi(const Nguoi& nguoi) {
    string hoTen = nguoi.ho;
    if (!nguoi.tenDem.empty()) hoTen += " " + nguoi.tenDem;
    if (!nguoi.ten.empty()) hoTen += " " + nguoi.ten;
    return hoTen;
}

inline string hoTenHocSinh(const HocSinh& hs) {
    return hoTenNguoi(hs.thongTin);
}

inline float tinhHanhKiemNam(const HocSinh& hs) {
    if (hs.hanhKiemHK1 > 0.0f && hs.hanhKiemHK2 > 0.0f)
        return (hs.hanhKiemHK1 + hs.hanhKiemHK2) / 2.0f;
    return (hs.hanhKiemHK1 > 0.0f) ? hs.hanhKiemHK1 : hs.hanhKiemHK2;
}

inline void inThongTinGiaoVien(const GiaoVien& gv) {
    std::cout << "GVCN: " << hoTenNguoi(gv.thongTin) << "\n";
    std::cout << "Ngay sinh: " << gv.thongTin.ngaySinh
              << "  Gioi tinh: " << gv.thongTin.gioiTinh << "\n";
    std::cout << "Mon giang day: " << gv.monGiangDay << "\n";
}

constexpr double HOC_PHI_CHUAN_MOT_HS = 1200000.0;

inline double tinhHocPhiConNo(const HocSinh& hs, double hocPhiChuan = HOC_PHI_CHUAN_MOT_HS) {
    double conNo = hocPhiChuan - hs.hocPhi;
    return (conNo > 0.0) ? conNo : 0.0;
}

inline bool daDongHocPhiDayDu(const HocSinh& hs, double hocPhiChuan = HOC_PHI_CHUAN_MOT_HS) {
    return hs.hocPhi >= hocPhiChuan;
}

inline string formatTien(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << value;
    return oss.str();
}

inline void inThongTinLopChiTiet(const LopHoc& lop) {
    std::cout << "--- Thong tin lop hien tai ---\n";
    std::cout << "Ten lop      : " << lop.tenLop << "\n";
    inThongTinGiaoVien(lop.giaoVienCN);
    std::cout << "Si so        : " << lop.siSo() << "\n";
    double tongThu = 0.0;
    double tongNo = 0.0;
    int hoanThanh = 0;
    int coNo = 0;
    int chuaDong = 0;
    for (const auto& hs : lop.danhSachHS) {
        double no = tinhHocPhiConNo(hs);
        tongThu += hs.hocPhi;
        tongNo += no;
        if (daDongHocPhiDayDu(hs)) hoanThanh++;
        else if (hs.hocPhi > 0.0) coNo++;
        else chuaDong++;
    }
    std::cout << "Tong thu     : " << std::fixed << std::setprecision(0) << tongThu << " VND\n";
    std::cout << "Tong no      : " << std::fixed << std::setprecision(0) << tongNo << " VND\n";
    std::cout << "Hoan thanh   : " << hoanThanh << " HS\n";
    std::cout << "Con no       : " << coNo << " HS\n";
    std::cout << "Chua dong    : " << chuaDong << " HS\n";
}

inline void inThongTinHocSinhFull(const HocSinh& hs) {
    std::cout << "--- Thong tin hoc sinh hien tai ---\n";
    std::cout << "ID            : " << hs.thongTin.IDMaDinhDanh << "\n";
    std::cout << "Ho va ten     : " << hoTenHocSinh(hs) << "\n";
    std::cout << "Ngay sinh     : " << hs.thongTin.ngaySinh << "\n";
    std::cout << "Gioi tinh     : " << hs.thongTin.gioiTinh << "\n";
    std::cout << "Khoi hoc      : " << hs.khoiHoc << "\n";
    std::cout << "Phu huynh     : " << hs.thongTinPhuHuynh << "\n";
    std::cout << "Hoc phi       : " << std::fixed << std::setprecision(0) << hs.hocPhi << " VND\n";
    std::cout << "Con no        : " << std::fixed << std::setprecision(0) << tinhHocPhiConNo(hs) << " VND\n";
    std::cout << "TB HK1        : " << std::fixed << std::setprecision(2) << hs.diemTBHK1
              << "  TB HK2: " << hs.diemTBHK2
              << "  TB Nam: " << hs.diemTBNam << "\n";
    std::cout << "Xep loai      : " << xepLoaiHocLuc(hs.diemTBNam) << "\n";
    std::cout << "Hanh kiem HK1 : " << hs.hanhKiemHK1
              << "  HK2: " << hs.hanhKiemHK2 << "\n";
}

inline void inDanhSachLopVaQuy(const vector<LopHoc>& dsLop) {
    std::cout << "--- Danh sach lop va bao cao quy hoc phi ---\n";
    std::cout << std::left << std::setw(5) << "STT"
              << std::setw(10) << "Ten lop"
              << std::setw(28) << "GVCN"
              << std::setw(6)  << "Si so"
              << std::setw(15) << "Tong thu"
              << std::setw(15) << "Tong no"
              << "Trang thai\n";
    std::cout << std::string(90, '-') << "\n";
    for (int i = 0; i < (int)dsLop.size(); ++i) {
        const auto& lop = dsLop[i];
        double tongThu = 0.0;
        double tongNo = 0.0;
        int hoanThanh = 0;
        int conNo = 0;
        int chuaDong = 0;
        for (const auto& hs : lop.danhSachHS) {
            tongThu += hs.hocPhi;
            double no = tinhHocPhiConNo(hs);
            tongNo += no;
            if (daDongHocPhiDayDu(hs)) hoanThanh++;
            else if (hs.hocPhi > 0.0) conNo++;
            else chuaDong++;
        }
        string status = (hoanThanh == lop.siSo() && lop.siSo() > 0) ? "Hoan thanh"
                        : (conNo > 0 || chuaDong > 0) ? "Con no" : "Chua sinh";
        std::cout << std::left << std::setw(5) << (i + 1)
                  << std::setw(10) << lop.tenLop
                  << std::setw(28) << lop.tenGiaoVienChuNhiem()
                  << std::setw(6)  << lop.siSo()
                  << std::setw(15) << std::fixed << std::setprecision(0) << tongThu
                  << std::setw(15) << std::fixed << std::setprecision(0) << tongNo
                  << status << "\n";
    }
    std::cout << std::string(90, '-') << "\n";
}

inline double tinhTyLe(int soLuong, int tong) {
    return (tong > 0) ? (double)soLuong * 100.0 / (double)tong : 0.0;
}

#pragma endregion

#pragma region Khoi tao
//-- T?o d? li?u m?u cho c�c m�n h?c --
//Khai b�o danh s�ch c�c m�n h?c c?a THPT ? Vi?t Nam

// Danh s�ch c�c m�n h?c THPT ? Vi?t Nam (UTF-8, c� d?u)
const vector<string> DANH_SACH_TEN_MON_HOC = {
    "Ng? van",
    "To�n",
    "V?t l�",
    "H�a h?c",
    "Sinh h?c",
    "L?ch s?",
    "�?a l�",
    "Ngo?i ng?",
    "Gi�o d?c c�ng d�n",
    "Tin h?c",
    "C�ng ngh?",
    "Gi�o d?c qu?c ph�ng - an ninh",
    "Th? d?c"
};

// H�m kh?i t?o danh s�ch c�c m�n h?c THPT v?i di?m s? m?c d?nh
// Tr? v? vector ch?a c�c struct MonHoc v?i t�n m�n h?c d� khai b�o v� di?m s? ban d?u l� r?ng
inline vector<MonHoc> khoiTaoDanhSachMonHoc() {
    vector<MonHoc> ds;
    for (const auto& tenMon : DANH_SACH_TEN_MON_HOC) {
        MonHoc mon;
        mon.tenMonHoc = tenMon;
        // hk1, hk2 kh?i t?o = 0.0f m?c d?nh
        ds.push_back(mon);
    }
    return ds;
}

// H�m kh?i t?o m?t l?p v?i 5 h?c sinh v� 1 gi�o vi�n ch? nhi?m
inline LopHoc khoiTaoLopMau() {
    LopHoc lop;
    lop.tenLop = "10A1";

    // Gi�o vi�n ch? nhi?m m?u
    lop.giaoVienCN.thongTin = {"GV1", "Nguy?n", "Van", "An", "01/01/1980", "Nam"};
    lop.giaoVienCN.monGiangDay = "To�n";

    // 5 h?c sinh m?u
    for (int i = 0; i < 5; ++i) {
        HocSinh hs;
        hs.thongTin.IDMaDinhDanh = "HS" + std::to_string(i + 1);
        hs.thongTin.ho = "L�";
        hs.thongTin.tenDem = "Th?";
        hs.thongTin.ten = "H?cSinh" + std::to_string(i + 1);
        hs.thongTin.ngaySinh = "01/09/2006";
        hs.thongTin.gioiTinh = (i % 2 == 0) ? "Nam" : "N?";
        hs.thongTinPhuHuynh = "Ph? huynh " + std::to_string(i + 1);
        hs.hocPhi = 1200000.0 + i * 100000;
        hs.danhSachMon = khoiTaoDanhSachMonHoc();
        // G�n di?m m?u cho m?t s? m�n (To�n, Ng? van)
        for (auto& mon : hs.danhSachMon) {
            if (mon.tenMonHoc == "To�n" || mon.tenMonHoc == "Ng? van") {
                mon.hk1 = {7.5f + i*0.2f, 7.0f, 8.0f + i*0.1f, 7.8f + i*0.1f, 7.5f + i*0.2f};
                mon.hk2 = {8.0f + i*0.1f, 7.5f, 8.5f + i*0.1f, 8.2f + i*0.1f, 8.0f + i*0.1f};
            }
        }
        hs.hanhKiemHK1 = 8.0f + i * 0.2f;
        hs.hanhKiemHK2 = 8.2f + i * 0.1f;
        hs.diemTBHK1   = 7.5f + i * 0.3f;
        hs.diemTBHK2   = 7.8f + i * 0.2f;
        hs.diemTBNam   = (hs.diemTBHK1 + hs.diemTBHK2 * 2.0f) / 3.0f;
        hs.khoiHoc = "Co b?n";
        lop.danhSachHS.push_back(hs);
    }

    return lop;
}
#pragma endregion

#pragma region XyLyQuanLyHocSinh
// H�m in danh s�ch h?c sinh c?a m?t l?p (c?t t�m t?t TB HK1/HK2/Nam, H?nh ki?m)
static void inDanhSachHocSinh(const LopHoc& lop) {
    string sep(90, '-');
    std::cout << "Lop: " << lop.tenLop << "  |  GVCN: " << lop.tenGiaoVienChuNhiem()
              << "  |  Si so: " << lop.siSo() << "\n" << sep << "\n";
    std::cout << "| " << std::setw(3)  << "STT"
              << " | " << std::setw(24) << "Ho va ten"
              << " | " << std::setw(6)  << "TB HK1"
              << " | " << std::setw(6)  << "TB HK2"
              << " | " << std::setw(6)  << "TB Nam"
              << " | " << std::setw(7)  << "HK Nam"
              << " | " << std::setw(10) << "Xep loai"
              << " |\n" << sep << "\n";
    for (int i = 0; i < (int)lop.danhSachHS.size(); ++i) {
        const HocSinh& hs = lop.danhSachHS[i];
        string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        float hkNam = (hs.hanhKiemHK1 + hs.hanhKiemHK2 > 0)
            ? (hs.hanhKiemHK1 + hs.hanhKiemHK2) / 2.0f : hs.hanhKiemHK1;
        std::cout << "| " << std::setw(3)  << (i + 1)
                  << " | " << std::setw(24) << hoTen
                  << " | " << std::setw(6)  << std::fixed << std::setprecision(1) << hs.diemTBHK1
                  << " | " << std::setw(6)  << hs.diemTBHK2
                  << " | " << std::setw(6)  << hs.diemTBNam
                  << " | " << std::setw(7)  << hkNam
                  << " | " << std::setw(10) << xepLoaiHocLuc(hs.diemTBNam)
                  << " |\n";
    }
    std::cout << sep << "\n";
}

// H�m in danh s�ch h?c sinh c?a nhi?u l?p (t�i s? d?ng h�m don l?p)
static void inDanhSachHocSinh(const vector<LopHoc>& dsLop) {
    for (const auto& lop : dsLop) inDanhSachHocSinh(lop);
}


// H�m th�m h?c sinh v�o cu?i danh s�ch c?a l?p
inline void themHocSinhVaoLop(LopHoc& lop, const HocSinh& hocSinh) {
    lop.danhSachHS.push_back(hocSinh);
}


// H�m th�m l?p v�o danh s�ch l?p
inline void themLopVaoDanhSach(vector<LopHoc>& dsLop, const LopHoc& lopMoi) {
    dsLop.push_back(lopMoi);
}


// H�m s?a th�ng tin h?c sinh theo ID d?nh danh
inline bool suaThongTinHocSinh(LopHoc& lop, const HocSinh& hocSinhMoi, const string& idMaDinhDanh) {
    for (auto& hs : lop.danhSachHS) {
        if (hs.thongTin.IDMaDinhDanh == idMaDinhDanh) {
            hs = hocSinhMoi;
            return true; // S?a th�nh c�ng
        }
    }
    return false; // Kh�ng t�m th?y h?c sinh
}


// H�m s?a th�ng tin l?p h?c theo t�n l?p trong danh s�ch c�c l?p
inline bool suaThongTinLop(vector<LopHoc>& dsLop, const LopHoc& lopMoi, const string& tenLop) {
    for (auto& lop : dsLop) {
        if (lop.tenLop == tenLop) {
            lop = lopMoi;
            return true; // S?a th�nh c�ng
        }
    }
    return false; // Kh�ng t�m th?y l?p
}


// H�m x�a h?c sinh kh?i l?p theo ID d?nh danh
inline bool xoaHocSinhTheoID(LopHoc& lop, const string& idMaDinhDanh) {
    for (auto it = lop.danhSachHS.begin(); it != lop.danhSachHS.end(); ++it) {
        if (it->thongTin.IDMaDinhDanh == idMaDinhDanh) {
            lop.danhSachHS.erase(it);
            return true; // X�a th�nh c�ng
        }
    }
    return false; // Kh�ng t�m th?y h?c sinh
}


// H�m x�a l?p kh?i danh s�ch l?p theo t�n l?p
inline bool xoaLopTheoTen(vector<LopHoc>& dsLop, const string& tenLop) {
    for (auto it = dsLop.begin(); it != dsLop.end(); ++it) {
        if (it->tenLop == tenLop) {
            dsLop.erase(it);
            return true; // X�a th�nh c�ng
        }
    }
    return false; // Kh�ng t�m th?y l?p
}


// H�m s?p x?p danh s�ch l?p theo t�n l?p (tang d?n)
inline void sapXepDanhSachLopTheoTen(vector<LopHoc>& dsLop) {
    std::sort(dsLop.begin(), dsLop.end(), [](const LopHoc& a, const LopHoc& b) {
        return a.tenLop < b.tenLop;
    });
}


// H�m s?p x?p danh s�ch h?c sinh trong l?p theo t�n (tang d?n)
inline void sapXepDanhSachHocSinhTheoTen(LopHoc& lop) {
    std::sort(lop.danhSachHS.begin(), lop.danhSachHS.end(), [](const HocSinh& a, const HocSinh& b) {
        std::string tenA = a.thongTin.tenDem + " " + a.thongTin.ten;
        std::string tenB = b.thongTin.tenDem + " " + b.thongTin.ten;
        return tenA < tenB;
    });
}

// TODO 8 (Fix): Helper d?c text an to�n t? XML � tr�nh crash khi element b? null
static inline string safeGetText(XMLElement* parent, const char* childTag, const string& def = "") {
    if (!parent) return def;
    XMLElement* child = parent->FirstChildElement(childTag);
    if (!child || !child->GetText()) return def;
    return child->GetText();
}

static inline float safeToFloat(const string& value, float def = 0.0f) {
    try {
        return value.empty() ? def : std::stof(value);
    }
    catch (...) {
        return def;
    }
}

static inline double safeToDouble(const string& value, double def = 0.0) {
    try {
        return value.empty() ? def : std::stod(value);
    }
    catch (...) {
        return def;
    }
}

static inline bool fileTonTai(const string& fileName) {
    std::ifstream ifs(fileName);
    return ifs.is_open();
}

static inline string timDuongDanXML(const string& fileName) {
    vector<string> candidates = {
        fileName,
        "..\\" + fileName,
        "x64\\Release\\" + fileName,
        "..\\..\\" + fileName
    };
    for (const auto& path : candidates) {
        if (fileTonTai(path)) return path;
    }
    return fileName;
}

// H�m d?c file THPT.xml v� tr? v? danh s�ch c�c l?p (d� fix null-check)
inline vector<LopHoc> docDanhSachLopTuXML(const string& fileName) {
    vector<LopHoc> dsLop;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Khong the mo file XML: " << fileName << std::endl;
        return dsLop;
    }
    XMLElement* root = doc.FirstChildElement("DanhSachLop");
    if (!root) return dsLop;

    for (XMLElement* lopElem = root->FirstChildElement("LopHoc"); lopElem; lopElem = lopElem->NextSiblingElement("LopHoc")) {
        LopHoc lop;
        lop.tenLop = safeGetText(lopElem, "TenLop");

        XMLElement* gvElem = lopElem->FirstChildElement("GiaoVienCN");
        if (gvElem) {
            lop.giaoVienCN.thongTin.ho       = safeGetText(gvElem, "Ho");
            lop.giaoVienCN.thongTin.tenDem   = safeGetText(gvElem, "TenDem");
            lop.giaoVienCN.thongTin.ten      = safeGetText(gvElem, "Ten");
            lop.giaoVienCN.thongTin.ngaySinh = safeGetText(gvElem, "NgaySinh");
            lop.giaoVienCN.thongTin.gioiTinh = safeGetText(gvElem, "GioiTinh");
            lop.giaoVienCN.monGiangDay       = safeGetText(gvElem, "MonGiangDay");
        }

        XMLElement* dsHSElem = lopElem->FirstChildElement("DanhSachHS");
        if (dsHSElem) {
            for (XMLElement* hsElem = dsHSElem->FirstChildElement("HocSinh"); hsElem; hsElem = hsElem->NextSiblingElement("HocSinh")) {
                HocSinh hs;
                XMLElement* ttElem = hsElem->FirstChildElement("ThongTin");
                if (ttElem) {
                    hs.thongTin.IDMaDinhDanh = safeGetText(ttElem, "IDMaDinhDanh");
                    hs.thongTin.ho           = safeGetText(ttElem, "Ho");
                    hs.thongTin.tenDem       = safeGetText(ttElem, "TenDem");
                    hs.thongTin.ten          = safeGetText(ttElem, "Ten");
                    hs.thongTin.ngaySinh     = safeGetText(ttElem, "NgaySinh");
                    hs.thongTin.gioiTinh     = safeGetText(ttElem, "GioiTinh");
                }
                hs.thongTinPhuHuynh = safeGetText(hsElem, "ThongTinPhuHuynh");
                string hocPhiStr    = safeGetText(hsElem, "HocPhi", "0");
                hs.hocPhi = safeToDouble(hocPhiStr);
                hs.khoiHoc = safeGetText(hsElem, "KhoiHoc");

                XMLElement* hkElem = hsElem->FirstChildElement("HanhKiem");
                if (hkElem) {
                    string v1 = safeGetText(hkElem, "HK1", safeGetText(hkElem, "GiaTri", "0"));
                    string v2 = safeGetText(hkElem, "HK2", v1);
                    hs.hanhKiemHK1 = safeToFloat(v1);
                    hs.hanhKiemHK2 = safeToFloat(v2);
                }
                XMLElement* dtbElem = hsElem->FirstChildElement("DiemTrungBinh");
                if (dtbElem) {
                    string hk1s = safeGetText(dtbElem, "HK1", safeGetText(dtbElem, "GiaTri", "0"));
                    string hk2s = safeGetText(dtbElem, "HK2", hk1s);
                    string nams = safeGetText(dtbElem, "Nam", hk1s);
                    hs.diemTBHK1 = safeToFloat(hk1s);
                    hs.diemTBHK2 = safeToFloat(hk2s);
                    hs.diemTBNam = safeToFloat(nams);
                }
                XMLElement* dsMonElem = hsElem->FirstChildElement("DanhSachMon");
                if (dsMonElem) {
                    for (XMLElement* monElem = dsMonElem->FirstChildElement("MonHoc"); monElem; monElem = monElem->NextSiblingElement("MonHoc")) {
                        MonHoc mon;
                        mon.tenMonHoc = safeGetText(monElem, "TenMonHoc");
                        auto readHK = [&](XMLElement* p, DiemMonHocHK& hk) {
                            if (!p) return;
                            auto gf = [&](const char* tag) -> float {
                                string s = safeGetText(p, tag, "0");
                                return safeToFloat(s);
                            };
                            hk.diem15p    = gf("Diem15p");
                            hk.diemDauGio = gf("DiemDauGio");
                            hk.diem1Tiet  = gf("Diem1Tiet");
                            hk.diemGiuaKy = gf("DiemGiuaKy");
                            hk.diemCuoiKy = gf("DiemCuoiKy");
                        };
                        readHK(monElem->FirstChildElement("HK1"), mon.hk1);
                        readHK(monElem->FirstChildElement("HK2"), mon.hk2);
                        XMLElement* diemSoElem = monElem->FirstChildElement("DiemSo");
                        if (diemSoElem && !mon.hk1.coNhapDiem() && !mon.hk2.coNhapDiem()) {
                            float diem = safeToFloat(safeGetText(diemSoElem, "GiaTri", "0"));
                            mon.hk1.diemCuoiKy = diem;
                            mon.hk2.diemCuoiKy = diem;
                        }
                        hs.danhSachMon.push_back(mon);
                    }
                }
                lop.danhSachHS.push_back(hs);
            }
        }
        dsLop.push_back(lop);
    }
    return dsLop;
}
#pragma endregion

// ============================================================
#pragma region TinhToan

// TODO 1: T�nh di?m TB c? nam t? danh s�ch m�n h?c
inline float tinhDiemTrungBinh(const vector<MonHoc>& danhSachMon) {
    if (danhSachMon.empty()) return 0.0f;
    float tong = 0.0f;
    int count = 0;
    for (const auto& mon : danhSachMon) {
        float tb = mon.tinhTBCaNam();
        if (tb > 0.0f) { tong += tb; count++; }
    }
    return (count > 0) ? tong / count : 0.0f;
}

// TODO 2: C?p nh?t di?m TB cho h?c sinh (t�nh l?i t? d?ng t? danh s�ch m�n)
inline void capNhatDiemTrungBinh(HocSinh& hs) {
    float tongHK1 = 0, cntHK1 = 0;
    float tongHK2 = 0, cntHK2 = 0;
    for (const auto& mon : hs.danhSachMon) {
        float t1 = mon.hk1.tinhTB(), t2 = mon.hk2.tinhTB();
        if (t1 > 0) { tongHK1 += t1; cntHK1++; }
        if (t2 > 0) { tongHK2 += t2; cntHK2++; }
    }
    hs.diemTBHK1 = (cntHK1 > 0) ? tongHK1 / cntHK1 : 0.0f;
    hs.diemTBHK2 = (cntHK2 > 0) ? tongHK2 / cntHK2 : 0.0f;
    if      (hs.diemTBHK1 == 0 && hs.diemTBHK2 == 0) hs.diemTBNam = 0.0f;
    else if (hs.diemTBHK1 == 0) hs.diemTBNam = hs.diemTBHK2;
    else if (hs.diemTBHK2 == 0) hs.diemTBNam = hs.diemTBHK1;
    else hs.diemTBNam = (hs.diemTBHK1 + hs.diemTBHK2 * 2.0f) / 3.0f;
}

// TODO 3: X?p lo?i h?c l?c theo thang di?m 10
inline string xepLoaiHocLuc(float diemTB) {
    if (diemTB >= 8.0f) return "Gi?i";
    if (diemTB >= 6.5f) return "Kh�";
    if (diemTB >= 5.0f) return "Trung b�nh";
    if (diemTB >= 3.5f) return "Y?u";
    return "K�m";
}

// TODO 4: X?p lo?i h?nh ki?m theo thang di?m 10
inline string xepLoaiHanhKiem(float diemHK) {
    if (diemHK >= 9.0f) return "T?t";
    if (diemHK >= 7.0f) return "Kh�";
    if (diemHK >= 5.0f) return "Trung b�nh";
    return "Y?u";
}

#pragma endregion

// ============================================================
#pragma region TimKiem

// TODO 5: T�m h?c sinh theo ID trong l?p � tr? v? pointer (nullptr n?u kh�ng th?y)
inline HocSinh* timHocSinhTheoID(LopHoc& lop, const string& id) {
    for (auto& hs : lop.danhSachHS)
        if (hs.thongTin.IDMaDinhDanh == id) return &hs;
    return nullptr;
}

// TODO 6: T�m h?c sinh theo t? kh�a t�n (kh�ng ph�n bi?t hoa/thu?ng)
inline vector<HocSinh*> timHocSinhTheoTen(LopHoc& lop, const string& tuKhoa) {
    vector<HocSinh*> ketQua;
    string tuKhoaLower = tuKhoa;
    std::transform(tuKhoaLower.begin(), tuKhoaLower.end(), tuKhoaLower.begin(),
                   [](unsigned char c){ return (char)::tolower(c); });
    for (auto& hs : lop.danhSachHS) {
        string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        std::transform(hoTen.begin(), hoTen.end(), hoTen.begin(),
                       [](unsigned char c){ return (char)::tolower(c); });
        if (hoTen.find(tuKhoaLower) != string::npos)
            ketQua.push_back(&hs);
    }
    return ketQua;
}

// TODO 7: L?c h?c sinh theo kh?i/ban h?c trong l?p (kh�ng ph�n bi?t hoa/thu?ng)
inline vector<HocSinh*> locHocSinhTheoKhoi(LopHoc& lop, const string& khoiHoc) {
    vector<HocSinh*> ketQua;
    string khoiLower = khoiHoc;
    std::transform(khoiLower.begin(), khoiLower.end(), khoiLower.begin(),
                   [](unsigned char c){ return (char)::tolower(c); });
    for (auto& hs : lop.danhSachHS) {
        string hsKhoi = hs.khoiHoc;
        std::transform(hsKhoi.begin(), hsKhoi.end(), hsKhoi.begin(),
                       [](unsigned char c){ return (char)::tolower(c); });
        if (hsKhoi == khoiLower) ketQua.push_back(&hs);
    }
    return ketQua;
}

// TODO 8: T�m l?p theo t�n trong danh s�ch nhi?u l?p
inline LopHoc* timLopTheoTen(vector<LopHoc>& dsLop, const string& tenLop) {
    for (auto& lop : dsLop)
        if (lop.tenLop == tenLop) return &lop;
    return nullptr;
}

#pragma endregion

// ============================================================
#pragma region ThongKe

// Struct ch?a k?t qu? th?ng k� h?c l?c to�n l?p
struct ThongKeHocLuc {
    int soGioi      = 0;
    int soKha       = 0;
    int soTrungBinh = 0;
    int soYeu       = 0;
    int soKem       = 0;
    float diemTBLop = 0.0f;
};

// TODO 9: Th?ng k� h?c l?c to�n l?p
inline ThongKeHocLuc thongKeHocLucLop(const LopHoc& lop) {
    ThongKeHocLuc tk;
    if (lop.danhSachHS.empty()) return tk;
    float tongDiem = 0.0f;
    for (const auto& hs : lop.danhSachHS) {
        string xl = xepLoaiHocLuc(hs.diemTBNam);
        if      (xl == "Gi?i")       tk.soGioi++;
        else if (xl == "Kh�")        tk.soKha++;
        else if (xl == "Trung b�nh") tk.soTrungBinh++;
        else if (xl == "Y?u")        tk.soYeu++;
        else                         tk.soKem++;
        tongDiem += hs.diemTBNam;
    }
    tk.diemTBLop = tongDiem / (float)lop.danhSachHS.size();
    return tk;
}

// TODO 10: In th?ng k� h?c l?c l?p ra console
inline void inThongKeHocLuc(const LopHoc& lop) {
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    int tongHS = lop.siSo();
    int soTuNhien = 0, soXaHoi = 0, soCoBan = 0;
    int daDongHocPhi = 0;
    double tongHocPhi = 0.0;
    const HocSinh* caoNhat = nullptr;
    const HocSinh* thapNhat = nullptr;

    for (const auto& hs : lop.danhSachHS) {
        if (hs.khoiHoc == "Tu nhien") soTuNhien++;
        else if (hs.khoiHoc == "Xa hoi") soXaHoi++;
        else soCoBan++;

        if (hs.hocPhi > 0.0) daDongHocPhi++;
        tongHocPhi += hs.hocPhi;

        if (!caoNhat || hs.diemTBNam > caoNhat->diemTBNam) caoNhat = &hs;
        if (!thapNhat || hs.diemTBNam < thapNhat->diemTBNam) thapNhat = &hs;
    }

    std::cout << "=== THONG KE HOC LUC LOP " << lop.tenLop << " ===\n";
    std::cout << "GVCN       : " << lop.tenGiaoVienChuNhiem() << "\n";
    std::cout << "Tong so HS : " << tongHS << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Diem TB lop: " << tk.diemTBLop << "\n\n";

    std::cout << std::left << std::setw(14) << "Hoc luc"
              << std::setw(10) << "So luong"
              << "Ty le\n" << std::string(34, '-') << "\n";
    std::cout << std::left << std::setw(14) << "Gioi"       << std::setw(10) << tk.soGioi      << tinhTyLe(tk.soGioi, tongHS)      << "%\n";
    std::cout << std::left << std::setw(14) << "Kha"        << std::setw(10) << tk.soKha       << tinhTyLe(tk.soKha, tongHS)       << "%\n";
    std::cout << std::left << std::setw(14) << "Trung binh" << std::setw(10) << tk.soTrungBinh << tinhTyLe(tk.soTrungBinh, tongHS) << "%\n";
    std::cout << std::left << std::setw(14) << "Yeu"        << std::setw(10) << tk.soYeu       << tinhTyLe(tk.soYeu, tongHS)       << "%\n";
    std::cout << std::left << std::setw(14) << "Kem"        << std::setw(10) << tk.soKem       << tinhTyLe(tk.soKem, tongHS)       << "%\n";

    std::cout << "\nPhan bo khoi hoc:\n";
    std::cout << "  Co ban   : " << soCoBan   << " HS\n";
    std::cout << "  Tu nhien : " << soTuNhien << " HS\n";
    std::cout << "  Xa hoi   : " << soXaHoi   << " HS\n";

    if (caoNhat) {
        std::cout << "\nHoc sinh diem cao nhat : [" << caoNhat->thongTin.IDMaDinhDanh << "] "
                  << hoTenHocSinh(*caoNhat) << " - TB nam " << caoNhat->diemTBNam << "\n";
    }
    if (thapNhat) {
        std::cout << "Hoc sinh diem thap nhat: [" << thapNhat->thongTin.IDMaDinhDanh << "] "
                  << hoTenHocSinh(*thapNhat) << " - TB nam " << thapNhat->diemTBNam << "\n";
    }

    std::cout << "\nHoc phi:\n";
    std::cout << "  Da dong   : " << daDongHocPhi << "/" << tongHS << " HS\n";
    std::cout << "  Chua dong : " << (tongHS - daDongHocPhi) << "/" << tongHS << " HS\n";
    std::cout << "  Tong thu  : " << std::fixed << std::setprecision(0) << tongHocPhi << " VND\n";
}

#pragma endregion

// ============================================================
#pragma region LuuXML

// TODO 11: Ghi danh s�ch l?p v�o file XML (d?i x?ng v?i docDanhSachLopTuXML)
inline bool ghiDanhSachLopVaoXML(const vector<LopHoc>& dsLop, const string& fileName) {
    tinyxml2::XMLDocument doc;
    XMLElement* root = doc.NewElement("DanhSachLop");
    doc.InsertFirstChild(root);

    auto addText = [&](XMLElement* parent, const char* tag, const string& val) {
        XMLElement* e = doc.NewElement(tag);
        e->SetText(val.c_str());
        parent->InsertEndChild(e);
    };

    for (const auto& lop : dsLop) {
        XMLElement* lopElem = doc.NewElement("LopHoc");
        root->InsertEndChild(lopElem);
        addText(lopElem, "TenLop", lop.tenLop);

        XMLElement* gvElem = doc.NewElement("GiaoVienCN");
        lopElem->InsertEndChild(gvElem);
        addText(gvElem, "Ho",          lop.giaoVienCN.thongTin.ho);
        addText(gvElem, "TenDem",      lop.giaoVienCN.thongTin.tenDem);
        addText(gvElem, "Ten",         lop.giaoVienCN.thongTin.ten);
        addText(gvElem, "NgaySinh",    lop.giaoVienCN.thongTin.ngaySinh);
        addText(gvElem, "GioiTinh",    lop.giaoVienCN.thongTin.gioiTinh);
        addText(gvElem, "MonGiangDay", lop.giaoVienCN.monGiangDay);

        XMLElement* dsHSElem = doc.NewElement("DanhSachHS");
        lopElem->InsertEndChild(dsHSElem);

        for (const auto& hs : lop.danhSachHS) {
            XMLElement* hsElem = doc.NewElement("HocSinh");
            dsHSElem->InsertEndChild(hsElem);

            XMLElement* ttElem = doc.NewElement("ThongTin");
            hsElem->InsertEndChild(ttElem);
            addText(ttElem, "IDMaDinhDanh", hs.thongTin.IDMaDinhDanh);
            addText(ttElem, "Ho",           hs.thongTin.ho);
            addText(ttElem, "TenDem",       hs.thongTin.tenDem);
            addText(ttElem, "Ten",          hs.thongTin.ten);
            addText(ttElem, "NgaySinh",     hs.thongTin.ngaySinh);
            addText(ttElem, "GioiTinh",     hs.thongTin.gioiTinh);

            addText(hsElem, "ThongTinPhuHuynh", hs.thongTinPhuHuynh);
            addText(hsElem, "HocPhi",  std::to_string(hs.hocPhi));
            addText(hsElem, "KhoiHoc", hs.khoiHoc);

            XMLElement* hkElem = doc.NewElement("HanhKiem");
            hsElem->InsertEndChild(hkElem);
            addText(hkElem, "HK1", std::to_string(hs.hanhKiemHK1));
            addText(hkElem, "HK2", std::to_string(hs.hanhKiemHK2));

            XMLElement* dtbElem = doc.NewElement("DiemTrungBinh");
            hsElem->InsertEndChild(dtbElem);
            addText(dtbElem, "HK1", std::to_string(hs.diemTBHK1));
            addText(dtbElem, "HK2", std::to_string(hs.diemTBHK2));
            addText(dtbElem, "Nam", std::to_string(hs.diemTBNam));

            XMLElement* dsMonElem = doc.NewElement("DanhSachMon");
            hsElem->InsertEndChild(dsMonElem);
            for (const auto& mon : hs.danhSachMon) {
                XMLElement* monElem = doc.NewElement("MonHoc");
                dsMonElem->InsertEndChild(monElem);
                addText(monElem, "TenMonHoc", mon.tenMonHoc);
                auto writeHK = [&](const char* tag, const DiemMonHocHK& hk) {
                    XMLElement* e = doc.NewElement(tag);
                    monElem->InsertEndChild(e);
                    addText(e, "Diem15p",    std::to_string(hk.diem15p));
                    addText(e, "DiemDauGio", std::to_string(hk.diemDauGio));
                    addText(e, "Diem1Tiet",  std::to_string(hk.diem1Tiet));
                    addText(e, "DiemGiuaKy", std::to_string(hk.diemGiuaKy));
                    addText(e, "DiemCuoiKy", std::to_string(hk.diemCuoiKy));
                };
                writeHK("HK1", mon.hk1);
                writeHK("HK2", mon.hk2);
            }
        }
    }
    return doc.SaveFile(fileName.c_str()) == XML_SUCCESS;
}

#pragma endregion

// ============================================================
#pragma region XuatBaoCao

inline void writeReportHeader(std::ofstream& ofs, const LopHoc& lop) {
    std::time_t now = std::time(nullptr);
    std::tm tmNowStorage;
    std::tm* tmNow = nullptr;
#ifdef _MSC_VER
    if (localtime_s(&tmNowStorage, &now) == 0) tmNow = &tmNowStorage;
#else
    tmNow = std::localtime(&now);
#endif
    char timeStr[32] = "--/--/---- --:--:--";
    if (tmNow) std::strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", tmNow);

    ofs << std::string(100, '=') << "\n";
    ofs << "               TRU?NG THPT [T�N TRU?NG]               \n";
    ofs << "        B�O C�O CHI TI?T H?C SINH & H?C PH� L?P       \n";
    ofs << std::string(100, '=') << "\n";
    ofs << "Ng�y in b�o c�o : " << timeStr << "\n";
    ofs << "L?p            : " << lop.tenLop << "\n";
    ofs << "Gi�o vi�n CN    : " << lop.tenGiaoVienChuNhiem() << "\n";
    ofs << "M�n GVCN       : " << lop.giaoVienCN.monGiangDay << "\n";
    ofs << "Si s?          : " << lop.siSo() << "\n";
    ofs << "H?c ph� chu?n   : " << std::fixed << std::setprecision(0) << HOC_PHI_CHUAN_MOT_HS << " VND/h?c sinh\n";
    ofs << std::string(100, '=') << "\n\n";
}

inline void writeHocPhiSummary(std::ofstream& ofs, const LopHoc& lop) {
    int daHoanThanh = 0;
    int coNo = 0;
    int chuaDong = 0;
    double tongThu = 0.0;
    double tongNo = 0.0;

    for (const auto& hs : lop.danhSachHS) {
        double no = tinhHocPhiConNo(hs);
        tongThu += hs.hocPhi;
        tongNo += no;
        if (daDongHocPhiDayDu(hs)) daHoanThanh++;
        else if (hs.hocPhi > 0.0) coNo++;
        else chuaDong++;
    }

    ofs << "TH?NG K� H?C PH� L?P:\n";
    ofs << std::string(70, '-') << "\n";
    ofs << std::left << std::setw(25) << "T?ng s? h?c sinh" << ": " << lop.siSo() << "\n";
    ofs << std::left << std::setw(25) << "�� d�ng d?y d?" << ": " << daHoanThanh << " HS\n";
    ofs << std::left << std::setw(25) << "��ng m?t ph?n" << ": " << coNo << " HS\n";
    ofs << std::left << std::setw(25) << "Chua d�ng" << ": " << chuaDong << " HS\n";
    ofs << std::left << std::setw(25) << "T?ng thu" << ": " << std::fixed << std::setprecision(0) << tongThu << " VND\n";
    ofs << std::left << std::setw(25) << "T?ng n?" << ": " << tongNo << " VND\n";
    ofs << std::left << std::setw(25) << "T? l? thu" << ": " << std::fixed << std::setprecision(1)
        << tinhTyLe(daHoanThanh + coNo, lop.siSo()) << " %\n";
    ofs << std::string(70, '-') << "\n\n";
}

inline void writeStudentFeeTable(std::ofstream& ofs, const LopHoc& lop) {
    ofs << "B?NG H?C PH� CHI TI?T:\n";
    ofs << std::string(110, '-') << "\n";
    ofs << std::left
        << std::setw(5)  << "STT"
        << std::setw(12) << "ID"
        << std::setw(28) << "H? v� t�n"
        << std::setw(15) << "�� d�ng"
        << std::setw(15) << "N?"
        << std::setw(12) << "Tr?ng th�i"
        << "Ph? huynh\n";
    ofs << std::string(110, '-') << "\n";

    for (int i = 0; i < (int)lop.danhSachHS.size(); ++i) {
        const HocSinh& hs = lop.danhSachHS[i];
        double no = tinhHocPhiConNo(hs);
        string status = daDongHocPhiDayDu(hs) ? "Ho�n th�nh"
                        : (hs.hocPhi > 0.0 ? "C�n n?" : "Chua d�ng");
        ofs << std::left
            << std::setw(5)  << (i + 1)
            << std::setw(12) << hs.thongTin.IDMaDinhDanh
            << std::setw(28) << hoTenHocSinh(hs)
            << std::setw(15) << std::fixed << std::setprecision(0) << hs.hocPhi
            << std::setw(15) << std::fixed << std::setprecision(0) << no
            << std::setw(12) << status
            << hs.thongTinPhuHuynh << "\n";
    }
    ofs << std::string(110, '-') << "\n\n";
}

inline void writeStudentSubjectDetails(std::ofstream& ofs, const HocSinh& hs) {
    ofs << "\n[" << hs.thongTin.IDMaDinhDanh << "] " << hoTenHocSinh(hs) << "\n";
    ofs << std::left
        << std::setw(30) << "M�n h?c"
        << std::setw(7)  << "15p1"
        << std::setw(7)  << "DG1"
        << std::setw(7)  << "1T1"
        << std::setw(7)  << "GK1"
        << std::setw(7)  << "CK1"
        << std::setw(8)  << "TB HK1"
        << std::setw(7)  << "15p2"
        << std::setw(7)  << "DG2"
        << std::setw(7)  << "1T2"
        << std::setw(7)  << "GK2"
        << std::setw(7)  << "CK2"
        << std::setw(8)  << "TB HK2"
        << "TB Nam\n";
    ofs << std::string(114, '-') << "\n";
    auto fmt = [&](float v) {
        if (v <= 0.0f) return std::string("---");
        char buf[16]; std::snprintf(buf, sizeof(buf), "%.1f", v);
        return std::string(buf);
    };
    for (const auto& mon : hs.danhSachMon) {
        if (!mon.hk1.coNhapDiem() && !mon.hk2.coNhapDiem()) continue;
        ofs << std::left
            << std::setw(30) << mon.tenMonHoc
            << std::setw(7)  << fmt(mon.hk1.diem15p)
            << std::setw(7)  << fmt(mon.hk1.diemDauGio)
            << std::setw(7)  << fmt(mon.hk1.diem1Tiet)
            << std::setw(7)  << fmt(mon.hk1.diemGiuaKy)
            << std::setw(7)  << fmt(mon.hk1.diemCuoiKy)
            << std::setw(8)  << fmt(mon.hk1.tinhTB())
            << std::setw(7)  << fmt(mon.hk2.diem15p)
            << std::setw(7)  << fmt(mon.hk2.diemDauGio)
            << std::setw(7)  << fmt(mon.hk2.diem1Tiet)
            << std::setw(7)  << fmt(mon.hk2.diemGiuaKy)
            << std::setw(7)  << fmt(mon.hk2.diemCuoiKy)
            << std::setw(8)  << fmt(mon.hk2.tinhTB())
            << fmt(mon.tinhTBCaNam()) << "\n";
    }
    ofs << std::string(114, '-') << "\n";
    ofs << "  TB HK1: " << std::fixed << std::setprecision(2) << hs.diemTBHK1
        << "  TB HK2: " << hs.diemTBHK2
        << "  TB Nam: " << hs.diemTBNam << "\n";
    double no = tinhHocPhiConNo(hs);
    ofs << "  Hoc phi da dong: " << std::fixed << std::setprecision(0) << hs.hocPhi
        << " VND  |  No: " << no << " VND\n";
}

inline void writeReportFooter(std::ofstream& ofs) {
    ofs << "\n" << std::string(100, '=') << "\n";
    ofs << "GHI CH�:\n";
    ofs << "- H?c ph� chu?n m?i h?c sinh du?c t�nh b?ng " << std::fixed << std::setprecision(0)
        << HOC_PHI_CHUAN_MOT_HS << " VND.\n";
    ofs << "- S? n? ch�nh l� ph?n h?c ph� chua thanh to�n.\n";
    ofs << "- B�o c�o n�y d�ng d? theo d�i qu? h?c ph� v� t�nh tr?ng n? c?a l?p.\n";
    ofs << std::string(100, '=') << "\n";
    ofs << "Ng�y l?p b�o c�o: _____________________________\n";
    ofs << "Ngu?i l?p        : _____________________________\n";
    ofs << "Gi�o vi�n CN     : _____________________________\n";
}

// TODO 12: Xu?t b�o c�o danh s�ch h?c sinh ra file .txt
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;

    ThongKeHocLuc tk = thongKeHocLucLop(lop);

    writeReportHeader(ofs, lop);
    ofs << "TH?NG K� H?C L?C L?P:\n";
    ofs << std::string(70, '-') << "\n";
    ofs << std::left << std::setw(15) << "X?p lo?i"
        << std::setw(10) << "S? HS"
        << std::setw(12) << "T? l?(%)"
        << "\n";
    ofs << std::string(70, '-') << "\n";
    ofs << std::left << std::setw(15) << "Gi?i" << std::setw(10) << tk.soGioi
        << std::setw(12) << std::fixed << std::setprecision(1) << tinhTyLe(tk.soGioi, lop.siSo()) << "\n";
    ofs << std::left << std::setw(15) << "Kh�" << std::setw(10) << tk.soKha
        << std::setw(12) << std::fixed << std::setprecision(1) << tinhTyLe(tk.soKha, lop.siSo()) << "\n";
    ofs << std::left << std::setw(15) << "Trung b�nh" << std::setw(10) << tk.soTrungBinh
        << std::setw(12) << std::fixed << std::setprecision(1) << tinhTyLe(tk.soTrungBinh, lop.siSo()) << "\n";
    ofs << std::left << std::setw(15) << "Y?u" << std::setw(10) << tk.soYeu
        << std::setw(12) << std::fixed << std::setprecision(1) << tinhTyLe(tk.soYeu, lop.siSo()) << "\n";
    ofs << std::left << std::setw(15) << "K�m" << std::setw(10) << tk.soKem
        << std::setw(12) << std::fixed << std::setprecision(1) << tinhTyLe(tk.soKem, lop.siSo()) << "\n";
    ofs << std::string(70, '-') << "\n\n";

    writeHocPhiSummary(ofs, lop);
    writeStudentFeeTable(ofs, lop);

    ofs << "B?NG T?NG H?P H?C SINH:\n";
    ofs << std::string(140, '-') << "\n";
    ofs << std::left
        << std::setw(5)  << "STT"
        << std::setw(12) << "ID"
        << std::setw(28) << "H? v� t�n"
        << std::setw(12) << "Ng.sinh"
        << std::setw(8)  << "GT"
        << std::setw(10) << "Kh?i"
        << std::setw(8)  << "TB HK1"
        << std::setw(8)  << "TB HK2"
        << std::setw(8)  << "TB Nam"
        << std::setw(12) << "H?c l?c"
        << std::setw(10) << "HK"
        << std::setw(15) << "�� d�ng"
        << std::setw(12) << "N?"
        << std::setw(12) << "Tr?ng th�i"
        << "Ph? huynh\n";
    ofs << std::string(140, '-') << "\n";
    for (int i = 0; i < (int)lop.danhSachHS.size(); ++i) {
        const HocSinh& hs = lop.danhSachHS[i];
        float hkNam = tinhHanhKiemNam(hs);
        double no = tinhHocPhiConNo(hs);
        string status = daDongHocPhiDayDu(hs) ? "Ho�n th�nh"
                        : (hs.hocPhi > 0.0 ? "C�n n?" : "Chua d�ng");
        ofs << std::left
            << std::setw(5)  << (i + 1)
            << std::setw(12) << hs.thongTin.IDMaDinhDanh
            << std::setw(28) << hoTenHocSinh(hs)
            << std::setw(12) << hs.thongTin.ngaySinh
            << std::setw(8)  << hs.thongTin.gioiTinh
            << std::setw(10) << hs.khoiHoc
            << std::setw(8)  << std::fixed << std::setprecision(1) << hs.diemTBHK1
            << std::setw(8)  << hs.diemTBHK2
            << std::setw(8)  << hs.diemTBNam
            << std::setw(12) << xepLoaiHocLuc(hs.diemTBNam)
            << std::setw(10) << xepLoaiHanhKiem(hkNam)
            << std::setw(15) << std::fixed << std::setprecision(0) << hs.hocPhi
            << std::setw(12) << std::fixed << std::setprecision(0) << no
            << std::setw(12) << status
            << hs.thongTinPhuHuynh << "\n";
    }
    ofs << std::string(140, '-') << "\n\n";

    ofs << "B?NG �I?M T?NG H?P THEO M�N:\n";
    for (const auto& hs : lop.danhSachHS) {
        writeStudentSubjectDetails(ofs, hs);
    }

    writeReportFooter(ofs);
    ofs.close();
    return true;
}

inline bool xuatDanhSachLopRaTxt(const vector<LopHoc>& dsLop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;

    ofs << std::string(120, '=') << "\n";
    ofs << "BANG DANH SACH LOP VA QUY HOC PHI TOAN TRUONG\n";
    ofs << std::string(120, '=') << "\n";
    ofs << std::left
        << std::setw(5)  << "STT"
        << std::setw(12) << "Ten lop"
        << std::setw(28) << "GVCN"
        << std::setw(8)  << "Si so"
        << std::setw(15) << "Tong thu"
        << std::setw(15) << "Tong no"
        << "Trang thai\n";
    ofs << std::string(120, '-') << "\n";

    for (int i = 0; i < (int)dsLop.size(); ++i) {
        const auto& lop = dsLop[i];
        double tongThu = 0.0;
        double tongNo = 0.0;
        int hoanThanh = 0;
        int conNo = 0;
        int chuaDong = 0;
        for (const auto& hs : lop.danhSachHS) {
            tongThu += hs.hocPhi;
            double no = tinhHocPhiConNo(hs);
            tongNo += no;
            if (daDongHocPhiDayDu(hs)) hoanThanh++;
            else if (hs.hocPhi > 0.0) conNo++;
            else chuaDong++;
        }
        string status = (hoanThanh == lop.siSo() && lop.siSo() > 0) ? "Hoan thanh"
                        : (conNo > 0 || chuaDong > 0) ? "Con no"
                        : "Chua dong";
        ofs << std::left
            << std::setw(5)  << (i + 1)
            << std::setw(12) << lop.tenLop
            << std::setw(28) << lop.tenGiaoVienChuNhiem()
            << std::setw(8)  << lop.siSo()
            << std::setw(15) << std::fixed << std::setprecision(0) << tongThu
            << std::setw(15) << std::fixed << std::setprecision(0) << tongNo
            << status << "\n";
    }

    ofs << std::string(120, '-') << "\n\n";
    ofs << "GHI CHU:\n";
    ofs << "- Tong thu la tong so tien da hoc sinh nop trong lop.\n";
    ofs << "- Tong no la tong so tien can thu them.\n";
    ofs << "- Trang thai 'Con no' bao gom lop con hoc phi da nop mot phan hoac chua nop.\n";
    ofs << std::string(120, '=') << "\n";
    ofs.close();
    return true;
}

#pragma endregion

// ============================================================
#pragma region Validation

// TODO 13: Validate di?m h?p l? (0 � 10)
inline bool laDiemHopLe(float diem) {
    return diem >= 0.0f && diem <= 10.0f;
}

// TODO 14: Validate ng�y sinh d?nh d?ng DD/MM/YYYY
inline bool laNgaySinhHopLe(const string& s) {
    if (s.size() != 10) return false;
    if (s[2] != '/' || s[5] != '/') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

#pragma endregion

// ============================================================
#pragma region NhapLieu

inline void xoaDongNhapConLai() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline float nhapDiemTrongKhoang(const char* label) {
    float diem = -1.0f;
    while (true) {
        std::cout << label;
        std::cin >> diem;
        if (std::cin.fail()) {
            std::cin.clear();
            xoaDongNhapConLai();
            std::cout << "  [!] Vui long nhap so.\n";
            continue;
        }
        if (!laDiemHopLe(diem)) {
            std::cout << "  [!] Diem phai tu 0 den 10.\n";
            continue;
        }
        return diem;
    }
}

inline double nhapHocPhiHopLe() {
    double hocPhi = -1.0;
    while (true) {
        std::cin >> hocPhi;
        if (std::cin.fail()) {
            std::cin.clear();
            xoaDongNhapConLai();
            std::cout << "  [!] Vui long nhap so tien hop le: ";
            continue;
        }
        if (hocPhi < 0.0) {
            std::cout << "  [!] Hoc phi khong duoc am: ";
            continue;
        }
        xoaDongNhapConLai();
        return hocPhi;
    }
}

// TODO 15: Nh?p th�ng tin m?t h?c sinh t? b�n ph�m (c� validation d?y d?)
inline HocSinh nhapThongTinHocSinh() {
    HocSinh hs;
    std::cout << "ID ma dinh danh : "; std::getline(std::cin, hs.thongTin.IDMaDinhDanh);
    std::cout << "Ho              : "; std::getline(std::cin, hs.thongTin.ho);
    std::cout << "Ten dem         : "; std::getline(std::cin, hs.thongTin.tenDem);
    std::cout << "Ten             : "; std::getline(std::cin, hs.thongTin.ten);
    do {
        std::cout << "Ngay sinh (DD/MM/YYYY): ";
        std::getline(std::cin, hs.thongTin.ngaySinh);
        if (!laNgaySinhHopLe(hs.thongTin.ngaySinh))
            std::cout << "  [!] Dinh dang khong hop le, vui long nhap lai (VD: 15/08/2007).\n";
    } while (!laNgaySinhHopLe(hs.thongTin.ngaySinh));
    std::cout << "Gioi tinh (Nam/Nu)              : "; std::getline(std::cin, hs.thongTin.gioiTinh);
    std::cout << "Thong tin phu huynh             : "; std::getline(std::cin, hs.thongTinPhuHuynh);
    std::cout << "Hoc phi                         : "; hs.hocPhi = nhapHocPhiHopLe();
    std::cout << "Khoi hoc (Co ban/Tu nhien/Xa hoi): "; std::getline(std::cin, hs.khoiHoc);

    hs.danhSachMon = khoiTaoDanhSachMonHoc();
    std::cout << "\nNhap diem theo tung hoc ky cho moi mon (0 = chua co diem):\n";
    for (auto& mon : hs.danhSachMon) {
        std::cout << "\n  [" << mon.tenMonHoc << "]\n";
        auto nhapDiem = [&](const char* label) -> float {
            string prompt = "    " + std::string(label) + ": ";
            return nhapDiemTrongKhoang(prompt.c_str());
        };
        std::cout << "  --- Hoc ky 1 ---\n";
        mon.hk1.diem15p    = nhapDiem("15 phut");
        mon.hk1.diemDauGio = nhapDiem("Dau gio");
        mon.hk1.diem1Tiet  = nhapDiem("1 tiet");
        mon.hk1.diemGiuaKy = nhapDiem("Giua ky 1");
        mon.hk1.diemCuoiKy = nhapDiem("Cuoi ky 1");
        std::cout << "  --- Hoc ky 2 ---\n";
        mon.hk2.diem15p    = nhapDiem("15 phut");
        mon.hk2.diemDauGio = nhapDiem("Dau gio");
        mon.hk2.diem1Tiet  = nhapDiem("1 tiet");
        mon.hk2.diemGiuaKy = nhapDiem("Giua ky 2");
        mon.hk2.diemCuoiKy = nhapDiem("Cuoi ky 2");
    }
    xoaDongNhapConLai();

    auto nhapHK = [&](const char* label) -> float {
        string prompt = std::string(label) + " (0-10): ";
        return nhapDiemTrongKhoang(prompt.c_str());
    };
    hs.hanhKiemHK1 = nhapHK("Hanh kiem HK1");
    hs.hanhKiemHK2 = nhapHK("Hanh kiem HK2");
    xoaDongNhapConLai();

    capNhatDiemTrungBinh(hs); // T? d?ng t�nh di?m TB
    return hs;
}

#pragma endregion

// ============================================================
#pragma region QuanLyLop

// Nh?p th�ng tin gi�o vi�n t? b�n ph�m
inline GiaoVien nhapThongTinGiaoVien() {
    GiaoVien gv;
    std::cout << "Ho GVCN      : "; std::getline(std::cin, gv.thongTin.ho);
    std::cout << "Ten dem      : "; std::getline(std::cin, gv.thongTin.tenDem);
    std::cout << "Ten          : "; std::getline(std::cin, gv.thongTin.ten);
    do {
        std::cout << "Ngay sinh (DD/MM/YYYY): ";
        std::getline(std::cin, gv.thongTin.ngaySinh);
        if (!laNgaySinhHopLe(gv.thongTin.ngaySinh))
            std::cout << "  [!] Dinh dang khong hop le, nhap lai.\n";
    } while (!laNgaySinhHopLe(gv.thongTin.ngaySinh));
    std::cout << "Gioi tinh    : "; std::getline(std::cin, gv.thongTin.gioiTinh);
    std::cout << "Mon giang day: "; std::getline(std::cin, gv.monGiangDay);
    return gv;
}

// Nh?p th�ng tin m?t l?p h?c t? b�n ph�m (kh�ng c� HS, s? th�m HS sau)
inline LopHoc nhapThongTinLop() {
    LopHoc lop;
    std::cout << "Ten lop (VD: 10A1): "; std::getline(std::cin, lop.tenLop);
    std::cout << "\n-- Thong tin Giao Vien Chu Nhiem --\n";
    lop.giaoVienCN = nhapThongTinGiaoVien();
    return lop;
}

#pragma endregion

// ============================================================
#pragma region ToanTruong

// Struct luu k?t qu? t�m ki?m to�n tru?ng (t�n l?p + con tr? h?c sinh)
struct KetQuaTimKiem {
    string tenLop;
    HocSinh* hocSinh = nullptr;
};

// T�m ki?m h?c sinh theo t�n ho?c ID tr�n to�n b? danh s�ch l?p
inline vector<KetQuaTimKiem> timKiemToanTruong(vector<LopHoc>& dsLop, const string& tuKhoa) {
    vector<KetQuaTimKiem> ketQua;
    string tuKhoaLower = tuKhoa;
    std::transform(tuKhoaLower.begin(), tuKhoaLower.end(), tuKhoaLower.begin(),
                   [](unsigned char c){ return (char)::tolower(c); });
    for (auto& lop : dsLop) {
        for (auto& hs : lop.danhSachHS) {
            bool matchID = (hs.thongTin.IDMaDinhDanh == tuKhoa);
            string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
            string hoTenLower = hoTen;
            std::transform(hoTenLower.begin(), hoTenLower.end(), hoTenLower.begin(),
                           [](unsigned char c){ return (char)::tolower(c); });
            if (matchID || hoTenLower.find(tuKhoaLower) != string::npos)
                ketQua.push_back({lop.tenLop, &hs});
        }
    }
    return ketQua;
}

// Struct k?t qu? th?ng k� to�n tru?ng
struct ThongKeToanTruong {
    int tongSoLop     = 0;
    int tongSoHS      = 0;
    int soGioi        = 0;
    int soKha         = 0;
    int soTrungBinh   = 0;
    int soYeu         = 0;
    int soKem         = 0;
    float diemTBTruong = 0.0f;
};

// Th?ng k� h?c l?c to�n tru?ng
inline ThongKeToanTruong thongKeToanTruong(const vector<LopHoc>& dsLop) {
    ThongKeToanTruong tk;
    tk.tongSoLop = (int)dsLop.size();
    float tongDiem = 0.0f;
    for (const auto& lop : dsLop) {
        tk.tongSoHS += lop.siSo();
        for (const auto& hs : lop.danhSachHS) {
            string xl = xepLoaiHocLuc(hs.diemTBNam);
            if      (xl == "Gioi")       tk.soGioi++;
            else if (xl == "Kha")        tk.soKha++;
            else if (xl == "Trung binh") tk.soTrungBinh++;
            else if (xl == "Yeu")        tk.soYeu++;
            else                         tk.soKem++;
            tongDiem += hs.diemTBNam;
        }
    }
    tk.diemTBTruong = (tk.tongSoHS > 0) ? tongDiem / tk.tongSoHS : 0.0f;
    return tk;
}

// In th?ng k� to�n tru?ng ra console
inline void inThongKeToanTruong(const vector<LopHoc>& dsLop) {
    ThongKeToanTruong tk = thongKeToanTruong(dsLop);
    std::cout << "=== THONG KE TOAN TRUONG ===\n";
    std::cout << "Tong so lop   : " << tk.tongSoLop    << "\n";
    std::cout << "Tong so HS    : " << tk.tongSoHS     << "\n";
    std::cout << "Gioi          : " << tk.soGioi       << "\n";
    std::cout << "Kha           : " << tk.soKha        << "\n";
    std::cout << "Trung binh    : " << tk.soTrungBinh  << "\n";
    std::cout << "Yeu           : " << tk.soYeu        << "\n";
    std::cout << "Kem           : " << tk.soKem        << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Diem TB truong: " << tk.diemTBTruong << "\n\n";
    std::cout << std::left << std::setw(8) << "Lop" << std::setw(7) << "Si so"
              << std::setw(7) << "Gioi" << std::setw(6) << "Kha"
              << std::setw(11) << "Trung binh" << std::setw(6) << "Yeu"
              << std::setw(5) << "Kem" << "  Diem TB\n";
    std::cout << std::string(58, '-') << "\n";
    for (const auto& lop : dsLop) {
        ThongKeHocLuc tkl = thongKeHocLucLop(lop);
        std::cout << std::left
                  << std::setw(8) << lop.tenLop
                  << std::setw(7) << lop.siSo()
                  << std::setw(7) << tkl.soGioi
                  << std::setw(6) << tkl.soKha
                  << std::setw(11) << tkl.soTrungBinh
                  << std::setw(6) << tkl.soYeu
                  << std::setw(5) << tkl.soKem
                  << "  " << std::fixed << std::setprecision(2) << tkl.diemTBLop << "\n";
    }
}

#pragma endregion

// ============================================================
#pragma region XepHang

// In b?ng x?p h?ng h?c sinh trong l?p theo di?m TB (b?n sao, kh�ng thay d?i th? t? g?c)
inline void inXepHangHocSinh(const LopHoc& lop) {
    vector<const HocSinh*> ds;
    for (const auto& hs : lop.danhSachHS) ds.push_back(&hs);
    std::sort(ds.begin(), ds.end(), [](const HocSinh* a, const HocSinh* b) {
        return a->diemTBNam > b->diemTBNam;
    });
    std::cout << "=== XEP HANG HOC SINH LOP " << lop.tenLop << " ===\n";
    std::cout << std::left
              << std::setw(5)  << "Hang"
              << std::setw(8)  << "ID"
              << std::setw(25) << "Ho va ten"
              << std::setw(8)  << "TB Nam"
              << std::setw(12) << "Hoc luc"
              << "HK Nam\n"
              << std::string(72, '-') << "\n";
    for (int i = 0; i < (int)ds.size(); ++i) {
        const HocSinh* hs = ds[i];
        string hoTen = hs->thongTin.ho + " " + hs->thongTin.tenDem + " " + hs->thongTin.ten;
        float hkNam = (hs->hanhKiemHK1 + hs->hanhKiemHK2 > 0)
            ? (hs->hanhKiemHK1 + hs->hanhKiemHK2) / 2.0f : hs->hanhKiemHK1;
        std::cout << std::left
                  << std::setw(5)  << (i + 1)
                  << std::setw(8)  << hs->thongTin.IDMaDinhDanh
                  << std::setw(25) << hoTen
                  << std::setw(8)  << std::fixed << std::setprecision(2) << hs->diemTBNam
                  << std::setw(12) << xepLoaiHocLuc(hs->diemTBNam)
                  << xepLoaiHanhKiem(hkNam) << "\n";
    }
}

// L?y danh s�ch h?c sinh y?u/k�m trong m?t l?p
inline vector<const HocSinh*> layHSYeuKemTrongLop(const LopHoc& lop) {
    vector<const HocSinh*> ds;
    for (const auto& hs : lop.danhSachHS) {
        string xl = xepLoaiHocLuc(hs.diemTBNam);
        if (xl == "Yeu" || xl == "Kem") ds.push_back(&hs);
    }
    return ds;
}

// In danh s�ch c?nh b�o h?c sinh y?u/k�m to�n tru?ng
inline void inDanhSachCanhBao(const vector<LopHoc>& dsLop) {
    bool coHS = false;
    std::cout << "=== CANH BAO: HOC SINH YEU/KEM TOAN TRUONG ===\n";
    for (const auto& lop : dsLop) {
        auto dsYK = layHSYeuKemTrongLop(lop);
        if (dsYK.empty()) continue;
        coHS = true;
        std::cout << "\nLop " << lop.tenLop << " (" << dsYK.size() << " HS):\n";
        for (const auto* hs : dsYK) {
            string hoTen = hs->thongTin.ho + " " + hs->thongTin.tenDem + " " + hs->thongTin.ten;
            std::cout << "  [" << hs->thongTin.IDMaDinhDanh << "] " << hoTen
                      << "  TB: " << hs->diemTBNam
                      << " (" << xepLoaiHocLuc(hs->diemTBNam) << ")\n";
        }
    }
    if (!coHS) std::cout << "Khong co hoc sinh yeu/kem.\n";
}

#pragma endregion

// ============================================================
#pragma region HocPhi

// C?p nh?t h?c ph� cho h?c sinh theo ID trong l?p
inline bool capNhatHocPhi(LopHoc& lop, const string& id, double hocPhiMoi) {
    for (auto& hs : lop.danhSachHS) {
        if (hs.thongTin.IDMaDinhDanh == id) { hs.hocPhi = hocPhiMoi; return true; }
    }
    return false;
}

// In b�o c�o h?c ph� c?a m?t l?p (ai chua d�ng � hocPhi == 0)
inline void inBaoCaoHocPhiLop(const LopHoc& lop) {
    double tongThu = 0.0;
    double tongNo = 0.0;
    int soHoanThanh = 0;
    int soConNo = 0;
    int soChuaDong = 0;
    std::cout << "=== HOC PHI LOP " << lop.tenLop << " ===\n";
    std::cout << std::left << std::setw(8) << "ID" << std::setw(25) << "Ho va ten"
              << std::setw(16) << "Da dong" << std::setw(16) << "Con no" << "Trang thai\n"
              << std::string(85, '-') << "\n";
    for (const auto& hs : lop.danhSachHS) {
        string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        double no = tinhHocPhiConNo(hs);
        string tt;
        if (daDongHocPhiDayDu(hs)) { tt = "Hoan thanh"; soHoanThanh++; }
        else if (hs.hocPhi > 0.0) { tt = "Con no"; soConNo++; }
        else { tt = "Chua dong"; soChuaDong++; }
        std::cout << std::left << std::setw(8) << hs.thongTin.IDMaDinhDanh
                  << std::setw(25) << hoTen
                  << std::setw(16) << std::fixed << std::setprecision(0) << hs.hocPhi
                  << std::setw(16) << std::fixed << std::setprecision(0) << no
                  << tt << "\n";
        tongThu += hs.hocPhi;
        tongNo += no;
    }
    std::cout << std::string(85, '-') << "\n";
    std::cout << "Da dong day du  : " << soHoanThanh << "/" << lop.siSo() << " HS\n";
    std::cout << "Con no          : " << soConNo << " HS\n";
    std::cout << "Chua dong       : " << soChuaDong << " HS\n";
    std::cout << "Tong thu        : " << std::fixed << std::setprecision(0) << tongThu << " VND\n";
    std::cout << "Tong no         : " << std::fixed << std::setprecision(0) << tongNo << " VND\n";
}

// In b�o c�o h?c ph� t?ng h?p to�n tru?ng
inline void inBaoCaoHocPhiToanTruong(const vector<LopHoc>& dsLop) {
    double tongToanTruong = 0.0;
    double tongNoToanTruong = 0.0;
    int tongChuaDong = 0;
    int tongConNo = 0;
    int tongHoanThanh = 0;
    std::cout << "=== BAO CAO HOC PHI TOAN TRUONG ===\n";
    std::cout << std::left << std::setw(8) << "Lop" << std::setw(10) << "Si so"
              << std::setw(12) << "Hoan thanh" << std::setw(12) << "Con no" << "Tong thu\n"
              << std::string(60, '-') << "\n";
    for (const auto& lop : dsLop) {
        double tongLop = 0.0;
        double noLop = 0.0;
        int hoanThanh = 0;
        int conNo = 0;
        for (const auto& hs : lop.danhSachHS) {
            tongLop += hs.hocPhi;
            if (daDongHocPhiDayDu(hs)) hoanThanh++;
            else if (hs.hocPhi > 0.0) { conNo++; noLop += tinhHocPhiConNo(hs); }
            else { noLop += tinhHocPhiConNo(hs); }
        }
        std::cout << std::left << std::setw(8) << lop.tenLop
                  << std::setw(10) << lop.siSo()
                  << std::setw(12) << hoanThanh
                  << std::setw(12) << conNo
                  << std::fixed << std::setprecision(0) << tongLop << "\n";
        tongToanTruong += tongLop;
        tongNoToanTruong += noLop;
        tongHoanThanh += hoanThanh;
        tongConNo += conNo;
    }
    std::cout << std::string(60, '-') << "\n";
    std::cout << "Tong hoan thanh: " << tongHoanThanh << " HS\n";
    std::cout << "Tong con no    : " << tongConNo << " HS\n";
    std::cout << "Tong no       : " << std::fixed << std::setprecision(0) << tongNoToanTruong << " VND\n";
    std::cout << "Tong da thu   : " << std::fixed << std::setprecision(0) << tongToanTruong << " VND\n";
}

#pragma endregion

// ============================================================
#pragma region DiemChiTiet

// In b?ng di?m chi ti?t c?a m?t h?c sinh (t?ng m�n, t?ng lo?i di?m, c? 2 HK)
inline void inDiemChiTietHocSinh(const HocSinh& hs) {
    string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
    string sep(90, '=');
    std::cout << sep << "\n";
    std::cout << "BANG DIEM CHI TIET: " << hoTen
              << "  [ID: " << hs.thongTin.IDMaDinhDanh << "]\n";
    std::cout << sep << "\n";
    std::cout << std::left
              << std::setw(30) << "Mon hoc"
              << std::setw(9)  << "15ph HK1"
              << std::setw(9)  << "DG HK1"
              << std::setw(9)  << "1T HK1"
              << std::setw(8)  << "GK1"
              << std::setw(8)  << "CK1"
              << std::setw(7)  << "TB HK1"
              << " | "
              << std::setw(9)  << "15ph HK2"
              << std::setw(9)  << "DG HK2"
              << std::setw(9)  << "1T HK2"
              << std::setw(8)  << "GK2"
              << std::setw(8)  << "CK2"
              << std::setw(7)  << "TB HK2"
              << " | TB Nam\n"
              << std::string(90, '-') << "\n";
    for (const auto& mon : hs.danhSachMon) {
        if (!mon.hk1.coNhapDiem() && !mon.hk2.coNhapDiem()) continue;
        auto fmtD = [](float d) -> string {
            if (d <= 0) return "---";
            char buf[8]; std::snprintf(buf, sizeof(buf), "%.1f", d);
            return buf;
        };
        std::cout << std::left << std::setw(30) << mon.tenMonHoc
                  << std::setw(9) << fmtD(mon.hk1.diem15p)
                  << std::setw(9) << fmtD(mon.hk1.diemDauGio)
                  << std::setw(9) << fmtD(mon.hk1.diem1Tiet)
                  << std::setw(8) << fmtD(mon.hk1.diemGiuaKy)
                  << std::setw(8) << fmtD(mon.hk1.diemCuoiKy)
                  << std::setw(7) << fmtD(mon.hk1.tinhTB())
                  << " | "
                  << std::setw(9) << fmtD(mon.hk2.diem15p)
                  << std::setw(9) << fmtD(mon.hk2.diemDauGio)
                  << std::setw(9) << fmtD(mon.hk2.diem1Tiet)
                  << std::setw(8) << fmtD(mon.hk2.diemGiuaKy)
                  << std::setw(8) << fmtD(mon.hk2.diemCuoiKy)
                  << std::setw(7) << fmtD(mon.hk2.tinhTB())
                  << " | " << fmtD(mon.tinhTBCaNam()) << "\n";
    }
    std::cout << std::string(90, '-') << "\n";
    std::cout << "TB HK1: " << std::fixed << std::setprecision(2) << hs.diemTBHK1
              << "  TB HK2: " << hs.diemTBHK2
              << "  TB Nam: " << hs.diemTBNam
              << "  Xep loai: " << xepLoaiHocLuc(hs.diemTBNam) << "\n";
    std::cout << "HK HK1: " << xepLoaiHanhKiem(hs.hanhKiemHK1)
              << "  HK HK2: " << xepLoaiHanhKiem(hs.hanhKiemHK2) << "\n";
    double no = tinhHocPhiConNo(hs);
    std::cout << "Hoc phi da dong: " << std::fixed << std::setprecision(0) << hs.hocPhi << " VND"
              << "  No: " << no << " VND"
              << "  Trang thai: " << (daDongHocPhiDayDu(hs) ? "Hoan thanh" : (hs.hocPhi > 0.0 ? "Con no" : "Chua dong")) << "\n";
    std::cout << sep << "\n";
}

#pragma endregion
