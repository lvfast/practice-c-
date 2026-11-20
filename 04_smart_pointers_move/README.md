# Bài 04 — Smart pointer, ownership và move

## Mục tiêu và lý thuyết

`unique_ptr<T>` sở hữu độc quyền một đối tượng. Khi owner bị hủy/thay thế, đối tượng cũ được hủy. Dùng `make_unique`, không dùng raw `new`/`delete`.

- `owner.get()` mượn địa chỉ, **không** chuyển ownership. Không tự delete địa chỉ đó.
- `std::move(x)` cho phép chọn thao tác move; bản thân lời gọi này không chuyển dữ liệu. Việc move xảy ra trong constructor/assignment/parameter nhận rvalue phù hợp.
- unique_ptr không copy được. Sau khi move thành công sang owner khác, source rỗng. Với các kiểu khác, moved-from thường chỉ bảo đảm hợp lệ, không mặc định rỗng.
- Đối tượng chứa string/vector thường để compiler quản lý copy/move/destructor theo Rule of Zero. Copy dữ liệu và copy ownership là hai vấn đề khác nhau.
- `reset()` thay/hủy đối tượng sở hữu; `release()` bỏ ownership và trả raw pointer, có thể gây leak nếu không chuyển sang owner khác. Bài này không cần release.

`Device` được cung cấp sẵn. Destructor ảo cho phép hủy derived object qua owner base. Test dùng `TrackedDevice` riêng để đếm hủy; không thêm biến đếm/test API vào Device.

## Đề và hợp đồng API

| API | Yêu cầu |
|---|---|
| `make_device(id)` | id>0 → unique_ptr giữ Device có ID đó; ngược lại nullptr |
| `borrowed_id(const Device*)` | Trả id, hoặc -1 nếu nullptr; không giữ lại/giải phóng đối tượng |
| `take_device(unique_ptr<Device>&)` | Trả owner mới của đúng đối tượng cũ; source rỗng. Source rỗng → kết quả rỗng |
| `replace_device(slot, replacement)` | Chuyển replacement vào slot, đối tượng cũ bị hủy đúng một lần; nullptr làm slot rỗng |

Không tạo bản sao Device để giả vờ chuyển ownership. Địa chỉ object phải giữ nguyên khi chuyển. Không dereference một pointer mượn sau khi owner của nó đã bị thay/hủy.

Ví dụ: factory(7) → owner hợp lệ. Sau take_device(source), source rỗng và owner mới vẫn id 7. Demo in `source empty=true borrowed id=7`.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 04 -Mode test
.\practice\run.ps1 -Lesson 04 -Mode demo
```

Linux/WSL: `bash practice/run.sh 04 test`.

## Gợi ý theo mức

1. Vẽ bằng giấy owner và đối tượng trước/sau mỗi lời gọi.
2. Tham số unique_ptr truyền bằng giá trị đòi hỏi caller chuyển ownership khi đang có owner.
3. Hãy để assignment/destructor của unique_ptr làm nhiệm vụ thu hồi tài nguyên, không viết cơ chế giải phóng riêng.

## Mở rộng: shared_ptr và weak_ptr

`shared_ptr` biểu diễn sở hữu chung thực sự; đối tượng được hủy khi hết strong owners. Hai đối tượng giữ shared_ptr đến nhau có thể tạo vòng giữ sống. `weak_ptr` không tăng strong count; `lock()` tạo owner tạm nếu đối tượng còn sống. Không chọn shared_ptr chỉ vì muốn truyền đối tượng sang hàm hoặc thread: tham chiếu hoặc move unique_ptr thường đủ.

Tự phân tích: cha sở hữu con, con chỉ cần truy cập cha — liên kết nào nên sở hữu? Nếu dùng weak_ptr, xử lý lock thất bại ra sao? Phần này không cần code để qua test chính.

## Checklist và câu hỏi

- [ ] Test xác nhận cùng địa chỉ sau chuyển và destructor chạy đúng một lần.
- [ ] Test thay/clear slot không leak hoặc double free.
- [ ] Giải thích `get()` khác `release()` và std::move khác copy.
- [ ] Vì sao một pointer mượn có thể dangling dù owner dùng smart pointer?
- [ ] Khi nào thực sự cần shared ownership thay vì chỉ mượn?

Liên quan: CMN-COD-01, 02; ownership cho interface và payload queue.
