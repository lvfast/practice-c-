# Bài 10 — Queue đa luồng và trạng thái dùng chung

## Mục tiêu và lý thuyết

Hai thread đọc/ghi cùng dữ liệu mà không đồng bộ có thể tạo data race và undefined behavior. `volatile` không sửa được vấn đề này. `mutex` bảo vệ cả invariant của dữ liệu; lock_guard/unique_lock giải phóng lock theo RAII.

`condition_variable` cho consumer chờ mà không busy-wait. Wait có thể thức dậy giả, nên cần **predicate** dựa trên state đang được bảo vệ bằng cùng mutex. unique_lock cho phép wait nhả và lấy lại lock. Thay đổi state phải được bảo vệ, còn thông báo cho worker cần đúng với thay đổi đó.

Queue bảo vệ phần tử trong queue, **không tự bảo vệ biến “giá trị mới nhất” nằm ngoài nó**. LatestValue cần mutex riêng, kể cả hàm read là const: mutex có thể mutable mà không làm thay đổi giá trị logic của object.

Thread chạy callable/lambda. Capture `&` chỉ mượn dữ liệu, phải bảo đảm dữ liệu sống tới lúc worker dừng. std::thread còn joinable khi bị hủy sẽ gây terminate; join mọi worker trước khi các dữ liệu chúng mượn hết scope. Exception không được thoát khỏi entry function của thread.

## Đề và hợp đồng API

Hoàn thiện template trong header và LatestValue trong source. Giữ nguyên các member đã cung cấp.

| API | Yêu cầu |
|---|---|
| `push(T value)` | Khi mở: đưa vào cuối queue bằng move, trả true, đánh thức consumer. Khi đóng: false |
| `pop()` | Chờ nếu queue mở và rỗng; có phần tử thì lấy theo FIFO; đóng và rỗng thì nullopt |
| `close()` | Đặt trạng thái đóng, không xóa các phần tử còn lại; đánh thức mọi consumer; gọi lặp lại an toàn |
| `LatestValue::update(int)` | Ghi giá trị dưới mutex |
| `LatestValue::read() const` | Trả bản sao optional dưới mutex; ban đầu nullopt, giá trị 0 vẫn hợp lệ |

Queue không giới hạn dung lượng, hỗ trợ kiểu move-only như unique_ptr. T phải là kiểu có thể move vào/ra queue. Queue không copy được. Payload nullptr trong một unique_ptr vẫn là **một phần tử**, không phải tín hiệu kết thúc queue.

Vì push nhận by-value, caller truyền std::move(payload) có thể mất ownership ngay cả khi queue đã đóng; không hứa khôi phục tham số. Khi push chạy đồng thời close, thao tác nào giữ lock trước quyết định phần tử có được nhận; sau close hoàn tất không được nhận thêm.

Caller chịu trách nhiệm close và join trước khi queue bị hủy. Không tự detach thread, không sleep để đồng bộ, không trả reference tới phần tử đã pop. Nếu nối với xử lý tín hiệu sau này, lấy dữ liệu ra dưới lock rồi xử lý bên ngoài vùng lock.

Ví dụ: push 10, push 20, close, pop → 10, pop → 20, pop → nullopt; push 30 sau close → false.

## Chạy và chống treo

```powershell
.\practice\run.ps1 -Lesson 10 -Mode test
.\practice\run.ps1 -Lesson 10 -Mode demo
# Khi debug máy chậm, có thể tăng giới hạn tiến trình:
.\practice\run.ps1 -Lesson 10 -TimeoutSeconds 30
```

Linux/WSL: `bash practice/run.sh 10 test`. Demo cơ bản chạy queue tuần tự, in `received=21`; test.cpp mới chạy producer/consumer và đọc/ghi snapshot đồng thời.

Test dùng promise/future để phối hợp worker; packaged_task chuyển exception về thread chạy test. `support/thread_test.hpp` là hạ tầng join/cleanup cho sẵn, không phải lời giải queue. Thời hạn chờ ngắn chỉ phát hiện pop trả sớm, thời hạn dài phát hiện không thức dậy; không dùng sleep làm cơ chế đúng/sai. Runner dừng tiến trình sau 10 giây nếu code sai gây deadlock khi join.

## Gợi ý theo mức

1. Liệt kê bốn trạng thái: mở+rỗng, mở+có dữ liệu, đóng+có dữ liệu, đóng+rỗng.
2. Predicate chờ phải cho phép thoát khi có dữ liệu **hoặc** đã đóng.
3. Tách thời điểm cập nhật state, notify và lấy payload ra; mọi truy cập items_/closed_ phải cùng cơ chế khóa.

## Mở rộng

- `atomic<int>` có thể thay mutex cho một số nguyên độc lập, nhưng không tự bảo vệ queue và invariant nhiều trường.
- `chrono` biểu diễn duration/time_point; timeout để phát hiện lỗi không thay cho đồng bộ đúng.
- `std::function<void()>` giữ callable có chữ ký cố định; template callable có thể giữ được kiểu move-only mà std::function C++17 không nhận. Lambda chỉ là một trong các loại callable.
- Có thể tự mở rộng demo thành hai thread sau khi test chính PASS. Không phải làm gateway thật.

## Checklist và câu hỏi

- [ ] FIFO 500 phần tử không mất/trùng; payload unique_ptr hoạt động.
- [ ] close queue rỗng đánh thức consumer; queue còn dữ liệu được drain; push sau close bị từ chối.
- [ ] Snapshot có khóa ở cả read và update; mọi worker được join.
- [ ] Giải thích vì sao `if (empty) wait()` có thể sai và vì sao cần notify_all khi đóng.
- [ ] Vì sao test chạy PASS nhiều lần chưa chứng minh chương trình không có data race? Dùng ThreadSanitizer khi môi trường hỗ trợ.

Liên quan: T4-REQ-05, CMN-COD-06; chưa kết nối CAN hay UDS.
