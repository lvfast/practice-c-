# Bài 01 — Hàm, struct, tham chiếu và const

## Mục tiêu và lý thuyết

`Reading` gồm `sensor_id` và `value`. Truyền bằng giá trị tạo bản sao; `Reading&` tham chiếu đến bản gốc; `const Reading&` cho phép đọc bản gốc mà không sao chép hoặc sửa nó qua tham chiếu đó.

Header công bố kiểu và chữ ký hàm. Source định nghĩa thân hàm. `#pragma once` tránh include lặp; `namespace lab01` gom tên để tránh xung đột. Compiler kiểm tra từng translation unit, linker nối các định nghĩa lại: hãy thử bỏ `lesson.cpp` khỏi lệnh build để hiểu lỗi undefined reference.

## Đề và hợp đồng API

Tự làm ba hàm trong `lesson.cpp`, giữ nguyên chữ ký ở `lesson.hpp`:

| Hàm | Yêu cầu |
|---|---|
| `is_valid(const Reading&)` | true khi ID > 0 và value hữu hạn, thuộc [0, 100] |
| `try_update(Reading&, double)` | Kiểm tra ID hiện tại và giá trị mới; hợp lệ thì sửa value và trả true; sai thì false, không sửa gì |
| `copy_with_offset(Reading, double)` | Trả bản sao có value cộng offset; bản gốc không đổi. Reading gốc hoặc kết quả sai/không hữu hạn thì ném `std::invalid_argument` |

Không đổi ID khi cập nhật. Không dùng biến global. Giá trị NaN không được xem là hợp lệ chỉ vì một phép so sánh trả false; dùng `<cmath>`.

Ví dụ: `{3, 10}` cộng offset `5` cho `{3, 15}`; bản gốc vẫn `{3, 10}`. Giá trị `0` và `100` hợp lệ, ID `0` không hợp lệ.

## Chạy

Từ thư mục gốc:

```powershell
.\practice\run.ps1 -Lesson 01 -Mode test
.\practice\run.ps1 -Lesson 01 -Mode demo
```

Linux/WSL: `bash practice/run.sh 01 test` (thay `test` bằng `demo` khi cần).

Demo sau khi làm: `original=25 copy=30`. Ban đầu test/demo báo TODO, exit 2.

## Gợi ý theo mức

1. Đọc kiểu tham số để biết hàm nào có thể sửa dữ liệu của người gọi.
2. Viết điều kiện hợp lệ một lần; kiểm tra trước khi gán.
3. Với phép cập nhật có thể thất bại, tính/kiểm tra kết quả trước rồi mới thay state.

## Checklist và câu hỏi

- [ ] Tất cả test PASS, kể cả NaN/infinity và không đổi dữ liệu khi thất bại.
- [ ] Demo chạy được, build không warning.
- [ ] Giải thích khác biệt `Reading`, `Reading&`, `const Reading&`.
- [ ] Vì sao không đặt toàn bộ định nghĩa hàm thường trong header rồi include ở nhiều source?
- [ ] Exception ở `copy_with_offset` khác cách báo false của `try_update` ở điểm nào?

Liên quan: nền tảng chia API và kiểm tra lỗi, CMN-COD-05; chưa xử lý giao thức Topic 4.
