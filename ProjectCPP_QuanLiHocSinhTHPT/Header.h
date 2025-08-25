#pragma once
#include <iostream> // Thư viện cung cấp các đối tượng nhập/xuất như cin, cout
#include <iomanip> // Add this include at the top of your file for std::setw, std::setprecision, std::fixed
#include <string>   // Thư viện cho kiểu dữ liệu string
#include <vector>   // Thư viện cho kiểu dữ liệu vector
#include <algorithm>
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

// Struct lưu thông tin môn học
struct MonHoc {
    string tenMonHoc;            // Tên môn học
    DiemSo danhSachDiem; // Điểm cho môn học
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
    double hocPhi;               // Học phí
    vector<MonHoc> danhSachMon;  // Các môn học & điểm
    DiemSo hanhKiem;             // Hạnh kiểm
    //Điểm tổng
    DiemSo diemTrungBinh;// Điểm trung bình tên môn học = Điểm trung bình.
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
        mon.danhSachDiem = {"", 0.0f}; // Khởi tạo điểm rỗng
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
        hs.hanhKiem = {"Hanh kiem", 8.0f + i * 0.2f};
        hs.diemTrungBinh = {"Diem trung binh", 7.5f + i * 0.3f};
        hs.khoiHoc = "Co ban";
        lop.danhSachHS.push_back(hs);
    }

    return lop;
}
#pragma endregion

#pragma region XyLyQuanLyHocSinh
static void inDanhSachHocSinh(const LopHoc& lop) {
    // In tiêu đề bảng
    std::cout << "--------------------------------------------------------------------------------------------------\n";
    std::cout << "| " << std::setw(3) << "STT"
        << " | " << std::setw(20) << "ho va ten"
        << " | " << std::setw(4) << "TBM"
        << " | " << std::setw(10) << "Hanh Kiem"
        << " | " << std::setw(5) << "Toan"
        << " | " << std::setw(5) << "Ly"
        << " | " << std::setw(5) << "Hoa"
        << " | " << std::setw(5) << "Sinh"
        << " | " << std::setw(5) << "Tin"
        << " | " << std::setw(5) << "van"
        << " | " << std::setw(5) << "Su"
        << " | " << std::setw(5) << "Dia"
        << " | " << std::setw(3) << "QP"
        << " |\n";
    std::cout << "--------------------------------------------------------------------------------------------------\n";

    int soHS = std::min(5, static_cast<int>(lop.danhSachHS.size()));
    for (int i = 0; i < soHS; ++i) {
        const HocSinh& hs = lop.danhSachHS[i];
        std::string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
        std::cout << "| " << std::setw(3) << (i + 1)
            << " | " << std::setw(20) << hoTen
            << " | " << std::setw(4) << std::fixed << std::setprecision(1) << hs.diemTrungBinh.giaTri
            << " | " << std::setw(10) << hs.hanhKiem.giaTri;

        // Giả sử thứ tự môn học là: Toan, Ly, Hoa, Sinh, Tin, van, Su, Dia, QP
        for (const auto& tenMon : { "Toan", "Ly", "Hoa", "Sinh", "Tin", "van", "Su", "Dia", "QP" }) {
            double diem = 0.0;
            for (const MonHoc mon : hs.danhSachMon) {
                if (mon.tenMonHoc == tenMon) {
                    diem = mon.danhSachDiem.giaTri;
                    break;
                }
            }
            std::cout << " | " << std::setw(5) << diem;
        }
        std::cout << " |\n";
    }
    std::cout << "--------------------------------------------------------------------------------------------------\n";

    // In menu thao tác bên dưới danh sách
    std::cout << "1. In toan bo HS\n";
    std::cout << "2. Them HS\n";
    std::cout << "3. Xoa HS\n";
    std::cout << "4. Sua thong tin\n";
    std::cout << "5. Thoat\n";
}

// Hàm in danh sách học sinh của nhiều lớp
static void inDanhSachHocSinh(const vector<LopHoc>& dsLop) {
    for (const auto& lop : dsLop) {
        std::cout << "Lop: " << lop.tenLop << " | GVCN: " << lop.tenGiaoVienChuNhiem() << "\n";
        std::cout << "--------------------------------------------------------------------------------------------------\n";
        std::cout << "| " << std::setw(3) << "STT"
            << " | " << std::setw(20) << "ho va ten"
            << " | " << std::setw(4) << "TBM"
            << " | " << std::setw(10) << "Hanh Kiem"
            << " | " << std::setw(5) << "Toan"
            << " | " << std::setw(5) << "Ly"
            << " | " << std::setw(5) << "Hoa"
            << " | " << std::setw(5) << "Sinh"
            << " | " << std::setw(5) << "Tin"
            << " | " << std::setw(5) << "van"
            << " | " << std::setw(5) << "Su"
            << " | " << std::setw(5) << "Dia"
            << " | " << std::setw(3) << "QP"
            << " |\n";
        std::cout << "--------------------------------------------------------------------------------------------------\n";

        int soHS = static_cast<int>(lop.danhSachHS.size());
        for (int i = 0; i < soHS; ++i) {
            const HocSinh& hs = lop.danhSachHS[i];
            std::string hoTen = hs.thongTin.ho + " " + hs.thongTin.tenDem + " " + hs.thongTin.ten;
            std::cout << "| " << std::setw(3) << (i + 1)
                << " | " << std::setw(20) << hoTen
                << " | " << std::setw(4) << std::fixed << std::setprecision(1) << hs.diemTrungBinh.giaTri
                << " | " << std::setw(10) << hs.hanhKiem.giaTri;

            for (const auto& tenMon : { "Toan", "Ly", "Hoa", "Sinh", "Tin", "van", "Su", "Dia", "QP" }) {
                double diem = 0.0;
                for (const MonHoc& mon : hs.danhSachMon) {
                    if (mon.tenMonHoc == tenMon) {
                        diem = mon.danhSachDiem.giaTri;
                        break;
                    }
                }
                std::cout << " | " << std::setw(5) << diem;
            }
            std::cout << " |\n";
        }
        std::cout << "--------------------------------------------------------------------------------------------------\n";
    }
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

// Hàm đọc file THPT.xml và trả về danh sách các lớp
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
        lop.tenLop = lopElem->FirstChildElement("TenLop")->GetText();

        // GiaoVienCN
        XMLElement* gvElem = lopElem->FirstChildElement("GiaoVienCN");
        if (gvElem) {
            lop.giaoVienCN.thongTin.ho = gvElem->FirstChildElement("Ho")->GetText();
            lop.giaoVienCN.thongTin.tenDem = gvElem->FirstChildElement("TenDem")->GetText();
            lop.giaoVienCN.thongTin.ten = gvElem->FirstChildElement("Ten")->GetText();
            lop.giaoVienCN.thongTin.ngaySinh = gvElem->FirstChildElement("NgaySinh")->GetText();
            lop.giaoVienCN.thongTin.gioiTinh = gvElem->FirstChildElement("GioiTinh")->GetText();
            lop.giaoVienCN.monGiangDay = gvElem->FirstChildElement("MonGiangDay")->GetText();
        }

        // DanhSachHS
        XMLElement* dsHSElem = lopElem->FirstChildElement("DanhSachHS");
        if (dsHSElem) {
            for (XMLElement* hsElem = dsHSElem->FirstChildElement("HocSinh"); hsElem; hsElem = hsElem->NextSiblingElement("HocSinh")) {
                HocSinh hs;
                XMLElement* ttElem = hsElem->FirstChildElement("ThongTin");
                if (ttElem) {
                    hs.thongTin.IDMaDinhDanh = ttElem->FirstChildElement("IDMaDinhDanh")->GetText();
                    hs.thongTin.ho = ttElem->FirstChildElement("Ho")->GetText();
                    hs.thongTin.tenDem = ttElem->FirstChildElement("TenDem")->GetText();
                    hs.thongTin.ten = ttElem->FirstChildElement("Ten")->GetText();
                    hs.thongTin.ngaySinh = ttElem->FirstChildElement("NgaySinh")->GetText();
                    hs.thongTin.gioiTinh = ttElem->FirstChildElement("GioiTinh")->GetText();
                }
                hs.thongTinPhuHuynh = hsElem->FirstChildElement("ThongTinPhuHuynh")->GetText();
                hs.hocPhi = std::stod(hsElem->FirstChildElement("HocPhi")->GetText());
                hs.khoiHoc = hsElem->FirstChildElement("KhoiHoc")->GetText();

                // hanhKiem
                XMLElement* hkElem = hsElem->FirstChildElement("HanhKiem");
                if (hkElem) {
                    hs.hanhKiem.loaiDiem = hkElem->FirstChildElement("LoaiDiem")->GetText();
                    hs.hanhKiem.giaTri = std::stof(hkElem->FirstChildElement("GiaTri")->GetText());
                }
                // diemTrungBinh
                XMLElement* dtbElem = hsElem->FirstChildElement("DiemTrungBinh");
                if (dtbElem) {
                    hs.diemTrungBinh.loaiDiem = dtbElem->FirstChildElement("LoaiDiem")->GetText();
                    hs.diemTrungBinh.giaTri = std::stof(dtbElem->FirstChildElement("GiaTri")->GetText());
                }
                // danhSachMon
                XMLElement* dsMonElem = hsElem->FirstChildElement("DanhSachMon");
                if (dsMonElem) {
                    for (XMLElement* monElem = dsMonElem->FirstChildElement("MonHoc"); monElem; monElem = monElem->NextSiblingElement("MonHoc")) {
                        MonHoc mon;
                        mon.tenMonHoc = monElem->FirstChildElement("TenMonHoc")->GetText();
                        XMLElement* diemElem = monElem->FirstChildElement("DiemSo");
                        if (diemElem) {
                            mon.danhSachDiem.loaiDiem = diemElem->FirstChildElement("LoaiDiem")->GetText();
                            mon.danhSachDiem.giaTri = std::stof(diemElem->FirstChildElement("GiaTri")->GetText());
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
