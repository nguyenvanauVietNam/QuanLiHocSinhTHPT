
#include "Header.h"

// ----------------------------------------------------------------
// Menu quản lý từng lớp học
// ----------------------------------------------------------------
void menuLop(LopHoc& lop, const string& xmlFile, vector<LopHoc>& dsLop) {
    int luaChon = 0;
    do {
        std::cout << "\n--- LOP: " << lop.tenLop
                  << "  |  GVCN: " << lop.tenGiaoVienChuNhiem()
                  << "  |  Si so: " << lop.siSo() << " ---\n";
        std::cout << " 1.  In danh sach hoc sinh\n";
        std::cout << " 2.  Them hoc sinh\n";
        std::cout << " 3.  Xoa hoc sinh\n";
        std::cout << " 4.  Sua thong tin hoc sinh\n";
        std::cout << " 5.  Tim kiem hoc sinh (ten / ID)\n";
        std::cout << " 6.  Thong ke hoc luc lop\n";
        std::cout << " 7.  Loc theo khoi hoc\n";
        std::cout << " 8.  Xep hang hoc sinh theo diem TB\n";
        std::cout << " 9.  Danh sach HS yeu/kem lop nay\n";
        std::cout << " 10. Quan ly hoc phi lop nay\n";
        std::cout << " 11. Xuat bao cao ra file .txt\n";
        std::cout << " 12. Luu tat ca vao XML\n";
        std::cout << " 13. Sap xep HS theo ten (A-Z)\n";
        std::cout << " 14. Sap xep HS theo diem TB (cao-thap)\n";
        std::cout << " 15. Sua ten lop / thong tin GVCN\n";
        std::cout << " 16. Xem diem chi tiet 1 hoc sinh\n";
        std::cout << " 0.  Quay lai\n";
        std::cout << "Lua chon: ";
        std::cin >> luaChon;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[!] Vui long nhap so nguyen.\n";
            luaChon = -1;
            continue;
        }
        std::cin.ignore();

        switch (luaChon) {
        case 1:
            inDanhSachHocSinh(lop);
            break;

        case 2: {
            std::cout << "\n-- THEM HOC SINH MOI --\n";
            HocSinh hs = nhapThongTinHocSinh();
            if (timHocSinhTheoID(lop, hs.thongTin.IDMaDinhDanh) != nullptr)
                std::cout << "[!] ID '" << hs.thongTin.IDMaDinhDanh << "' da ton tai trong lop. Khong them.\n";
            else {
                themHocSinhVaoLop(lop, hs);
                std::cout << "[OK] Da them hoc sinh thanh cong.\n";
            }
            break;
        }

        case 3: {
            std::cout << "Nhap ID hoc sinh can xoa: ";
            string id; std::getline(std::cin, id);
            if (xoaHocSinhTheoID(lop, id))
                std::cout << "[OK] Da xoa hoc sinh ID=" << id << ".\n";
            else
                std::cout << "[!] Khong tim thay ID: " << id << ".\n";
            break;
        }

        case 4: {
            std::cout << "Nhap ID hoc sinh can sua: ";
            string id; std::getline(std::cin, id);
            HocSinh* hs = timHocSinhTheoID(lop, id);
            if (!hs) { std::cout << "[!] Khong tim thay ID: " << id << ".\n"; break; }
            std::cout << "\n-- NHAP THONG TIN MOI (ID giu nguyen: " << id << ") --\n";
            HocSinh hsMoi = nhapThongTinHocSinh();
            hsMoi.thongTin.IDMaDinhDanh = id;
            suaThongTinHocSinh(lop, hsMoi, id);
            std::cout << "[OK] Da cap nhat.\n";
            break;
        }

        case 5: {
            std::cout << "Nhap tu khoa (ten hoac ID): ";
            string tk; std::getline(std::cin, tk);
            HocSinh* hsID = timHocSinhTheoID(lop, tk);
            if (hsID) {
                std::cout << "[" << hsID->thongTin.IDMaDinhDanh << "] "
                          << hsID->thongTin.ho << " " << hsID->thongTin.tenDem << " " << hsID->thongTin.ten
                          << "  |  TB: " << hsID->diemTBNam
                          << " (" << xepLoaiHocLuc(hsID->diemTBNam) << ")"
                          << "  |  HK1: " << xepLoaiHanhKiem(hsID->hanhKiemHK1)
                          << "  HK2: " << xepLoaiHanhKiem(hsID->hanhKiemHK2) << "\n";
                break;
            }
            auto dsKQ = timHocSinhTheoTen(lop, tk);
            if (dsKQ.empty()) std::cout << "[!] Khong tim thay ket qua nao.\n";
            else {
                std::cout << "Tim thay " << dsKQ.size() << " ket qua:\n";
                for (const auto* h : dsKQ)
                    std::cout << "  [" << h->thongTin.IDMaDinhDanh << "] "
                              << h->thongTin.ho << " " << h->thongTin.tenDem << " " << h->thongTin.ten
                              << "  TB: " << h->diemTBNam
                              << " (" << xepLoaiHocLuc(h->diemTBNam) << ")\n";
            }
            break;
        }

        case 6:
            inThongKeHocLuc(lop);
            break;

        case 7: {
            std::cout << "Nhap khoi hoc (Co ban / Tu nhien / Xa hoi): ";
            string khoi; std::getline(std::cin, khoi);
            auto dsKhoi = locHocSinhTheoKhoi(lop, khoi);
            if (dsKhoi.empty())
                std::cout << "[!] Khong co hoc sinh thuoc khoi: " << khoi << "\n";
            else {
                std::cout << "Hoc sinh khoi '" << khoi << "' (" << dsKhoi.size() << " HS):\n";
                for (const auto* h : dsKhoi)
                    std::cout << "  [" << h->thongTin.IDMaDinhDanh << "] "
                              << h->thongTin.ho << " " << h->thongTin.tenDem << " " << h->thongTin.ten << "\n";
            }
            break;
        }

        case 8:
            inXepHangHocSinh(lop);
            break;

        case 9: {
            auto dsYK = layHSYeuKemTrongLop(lop);
            if (dsYK.empty()) std::cout << "Lop " << lop.tenLop << " khong co HS yeu/kem.\n";
            else {
                std::cout << "HS yeu/kem lop " << lop.tenLop << " (" << dsYK.size() << " HS):\n";
                for (const auto* hs : dsYK) {
                    string hoTen = hs->thongTin.ho + " " + hs->thongTin.tenDem + " " + hs->thongTin.ten;
                    std::cout << "  [" << hs->thongTin.IDMaDinhDanh << "] " << hoTen
                              << "  TB: " << hs->diemTBNam
                              << " (" << xepLoaiHocLuc(hs->diemTBNam) << ")\n";
                }
            }
            break;
        }

        case 10: {
            inBaoCaoHocPhiLop(lop);
            std::cout << "\nCap nhat hoc phi cho HS? (y/n): ";
            string confirm; std::getline(std::cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                std::cout << "Nhap ID hoc sinh: ";
                string id; std::getline(std::cin, id);
                std::cout << "Nhap hoc phi moi: ";
                double hp; std::cin >> hp; std::cin.ignore();
                if (capNhatHocPhi(lop, id, hp))
                    std::cout << "[OK] Da cap nhat hoc phi.\n";
                else
                    std::cout << "[!] Khong tim thay ID: " << id << ".\n";
            }
            break;
        }

        case 11: {
            string outFile = lop.tenLop + "_baocao.txt";
            if (xuatBaoCaoTxt(lop, outFile))
                std::cout << "[OK] Da xuat bao cao: " << outFile << "\n";
            else
                std::cout << "[!] Khong the ghi file: " << outFile << "\n";
            break;
        }

        case 12:
            if (ghiDanhSachLopVaoXML(dsLop, xmlFile))
                std::cout << "[OK] Da luu du lieu vao " << xmlFile << "\n";
            else
                std::cout << "[!] Loi khi luu file XML.\n";
            break;

        case 13:
            sapXepDanhSachHocSinhTheoTen(lop);
            std::cout << "[OK] Da sap xep HS theo ten (A-Z).\n";
            break;

        case 14:
            std::sort(lop.danhSachHS.begin(), lop.danhSachHS.end(),
                [](const HocSinh& a, const HocSinh& b) {
                    return a.diemTBNam > b.diemTBNam;
                });
            std::cout << "[OK] Da sap xep HS theo diem TB (cao -> thap).\n";
            break;

        case 15: {
            std::cout << "Ten lop moi (Enter de giu '" << lop.tenLop << "'): ";
            string tenMoi; std::getline(std::cin, tenMoi);
            if (!tenMoi.empty()) lop.tenLop = tenMoi;
            std::cout << "Cap nhat thong tin GVCN? (y/n): ";
            string c; std::getline(std::cin, c);
            if (c == "y" || c == "Y") {
                std::cout << "\n-- NHAP THONG TIN GVCN MOI --\n";
                lop.giaoVienCN = nhapThongTinGiaoVien();
            }
            std::cout << "[OK] Da cap nhat thong tin lop.\n";
            break;
        }

        case 16: {
            std::cout << "Nhap ID hoc sinh can xem diem chi tiet: ";
            string id; std::getline(std::cin, id);
            HocSinh* hs = timHocSinhTheoID(lop, id);
            if (!hs) std::cout << "[!] Khong tim thay ID: " << id << ".\n";
            else inDiemChiTietHocSinh(*hs);
            break;
        }

        case 0:
            break;

        default:
            std::cout << "[!] Lua chon khong hop le.\n";
        }
    } while (luaChon != 0);
}

// ----------------------------------------------------------------
// Menu chính — quản lý toàn trường
// ----------------------------------------------------------------
int main() {
    const string xmlFile = "THPT.xml";

    vector<LopHoc> dsLop = docDanhSachLopTuXML(xmlFile);
    if (dsLop.empty()) {
        dsLop.push_back(khoiTaoLopMau());
        std::cout << "[!] Khong doc duoc XML, dung du lieu mau (lop 10A1).\n";
    }

    int luaChon = 0;
    do {
        int n = (int)dsLop.size();
        std::cout << "\n========== QUAN LI HOC SINH THPT ==========\n";
        std::cout << "--- Danh sach lop ---\n";
        std::cout << std::left << std::setw(5) << "STT"
                  << std::setw(10) << "Ten lop"
                  << std::setw(28) << "GVCN"
                  << std::setw(10) << "Si so" << "\n";
        std::cout << string(55, '-') << "\n";
        for (int i = 0; i < n; ++i)
            std::cout << std::left << std::setw(5) << (i + 1)
                      << std::setw(10) << dsLop[i].tenLop
                      << std::setw(28) << dsLop[i].tenGiaoVienChuNhiem()
                      << std::setw(10) << dsLop[i].siSo() << "\n";
        std::cout << string(55, '-') << "\n";
        std::cout << "--- He thong ---\n";
        std::cout << "  " << (n + 1) << ". Them lop moi\n";
        std::cout << "  " << (n + 2) << ". Xoa lop\n";
        std::cout << "  " << (n + 3) << ". Tim kiem toan truong\n";
        std::cout << "  " << (n + 4) << ". Thong ke toan truong\n";
        std::cout << "  " << (n + 5) << ". Canh bao HS yeu/kem toan truong\n";
        std::cout << "  " << (n + 6) << ". Bao cao hoc phi toan truong\n";
        std::cout << "  0. Thoat\n";
        std::cout << "Lua chon: ";
        std::cin >> luaChon;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[!] Vui long nhap so nguyen.\n";
            luaChon = -1;
            continue;
        }
        std::cin.ignore();

        if (luaChon >= 1 && luaChon <= n) {
            menuLop(dsLop[luaChon - 1], xmlFile, dsLop);
        }
        else if (luaChon == n + 1) {
            // Thêm lớp mới
            std::cout << "\n-- THEM LOP MOI --\n";
            LopHoc lopMoi = nhapThongTinLop();
            bool trung = false;
            for (const auto& l : dsLop)
                if (l.tenLop == lopMoi.tenLop) { trung = true; break; }
            if (trung)
                std::cout << "[!] Lop " << lopMoi.tenLop << " da ton tai.\n";
            else {
                themLopVaoDanhSach(dsLop, lopMoi);
                std::cout << "[OK] Da them lop " << lopMoi.tenLop << ".\n";
            }
        }
        else if (luaChon == n + 2) {
            // Xóa lớp
            std::cout << "Nhap ten lop can xoa: ";
            string tenLop; std::getline(std::cin, tenLop);
            std::cout << "Xac nhan xoa lop " << tenLop << "? (y/n): ";
            string confirm; std::getline(std::cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                if (xoaLopTheoTen(dsLop, tenLop))
                    std::cout << "[OK] Da xoa lop " << tenLop << ".\n";
                else
                    std::cout << "[!] Khong tim thay lop: " << tenLop << ".\n";
            }
        }
        else if (luaChon == n + 3) {
            // Tìm kiếm toàn trường
            std::cout << "Nhap tu khoa (ten / ID): ";
            string tk; std::getline(std::cin, tk);
            auto ketQua = timKiemToanTruong(dsLop, tk);
            if (ketQua.empty()) {
                std::cout << "[!] Khong tim thay ket qua.\n";
            } else {
                std::cout << "Tim thay " << ketQua.size() << " ket qua:\n";
                for (const auto& kq : ketQua) {
                    const HocSinh* hs = kq.hocSinh;
                    string hoTen = hs->thongTin.ho + " " + hs->thongTin.tenDem + " " + hs->thongTin.ten;
                    std::cout << "  [Lop " << kq.tenLop << "] ["
                              << hs->thongTin.IDMaDinhDanh << "] " << hoTen
                              << "  TB: " << hs->diemTBNam
                              << " (" << xepLoaiHocLuc(hs->diemTBNam) << ")\n";
                }
            }
        }
        else if (luaChon == n + 4) {
            inThongKeToanTruong(dsLop);
        }
        else if (luaChon == n + 5) {
            inDanhSachCanhBao(dsLop);
        }
        else if (luaChon == n + 6) {
            inBaoCaoHocPhiToanTruong(dsLop);
        }
        else if (luaChon != 0) {
            std::cout << "[!] Lua chon khong hop le.\n";
        }
    } while (luaChon != 0);

    std::cout << "Tam biet!\n";
    return 0;
}
