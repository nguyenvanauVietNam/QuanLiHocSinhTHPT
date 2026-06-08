# 🚀 HƯỚNG DẪN CẢI THIỆN REPORT - PHẦN THỰC HÀNH

## **BƯỚC 1: Thêm Header Chuyên Nghiệp**

### Code cũ (Header.h - dòng 784):
```cpp
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    
    // ... (code cũ)
    ofs << "BAO CAO CHI TIET LOP: " << lop.tenLop << "\n";
```

### Code mới - THÊM FUNCTION HELPER:
```cpp
// Thêm hàm helper tạo header chuyên nghiệp
inline void writeReportHeader(std::ofstream& ofs, const LopHoc& lop) {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", &tm);
    
    ofs << string(100, '=') << "\n";
    ofs << std::right << std::setw(50) << "TRƯỜNG THPT [TÊN TRƯỜNG]\n";
    ofs << std::right << std::setw(52) << "BÁO CÁO HỌC SINH\n";
    ofs << string(100, '=') << "\n";
    ofs << "Ngày in báo cáo: " << timeStr << "\n";
    ofs << "Lớp:             " << lop.tenLop << "\n";
    ofs << "GVCN:            " << lop.tenGiaoVienChuNhiem() << "\n";
    ofs << "Môn dạy:         " << lop.giaoVienCN.monGiangDay << "\n";
    ofs << "Số HS:           " << lop.siSo() << "\n";
    ofs << string(100, '=') << "\n\n";
}

// Cập nhật function xuatBaoCaoTxt:
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    
    // 👈 THAY THÀNH:
    writeReportHeader(ofs, lop);
    
    // ... rest of code remains same but DELETE old header code
}
```

---

## **BƯỚC 2: Thêm Thống Kê Chi Tiết Với Tỷ Lệ %**

### Code cũ:
```cpp
ofs << "Hoc luc             : Gioi " << tk.soGioi << ", Kha " << tk.soKha
    << ", TB " << tk.soTrungBinh << ", Yeu " << tk.soYeu << ", Kem " << tk.soKem << "\n";
```

### Code mới:
```cpp
// Thêm hàm tính tỷ lệ:
inline void writeThongKeHocLuc(std::ofstream& ofs, const ThongKeHocLuc& tk, int totalHS) {
    if (totalHS == 0) totalHS = 1; // Avoid division by zero
    
    ofs << "\nTHỐNG KÊ HỌC LỰC LỚP:\n";
    ofs << string(60, '-') << "\n";
    ofs << std::left << std::setw(12) << "Xếp loại"
        << std::setw(10) << "Số HS"
        << std::setw(15) << "Tỷ lệ (%)"
        << "Biểu diễn\n";
    ofs << string(60, '-') << "\n";
    
    auto printRow = [&](const string& label, int count) {
        double percent = (count * 100.0) / totalHS;
        int bars = count / 2; // Simple bar chart
        ofs << std::left << std::setw(12) << label
            << std::setw(10) << count
            << std::setw(15) << std::fixed << std::setprecision(1) << percent
            << string(bars, '█') << "\n";
    };
    
    printRow("Giỏi", tk.soGioi);
    printRow("Khá", tk.soKha);
    printRow("Trung bình", tk.soTrungBinh);
    printRow("Yếu", tk.soYeu);
    printRow("Kém", tk.soKem);
    
    ofs << string(60, '-') << "\n";
    ofs << std::left << std::setw(12) << "TỔNG"
        << std::setw(10) << totalHS
        << std::setw(15) << "100.0%\n";
    ofs << string(60, '-') << "\n\n";
}

// Dùng trong xuatBaoCaoTxt:
int daDongHocPhi = 0;
double tongHocPhi = 0.0;
for (const auto& hs : lop.danhSachHS) {
    if (hs.hocPhi > 0.0) daDongHocPhi++;
    tongHocPhi += hs.hocPhi;
}

writeReportHeader(ofs, lop);
writeThongKeHocLuc(ofs, tk, lop.siSo());  // 👈 THÊM DÒNG NÀY

// ... rest remains same
```

---

## **BƯỚC 3: Fix Bảng Dữ Liệu - Tính Width Động**

### Code cũ (WRONG - hard-coded 130):
```cpp
ofs << string(130, '-') << "\n";  // ❌ Không responsive
ofs << std::left
    << std::setw(5)  << "STT"
    << std::setw(12) << "ID"
    // ...
    << "Phu huynh"  // ❌ KHÔNG CÓ SETW - LỖI FORMATTING!
    << "\n" << string(130, '-') << "\n";
```

### Code mới - TẠO UTILITY CLASS:

```cpp
// Thêm vào Header.h (sau struct definitions)
class TableFormatter {
public:
    struct Column {
        string header;
        int width;
        bool isFixed;  // true = cắt ngắn, false = auto wrap
    };
    
private:
    vector<Column> columns;
    int totalWidth;
    
public:
    TableFormatter() : totalWidth(0) {}
    
    void addColumn(const string& header, int width, bool isFixed = true) {
        columns.push_back({header, width, isFixed});
        totalWidth += width;
    }
    
    string printHeader() {
        string result = printSeparator();
        for (const auto& col : columns) {
            result += std::left;
            result += std::setw(col.width);
            result += col.header;
        }
        result += "\n" + printSeparator();
        return result;
    }
    
    string printSeparator() {
        return string(totalWidth, '-') + "\n";
    }
    
    // Tiện ích padding string
    string padColumn(const string& text, int width, bool isFixed) {
        if (isFixed && text.length() > width - 3) {
            return text.substr(0, width - 3) + "...";
        }
        return std::left;
    }
};

// Cách dùng:
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    
    writeReportHeader(ofs, lop);
    
    // TẠO BẢNG ĐỘNG:
    TableFormatter table;
    table.addColumn("STT", 5);
    table.addColumn("ID", 12);
    table.addColumn("Họ và tên", 28);
    table.addColumn("Ngày sinh", 12);
    table.addColumn("Giới tính", 10);
    table.addColumn("Khối", 10);
    table.addColumn("TB HK1", 8);
    table.addColumn("TB HK2", 8);
    table.addColumn("TB Năm", 8);
    table.addColumn("Học lực", 12);
    table.addColumn("HK năm", 12);
    table.addColumn("Học phí (VND)", 15);
    table.addColumn("Phụ huynh", 20);  // 👈 GIỜ CÓ WIDTH RỒI!
    
    ofs << table.printHeader();
    
    // ... in dữ liệu học sinh ...
    
    ofs << table.printSeparator();
    ofs.close();
    return true;
}
```

---

## **BƯỚC 4: Thêm Validation Dữ Liệu**

### Code mới - Thêm vào Header.h:
```cpp
struct BaoCaoValidation {
    vector<string> errors;
    vector<string> warnings;
    
    bool isValid() const { return errors.empty(); }
    
    void addError(const string& msg) {
        errors.push_back("❌ " + msg);
    }
    
    void addWarning(const string& msg) {
        warnings.push_back("⚠️  " + msg);
    }
    
    void print(std::ostream& out = std::cout) {
        if (!errors.empty()) {
            out << "\n=== LỖIS NGHIÊM TRỌNG ===\n";
            for (const auto& e : errors) out << e << "\n";
        }
        if (!warnings.empty()) {
            out << "\n=== CẢNH BÁO ===\n";
            for (const auto& w : warnings) out << w << "\n";
        }
    }
};

inline BaoCaoValidation validateLopBeforeReport(const LopHoc& lop) {
    BaoCaoValidation result;
    
    if (lop.danhSachHS.empty()) {
        result.addError("Lớp không có học sinh");
        return result;
    }
    
    for (int i = 0; i < (int)lop.danhSachHS.size(); ++i) {
        const auto& hs = lop.danhSachHS[i];
        
        // Kiểm tra ID
        if (hs.thongTin.IDMaDinhDanh.empty()) {
            result.addError("HS #" + std::to_string(i+1) + " không có ID");
        }
        
        // Kiểm tra ngày sinh
        if (!laNgaySinhHopLe(hs.thongTin.ngaySinh)) {
            result.addWarning("HS [" + hs.thongTin.IDMaDinhDanh 
                            + "] ngày sinh không đúng format (DD/MM/YYYY)");
        }
        
        // Kiểm tra điểm
        if (hs.diemTBNam > 10.0f) {
            result.addError("HS [" + hs.thongTin.IDMaDinhDanh 
                          + "] điểm TB > 10: " + std::to_string(hs.diemTBNam));
        }
        
        // Kiểm tra một môn học
        for (const auto& mon : hs.danhSachMon) {
            if (mon.tenMonHoc.empty()) {
                result.addWarning("HS [" + hs.thongTin.IDMaDinhDanh 
                                + "] có môn học không có tên");
            }
        }
    }
    
    // Kiểm tra GVCN
    if (lop.giaoVienCN.thongTin.ho.empty()) {
        result.addWarning("Lớp " + lop.tenLop + " GVCN không có tên");
    }
    
    return result;
}

// Cập nhật xuatBaoCaoTxt:
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    // ✅ KIỂM TRA DỮ LIỆU TRƯỚC:
    auto validation = validateLopBeforeReport(lop);
    if (!validation.isValid()) {
        validation.print(std::cerr);
        return false;  // Dừng nếu có lỗi nghiêm trọng
    }
    
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    
    writeReportHeader(ofs, lop);
    
    // ... rest of code ...
    ofs.close();
    return true;
}
```

---

## **BƯỚC 5: Thêm Footer & Ghi Chú**

### Code mới - Thêm hàm:
```cpp
inline void writeReportFooter(std::ofstream& ofs, const LopHoc& lop) {
    ofs << "\n" << string(100, '=') << "\n";
    ofs << "GHI CHÚ:\n";
    ofs << "- Điểm TB năm = (TB_HK1 + TB_HK2 × 2) / 3 (Thông tư 22/2021)\n";
    ofs << "- Xếp loại: Giỏi ≥ 8.0, Khá 6.5-7.9, TB 5.0-6.4, Yếu 3.5-4.9, Kém < 3.5\n";
    ofs << "- Hạnh kiểm: Tốt ≥ 8.0, Khá 6.5-7.9, TB 5.0-6.4, Yếu < 5.0\n";
    ofs << "- Báo cáo này được in tự động, có giá trị nội bộ\n";
    ofs << string(100, '=') << "\n";
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char dateStr[20];
    std::strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", &tm);
    
    ofs << "\n";
    ofs << "Ngày: " << dateStr << "\n";
    ofs << "Người lập báo cáo: ___________________\n";
    ofs << "Giáo viên chủ nhiệm: ___________________\n";
    ofs << "Hiệu trưởng: ___________________\n";
}

// Gọi trong xuatBaoCaoTxt:
writeReportFooter(ofs, lop);
```

---

## **BƯỚC 6: Thêm Hỗ Trợ Thống Kê Học Phí Chi Tiết**

### Code mới:
```cpp
inline void writeHocPhiStatistics(std::ofstream& ofs, const LopHoc& lop) {
    double tongThu = 0.0, tongConNo = 0.0;
    int daDong = 0, chuaDong = 0;
    
    for (const auto& hs : lop.danhSachHS) {
        if (hs.hocPhi > 0) { daDong++; tongThu += hs.hocPhi; }
        else { chuaDong++; tongConNo += 500000.0; }  // Giả định mỗi HS 500k
    }
    
    ofs << "\nTHỐNG KÊ HỌC PHÍ:\n";
    ofs << string(60, '-') << "\n";
    ofs << "Đã nộp:      " << daDong << " HS  |  "
        << std::fixed << std::setprecision(0) << tongThu << " VND\n";
    ofs << "Chưa nộp:    " << chuaDong << " HS  |  "
        << tongConNo << " VND (dự kiến)\n";
    ofs << "Tổng:        " << lop.siSo() << " HS  |  "
        << (tongThu + tongConNo) << " VND\n";
    ofs << string(60, '-') << "\n\n";
}
```

---

## **BƯỚC 7: QUICK REFERENCE - ĐỆ NHẤT THAY ĐỔI**

### Chỉnh sửa trong xuatBaoCaoTxt (khoảng dòng 784-860):

```cpp
inline bool xuatBaoCaoTxt(const LopHoc& lop, const string& fileName) {
    // BƯỚC 1: Validate
    auto validation = validateLopBeforeReport(lop);
    if (!validation.isValid()) {
        validation.print();
        return false;
    }
    
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return false;
    
    ThongKeHocLuc tk = thongKeHocLucLop(lop);
    int daDongHocPhi = 0;
    double tongHocPhi = 0.0;
    
    for (const auto& hs : lop.danhSachHS) {
        if (hs.hocPhi > 0.0) daDongHocPhi++;
        tongHocPhi += hs.hocPhi;
    }
    
    // BƯỚC 2: Header mới
    writeReportHeader(ofs, lop);
    
    // BƯỚC 3: Thống kê chi tiết
    writeThongKeHocLuc(ofs, tk, lop.siSo());
    writeHocPhiStatistics(ofs, lop);
    
    // BƯỚC 4: Bảng danh sách (giữ code cũ nhưng fix formatting)
    ofs << string(130, '-') << "\n";
    ofs << std::left
        << std::setw(5)  << "STT"
        << std::setw(12) << "ID"
        << std::setw(28) << "Ho va ten"
        << std::setw(12) << "Ngay sinh"
        << std::setw(10) << "Gioi tinh"
        << std::setw(10) << "Khoi"
        << std::setw(8)  << "TB HK1"
        << std::setw(8)  << "TB HK2"
        << std::setw(8)  << "TB Nam"
        << std::setw(12) << "Hoc luc"
        << std::setw(12) << "HK nam"
        << std::setw(15) << "Hoc phi"
        << std::setw(20) << "Phu huynh"  // ✅ THÊM SETW!
        << "\n" << string(130, '-') << "\n";
    
    // ... rest of old code ...
    
    // BƯỚC 5: Footer
    writeReportFooter(ofs, lop);
    
    ofs.close();
    return true;
}
```

---

## **BƯỚC 8: TESTING**

Sau khi implement, test với:

```cpp
// Main.cpp - test code
LopHoc lop = dsLop[0];
string testFile = "test_report.txt";

auto validation = validateLopBeforeReport(lop);
if (validation.isValid()) {
    if (xuatBaoCaoTxt(lop, testFile)) {
        std::cout << "✅ Báo cáo xuất thành công: " << testFile << "\n";
    }
}
```

---

## **THỜI GIAN IMPLEMENT**

| Bước | Nhiệm vụ | Thời gian |
|------|----------|----------|
| 1 | Thêm header | 15 min |
| 2 | Thống kê % | 20 min |
| 3 | Fix table formatting | 20 min |
| 4 | Validation | 15 min |
| 5 | Footer | 10 min |
| 6 | Học phí chi tiết | 15 min |
| 7 | Test & fix bugs | 20 min |
| **TỔNG** | | **~2 giờ 10 phút** |

**Sau hoàn thành → Báo cáo sẽ chuyên nghiệp hơn 80%!**

---
