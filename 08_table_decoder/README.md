# Bài 08 — Một thuật toán, nhiều định nghĩa dữ liệu

## Mục tiêu và lý thuyết

Decoder theo bảng tách **dữ liệu mô tả** khỏi **thuật toán xử lý**. Bổ sung một loại đo mới thường là thêm một dòng bảng, thay vì thêm một nhánh if chứa thuật toán khác.

`Definition` mô tả một trường nằm trong một byte của packet giả. `inline constexpr std::array` giữ bảng cố định và dùng được trong nhiều translation unit (inline variable từ C++17). `Measurement` giữ enum loại đo và giá trị số.

## Đề và hợp đồng API

1. Thay bảng `definitions{}` trống trong header bằng ba dòng sau.
2. Tự viết `Decoder::decode` bằng một thuật toán tra bảng → validate → trích bit → scale.

| code | metric | byte_offset | start_bit | width | scale | offset |
|---|---|---|---|---|---|---|
| 0x701 | temperature | 0 | 0 | 8 | 0.5 | -20.0 |
| 0x702 | enabled | 1 | 6 | 1 | 1.0 | 0.0 |
| 0x703 | mode | 0 | 0 | 2 | 1.0 | 0.0 |

- Code không có trong bảng: tăng unknown đúng một lần/packet, trả nullopt, không tăng short.
- Code có trong bảng nhưng payload không tới byte cần đọc: tăng short đúng một lần, trả nullopt.
- Packet hợp lệ: trả Measurement theo bảng, không tăng counter lỗi. Cho phép byte dư.
- Hai counter bắt đầu 0 và tích lũy riêng cho từng Decoder.
- Không hard-code `if (code == 0x701)` để giải mã riêng; tên code chỉ xuất hiện trong bảng và dữ liệu demo/test.

Bảng trong bài do lập trình viên kiểm soát, phải có width/start hợp lệ như đã cho; chưa cần nhận bảng cấu hình tùy ý từ file. Mọi phép đọc payload vẫn phải kiểm tra độ dài.

Ví dụ: packet `{0x701,{100}}` → temperature 30.0. `{0x702,{0xFF,0xBF}}` → enabled 0, vì bit 6 của byte 1 tắt. `{0x703,{0xFE}}` → mode 2.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 08 -Mode test
.\practice\run.ps1 -Lesson 08 -Mode demo
```

Linux/WSL: `bash practice/run.sh 08 test`. Demo sau khi làm: `temperature=30`.

## Gợi ý theo mức

1. Giữ một biến chỉ tới Definition tìm được; không copy logic vào từng metric.
2. Unknown và short là hai kết quả khác nhau; tra code trước khi suy luận độ dài.
3. Dùng công thức chung `raw * scale + offset` và enum từ Definition.

## Mở rộng và checklist

`variant<double,bool,Mode>` có thể biểu diễn từng loại giá trị chặt chẽ hơn. Bài chính dùng double cho cả 0/1/mode để tập trung vào table-driven design; không bắt buộc thêm variant.

- [ ] Bảng constexpr đúng; mọi test PASS, cả mã lạ có payload rỗng.
- [ ] Thêm metric mới không đòi hỏi sao chép thuật toán.
- [ ] Giải thích khi nào bảng constexpr phù hợp, khi nào cần cấu hình runtime.
- [ ] Nếu dữ liệu từ file được dùng làm Definition, cần validate thêm những gì?

Liên quan: T4-REQ-02, 03, 04. Đây là packet cảm biến giả, không dùng bảng tín hiệu CAN của đề.
