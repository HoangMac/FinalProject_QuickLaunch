# FinalProject_QuickLaunch

1512176 - Mạc Minh Hoàng - macminhhoang97@gmail.com

**********Chức năng**********
+ Khi ấn tổ hợp phím Ctrl + H, sẽ xuất hiện hộp thoại gồm 1 textbox & 1 listbox
+ Khi gõ vào ô textbox, listbox sẽ gợi ý các app tương ứng, sắp xếp theo độ ưu tiên
+ Ấn nút mũi tên lên/xuống để chọn
+ Hiển thị icon của app đang chọn
+ Ấn nút Enter để chạy app được chọn
+ Thống kê số lần chạy các apps

**********Luồng sự kiện chính**********
- Khi chạy chương trình, xuất hiện 1 icon ở notification area. (Nếu chạy lần đầu, chương trình sẽ yêu cầu người dùng scan trước khi sử dụng)

- Click chuột phải vào nó để xem menu :
	+ Scan to build database : Quét các app và xây dựng 1 danh sách
	Scan : có 2 lựa chọn là quét toàn bộ và chọn thư 	mục để quét
	Edit : Chọn app để xoá khỏi danh sách

	+ View statistics : Hiển thị biểu đồ thống kê các 	app được chạy thường xuyên và số lần chạy

	+ Exit : Thoát chương trình

- Ấn Ctrl + H để xuất hiện hộp thoại gồm 1 textbox & 1 listbox. Gõ tên app muốn chạy vào textbox, listbox sẽ gợi ý các apps tương ứng, sắp xếp theo độ ưu tiên
- Ấn nút mũi tên lên/xuống để chọn app
- Ấn nút Enter để chạy app được chọn

**********Luồng sự kiện phụ**********

1.
Khi chạy chương trình lần đầu mà bỏ qua bước scan
-> Thông báo bắt buộc người dùng phải scan

2.
Chưa chạy app nào mà chọn view statistics
-> Thông báo chưa app nào được chạy

Lưu ý: 
- Do ổ C chứa các file hệ thống nên khi quét, chương trình sẽ chỉ quét trong thư mục Start Menu
- Chương trình có thể quét cả file shortcut (.lnk)

**********Link demo Youtube**********
https://youtu.be/4aKoJ6IxLQM

**********Link Github**********
https://github.com/HoangMac/FinalProject_QuickLaunch.git
