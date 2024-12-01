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









--Khi một học sinh chuyển lớp :
DELETE FROM ChiTietHocSinh_Lop
WHERE MaHS = 1;  -- Xóa học sinh có mã 1 khỏi các lớp cũ
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES (1, 6);  -- Thêm học sinh vào lớp mới
