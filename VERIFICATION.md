# Kết quả kiểm tra bộ khung

Ngày kiểm tra: **2026-11-10**. Compiler: **GCC 15.2.0, MSYS2 UCRT64 trên Windows**.

## Đã thực thi

Lệnh chính: `python practice/tools/check_scaffold.py`.

- Build lại cả test và demo của 10 bài đầu bằng `-std=c++17 -Wall -Wextra -Werror -pthread`: thành công, không warning.
- Chạy 20 executable đó: đều exit **2**, báo TODO đúng dự kiến, không có FAIL.
- Tổng **87 test của bài 01–10 đang TODO**: lần lượt 11, 7, 7, 7, 9, 9, 11, 6, 10, 10. Đây là trạng thái chưa làm, không phải 87 test đã PASS.
- Kiểm tra hạ tầng runner: phân biệt PASS/exit 0, TODO/exit 2, assertion sai/exit 1 và exception ngoài dự kiến/exit 1.
- Regression dọn dẹp thread: tái hiện được việc join treo khi thiếu tín hiệu mở start gate trong quá trình unwind; sau bổ sung cleanup, test chạy PASS/exit 0.
- Thư viện được cung cấp ở bài 11: **4 smoke test PASS**, demo build trực tiếp bằng g++ và in `count=2 mean=20`.
- PowerShell runner: kiểm tra toàn bộ 10 khung bằng mode check thành công; kiểm tra riêng mode test/demo trả exit 2; bài 10 được kiểm tra lại sau sửa cleanup.
- Bash script: kiểm tra cú pháp bằng `bash -n` thành công.

Các log từng test/demo do checker lưu trong `build/`. Script `check` xác nhận khung còn TODO, không thay cho việc chấm lời giải bằng mode `test`.

## Rà soát độc lập

Một reviewer đọc code/contracts và kiểm tra cú pháp bằng GCC. Không có lỗi Critical/Important được báo. Đã xử lý các điểm reviewer nêu:

- Thêm test update sửa được một giá trị cũ sai khi ID và giá trị mới hợp lệ; reject NaN/infinity và giữ nguyên state khi thất bại.
- Thêm ASCII in được ở hai biên 0x20/0x7E và reject 0x1F/0x7F.
- Thêm fixture/test thiếu token, overflow, số âm và giá trị 0/100 có whitespace.
- Dọn dẹp start gate trước khi join nếu việc tạo worker tiếp theo thất bại; có regression test riêng.

Không còn điểm rà soát nào được để lại để sửa trong bộ khung.

## Chưa kiểm tra trong môi trường này

- **CMake/GoogleTest/CTest:** chưa có công cụ/package trong môi trường đã kiểm tra. Đã viết 11 GoogleTest nhưng chưa build/chạy bằng GoogleTest. Bài 11 vẫn có các TODO packaging để người học tự làm; CTest pending phải được thay bằng đăng ký test thật.
- **Linux/WSL runtime:** chưa có môi trường dùng được để thực thi. Không tuyên bố tương thích runtime đã được xác nhận trên Linux. Thử chạy Bash trong MSYS2 từ sandbox Windows gặp lỗi quyền trên đường dẫn dạng /c khi tạo build directory; chỉ xác nhận syntax Bash ở đây.
- **Sanitizer/race detection:** chưa chạy. Test concurrency và review không chứng minh mọi lịch chạy đều không có race; lời giải sau này vẫn cần review các truy cập state dùng chung.

Không cài dependencies, không tạo lời giải hoàn chỉnh cho các bài TODO, không sửa thư mục Topic 4.
