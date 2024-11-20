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
 MaMon varchar(20) not null references Mon(MaMon)
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
 CREATE TABLE ChiTietHocSinh_Lop (
    MaHS INT NOT NULL REFERENCES HocSinh(MaHS),      
    MaLop INT NOT NULL REFERENCES Lop(MaLop),       
    PRIMARY KEY (MaHS, MaLop)                     
);

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
	MaLop int not null references Lop(MaLop),
    tx1 float NULL,  -- Điểm thường xuyên lần 1
    tx2 float NULL,  -- Điểm thường xuyên lần 2
    tx3 float NULL,  --
    gk float NULL,  -- Giua ki
    ck float NULL,  -- Cuoi Ki
    DiemTB float NULL,     -- Điểm trung bình
    KetQua nvarchar(10) CHECK (KetQua IN ('Dat', 'Khong Dat')),  -- Kết quả đạt/không đạt
    PRIMARY KEY (MaHS, MaMon, MaHK,MaLop)
);


create table TaiKhoan (
userName nvarchar(50) primary key not null,
passWord nvarchar(50) not null
)

-- truy xuất mã hs, họ tên,tên môn,học kì,tên năm học, điểm tx1,tx2.yx3,gk,ck,điểm tb,kết quả của môn toán
SELECT 
    hs.MaHS,
    hs.HoTen,
    mon.TenMon,
    hk.TenHK,               -- Học kỳ
    l.TenNamHoc,            -- Năm học
    dt.tx1 AS DiemTX_Lan1,
    dt.tx2 AS DiemTX_Lan2,
    dt.tx3 AS DiemTX_Lan3,
    dt.gk AS DiemGiuaKi,
    dt.ck AS DiemCuoiKi,
    CASE 
        WHEN dt.tx1 IS NOT NULL AND dt.tx2 IS NOT NULL AND dt.tx3 IS NOT NULL 
             AND dt.gk IS NOT NULL AND dt.ck IS NOT NULL 
        THEN 
            ROUND(
                (dt.tx1 + dt.tx2 + dt.tx3) * 0.1 / 3 +  -- Điểm thường xuyên chiếm 10%
                dt.gk * 0.3 +                           -- Điểm giữa kỳ chiếm 30%
                dt.ck * 0.6, 2                          -- Điểm cuối kỳ chiếm 60%
            , 2)
        ELSE 
            NULL  -- Nếu thiếu bất kỳ điểm nào, để NULL
    END AS DiemTB,              -- Điểm trung bình đã tính nếu có đủ điểm
    dt.KetQua                   -- Kết quả đạt/không đạt
FROM 
    HocSinh hs
JOIN 
    DiemTongHop dt ON hs.MaHS = dt.MaHS
JOIN 
    Mon mon ON dt.MaMon = mon.MaMon
JOIN 
    HocKi hk ON dt.MaHK = hk.MaHK
JOIN 
    Lop l ON dt.MaLop = l.MaLop
WHERE 
    mon.MaMon = 'Toan';  -- Lọc theo mã môn học là 'Toán'



--=============================================--
-- Cập nhật điểm cho học sinh
UPDATE DiemTongHop
SET 
    Diem15p_1 = 8.0, 
    Diem15p_2 = 7.5, 
    Diem15p_3 = 8.5,
    Diem45p_1 = 7.0, 
    Diem45p_2 = 8.0,
    KetQua = 'Dat'
WHERE 
    MaHS = 1 AND MaMon = 'Van' AND MaHK = 1;


--Khi một học sinh chuyển lớp :
DELETE FROM ChiTietHocSinh_Lop
WHERE MaHS = 1;  -- Xóa học sinh có mã 1 khỏi các lớp cũ
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES (1, 6);  -- Thêm học sinh vào lớp mới


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
INSERT INTO GiaoVien (TenGV, NgaySinh, GioiTinh, DiaChi, SDT, Email,MaMon) VALUES
( 'Nguyen Van A', '1980-03-12', 'Nam', 'Hà Nội', 1234567890, 'nguyenvana@gmail.com','Toan'),
('Tran Thi B', '1985-07-23', 'Nữ', 'Hà Nội', 1234567891, 'tranthib@gmail.com','Ly'),
('Le Van C', '1978-11-02', 'Nam', 'Hải Phòng', 1234567892, 'levanc@gmail.com','Hoa'),
('Hoang Thi D', '1983-09-05', 'Nữ', 'Đà Nẵng', 1234567893, 'hoangthid@gmail.com','Sinh'),
('Pham Van E', '1979-06-20', 'Nam', 'TP.HCM', 1234567894, 'phamvane@gmail.com','Su'),
('Đo Thi F', '1986-02-14', 'Nữ', 'Hà Nội', 1234567895, 'dothif@gmail.com','Dia'),
('Nguyen Thi G', '1981-01-01', 'Nữ', 'Huế', 1234567896, 'nguyenthig@gmail.com','GDCD'),
( 'Truong Văn H', '1977-12-11', 'Nam', 'Cần Thơ', 1234567897, 'truongvanh@gmail.com','Tin'),
('Vu Văn I', '1982-05-18', 'Nam', 'Quảng Ninh', 1234567898, 'vuvani@gmail.com','CN'),
('Luong Thị J', '1984-08-29', 'Nữ', 'Hà Nội', 1234567899, 'luongthij@gmail.com','NN'),
('Phan Văn K', '1987-04-22', 'Nam', 'Hà Tĩnh', 1234567800, 'phanvank@gmail.com','TheDuc'),
( 'Nguyen Thị L', '1980-10-30', 'Nữ', 'Đà Lạt', 1234567801, 'nguyenthil@gmail.com','Van'),
('Bui Van M', '1983-03-03', 'Nam', 'Phú Thọ', 1234567802, 'buivanm@gmail.com','Anh');
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
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, MaLop, tx1, tx2, tx3, gk, ck, DiemTB, KetQua) VALUES
(1, 'Toan', 1, 1, 7.5, 8.0, NULL, 6.5, 7.0, 7.25, 'Dat'),
(2, 'Ly', 1, 2, 8.0, 7.5, 9.0, 8.5, 7.0, 7.75, 'Dat'),
(3, 'Hoa', 1, 3, NULL, 6.5, 7.0, 8.0, 7.5, 7.0, 'Dat'),
(4, 'Sinh', 2, 4, 6.0, 7.0, NULL, 7.5, 8.0, 7.25, 'Khong Dat'),
(5, 'Su', 2, 5, 8.5, 9.0, 7.5, 7.0, 8.0, 8.0, 'Dat');

--
-- Học sinh 1
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop) VALUES
(1, 1),  -- Học sinh 1 học lớp 10C1
(1, 6),  -- Học sinh 1 học lớp 11B1
(1, 11); -- Học sinh 1 học lớp 12A1
-- Học sinh 2
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop) VALUES
(2, 2),  -- Học sinh 2 học lớp 10C2
(2, 7),  -- Học sinh 2 học lớp 11B2
(2, 12); -- Học sinh 2 học lớp 12A2

-- Học sinh 3
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop) VALUES
(3, 3),  -- Học sinh 3 học lớp 10C3
(3, 8),  -- Học sinh 3 học lớp 11B3
(3, 13); -- Học sinh 3 học lớp 12A3

-- Học sinh 4
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop) VALUES
(4, 4),  -- Học sinh 4 học lớp 10C4
(4, 9),  -- Học sinh 4 học lớp 11B4
(4, 14); -- Học sinh 4 học lớp 12A4

-- Học sinh 5
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop) VALUES
(5, 5),  -- Học sinh 5 học lớp 10C5
(5, 10), -- Học sinh 5 học lớp 11B5
(5, 15); -- Học sinh 5 học lớp 12A5

