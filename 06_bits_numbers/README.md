# Bài 06 — Kiểu số, bit và buffer hex

## Mục tiêu và lý thuyết

`uint8_t`/`uint16_t` từ `<cstdint>` thể hiện độ rộng dữ liệu giao tiếp. Hệ hex chỉ là cách viết số: `0x10` bằng 16. `&` lọc bit, `|` bật bit, `~` đảo bit, `^` đảo các bit được chọn; `<<`/`>>` dịch bit. Bit 0 là bit có trọng số thấp nhất.

Kiểu nhỏ như uint8_t thường được **integer promotion** thành int trước khi tính toán. Biến unsigned không có nghĩa mọi biểu thức chứa nó đều unsigned. Dùng kiểu unsigned có độ rộng phù hợp trước khi dịch; không dịch số vị trí âm hoặc >= độ rộng kiểu thực sự đang được dịch.

Tràn signed integer là undefined behavior; unsigned tính theo modulo. Ép số lớn sang kiểu nhỏ có thể mất dữ liệu. Hãy kiểm tra phạm vi trước khi cast; static_cast thể hiện ý định, không tự chứng minh dữ liệu an toàn. So sánh int âm với size_t có thể cho kết quả bất ngờ do đổi kiểu.

`enum class` giữ tên và kiểu riêng; không tự chuyển sang số nguyên như enum truyền thống. `constexpr` cho hằng có tên; tránh lặp các magic number về scale/offset hay số bit.

## Đề và hợp đồng API

| API | Yêu cầu |
|---|---|
| `read_bit(byte,index)` | index 0..7 → optional<bool>; ngoài phạm vi → nullopt |
| `write_bit(byte&,index,enabled)` | Bật/tắt đúng một bit, giữ các bit khác; index sai → false, không sửa |
| `extract_bits(byte,start,width)` | Trường nằm trọn trong byte, width 1..8 → giá trị unsigned đã dịch về bit 0; sai → nullopt |
| `scaled_value(raw)` | `raw * 0.25 - 40.0`, dùng hằng đã cho |
| `decode_mode(byte)` | Hai bit thấp: 0 off, 1 idle, 2 run, 3 error; bỏ qua bit cao |
| `hex_string(bytes)` | Hai chữ số hex HOA mỗi byte, cách nhau một space, không space cuối; buffer rỗng → string rỗng |

Kiểm tra start trước, rồi kiểm tra width bằng cách không làm tràn phép cộng start+width. API nhận unsigned nên test cả một index rất lớn, không chỉ 8.

Ví dụ: `0xB6` (10110110), start 2/width 3 → 5. `0x55` bật bit 7 → `0xD5`. `{0,10,128,255}` → `"00 0A 80 FF"`.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 06 -Mode test
.\practice\run.ps1 -Lesson 06 -Mode demo
```

Linux/WSL: `bash practice/run.sh 06 test`. Demo sau khi làm: `D5`, rồi `scaled(100)=-15`.

## Gợi ý theo mức

1. Vẽ mask trên giấy với width 1, 3 và 8 trước khi tổng quát hóa.
2. Chia thành validate → trích raw → scale → biểu diễn.
3. `uint8_t` thường là alias của unsigned char, nên ostream có thể in ký tự; cast sang kiểu số khi format. Dùng ostringstream cục bộ để không đổi trạng thái std::cout của người gọi.

## Checklist và câu hỏi

- [ ] Bit 0, bit 7, width 8, width 0 và index lớn đều đúng.
- [ ] Không có shift ngoài phạm vi hoặc phép cộng chỉ số tràn.
- [ ] `hex_string` không tự thêm newline hay khoảng trắng cuối.
- [ ] Giải thích vì sao `(1 << n)` cần xem lại khi n lớn hoặc kiểu mặc định là int.
- [ ] Khi nào mask rồi mới cast, khi nào cần cast trước khi shift?

Liên quan: T4-REQ-02, 04; CMN-COD-02, 04, 05.
