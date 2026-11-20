# Bài 05 — STL, optional và chuỗi byte

## Mục tiêu và lý thuyết

`std::array<T,N>` có kích thước cố định trong kiểu; `vector<T>` có số phần tử thay đổi; `string` giữ chuỗi ký tự có độ dài, kể cả ký tự NUL nằm giữa. `.size()` khác capacity; `reserve()` không tạo phần tử để truy cập bằng index. `.at()` kiểm tra phạm vi; `operator[]` yêu cầu caller bảo đảm index đúng.

`optional<T>` biểu diễn có/không kết quả mà không cần một “giá trị lỗi” như 0. `if (result)` kiểm tra có giá trị; `*result` đọc sau khi đã kiểm tra. `optional<bool>{false}` vẫn có giá trị, không phải nullopt.

Range-for duyệt phần tử; `const auto&` tránh copy khi chỉ đọc. Lambda là callable nhỏ, có thể truyền cho thuật toán như `find_if`. Dùng lambda ở ít nhất một thao tác lọc/tìm, nhưng không cần ép tất cả hàm dùng algorithm.

`vector` tăng capacity có thể di chuyển vùng chứa và làm dangling pointer/reference/iterator đang mượn. Không vừa duyệt bằng iterator cũ vừa push_back mà không xét invalidation. `string.c_str()` cũng chỉ có lifetime gắn với string và có thể mất hiệu lực sau sửa string.

## Đề và hợp đồng API

| API | Yêu cầu |
|---|---|
| `filter_range(values, low, high)` | Giữ các giá trị trong [low,high], đúng thứ tự và số lần xuất hiện; low>high → vector rỗng |
| `first_at_least(values, threshold)` | Phần tử đầu tiên >= threshold; không có → nullopt; không tìm minimum |
| `to_bytes(text)` | Giữ tất cả byte, cả NUL và 0xFF, không thêm terminator |
| `fixed_ascii(text, width)` | Đúng width byte và mọi byte thuộc ASCII in được 0x20..0x7E → buffer; sai → nullopt. Chuỗi rỗng với width=0 hợp lệ |

Không sửa vector/string đầu vào. `to_bytes` là chuyển byte tổng quát, còn fixed_ascii mới kiểm tra ASCII. Tránh sign extension khi char có dấu. Trong demo, dùng std::array chứa một vài dữ liệu cố định nếu muốn luyện chuyển sang vector; header đã include array để bạn thử.

Ví dụ: filter `{5,-1,3,5,9}` với [3,5] → `{5,3,5}`. `string("A\0Z",3)` → `{0x41,0x00,0x5A}`, không phải chỉ `{0x41}`. `"DEV7"` width 4 → bốn byte, không có byte kết thúc thứ năm.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 05 -Mode test
.\practice\run.ps1 -Lesson 05 -Mode demo
```

Linux/WSL: `bash practice/run.sh 05 test`. Demo in các giá trị 5 và 3; sau đó có ba byte 65, 0, 90.

## Gợi ý theo mức

1. Đọc size của string, không dùng strlen cho buffer có NUL.
2. Phân biệt “optional có vector rỗng” với “không có vector”.
3. Khi chuyển char sang byte, xét char có thể là signed trên compiler hiện tại.

## Checklist và câu hỏi

- [ ] Test giữ byte NUL/0xFF và độ dài chính xác đều PASS.
- [ ] Đã dùng một lambda và một range-for trong bài làm.
- [ ] Giải thích khi nào array phù hợp hơn vector.
- [ ] Nếu giữ `auto* p = values.data()` rồi push_back, khi nào p không còn dùng được?
- [ ] Vì sao std::string có độ dài rõ ràng vẫn có thể chứa NUL?

Liên quan: buffer và ASCII cố định cho T4-REQ-07..09; optional/lambda/array/range-for trong CMN-COD-02.
