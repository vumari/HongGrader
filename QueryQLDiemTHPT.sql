create database QLDiemTHPT
use QLDiemTHPT
 
 create table Mon (
 MaMon varchar(20) primary key not null,
 TenMon nvarchar(50) not null
 )
 create table GiaoVien (
 MaGV int IDENTITY PRIMARY KEY not null,
 TenGV nvarchar(50) not null,
 NgaySinh date not null,
 GioiTinh nvarchar(10) not null,
 DiaChi nvarchar(100) not null,
 SDT int not null,
 Email nvarchar(50) not null,
 )
 create table NamHoc(
 TenNamHoc varchar(30) primary key not null
 )

 create table Lop (
 MaLop int IDENTITY PRIMARY KEY not null,
 TenLop varchar(20) not null,
 MaGVCN int not null references GiaoVien(MaGV),
 TenNamHoc varchar(30) not null references NamHoc(TenNamHoc)
 )

 create table HocSinh (
 MaHS int IDENTITY PRIMARY KEY not null,--chuỗi tự tăng 
 HoTen nvarchar(50) not null,
 NgaySinh date not null,
 GioiTinh nvarchar(10) not null,
 DanToc nvarchar(10) not null,
 NoiSinh nvarchar(100) not null
 )
 create table HocKi (
 MaHK int IDENTITY PRIMARY KEY not null,
 TenHK nvarchar(20) not null, --"Học kì 1" và "Học kì 2"--
 )
 
-- Bảng Diem 
-- Cập nhật bảng DiemTongHop với các cột mới cho các lần điểm
CREATE TABLE DiemTongHop (
    MaHS int NOT NULL REFERENCES HocSinh(MaHS),
    MaMon varchar(20) NOT NULL REFERENCES Mon(MaMon),
    MaHK int NOT NULL REFERENCES HocKi(MaHK),
	TenNamHoc varchar(30) not null references NamHoc(TenNamHoc),
    Diem15p_1 float NULL,  -- Điểm 15 phút lần 1
    Diem15p_2 float NULL,  -- Điểm 15 phút lần 2
    Diem15p_3 float NULL,  -- Điểm 15 phút lần 3
    Diem45p_1 float NULL,  -- Điểm 45 phút lần 1
    Diem45p_2 float NULL,  -- Điểm 45 phút lần 2
    DiemTB float NULL,     -- Điểm trung bình
    KetQua nvarchar(10) CHECK (KetQua IN ('Dat', 'Khong Dat')),  -- Kết quả đạt/không đạt
    PRIMARY KEY (MaHS, MaMon, MaHK,TenNamHoc)
);


create table TaiKhoan (
userName nvarchar(50) primary key not null,
passWord nvarchar(50) not null
)

-- truy xuất mã hs, họ tên,tên môn,học kì,tên năm học, điểm 15p,45p,điểm tb,kết quả của môn văn
SELECT 
    hs.MaHS,
    hs.HoTen,
    mon.TenMon,
    hk.TenHK,               -- Học kỳ
    dt.TenNamHoc,           -- Năm học
    -- Điểm 15 phút lần 1
    dt.Diem15p_1 AS Diem15P_Lan1,
    -- Điểm 15 phút lần 2
    dt.Diem15p_2 AS Diem15P_Lan2,
    -- Điểm 15 phút lần 3
    dt.Diem15p_3 AS Diem15P_Lan3,
    -- Điểm 45 phút lần 1
    dt.Diem45p_1 AS Diem45P_Lan1,
    -- Điểm 45 phút lần 2
    dt.Diem45p_2 AS Diem45P_Lan2,
    -- Tính điểm trung bình từ các điểm 15p và 45p
    CASE 
        WHEN dt.Diem15p_1 IS NOT NULL AND dt.Diem45p_1 IS NOT NULL THEN (dt.Diem15p_1 + dt.Diem45p_1) / 2
        WHEN dt.Diem15p_2 IS NOT NULL AND dt.Diem45p_2 IS NOT NULL THEN (dt.Diem15p_2 + dt.Diem45p_2) / 2
        WHEN dt.Diem15p_3 IS NOT NULL THEN dt.Diem15p_3
        ELSE NULL
    END AS DiemTB,
    -- Kết quả đạt/không đạt
    dt.KetQua
FROM 
    HocSinh hs
JOIN 
    DiemTongHop dt ON hs.MaHS = dt.MaHS
JOIN 
    Mon mon ON dt.MaMon = mon.MaMon
JOIN 
    HocKi hk ON dt.MaHK = hk.MaHK
WHERE 
    mon.MaMon = 'Toan'  -- thay tùy mã
GROUP BY 
    hs.MaHS, hs.HoTen, mon.TenMon, hk.TenHK, dt.TenNamHoc, dt.Diem15p_1, dt.Diem15p_2, dt.Diem15p_3, dt.Diem45p_1, dt.Diem45p_2, dt.DiemTB, dt.KetQua;

--=============================================--
-- Cập nhật điểm cho học sinh
UPDATE DiemTongHop
SET 
    Diem15p_1 = 8.0, 
    Diem15p_2 = 7.5, 
    Diem15p_3 = 8.5,
    Diem45p_1 = 7.0, 
    Diem45p_2 = 8.0,
    DiemTB = (8.0 + 7.0) / 2,  -- Ví dụ tính điểm trung bình từ điểm 15p và 45p
    KetQua = 'Dat'
WHERE 
    MaHS = 1 AND MaMon = 'Văn' AND MaHK = 'HK1_24-25';




--================================================
--Thêm một số dữ liệu mặc định
--Môn
INSERT INTO Mon (MaMon, TenMon) VALUES
('Toan', 'Toán'),
('Van', 'Ngữ Văn'),
('Anh', 'Tiếng Anh'),
('Ly', 'Vật Lý'),
('Hoa', 'Hóa Học'),
('Sinh', 'Sinh Học'),
('Su', 'Lịch Sử'),
('Dia', 'Địa Lý'),
('GDCD', 'Giáo Dục Công Dân'),
('Tin', 'Tin Học'),
('CN', 'Công Nghệ'),
('NN', 'Ngoại Ngữ Khác'),
('TheDuc', 'Thể Dục');
--Giáo viên
INSERT INTO GiaoVien (TenGV, NgaySinh, GioiTinh, DiaChi, SDT, Email) VALUES
( 'Nguyen Van A', '1980-03-12', 'Nam', 'Hà Nội', 1234567890, 'nguyenvana@gmail.com'),
('Tran Thi B', '1985-07-23', 'Nữ', 'Hà Nội', 1234567891, 'tranthib@gmail.com'),
('Le Van C', '1978-11-02', 'Nam', 'Hải Phòng', 1234567892, 'levanc@gmail.com'),
('Hoang Thi D', '1983-09-05', 'Nữ', 'Đà Nẵng', 1234567893, 'hoangthid@gmail.com'),
('Pham Van E', '1979-06-20', 'Nam', 'TP.HCM', 1234567894, 'phamvane@gmail.com'),
('Đo Thi F', '1986-02-14', 'Nữ', 'Hà Nội', 1234567895, 'dothif@gmail.com'),
('Nguyen Thi G', '1981-01-01', 'Nữ', 'Huế', 1234567896, 'nguyenthig@gmail.com'),
( 'Truong Văn H', '1977-12-11', 'Nam', 'Cần Thơ', 1234567897, 'truongvanh@gmail.com'),
('Vu Văn I', '1982-05-18', 'Nam', 'Quảng Ninh', 1234567898, 'vuvani@gmail.com'),
('Luong Thị J', '1984-08-29', 'Nữ', 'Hà Nội', 1234567899, 'luongthij@gmail.com'),
('Phan Văn K', '1987-04-22', 'Nam', 'Hà Tĩnh', 1234567800, 'phanvank@gmail.com'),
( 'Nguyen Thị L', '1980-10-30', 'Nữ', 'Đà Lạt', 1234567801, 'nguyenthil@gmail.com'),
('Bui Van M', '1983-03-03', 'Nam', 'Phú Thọ', 1234567802, 'buivanm@gmail.com');
-- Thêm các năm học vào bảng NamHoc
INSERT INTO NamHoc (TenNamHoc) VALUES
('2024-2025'),
('2025-2026'),
('2026-2027');

--Lớp
-- Lớp 10
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('10C1', 1, '2024-2025'),
('10C2', 2, '2024-2025'),
('10C3', 3, '2024-2025'),
('10C4', 4, '2024-2025'),
('10C5', 5, '2024-2025');

-- Lớp 11
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('11B1', 6, '2024-2025'),
('11B2', 7, '2024-2025'),
('11B3', 8, '2024-2025'),
('11B4', 9, '2024-2025'),
('11B5', 10, '2024-2025'),
('11B6', 1, '2024-2025'),
('11B7', 2, '2024-2025'),
('11B8', 3, '2024-2025'),
('11B9', 4, '2024-2025'),
('11B10', 5, '2024-2025');

-- Lớp 12
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('12A1', 6, '2024-2025'),
('12A2', 7, '2024-2025'),
('12A3', 8, '2024-2025'),
('12A4', 9, '2024-2025'),
('12A5', 10, '2024-2025'),
('12A6', 1, '2024-2025'),
('12A7', 2, '2024-2025'),
('12A8', 3, '2024-2025'),
('12A9', 4, '2024-2025'),
('12A10', 5, '2024-2025');

--Học sinh( Mã học sinh sẽ là chuỗi int tự tăng nên ko cần nhập
-- Them hoc sinh vao bang HocSinh
INSERT INTO HocSinh (HoTen, NgaySinh, GioiTinh, DanToc, NoiSinh) VALUES
('Nguyen Van A', '2007-05-10', 'Nam', 'Kinh', 'Ha Noi'),
('Tran Thi B', '2007-03-12', 'Nu', 'Kinh', 'TP Ho Chi Minh'),
('Pham Minh C', '2007-07-22', 'Nam', 'Tay', 'Lao Cai'),
('Le Thu D', '2007-09-15', 'Nu', 'Tay', 'Ha Giang'),
('Doan Huong E', '2007-01-20', 'Nu', 'Muong', 'Thanh Hoa');

--Học kì
INSERT INTO HocKi ( TenHK) VALUES
( 'Học Kì 1'),
( 'Học Kì 2');
--Tài khoản
INSERT INTO TaiKhoan (userName, passWord) VALUES
('1', '1')
--Thêm điểm
-- Them diem cho hoc sinh vao bang DiemTongHop
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, Diem15p_1, Diem15p_2, Diem15p_3, Diem45p_1, Diem45p_2, DiemTB, KetQua) VALUES
(1, 'Toan', 1, '2024-2025', 7.5, 8.0, NULL, 6.5, 7.0, 7.25, 'Dat'),
(2, 'Ly', 1, '2024-2025', 8.0, 7.5, 9.0, 8.5, 7.0, 7.75, 'Dat'),
(3, 'Hoa', 1, '2024-2025', NULL, 6.5, 7.0, 8.0, 7.5, 7.0, 'Dat'),
(4, 'Sinh', 2, '2024-2025', 6.0, 7.0, NULL, 7.5, 8.0, 7.25, 'Khong Dat'),
(5, 'Su', 2, '2024-2025', 8.5, 9.0, 7.5, 7.0, 8.0, 8.0, 'Dat');






