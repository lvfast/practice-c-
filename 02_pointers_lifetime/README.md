# Bài 02 — Con trỏ, buffer và vòng đời

## Mục tiêu và lý thuyết

- `&object` lấy địa chỉ; `*pointer` truy cập đối tượng tại địa chỉ đó. Dereference chỉ hợp lệ khi con trỏ chỉ đến đối tượng còn sống.
- `T*`: có thể đổi con trỏ và sửa đối tượng. `const T*`: không sửa đối tượng qua con trỏ này. `T* const`: không đổi địa chỉ nhưng vẫn có thể sửa đối tượng.
- `nullptr` thể hiện không có đối tượng. Con trỏ không tự cho biết kích thước buffer hay ai sở hữu nó.
- Biến có automatic storage duration sống đến hết scope; bộ nhớ động sống theo owner quản lý nó. “Stack/heap” là cách gọi thông dụng, không thay thế quy tắc lifetime.
- Mảng built-in thường chuyển thành con trỏ khi truyền vào hàm; `sizeof(pointer)` không phải số phần tử. `std::array` giữ kích thước; `.data()` cho con trỏ mượn.

Trong bài này mọi con trỏ đều **không sở hữu**. Caller bảo đảm vùng nhớ hợp lệ, còn sống và có ít nhất `size` byte; hàm không thể xác minh một địa chỉ tùy ý có thật sự hợp lệ hay không.

## Đề và hợp đồng API

| Hàm | Yêu cầu |
|---|---|
| `read_byte(const uint8_t*, size, index)` | Trả byte qua optional. nullptr, size=0 hoặc index>=size → nullopt |
| `write_byte(uint8_t*, size, index, value)` | Cập nhật đúng một byte; false nếu đầu vào không hợp lệ, buffer không đổi |
| `fill_bytes(uint8_t*, size, value)` | Ghi đúng size byte. size=0 → true, kể cả nullptr; nullptr với size>0 → false |

Không giữ lại địa chỉ sau khi hàm kết thúc. Không cấp phát/giải phóng bộ nhớ. Không tính `data + index` trước khi kiểm tra phạm vi.

Ví dụ: `{1,2,3}`, ghi index 1 thành 42 → `{1,42,3}`. Index 3 không hợp lệ. Fill vùng con bắt đầu `buffer.data()+1`, dài 2 chỉ sửa hai byte giữa.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 02 -Mode test
.\practice\run.ps1 -Lesson 02 -Mode demo
```

Linux/WSL: `bash practice/run.sh 02 test`. Demo sau khi làm: `byte[1]=42`.

## Gợi ý theo mức

1. Một optional chứa byte `0` vẫn là có kết quả; đừng dùng giá trị byte để đoán có lỗi.
2. Tách điều kiện “không có phần tử cần ghi” khỏi điều kiện “thiếu vùng nhớ”.
3. Pointer arithmetic chỉ được dùng trong cùng mảng hoặc đến one-past-end; one-past-end không được dereference.

## Chỉ đọc và phân tích — KHÔNG chép vào chương trình để chạy

```cpp
int* escaped;
{
    int local = 7;
    escaped = &local;
}
// *escaped ở đây là undefined behavior: local đã hết lifetime.
```

Tương tự, tham chiếu trả về một biến cục bộ sẽ bị dangling. Không kiểm thử UB bằng cách “chạy xem có crash không”: không crash không có nghĩa đúng. Sau này `vector::push_back` có thể làm mất hiệu lực pointer/reference; xem bài 05.

## Checklist và câu hỏi

- [ ] Test null/empty/index bằng size và vùng con đều PASS.
- [ ] Không đọc hay ghi ngoài vùng đã được caller chỉ định.
- [ ] Giải thích vì sao con trỏ thường không bị cấm bởi quy tắc “không raw new/delete”.
- [ ] So sánh khi nào dùng tham chiếu, khi nào dùng con trỏ có thể nullptr.
- [ ] Vì sao chỉ kiểm tra nullptr vẫn chưa chứng minh con trỏ an toàn?

Liên quan: buffer có giới hạn, lifetime trước khi học RAII/smart pointer; CMN-COD-01 và T4-REQ-04.
