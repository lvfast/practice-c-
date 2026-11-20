# Bài 07 — Template và số nguyên big-endian

## Mục tiêu và lý thuyết

Function template dùng một thuật toán cho nhiều kiểu. Định nghĩa template thường nằm trong header để nơi dùng nhìn thấy thân hàm lúc instantiate. `<type_traits>` cho biết kiểu là integer, signed, unsigned; `static_assert` kiểm tra lúc compile, còn `if constexpr` chỉ instantiate nhánh phù hợp.

Big-endian đặt byte có trọng số cao nhất trước. Số `0x1234` luôn được encode `{0x12,0x34}`, dù máy chạy là little-endian. Không dùng reinterpret_cast, union hoặc memcpy layout nguyên bản để giả vờ làm endian conversion.

`std::make_unsigned_t<T>` hữu ích để làm bit arithmetic; đừng lập tức cast một số unsigned vượt max(T) sang signed rồi cho rằng kết quả portable trong C++17. Quy ước trên wire ở bài này là **bù hai**, không dựa vào biểu diễn signed của host. Đặc biệt không tính `-INT64_MIN` bằng int64_t.

## Đề và hợp đồng API

Hai hàm ở `lesson.hpp`; `lesson.cpp` cố ý chỉ include header:

| API | Yêu cầu |
|---|---|
| `encode_be<T>(value)` | Trả array gồm sizeof(T) byte big-endian |
| `decode_be<T>(bytes,offset=0)` | Đọc đúng sizeof(T) byte bắt đầu tại offset, cho phép còn byte dư phía sau; thiếu/offset sai → nullopt |

Hỗ trợ signed/unsigned 8, 16, 32, 64 bit. Ràng buộc integer không phải bool đã có sẵn; không xóa static_assert. Không cần hỗ trợ enum, floating point hoặc integer mở rộng ngoài các độ rộng trên. Với char/native int, chỉ dùng khi bạn đã xác định signedness/độ rộng; test chính dùng fixed-width types.

Ví dụ độc lập, không dùng encode để tính expected của decode:

| Giá trị/kiểu | Byte mong đợi |
|---|---|
| uint16, 0x1234 | 12 34 |
| uint32, 0x12345678 | 12 34 56 78 |
| int16, -2 | FF FE |
| int8, -128 | 80 |
| int64, giá trị nhỏ nhất | 80 00 00 00 00 00 00 00 |

`decode<uint16>({AA,12,34,BB},1)` → 0x1234. `decode<uint32>({1,2,3})` → nullopt. Offset bằng size và offset SIZE_MAX phải được từ chối an toàn; tránh overflow khi tính `offset+sizeof(T)`.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 07 -Mode test
.\practice\run.ps1 -Lesson 07 -Mode demo
```

Linux/WSL: `bash practice/run.sh 07 test`. Demo sau khi làm in `12 34`.

## Gợi ý theo mức

1. Viết tay ví dụ 16 bit, xác định byte nào tương ứng shift nào; sau đó tổng quát theo sizeof(T).
2. Accumulate bằng unsigned đủ rộng, validate buffer trước khi truy cập.
3. Với signed, xét riêng vùng không âm và vùng có sign bit. Chuyển từ độ lớn có thể biểu diễn được; xử lý giá trị min mà không phủ định nó hoặc cast ngoài miền signed.

## Checklist và câu hỏi

- [ ] Tám kiểu số qua test zero/min/max; -1/-2 có byte bù hai đúng.
- [ ] Có kiểm tra byte mong đợi cụ thể, không chỉ encode rồi decode ngược lại.
- [ ] Không dùng host endian hoặc phép chuyển signed phụ thuộc implementation.
- [ ] Giải thích static_assert khác if runtime và if constexpr.
- [ ] Vì sao min signed khó hơn max signed? Vì sao không tạo mask bằng shift đúng 64 bit?

Liên quan: T4-REQ-11; byte buffer của T4-REQ-07, 09. Chưa có DID hay UDS handler.
