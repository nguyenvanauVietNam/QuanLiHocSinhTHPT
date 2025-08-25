# Tài liệu đặc tả yêu cầu phần mềm (SRS)  
## Phần mềm quản lý học sinh THPT  

### 1. Giới thiệu  
#### 1.1 Mục đích  
Phần mềm này được thiết kế để quản lý học sinh THPT tại Việt Nam, sử dụng ngôn ngữ lập trình C++ (không áp dụng lập trình hướng đối tượng, chỉ sử dụng `struct`). Phần mềm cung cấp các chức năng quản lý học sinh, giáo viên, lớp học, môn học, điểm số và phân ban.  

#### 1.2 Phạm vi  
Phần mềm tập trung vào:  
- Quản lý thông tin học sinh, bao gồm điểm số, học phí và môn học.  
- Quản lý thông tin giáo viên, bao gồm bộ môn và trách nhiệm chủ nhiệm lớp.  
- Quản lý thông tin lớp học, bao gồm danh sách học sinh và giáo viên chủ nhiệm.  
- Quản lý thông tin môn học, bao gồm tên môn học và điểm số.  
- Quản lý điểm số, bao gồm loại điểm và điểm số cụ thể.  
- Phân loại học sinh theo các ban (cơ bản, tự nhiên, xã hội).  

### 2. Yêu cầu chức năng  
#### 2.1 Quản lý học sinh  
- Lưu trữ và quản lý thông tin học sinh:  
  - Họ, tên đệm, tên.  
  - Ngày/tháng/năm sinh.  
  - Giới tính.  
  - Thông tin phụ huynh.  
  - Học phí.  
  - Điểm (môn học và hạnh kiểm) và môn học.  

#### 2.2 Quản lý giáo viên  
- Lưu trữ và quản lý thông tin giáo viên:  
  - Họ, tên đệm, tên.  
  - Ngày/tháng/năm sinh.  
  - Giới tính.  
  - Bộ môn giảng dạy.  
  - Lớp chủ nhiệm.  

#### 2.3 Quản lý lớp học  
- Lưu trữ và quản lý thông tin lớp học:  
  - Danh sách học sinh trong lớp.  
  - Giáo viên chủ nhiệm.  

#### 2.4 Quản lý môn học  
- Lưu trữ và quản lý thông tin môn học:  
  - Tên môn học.  
  - Điểm số liên quan đến môn học.  

#### 2.5 Quản lý điểm số  
- Lưu trữ và quản lý điểm số:  
  - Loại điểm (ví dụ: Hạnh kiểm/ học lực).  
  - Điểm số cụ thể.  


### 3. Yêu cầu phi chức năng  
- Phần mềm phải được lập trình bằng C++ sử dụng `struct` (không áp dụng OOP).  
- Phần mềm cần đảm bảo hiệu suất tốt và xử lý được tối đa 1.000 học sinh.  
- Giao diện người dùng sẽ dựa trên dòng lệnh (console).  

### 4. Thiết kế cấu trúc dữ liệu  
#### 4.1 Định nghĩa Struct  
- **Học sinh**:  
  - Họ, tên đệm, tên, ngày/tháng/năm sinh, giới tính, thông tin phụ huynh, học phí, điểm số và môn học.  
- **Giáo viên**:  
  - Họ, tên đệm, tên, ngày/tháng/năm sinh, giới tính, bộ môn giảng dạy, lớp chủ nhiệm.  
- **Lớp học**:  
  - Danh sách học sinh và giáo viên chủ nhiệm.  
- **Môn học**:  
  - Tên môn học và điểm số liên quan.  
- **Điểm**:  
  - Loại điểm và điểm số cụ thể.  
- **Phân ban**:  
  - Loại ban (cơ bản, tự nhiên, xã hội).  

### 5. Ràng buộc  
- Phần mềm không sử dụng lập trình hướng đối tượng (OOP).  
- Tất cả dữ liệu được quản lý bằng `struct` và mảng.  
- Phần mềm chạy trên môi trường dòng lệnh Windows.  

### 6. Nâng cấp trong tương lai  
- Hỗ trợ xuất dữ liệu ra file (ví dụ: CSV).  
- Thêm chức năng tìm kiếm và lọc thông tin học sinh, giáo viên.  
- Phát triển giao diện đồ họa (console).  

### 7. Tác giả  
- Phát triển bởi: [Nicolas Nguyen Van Au]  
- Liên hệ: [honguyentenau@gmail.com]