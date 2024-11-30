use QLDiemTHPT
go
--=======================================================================================================================================================
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
--Năm học 
INSERT INTO NamHoc (TenNamHoc,TrangThai) VALUES
('2022-2023',1),
('2023-2024',1),
('2024-2025',0);
--=======================================================================================================================================================
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
--=======================================================================================================================================================
--Lớp
-- Lớp 10 - Năm học 2022-2023
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('10C1', 1, '2022-2023'),
('10C2', 2, '2022-2023'),
('10C3', 3, '2022-2023');

-- Lớp 11 - Năm học 2022-2023
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('11B1', 6, '2022-2023'),
('11B2', 7, '2022-2023'),
('11B3', 8, '2022-2023');

-- Lớp 12 - Năm học 2022-2023
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('12A1', 6, '2022-2023'),
('12A2', 7, '2022-2023'),
('12A3', 8, '2022-2023');

-- Lớp 10 - Năm học 2023-2024
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('10C1', 1, '2023-2024'),
('10C2', 2, '2023-2024'),
('10C3', 3, '2023-2024');

-- Lớp 11 - Năm học 2023-2024
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('11B1', 6, '2023-2024'),
('11B2', 7, '2023-2024'),
('11B3', 8, '2023-2024');

-- Lớp 12 - Năm học 2023-2024
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('12A1', 6, '2023-2024'),
('12A2', 7, '2023-2024'),
('12A3', 8, '2023-2024');
-- Lớp 10
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('10C1', 1, '2024-2025'),
('10C2', 2, '2024-2025'),
('10C3', 3, '2024-2025');


-- Lớp 11
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('11B1', 6, '2024-2025'),
('11B2', 7, '2024-2025'),
('11B3', 8, '2024-2025');


-- Lớp 12
INSERT INTO Lop (TenLop, MaGVCN, TenNamHoc) VALUES
('12A1', 6, '2024-2025'),
('12A2', 7, '2024-2025'),
('12A3', 8, '2024-2025');
--=======================================================================================================================================================
--Học kì
INSERT INTO HocKi (TenHK) VALUES
('I'),
('II');
--=======================================================================================================================================================
--Tài khoản
INSERT INTO TaiKhoan (userName, passWord) VALUES
('1', '1')
--=======================================================================================================================================================
--Thêm 30 học sinh 2k7
INSERT INTO HocSinh (HoTen, NgaySinh, GioiTinh, DanToc, NoiSinh) VALUES
(N'Nguyễn Thị A', '2007-01-01', 0, N'Kinh', N'Hà Nội'),
(N'Nguyễn Thị B', '2007-02-14', 1, N'Kinh', N'Hà Nội'),
(N'Nguyễn Thị C', '2007-03-03', 0, N'Kinh', N'Bắc Giang'),
(N'Nguyễn Thị D', '2007-04-22', 1, N'Kinh', N'TP.HCM'),
(N'Nguyễn Thị E', '2007-05-05', 0, N'Kinh', N'Đà Nẵng'),
(N'Nguyễn Thị F', '2007-06-12', 1, N'Kinh', N'Hải Phòng'),
(N'Nguyễn Thị G', '2007-07-15', 0, N'Kinh', N'Bình Dương'),
(N'Nguyễn Thị H', '2007-08-18', 1, N'Kinh', N'Quảng Ninh'),
(N'Nguyễn Thị I', '2007-09-10', 0, N'Kinh', N'Hà Nội'),
(N'Nguyễn Thị J', '2007-10-02', 1, N'Kinh', N'Hưng Yên'),
(N'Nguyễn Thị K', '2007-11-20', 0, N'Kinh', N'Phú Thọ'),
(N'Nguyễn Thị L', '2007-12-25', 1, N'Kinh', N'Thái Bình'),
(N'Nguyễn Thị M', '2007-01-30', 0, N'Kinh', N'TP.HCM'),
(N'Nguyễn Thị N', '2007-02-08', 1, N'Kinh', N'Hà Nội'),
(N'Nguyễn Thị O', '2007-03-14', 0, N'Kinh', N'Vĩnh Phúc'),
(N'Nguyễn Thị P', '2007-04-18', 1, N'Kinh', N'Bắc Ninh'),
(N'Nguyễn Thị Q', '2007-05-22', 0, N'Kinh', N'Nam Định'),
(N'Nguyễn Thị R', '2007-06-26', 1, N'Kinh', N'Thái Nguyên'),
(N'Nguyễn Thị S', '2007-07-19', 0, N'Kinh', N'Phú Yên'),
(N'Nguyễn Thị T', '2007-08-14', 1, N'Kinh', N'Bà Rịa - Vũng Tàu'),
(N'Nguyễn Thị U', '2007-09-23', 0, N'Kinh', N'Cần Thơ'),
(N'Nguyễn Thị V', '2007-10-06', 1, N'Kinh', N'Quảng Nam'),
(N'Nguyễn Thị W', '2007-11-09', 0, N'Kinh', N'Tuyên Quang'),
(N'Nguyễn Thị X', '2007-12-05', 1, N'Kinh', N'Hà Tĩnh'),
(N'Nguyễn Thị Y', '2007-01-19', 0, N'Kinh', N'Bắc Giang'),
(N'Nguyễn Thị Z', '2007-02-25', 1, N'Kinh', N'Lạng Sơn'),
(N'Nguyễn Thị AA', '2007-03-08', 0, N'Kinh', N'Gia Lai'),
(N'Nguyễn Thị AB', '2007-04-11', 1, N'Kinh', N'Quảng Bình'),
(N'Nguyễn Thị AC', '2007-05-03', 0, N'Kinh', N'Sóc Trăng'),
(N'Nguyễn Thị AD', '2007-06-20', 1, N'Kinh', N'Thừa Thiên Huế');
-- chi tiết học sinh - lớp
-- Thêm 10 học sinh đầu vào lớp 10C1 năm học 2022-2023
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES
(1, 1),  -- MaHS = 1, MaLop = 1 (10C1 năm 2022-2023)
(2, 1),  -- MaHS = 2, MaLop = 1 (10C1 năm 2022-2023)
(3, 1),  -- MaHS = 3, MaLop = 1 (10C1 năm 2022-2023)
(4, 1),  -- MaHS = 4, MaLop = 1 (10C1 năm 2022-2023)
(5, 1),  -- MaHS = 5, MaLop = 1 (10C1 năm 2022-2023)
(6, 1),  -- MaHS = 6, MaLop = 1 (10C1 năm 2022-2023)
(7, 1),  -- MaHS = 7, MaLop = 1 (10C1 năm 2022-2023)
(8, 1),  -- MaHS = 8, MaLop = 1 (10C1 năm 2022-2023)
(9, 1),  -- MaHS = 9, MaLop = 1 (10C1 năm 2022-2023)
(10, 1); -- MaHS = 10, MaLop = 1 (10C1 năm 2022-2023)
-- Thêm 10 học sinh tiếp theo vào lớp 10C2 năm học 2022-2023
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES
(11, 2),  -- MaHS = 11, MaLop = 2 (10C2 năm 2022-2023)
(12, 2),  -- MaHS = 12, MaLop = 2 (10C2 năm 2022-2023)
(13, 2),  -- MaHS = 13, MaLop = 2 (10C2 năm 2022-2023)
(14, 2),  -- MaHS = 14, MaLop = 2 (10C2 năm 2022-2023)
(15, 2),  -- MaHS = 15, MaLop = 2 (10C2 năm 2022-2023)
(16, 2),  -- MaHS = 16, MaLop = 2 (10C2 năm 2022-2023)
(17, 2),  -- MaHS = 17, MaLop = 2 (10C2 năm 2022-2023)
(18, 2),  -- MaHS = 18, MaLop = 2 (10C2 năm 2022-2023)
(19, 2),  -- MaHS = 19, MaLop = 2 (10C2 năm 2022-2023)
(20, 2); -- MaHS = 20, MaLop = 2 (10C2 năm 2022-2023)
-- Thêm 10 học sinh còn lại vào lớp 10C3 năm học 2022-2023
INSERT INTO ChiTietHocSinh_Lop (MaHS, MaLop)
VALUES
(21, 3),  -- MaHS = 21, MaLop = 3 (10C3 năm 2022-2023)
(22, 3),  -- MaHS = 22, MaLop = 3 (10C3 năm 2022-2023)
(23, 3),  -- MaHS = 23, MaLop = 3 (10C3 năm 2022-2023)
(24, 3),  -- MaHS = 24, MaLop = 3 (10C3 năm 2022-2023)
(25, 3),  -- MaHS = 25, MaLop = 3 (10C3 năm 2022-2023)
(26, 3),  -- MaHS = 26, MaLop = 3 (10C3 năm 2022-2023)
(27, 3),  -- MaHS = 27, MaLop = 3 (10C3 năm 2022-2023)
(28, 3),  -- MaHS = 28, MaLop = 3 (10C3 năm 2022-2023)
(29, 3),  -- MaHS = 29, MaLop = 3 (10C3 năm 2022-2023)
(30, 3); -- MaHS = 30, MaLop = 3 (10C3 năm 2022-2023)
-- Thêm điểm tổng hợp cho học sinh MaHS = 1 trong lớp 10C1, năm học 2022-2023
-- Học kỳ I
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua)
VALUES
(1, 'Toan', 1, '2022-2023', 8, 7, 9, 8, 7.5, 8.5, 8, 'Dat'),
(1, 'Van', 1, '2022-2023', 7, 6.5, 8, 7.5, 7, 8, 7.4, 'Dat'),
(1, 'NN1', 1, '2022-2023', 9, 9, 8.5, 9, 8, 8.5, 8.7, 'Dat'),
(1, 'TD', 1, '2022-2023', 7.5, 7, 8, 7.5, 7.5, 8, 7.5, 'Dat'),
(1, 'QPAN', 1, '2022-2023', 8, 7.5, 9, 8.5, 8, 9, 8.5, 'Dat'),
(1, 'HDTN', 1, '2022-2023', 8, 8, 7, 7.5, 7.5, 7.5, 7.7, 'Dat'),
(1, 'GDDP', 1, '2022-2023', 7, 7.5, 7, 8, 7, 7, 7.3, 'Dat'),
(1, 'Su', 1, '2022-2023', 8, 9, 8.5, 8, 7.5, 8, 8.2, 'Dat'),
(1, 'Dia', 1, '2022-2023', 7.5, 7, 8, 8, 7, 7.5, 7.6, 'Dat'),
(1, 'KTPL', 1, '2022-2023', 8, 8.5, 7, 8, 8, 8, 8, 'Dat');
-- Học kỳ II
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua)
VALUES
(1, 'Toan', 2, '2022-2023', 8, 7.5, 9, 8, 7.5, 8, 8.0, 'Dat'),
(1, 'Van', 2, '2022-2023', 6.5, 7, 8, 7, 7, 8, 7.4, 'Dat'),
(1, 'NN1', 2, '2022-2023', 9, 8.5, 8, 8.5, 8.5, 8.5, 8.5, 'Dat'),
(1, 'TD', 2, '2022-2023', 7.5, 7, 7.5, 7.5, 7, 8, 7.4, 'Dat'),
(1, 'QPAN', 2, '2022-2023', 8, 8.5, 9, 8.5, 8, 9, 8.6, 'Dat'),
(1, 'HDTN', 2, '2022-2023', 7.5, 7.5, 7, 8, 7.5, 7.5, 7.5, 'Dat'),
(1, 'GDDP', 2, '2022-2023', 8, 8, 7.5, 7.5, 7.5, 7.5, 7.8, 'Dat'),
(1, 'Su', 2, '2022-2023', 8.5, 8, 8.5, 8.5, 8, 8, 8.3, 'Dat'),
(1, 'Dia', 2, '2022-2023', 7.5, 7.5, 7, 7.5, 7.5, 7.5, 7.5, 'Dat'),
(1, 'KTPL', 2, '2022-2023', 8, 7.5, 8, 8, 8, 8, 8, 'Dat');

-- Thêm điểm tổng hợp cho học sinh MaHS = 2 trong lớp 10C1, năm học 2022-2023
-- Học kỳ I
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua)
VALUES
(2, 'Toan', 1, '2022-2023', 8, 7, 9, 8, 7.5, 8.5, 8, 'Dat'),
(2, 'Van', 1, '2022-2023', 7, 6.5, 8, 7.5, 7, 8, 7.4, 'Dat'),
(2, 'NN1', 1, '2022-2023', 9, 9, 8.5, 9, 8, 8.5, 8.7, 'Dat'),
(2, 'TD', 1, '2022-2023', 7.5, 7, 8, 7.5, 7.5, 8, 7.5, 'Dat'),
(2, 'QPAN', 1, '2022-2023', 8, 7.5, 9, 8.5, 8, 9, 8.5, 'Dat'),
(2, 'HDTN', 1, '2022-2023', 8, 8, 7, 7.5, 7.5, 7.5, 7.7, 'Dat'),
(2, 'GDDP', 1, '2022-2023', 7, 7.5, 7, 8, 7, 7, 7.3, 'Dat'),
(2, 'Su', 1, '2022-2023', 8, 9, 8.5, 8, 7.5, 8, 8.2, 'Dat'),
(2, 'Dia', 1, '2022-2023', 7.5, 7, 8, 8, 7, 7.5, 7.6, 'Dat'),
(2, 'KTPL', 1, '2022-2023', 8, 8.5, 7, 8, 8, 8, 8, 'Dat');
-- Học kỳ II
INSERT INTO DiemTongHop (MaHS, MaMon, MaHK, TenNamHoc, tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua)
VALUES
(2, 'Toan', 2, '2022-2023', 8, 7.5, 9, 8, 7.5, 8, 8.0, 'Dat'),
(2, 'Van', 2, '2022-2023', 6.5, 7, 8, 7, 7, 8, 7.4, 'Dat'),
(2, 'NN1', 2, '2022-2023', 9, 8.5, 8, 8.5, 8.5, 8.5, 8.5, 'Dat'),
(2, 'TD', 2, '2022-2023', 7.5, 7, 7.5, 7.5, 7, 8, 7.4, 'Dat'),
(2, 'QPAN', 2, '2022-2023', 8, 8.5, 9, 8.5, 8, 9, 8.6, 'Dat'),
(2, 'HDTN', 2, '2022-2023', 7.5, 7.5, 7, 8, 7.5, 7.5, 7.5, 'Dat'),
(2, 'GDDP', 2, '2022-2023', 8, 8, 7.5, 7.5, 7.5, 7.5, 7.8, 'Dat'),
(2, 'Su', 2, '2022-2023', 8.5, 8, 8.5, 8.5, 8, 8, 8.3, 'Dat'),
(2, 'Dia', 2, '2022-2023', 7.5, 7.5, 7, 7.5, 7.5, 7.5, 7.5, 'Dat'),
(2, 'KTPL', 2, '2022-2023', 8, 7.5, 8, 8, 8, 8, 8, 'Dat');


