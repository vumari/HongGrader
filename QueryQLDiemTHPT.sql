create database QLDiemTHPT
go
use QLDiemTHPT
go
create table Mon (
MaMon varchar(20) primary key not null,
TenMon nvarchar(50) not null
)
 create table GiaoVien (
 MaGV int IDENTITY PRIMARY KEY not null,
 TenGV nvarchar(50) not null,
 NgaySinh date not null,
 GioiTinh bit not null,
 DiaChi nvarchar(100) not null,
 SDT varchar(10) not null,
 Email nvarchar(50) not null,
 MaMon varchar(20) not null references Mon(MaMon)
 )
 create table NamHoc(
 TenNamHoc varchar(30) primary key not null,
 TrangThai bit not null
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
 GioiTinh bit not null,
 DanToc nvarchar(16) not null,
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
    TenNamHoc varchar(30) not null references NamHoc(TenNamHoc),
    tx1 float NULL,  -- Điểm thường xuyên lần 1
    tx2 float NULL,  -- Điểm thường xuyên lần 2
    tx3 float NULL,  -- Điểm thường xuyên lần 3
	tx4 float NULL,  -- Điểm thường xuyên lần 4
    gk float NULL,  -- Giua ki
    ck float NULL,  -- Cuoi Ki
    DiemTB float NULL,     -- Điểm trung bình
    KetQua nvarchar(10) CHECK (KetQua IN ('Dat', 'Khong Dat')),  -- Kết quả đạt/không đạt
    PRIMARY KEY (MaHS, MaMon, MaHK,TenNamHoc)
);


create table TaiKhoan (
userName varchar(50) primary key not null ,
passWord varchar(50) not null
)

--================================================
--Thêm một số dữ liệu mặc định
--Môn
INSERT INTO Mon (MaMon, TenMon) VALUES
-- 7 môn bắt buộc
('Toan', N'Toán'),
('Van', N'Ngữ văn'),
('NN1', N'Ngoại ngữ 1'),
('TD', N'Giáo dục thể chất'),
('QPAN', N'Giáo dục quốc phòng – an ninh'),
('HDTN', N'Hoạt động trải nghiệp – hướng nghiệp'),
('GDDP', N'Giáo dục địa phương'),
-- 5 môn tự chọn từ 3 nhóm môn (mỗi nhóm ít nhất 1 môn)
--- Nhóm môn khoa học xã hội
('Su', N'Lịch sử'),
('Dia', N'Địa lý'),
('KTPL', N'Giáo dục kinh tế và pháp luật'),
-- Nhóm môn khoa học tự nhiên
('Ly', N'Vật lý'),
('Hoa', N'Hoá học'),
('Sinh', N'Sinh học'),
--- Nhóm môn công nghệ và nghệ thuật
('CN', N'Công nghệ'),
('Tin', N'Tin học'),
('AN', N'Âm nhạc'),
('MT', N'Mỹ thuật'),
--- Môn tự chọn
('DTTS', N'Tiếng dân tộc thiểu số'),
('NN2', N'Ngoại ngữ 2');

--Giáo viên
INSERT INTO GiaoVien (TenGV, NgaySinh, GioiTinh, DiaChi, SDT, Email,MaMon) VALUES
(N'Nguyễn Văn A', '1980-03-12', 0, N'Hà Nội', '1234567890', 'nguyenvana@gmail.com','Toan'),
(N'Trần Thị B', '1985-07-23', 1, N'Hà Nội', '1234567891', 'tranthib@gmail.com','Ly'),
(N'Lê Văn C', '1978-11-02', 0, N'Hải Phòng', '1234567892', 'levanc@gmail.com','Hoa'),
(N'Hoàng Thị D', '1983-09-05', 1, N'Đà Nẵng', '1234567893', 'hoangthid@gmail.com','Sinh'),
(N'Phạm Văn E', '1979-06-20', 0, N'TP.HCM', '1234567894', 'phamvane@gmail.com','Su'),
(N'Đỗ Thị F', '1986-02-14', 1, N'Hà Nội', '1234567895', 'dothif@gmail.com','Dia'),
(N'Nguyễn Thị G', '1981-01-01', 1, N'Huế', '1234567896', 'nguyenthig@gmail.com','KTPL'),
(N'Trương Văn H', '1977-12-11', 0, N'Cần Thơ', '1234567897', 'truongvanh@gmail.com','Tin'),
(N'Vũ Văn I', '1982-05-18', 0, N'Quảng Ninh', '1234567898', 'vuvani@gmail.com','CN'),
(N'Lường Thị J', '1984-08-29', 1, N'Hà Nội', '1234567899', 'luongthij@gmail.com','NN1'),
(N'Phan Văn K', '1987-04-22', 0, N'Hà Tĩnh', '1234567800', 'phanvank@gmail.com','TD'),
(N'Nguyễn Thị L', '1980-10-30', 1, N'Đà Lạt', '1234567801', 'nguyenthil@gmail.com','Van'),
(N'Bùi Văn M', '1983-03-03', 0, N'Phú Thọ', '1234567802', 'buivanm@gmail.com','QPAN');
-- Thêm các năm học vào bảng NamHoc
INSERT INTO NamHoc (TenNamHoc,TrangThai) VALUES
('2024-2025',0),
('2025-2026',0),
('2026-2027',0);

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
(N'Nguyễn Văn A', '2007-05-10', 0, N'Kinh', N'Hà Nội'),
(N'Trần Thị B', '2007-03-12', 1, N'Kinh', N'TP. Hồ Chí Minh'),
(N'Phan Minh C', '2007-07-22', 0, N'Tày', N'Lào Cai'),
(N'Lò Thu D', '2007-09-15', 1, N'H''Mông', N'Hà Giang'),
(N'Đoàn Hương E', '2007-01-20', 1, N'Mường', N'Thanh Hoá');

--Học kì
INSERT INTO HocKi (TenHK) VALUES
('I'),
('II');
--Tài khoản
INSERT INTO TaiKhoan (userName, passWord) VALUES
('1', '1')
--Thêm điểm
-- Them diem cho hoc sinh vao bang DiemTongHop
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, tx1, tx2, tx3, gk, ck, DiemTB, KetQua) VALUES
(1, 'Toan', 1, '2024-2025', 7.5, 8.0, NULL, 6.5, 7.0, 7.25, 'Dat'),
(2, 'Ly', 1,'2024-2025', 8.0, 7.5, 9.0, 8.5, 7.0, 7.75, 'Dat'),
(3, 'Hoa', 1,'2024-2025', NULL, 6.5, 7.0, 8.0, 7.5, 7.0, 'Dat'),
(4, 'Sinh', 2,'2024-2025', 6.0, 7.0, NULL, 7.5, 8.0, 7.25, 'Khong Dat'),
(5, 'Su', 2,'2024-2025', 8.5, 9.0, 7.5, 7.0, 8.0, 8.0, 'Dat');

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
	dt.tx4 AS DiemTX_Lan4,
    dt.gk AS DiemGiuaKi,
    dt.ck AS DiemCuoiKi,
    dt.DiemTB,              -- Điểm trung bình đã có sẵn trong bảng DiemTongHop
    dt.KetQua               -- Kết quả đạt/không đạt
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
    tx1 = 8.0,     -- Điểm thường xuyên lần 1
    tx2 = 7.5,     -- Điểm thường xuyên lần 2
    tx3 = 8.5,     -- Điểm thường xuyên lần 3
	gk = 7.0,      -- Điểm giữa kỳ
    ck = 8.0,      -- Điểm cuối kỳ
    KetQua = 'Dat' -- Kết quả đạt
WHERE 
    MaHS = 1 AND MaMon = 'Van' AND MaHK = 1;



--Khi một học sinh chuyển lớp :
DELETE FROM ChiTietHocSinh_Lop
WHERE MaHS = 1;  -- Xóa học sinh có mã 1 khỏi các lớp cũ
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES (1, 6);  -- Thêm học sinh vào lớp mới
