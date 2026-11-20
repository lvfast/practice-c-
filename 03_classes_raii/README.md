# Bài 03 — Class và RAII

## Mục tiêu và lý thuyết

Class giữ dữ liệu `private` và chỉ cho thay đổi qua API bảo vệ tính hợp lệ của đối tượng. Constructor tạo đối tượng hợp lệ; initializer list khởi tạo member trước khi vào thân constructor. `explicit` ngăn chuyển kiểu ngầm không mong muốn từ một số nguyên thành Sensor.

RAII gắn lifetime tài nguyên với lifetime đối tượng: `std::ifstream` đóng file khi bị hủy, kể cả khi rời scope do exception. Vì member đã quản lý tài nguyên, `LineReader` không cần tự viết destructor hay năm hàm copy/move: đó là **Rule of Zero**. Đừng thêm destructor chỉ để tự gọi `close()` nếu không có yêu cầu khác.

`LineReader` không copy được vì `ifstream` không copy được. Đây là hệ quả của ownership, không phải lý do dùng con trỏ thường để né quản lý tài nguyên.

## Đề và hợp đồng API

Các getter trong header đã cho sẵn; hoàn thiện constructor và các hàm trong source.

| API | Yêu cầu |
|---|---|
| `Sensor(int id)` | id>0; sai thì ném invalid_argument. Giá trị ban đầu 0 |
| `set_value(double)` | Chỉ chấp nhận giá trị hữu hạn trong [0,100]; false và không đổi state nếu sai |
| `LineReader(path)` | Mở file vào member ifstream; không mở được thì runtime_error có thông tin đường dẫn |
| `next()` | Trả một dòng, không gồm newline; dòng trống là string rỗng có giá trị. EOF → nullopt; lỗi I/O thực sự → runtime_error |

File mẫu có ba dòng: `alpha`, dòng trống, `omega`. Sau dòng cuối, gọi next nhiều lần vẫn nullopt. Dùng `std::getline`; không tự cấp phát buffer. Dữ liệu mẫu dùng LF; có thể chuẩn hóa CRLF nếu mở rộng đọc file khác.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 03 -Mode test
.\practice\run.ps1 -Lesson 03 -Mode demo
```

Linux/WSL: `bash practice/run.sh 03 test`. Runner tự chọn working directory đúng cho `data/lines.txt`.

Demo phải in sensor 7/value 45, ba dòng (bao gồm `[]`), rồi thông báo đã ra khỏi scope reader. Có thể đặt breakpoint ở hết scope để quan sát destructor của member trong debugger; việc mở lại file trong test không tự chứng minh mọi loại handle đều không rò rỉ.

## Gợi ý theo mức

1. Validate constructor khác validate setter: constructor không trả bool.
2. Tách “đọc được một dòng trống” khỏi “không đọc được dòng nào”.
3. Kiểm tra trạng thái stream để phân biệt EOF và I/O error; không coi tất cả failbit là EOF.

## Checklist và câu hỏi

- [ ] Constructor không để lọt ID không hợp lệ.
- [ ] Setter sai không làm hỏng giá trị cũ.
- [ ] Test phân biệt empty line và EOF; file thiếu báo lỗi.
- [ ] Giải thích tài nguyên nào LineReader sở hữu và ai đóng file.
- [ ] Nếu constructor ném exception, member đã khởi tạo có được hủy không?
- [ ] Vì sao không cần custom destructor ở đây? Điều gì thay đổi nếu class giữ một handle C thô?

Liên quan: CMN-COD-01, 05; nền tảng nguồn đọc file của T4-REQ-01.
