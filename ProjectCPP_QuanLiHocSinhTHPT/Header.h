#pragma once
#include <iostream> // Thư viện cung cấp các đối tượng nhập/xuất như cin, cout
#include <iomanip> // Add this include at the top of your file for std::setw, std::setprecision, std::fixed
#include <string>   // Thư viện cho kiểu dữ liệu string
#include <vector>   // Thư viện cho kiểu dữ liệu vector
#include <algorithm>
#include <fstream>   // Xuất file báo cáo
#include <cctype>    // isdigit, tolower
#include <limits>    // cin error handling
// Ensure this include is at the top of your file, before any usage of tinyxml2
#include "third_party/tinyxml2/tinyxml2.h"

using std::string;
using std::vector;
// Add this line after the include to bring the tinyxml2 namespace into scope
using namespace tinyxml2;

#pragma region Defines

// Struct lưu thông tin điểm số
struct DiemSo {
    string loaiDiem;       // Loại điểm: Hạnh kiểm, học lực, kiểm tra,...
    float giaTri = 0;          // Điểm số cụ thể
};

// Struct lưu điểm thành phần của 1 môn trong 1 học kỳ
// Bao gồm: 15 phút, đầu giờ (HS 1), 1 tiết (HS 2), giữa kỳ (HS 2), cuối kỳ (HS 3)
struct DiemMonHocHK {
    float diem15p    = 0.0f;  // Kiểm tra 15 phút     (hệ số 1)
    float diemDauGio = 0.0f;  // Kiểm tra đầu giờ    (hệ số 1)
    float diem1Tiet  = 0.0f;  // Kiểm tra 1 tiết     (hệ số 2)
    float diemGiuaKy = 0.0f;  // Điểm giữa kỳ        (hệ số 2)
    float diemCuoiKy = 0.0f;  // Điểm cuối kỳ        (hệ số 3)

    // TB học kỳ = Σ(điểm × hệ số) / Σ(hệ số), chỉ tính điểm > 0
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

// Struct lưu thông tin môn học (điểm tách theo HK1 / HK2)
struct MonHoc {
    string tenMonHoc;
    DiemMonHocHK hk1;   // Điểm học kỳ 1 (15p, đầu giờ, 1 tiết, GK1, CK1)
    DiemMonHocHK hk2;   // Điểm học kỳ 2 (15p, đầu giờ, 1 tiết, GK2, CK2)

    float tinhTBHK1()    const { return hk1.tinhTB(); }
    float tinhTBHK2()    const { return hk2.tinhTB(); }
    // TB cả năm = (TB_HK1 + TB_HK2 × 2) / 3 (Thông tư 22/2021)
    float tinhTBCaNam() const {
        float t1 = hk1.tinhTB(), t2 = hk2.tinhTB();
        if (t1 == 0 && t2 == 0) return 0.0f;
        if (t1 == 0) return t2;
        if (t2 == 0) return t1;
        return (t1 + t2 * 2.0f) / 3.0f;
    }
};

// Struct lưu thông tin người cơ bản
struct Nguoi {
    string IDMaDinhDanh;                   // ID nguoi
	string ho;                   // Họ
	string tenDem;               // Tên đệm
	string ten;                  // Tên
	string ngaySinh;             // Ngày/tháng/năm sinh
    string gioiTinh;             // Giới tính
};
// Struct lưu thông tin học sinh
struct HocSinh {
	Nguoi thongTin;          // Thông tin cá nhân
    string thongTinPhuHuynh;     // Thông tin phụ huynh
    double hocPhi = 0.0;         // Học phí
    vector<MonHoc> danhSachMon;  // Các môn học & điểm (HK1 + HK2)
    float hanhKiemHK1 = 0.0f;   // Điểm hạnh kiểm học kỳ 1 (0–10)
    float hanhKiemHK2 = 0.0f;   // Điểm hạnh kiểm học kỳ 2 (0–10)
    float diemTBHK1   = 0.0f;   // Điểm trung bình học kỳ 1
    float diemTBHK2   = 0.0f;   // Điểm trung bình học kỳ 2
    float diemTBNam   = 0.0f;   // Điểm trung bình cả năm (xếp loại, xếp hạng)
    string khoiHoc;              // Ban học: cơ bản, tự nhiên, xã hội
};

// Struct lưu thông tin giáo viên
struct GiaoVien {
	Nguoi thongTin;          // Thông tin cá nhân
    string monGiangDay;          // Bộ môn giảng dạy
};

// Struct lưu thông tin lớp học
struct LopHoc {
    string tenLop;               // Tên lớp (VD: 10A1, 11B2,...)
    vector<HocSinh> danhSachHS;  // Danh sách học sinh
    GiaoVien giaoVienCN;         // Giáo viên chủ nhiệm
	//Sĩ số lớp học
    int siSo() const { 
        return (int)danhSachHS.size(); // Trả về số lượng học sinh trong lớp
	}
	// Tên giáo viên chủ nhiệm
    string tenGiaoVienChuNhiem() const { 
        return giaoVienCN.thongTin.ho + " " + giaoVienCN.thongTin.tenDem + " " + giaoVienCN.thongTin.ten; 
	}
};
#pragma endregion

#pragma region Khoi tao
//-- Tạo dữ liệu mẫu cho các môn học --
//Khai báo danh sách các môn học của THPT ở Việt Nam

// Danh sách các môn học THPT ở Việt Nam (không dấu)
const vector<string> DANH_SACH_TEN_MON_HOC = {
    "Ngu van",
    "Toan",
    "Vat li",
    "Hoa hoc",
    "Sinh hoc",
    "Lich su",
    "Dia li",
    "Ngoai ngu",
    "Giao duc cong dan",
    "Tin hoc",
    "Cong nghe",
    "Giao duc quoc phong – an ninh",
    "The duc"
};

// Hàm khởi tạo danh sách các môn học THPT với điểm số mặc định
// Trả về vector chứa các struct MonHoc với tên môn học đã khai báo và điểm số ban đầu là rỗng
inline vector<MonHoc> khoiTaoDanhSachMonHoc() {
    vector<MonHoc> ds;
    for (const auto& tenMon : DANH_SACH_TEN_MON_HOC) {
        MonHoc mon;
        mon.tenMonHoc = tenMon;
        // hk1, hk2 khởi tạo = 0.0f mặc định
        ds.push_back(mon);
    }
    return ds;
}

// Hàm khởi tạo một lớp với 5 học sinh và 1 giáo viên chủ nhiệm
inline LopHoc khoiTaoLopMau() {
    LopHoc lop;
    lop.tenLop = "10A1";

    // Giáo viên chủ nhiệm mẫu
    lop.giaoVienCN.thongTin = {"Nguyen", "Van", "An", "01/01/1980", "Nam"};
    lop.giaoVienCN.monGiangDay = "Toan";

    // 5 học sinh mẫu
    for (int i = 0; i < 5; ++i) {
        HocSinh hs;
        hs.thongTin.IDMaDinhDanh = "HS" + std::to_string(i + 1);
        hs.thongTin.ho = "Le";
        hs.thongTin.tenDem = "Thi";
        hs.thongTin.ten = "HocSinh" + std::to_string(i + 1);
        hs.thongTin.ngaySinh = "01/09/2006";
        hs.thongTin.gioiTinh = (i % 2 == 0) ? "Nam" : "Nu";
        hs.thongTinPhuHuynh = "Phu huynh " + std::to_string(i + 1);
        hs.hocPhi = 1200000.0 + i * 100000;
        hs.danhSachMon = khoiTaoDanhSachMonHoc();
        // Gán điểm mẫu cho một số môn (Toán, Ngữ văn)
        for (auto& mon : hs.danhSachMon) {
            if (mon.tenMonHoc == "Toan" || mon.tenMonHoc == "Ngu van") {
                mon.hk1 = {7.5f + i*0.2f, 7.0f, 8.0f + i*0.1f, 7.8f + i*0.1f, 7.5f + i*0.2f};
                mon.hk2 = {8.0f + i*0.1f, 7.5f, 8.5f + i*0.1f, 8.2f + i*0.1f, 8.0f + i*0.1f};
            }
        }
        hs.hanhKiemHK1 = 8.0f + i * 0.2f;
        hs.hanhKiemHK2 = 8.2f + i * 0.1f;
        hs.diemTBHK1   = 7.5f + i * 0.3f;
        hs.diemTBHK2   = 7.8f + i * 0.2f;
        hs.diemTBNam   = (hs.diemTBHK1 + hs.diemTBHK2 * 2.0f) / 3.0f;
        hs.khoiHoc = "Co ban";
        lop.danhSachHS.push_back(hs);
    }

    return lop;
}
#pragma endregion

#pragma region XyLyQuanLyHocSinh
// Hàm in danh sách học sinh của một lớp (cột tóm tắt TB HK1/HK2/Năm, Hạnh kiểm)
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

// Hàm in danh sách học sinh của nhiều lớp (tái sử dụng hàm đơn lớp)
static void inDanhSachHocSinh(const vector<LopHoc>& dsLop) {
    for (const auto& lop : dsLop) inDanhSachHocSinh(lop);
}


// Hàm thêm học sinh vào cuối danh sách của lớp
inline void themHocSinhVaoLop(LopHoc& lop, const HocSinh& hocSinh) {
    lop.danhSachHS.push_back(hocSinh);
}


// Hàm thêm lớp vào danh sách lớp
inline void themLopVaoDanhSach(vector<LopHoc>& dsLop, const LopHoc& lopMoi) {
    dsLop.push_back(lopMoi);
}


// Hàm sửa thông tin học sinh theo ID định danh
inline bool suaThongTinHocSinh(LopHoc& lop, const HocSinh& hocSinhMoi, const string& idMaDinhDanh) {
    for (auto& hs : lop.danhSachHS) {
        if (hs.thongTin.IDMaDinhDanh == idMaDinhDanh) {
            hs = hocSinhMoi;
            return true; // Sửa thành công
        }
    }
    return false; // Không tìm thấy học sinh
}


// Hàm sửa thông tin lớp học theo tên lớp trong danh sách các lớp
inline bool suaThongTinLop(vector<LopHoc>& dsLop, const LopHoc& lopMoi, const string& tenLop) {
    for (auto& lop : dsLop) {
        if (lop.tenLop == tenLop) {
            lop = lopMoi;
            return true; // Sửa thành công
        }
    }
    return false; // Không tìm thấy lớp
}


// Hàm xóa học sinh khỏi lớp theo ID định danh
inline bool xoaHocSinhTheoID(LopHoc& lop, const string& idMaDinhDanh) {
    for (auto it = lop.danhSachHS.begin(); it != lop.danhSachHS.end(); ++it) {
        if (it->thongTin.IDMaDinhDanh == idMaDinhDanh) {
            lop.danhSachHS.erase(it);
            return true; // Xóa thành công
        }
    }
    return false; // Không tìm thấy học sinh
}


// Hàm xóa lớp khỏi danh sách lớp theo tên lớp
inline bool xoaLopTheoTen(vector<LopHoc>& dsLop, const string& tenLop) {
    for (auto it = dsLop.begin(); it != dsLop.end(); ++it) {
        if (it->tenLop == tenLop) {
            dsLop.erase(it);
            return true; // Xóa thành công
        }
    }
    return false; // Không tìm thấy lớp
}


// Hàm sắp xếp danh sách lớp theo tên lớp (tăng dần)
inline void sapXepDanhSachLopTheoTen(vector<LopHoc>& dsLop) {
    std::sort(dsLop.begin(), dsLop.end(), [](const LopHoc& a, const LopHoc& b) {
        return a.tenLop < b.tenLop;
    });
}


// Hàm sắp xếp danh sách học sinh trong lớp theo tên (tăng dần)
inline void sapXepDanhSachHocSinhTheoTen(LopHoc& lop) {
    std::sort(lop.danhSachHS.begin(), lop.danhSachHS.end(), [](const HocSinh& a, const HocSinh& b) {
        std::string tenA = a.thongTin.tenDem + " " + a.thongTin.ten;
        std::string tenB = b.thongTin.tenDem + " " + b.thongTin.ten;
        return tenA < tenB;
    });
}

// TODO 8 (Fix): Helper đọc text an toàn từ XML — tránh crash khi element bị null
static inline string safeGetText(XMLElement* parent, const char* childTag, const string& def = "") {
    if (!parent) return def;
    XMLElement* child = parent->FirstChildElement(childTag);
    if (!child || !child->GetText()) return def;
    return child->GetText();
}

// Hàm đọc file THPT.xml và trả về danh sách các lớp (đã fix null-check)
inline vector<LopHoc> docDanhSachLopTuXML(const string& fileName) {
    vector<LopHoc> dsLop;
    XMLDocument doc;
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
                hs.hocPhi = hocPhiStr.empty() ? 0.0 : std::stod(hocPhiStr);
                hs.khoiHoc = safeGetText(hsElem, "KhoiHoc");

                XMLElement* hkElem = hsElem->FirstChildElement("HanhKiem");
                if (hkElem) {
                    string v1 = safeGetText(hkElem, "HK1", "0");
                    string v2 = safeGetText(hkElem, "HK2", "0");
                    hs.hanhKiemHK1 = v1.empty() ? 0.0f : std::stof(v1);
                    hs.hanhKiemHK2 = v2.empty() ? 0.0f : std::stof(v2);
                }
                XMLElement* dtbElem = hsElem->FirstChildElement("DiemTrungBinh");
                if (dtbElem) {
                    string hk1s = safeGetText(dtbElem, "HK1", "0");
                    string hk2s = safeGetText(dtbElem, "HK2", "0");
                    string nams = safeGetText(dtbElem, "Nam", "0");
                    hs.diemTBHK1 = hk1s.empty() ? 0.0f : std::stof(hk1s);
                    hs.diemTBHK2 = hk2s.empty() ? 0.0f : std::stof(hk2s);
                    hs.diemTBNam = nams.empty() ? 0.0f : std::stof(nams);
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
                                return s.empty() ? 0.0f : std::stof(s);
                            };
                            hk.diem15p    = gf("Diem15p");
                            hk.diemDauGio = gf("DiemDauGio");
                            hk.diem1Tiet  = gf("Diem1Tiet");
                            hk.diemGiuaKy = gf("DiemGiuaKy");
                            hk.diemCuoiKy = gf("DiemCuoiKy");
                        };
                        readHK(monElem->FirstChildElement("HK1"), mon.hk1);
                        readHK(monElem->FirstChildElement("HK2"), mon.hk2);
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

// TODO 1: Tính điểm TB cả năm từ danh sách môn học
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

// TODO 2: Cập nhật điểm TB cho học sinh (tính lại tự động từ danh sách môn)
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

// TODO 3: Xếp loại học lực theo thang điểm 10
inline string xepLoaiHocLuc(float diemTB) {
    if (diemTB >= 8.0f) return "Gioi";
    if (diemTB >= 6.5f) return "Kha";
    if (diemTB >= 5.0f) return "Trung binh";
    if (diemTB >= 3.5f) return "Yeu";
    return "Kem";
}

// TODO 4: Xếp loại hạnh kiểm theo thang điểm 10
inline string xepLoaiHanhKiem(float diemHK) {
    if (diemHK >= 9.0f) return "Tot";
    if (diemHK >= 7.0f) return "Kha";
    if (diemHK >= 5.0f) return "Trung binh";
    return "Yeu";
}

#pragma endregion

// ============================================================
#pragma region TimKiem

// TODO 5: Tìm học sinh theo ID trong lớp — trả về pointer (nullptr nếu không thấy)
inline HocSinh* timHocSinhTheoID(LopHoc& lop, const string& id) {
    for (auto& hs : lop.danhSachHS)
        if (hs.thongTin.IDMaDinhDanh == id) return &hs;
    return nullptr;
}

// TODO 6: Tìm học sinh theo từ khóa tên (không phân biệt hoa/thường)
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

// TODO 7: Lọc học sinh theo khối/ban học trong lớp (không phân biệt hoa/thường)
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

// TODO 8: Tìm lớp theo tên trong danh sách nhiều lớp
inline LopHoc* timLopTheoTen(vector<LopHoc>& dsLop, const string& tenLop) {
    for (auto& lop : dsLop)
        if (lop.tenLop == tenLop) return &lop;
    return nullptr;
}

#pragma endregion

// ============================================================
#pragma region ThongKe

// Struct chứa kết quả thống kê học lực toàn lớp
struct ThongKeHocLuc {
    int soGioi      = 0;
    int soKha       = 0;
    int soTrungBinh = 0;
    int soYeu       = 0;
    int soKem       = 0;
    float diemTBLop = 0.0f;
};

// TODO 9: Thống kê học lực toàn lớp
inline ThongKeHocLuc thongKeHocLucLop(const LopHoc& lop) {
    ThongKeHocLuc tk;
    if (lop.danhSachHS.empty()) return tk;
    float tongDiem = 0.0f;
    for (const auto& hs : lop.danhSachHS) {
        string xl = xepLoaiHocLuc(hs.diemTBNam);
        if      (xl == "Gioi")       tk.soGioi++;
        else if (xl == "Kha")        tk.soKha++;
        else if (xl == "Trung binh") tk.soTrungBinh++;
        else if (xl == "Yeu")        tk.soYeu++;
        else                         tk.soKem++;
        tongDiem += hs.diemTBNam;
    }
    tk.diemTBLop = tongDiem / (float)lop.danhSachHS.size();
    return tk;
}

// TODO 10: In thống kê học lực lớp ra console
inline void inThongKeHocLuc(const LopHoc& lop) {
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    std::cout << "=== THONG KE HOC LUC LOP " << lop.tenLop << " ===\n";
    std::cout << "Tong so HS : " << lop.siSo()       << "\n";
    std::cout << "Gioi       : " << tk.soGioi        << "\n";
    std::cout << "Kha        : " << tk.soKha         << "\n";
    std::cout << "Trung binh : " << tk.soTrungBinh   << "\n";
    std::cout << "Yeu        : " << tk.soYeu         << "\n";
    std::cout << "Kem        : " << tk.soKem         << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Diem TB lop: " << tk.diemTBLop     << "\n";
}

#pragma endregion

// ============================================================
#pragma region LuuXML

// TODO 11: Ghi danh sách lớp vào file XML (đối xứng với docDanhSachLopTuXML)
inline bool ghiDanhSachLopVaoXML(const vector<LopHoc>& dsLop, const string& fileName) {
    XMLDocument doc;
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

// TODO 12: Xuất báo cáo danh sách học sinh ra file .txt
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    ofs << "BAO CAO LOP : " << lop.tenLop << "\n";
    ofs << "GVCN       : " << lop.tenGiaoVienChuNhiem() << "\n";
    ofs << "Si so      : " << lop.siSo() << "\n";
    ofs << string(80, '-') << "\n";
    ofs << std::left
        << std::setw(5)  << "STT"
        << std::setw(25) << "Ho va ten"
        << std::setw(8)  << "TB HK1"
        << std::setw(8)  << "TB HK2"
        << std::setw(8)  << "TB Nam"
        << std::setw(10) << "Hoc luc"
        << std::setw(8)  << "HK HK1"
        << std::setw(8)  << "HK HK2"
        << "\n" << string(80, '-') << "\n";
    for (int i = 0; i < (int)lop.danhSachHS.size(); ++i) {
        const HocSinh& hs = lop.danhSachHS[i];
        string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        ofs << std::left
            << std::setw(5)  << (i + 1)
            << std::setw(25) << hoTen
            << std::setw(8)  << std::fixed << std::setprecision(1) << hs.diemTBHK1
            << std::setw(8)  << hs.diemTBHK2
            << std::setw(8)  << hs.diemTBNam
            << std::setw(10) << xepLoaiHocLuc(hs.diemTBNam)
            << std::setw(8)  << xepLoaiHanhKiem(hs.hanhKiemHK1)
            << std::setw(8)  << xepLoaiHanhKiem(hs.hanhKiemHK2)
            << "\n";
    }
    ofs << string(80, '-') << "\n";
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    ofs << "Gioi: " << tk.soGioi << " | Kha: " << tk.soKha
        << " | TB: " << tk.soTrungBinh << " | Yeu: " << tk.soYeu
        << " | Kem: " << tk.soKem << "\n";
    ofs << "Diem TB lop: " << std::fixed << std::setprecision(2) << tk.diemTBLop << "\n";
    ofs.close();
    return true;
}

#pragma endregion

// ============================================================
#pragma region Validation

// TODO 13: Validate điểm hợp lệ (0 – 10)
inline bool laDiemHopLe(float diem) {
    return diem >= 0.0f && diem <= 10.0f;
}

// TODO 14: Validate ngày sinh định dạng DD/MM/YYYY
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

// TODO 15: Nhập thông tin một học sinh từ bàn phím (có validation đầy đủ)
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
    std::cout << "Hoc phi                         : "; std::cin >> hs.hocPhi; std::cin.ignore();
    std::cout << "Khoi hoc (Co ban/Tu nhien/Xa hoi): "; std::getline(std::cin, hs.khoiHoc);

    hs.danhSachMon = khoiTaoDanhSachMonHoc();
    std::cout << "\nNhap diem theo tung hoc ky cho moi mon (0 = chua co diem):\n";
    for (auto& mon : hs.danhSachMon) {
        std::cout << "\n  [" << mon.tenMonHoc << "]\n";
        auto nhapDiem = [&](const char* label) -> float {
            float d = -1.0f;
            do {
                std::cout << "    " << std::left << std::setw(20) << label << ": ";
                std::cin >> d;
                if (!laDiemHopLe(d)) std::cout << "    [!] Diem phai tu 0 den 10.\n";
            } while (!laDiemHopLe(d));
            return d;
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
    std::cin.ignore();

    auto nhapHK = [&](const char* label) -> float {
        float hk = -1.0f;
        do {
            std::cout << label << " (0-10): ";
            std::cin >> hk;
            if (!laDiemHopLe(hk)) std::cout << "  [!] Diem phai tu 0 den 10.\n";
        } while (!laDiemHopLe(hk));
        return hk;
    };
    hs.hanhKiemHK1 = nhapHK("Hanh kiem HK1");
    hs.hanhKiemHK2 = nhapHK("Hanh kiem HK2");
    std::cin.ignore();

    capNhatDiemTrungBinh(hs); // Tự động tính điểm TB
    return hs;
}

#pragma endregion

// ============================================================
#pragma region QuanLyLop

// Nhập thông tin giáo viên từ bàn phím
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

// Nhập thông tin một lớp học từ bàn phím (không có HS, sẽ thêm HS sau)
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

// Struct lưu kết quả tìm kiếm toàn trường (tên lớp + con trỏ học sinh)
struct KetQuaTimKiem {
    string tenLop;
    HocSinh* hocSinh = nullptr;
};

// Tìm kiếm học sinh theo tên hoặc ID trên toàn bộ danh sách lớp
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

// Struct kết quả thống kê toàn trường
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

// Thống kê học lực toàn trường
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

// In thống kê toàn trường ra console
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
    std::cout << string(58, '-') << "\n";
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

// In bảng xếp hạng học sinh trong lớp theo điểm TB (bản sao, không thay đổi thứ tự gốc)
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
              << string(72, '-') << "\n";
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

// Lấy danh sách học sinh yếu/kém trong một lớp
inline vector<const HocSinh*> layHSYeuKemTrongLop(const LopHoc& lop) {
    vector<const HocSinh*> ds;
    for (const auto& hs : lop.danhSachHS) {
        string xl = xepLoaiHocLuc(hs.diemTBNam);
        if (xl == "Yeu" || xl == "Kem") ds.push_back(&hs);
    }
    return ds;
}

// In danh sách cảnh báo học sinh yếu/kém toàn trường
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

// Cập nhật học phí cho học sinh theo ID trong lớp
inline bool capNhatHocPhi(LopHoc& lop, const string& id, double hocPhiMoi) {
    for (auto& hs : lop.danhSachHS) {
        if (hs.thongTin.IDMaDinhDanh == id) { hs.hocPhi = hocPhiMoi; return true; }
    }
    return false;
}

// In báo cáo học phí của một lớp (ai chưa đóng — hocPhi == 0)
inline void inBaoCaoHocPhiLop(const LopHoc& lop) {
    double tongThu = 0.0;
    int soDaDong = 0;
    std::cout << "=== HOC PHI LOP " << lop.tenLop << " ===\n";
    std::cout << std::left << std::setw(8) << "ID" << std::setw(25) << "Ho va ten"
              << std::setw(16) << "Hoc phi (VND)" << "Trang thai\n"
              << string(60, '-') << "\n";
    for (const auto& hs : lop.danhSachHS) {
        string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        string tt = (hs.hocPhi > 0) ? "Da dong" : "CHUA DONG";
        std::cout << std::left << std::setw(8) << hs.thongTin.IDMaDinhDanh
                  << std::setw(25) << hoTen
                  << std::setw(16) << std::fixed << std::setprecision(0) << hs.hocPhi
                  << tt << "\n";
        tongThu += hs.hocPhi;
        if (hs.hocPhi > 0) soDaDong++;
    }
    std::cout << string(60, '-') << "\n";
    std::cout << "Da dong  : " << soDaDong << "/" << lop.siSo() << " HS\n";
    std::cout << "Chua dong: " << (lop.siSo() - soDaDong) << " HS\n";
    std::cout << "Tong thu : " << std::fixed << std::setprecision(0) << tongThu << " VND\n";
}

// In báo cáo học phí tổng hợp toàn trường
inline void inBaoCaoHocPhiToanTruong(const vector<LopHoc>& dsLop) {
    double tongToanTruong = 0.0;
    int chuaDongToanTruong = 0;
    std::cout << "=== BAO CAO HOC PHI TOAN TRUONG ===\n";
    std::cout << std::left << std::setw(8) << "Lop" << std::setw(10) << "Si so"
              << std::setw(12) << "Chua dong" << "Tong thu (VND)\n"
              << string(50, '-') << "\n";
    for (const auto& lop : dsLop) {
        double tongLop = 0.0;
        int chuaDong = 0;
        for (const auto& hs : lop.danhSachHS) {
            tongLop += hs.hocPhi;
            if (hs.hocPhi == 0.0) chuaDong++;
        }
        std::cout << std::left << std::setw(8) << lop.tenLop
                  << std::setw(10) << lop.siSo()
                  << std::setw(12) << chuaDong
                  << std::fixed << std::setprecision(0) << tongLop << "\n";
        tongToanTruong    += tongLop;
        chuaDongToanTruong += chuaDong;
    }
    std::cout << string(50, '-') << "\n";
    std::cout << "Tong HS chua dong : " << chuaDongToanTruong << "\n";
    std::cout << "Tong da thu       : " << std::fixed << std::setprecision(0) << tongToanTruong << " VND\n";
}

#pragma endregion

// ============================================================
#pragma region DiemChiTiet

// In bảng điểm chi tiết của một học sinh (từng môn, từng loại điểm, cả 2 HK)
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
              << string(90, '-') << "\n";
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
    std::cout << string(90, '-') << "\n";
    std::cout << "TB HK1: " << std::fixed << std::setprecision(2) << hs.diemTBHK1
              << "  TB HK2: " << hs.diemTBHK2
              << "  TB Nam: " << hs.diemTBNam
              << "  Xep loai: " << xepLoaiHocLuc(hs.diemTBNam) << "\n";
    std::cout << "HK HK1: " << xepLoaiHanhKiem(hs.hanhKiemHK1)
              << "  HK HK2: " << xepLoaiHanhKiem(hs.hanhKiemHK2) << "\n";
    std::cout << sep << "\n";
}

#pragma endregion
