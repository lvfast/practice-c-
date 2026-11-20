# Luyện C++ trước Topic 4

Đây là **khung để bạn tự code**, chưa phải bài làm Vehicle Gateway/UDS. Bộ bài dành cho người đã biết biến, `if`, vòng lặp và hàm, nhưng cần học con trỏ, OOP và modern C++ có hệ thống.

Chỉ thư mục `practice` được bổ sung. Không cần sửa `common` hay bất kỳ thư mục topic nào.

## 1. Học theo thứ tự

| Bài | Nội dung chính | Mở đề |
|---|---|---|
| 01 | Hàm, struct, tham chiếu, const, header/source | [Hàm và dữ liệu](01_functions_data/README.md) |
| 02 | Con trỏ, buffer, lifetime, quyền mượn | [Con trỏ và vòng đời](02_pointers_lifetime/README.md) |
| 03 | Constructor, class, destructor, RAII | [Class và RAII](03_classes_raii/README.md) |
| 04 | unique_ptr, ownership, copy/move | [Smart pointer](04_smart_pointers_move/README.md) |
| 05 | array/vector/string/optional, lambda | [STL và chuỗi byte](05_stl_bytes/README.md) |
| 06 | Kiểu số, bit, mask, enum, hex | [Kiểu số và bit](06_bits_numbers/README.md) |
| 07 | Template, signed/unsigned, big-endian | [Template và endian](07_templates_endian/README.md) |
| 08 | constexpr, thuật toán xử lý theo bảng | [Decoder dữ liệu giả](08_table_decoder/README.md) |
| 09 | Interface, file, fake, trạng thái lỗi | [Interface và I/O](09_interfaces_io/README.md) |
| 10 | Thread, mutex, condition variable, queue | [Đa luồng](10_threads_queue/README.md) |
| 11 | Library/demo/test targets, CMake, GoogleTest | [Build và kiểm thử](11_cmake_gtest/README.md) |

Mỗi bài độc lập: không cần chép lời giải của bài trước vào bài sau. Ở bài 02, 05 và 07, `optional` xuất hiện sớm để thể hiện “không có kết quả”; bài 05 giải thích sâu hơn. Cú pháp template có sẵn trong bài 07/10 để bạn tập trung viết phần thân.

## 2. Bắt đầu từ bài 01

Các lệnh dưới đây chạy từ thư mục gốc `CPP_Telematics_Homework`.

**Windows / PowerShell** — cần `g++` trong PATH (máy hiện tại có GCC MSYS2 UCRT64):

```powershell
g++ --version
powershell -NoProfile -ExecutionPolicy Bypass -File .\practice\run.ps1 -Lesson 01 -Mode test
powershell -NoProfile -ExecutionPolicy Bypass -File .\practice\run.ps1 -Lesson 01 -Mode demo
```

PowerShell hiện tại chặn chạy file `.ps1` theo mặc định. Hai lệnh trên cho phép chạy trong **tiến trình vừa tạo**, không thay execution policy của máy hay user. Các đề bên dưới dùng dạng ngắn `.\practice\run.ps1 ...`; nếu bị chặn, thay bằng tiền tố `powershell -NoProfile -ExecutionPolicy Bypass -File` như trên. Không muốn dùng PowerShell runner thì chọn Bash hoặc tự build bằng g++.

Ví dụ tự build bài 01, không cần script:

```powershell
Push-Location practice/01_functions_data
g++ -std=c++17 -Wall -Wextra -Werror -pthread lesson.cpp test.cpp -o ../build/01_manual.exe
if ($LASTEXITCODE -eq 0) { & ../build/01_manual.exe }
Pop-Location
```

Tạo thư mục `practice/build` trước nếu chưa có. Với bài đa luồng, ưu tiên runner có timeout để dừng được một lời giải bị deadlock.

**Linux / WSL** — cần GCC có C++17, Bash và `timeout` từ coreutils:

```bash
g++ --version
bash practice/run.sh 01 test
bash practice/run.sh 01 demo
```

Runner tự build bằng `-std=c++17 -Wall -Wextra -Werror -pthread`, rồi chạy từ đúng thư mục bài để tìm thấy `data/`. Không cần CMake/GoogleTest cho bài 01–10. File thực thi nằm trong `practice/build`, không nằm cùng bài.

Chọn compiler khác bằng biến môi trường `CXX` trỏ đến **một executable**, không chứa các cờ compiler.

## 3. Cách tự làm một bài

1. Đọc README của bài: yêu cầu, hợp đồng API, ví dụ và lỗi/biên.
2. Mở `lesson.hpp` để xem chữ ký hàm/class; mở `test.cpp` để xem kết quả mong đợi.
3. Chạy test một lần. Ban đầu sẽ thấy `[TODO]`, không phải bài đã làm đúng.
4. Trong `lesson.cpp` (hoặc header với template), đặt tên cho các tham số đang bỏ trống, thay `practice::todo(...)` bằng phần tự viết. Giữ nguyên API và test.
5. Làm từng hàm, chạy lại test. Một test có nhiều kiểm tra: test chỉ PASS khi đã đi qua hết các kiểm tra đó.
6. Chạy demo, trả lời câu hỏi cuối đề, rồi đánh dấu checklist.

`support/` là hạ tầng được cung cấp, không phải phần cần tự viết. `lesson.cpp` và các vùng `TODO` trong header mới là bài làm. Bài 11 đánh dấu rõ thư viện cho sẵn và phần CMake cần hoàn thiện.

## 4. Đọc kết quả đúng

| Kết quả | Ý nghĩa |
|---|---|
| `[PASS]` | Test đã thực thi và thỏa toàn bộ kiểm tra |
| `[TODO]` | Đã gọi vào placeholder chưa làm |
| `[FAIL]` | Sai kết quả hoặc phát sinh exception ngoài dự kiến |
| `TIMEOUT` | Quá 10 giây; runner dừng tiến trình, có thể là deadlock/vòng lặp vô hạn |

Exit code: **0** = tất cả PASS; **1** = lỗi build/test; **2** = vẫn còn TODO; **124** = quá thời gian. Có cả FAIL và TODO thì ưu tiên exit 1. Runner không dùng `assert`, nên test không bị tắt bởi `NDEBUG`.

Chạy toàn bộ bài 01–10 sau khi tự làm:

```powershell
.\practice\run.ps1 -Lesson all -Mode test
# Chỉ kiểm tra build:
.\practice\run.ps1 -Lesson all -BuildOnly
```

```bash
bash practice/run.sh all test
```

**Kiểm tra khung chưa làm** dành cho người bảo trì:

```powershell
.\practice\run.ps1 -Lesson all -Mode check
python practice/tools/check_scaffold.py
```

```bash
bash practice/run.sh all check
```

`check` chỉ chấp nhận khung còn TODO. Nó thành công **không có nghĩa bạn đã giải đúng bài**; sau khi tự code hãy dùng `test`. Python là tùy chọn cho việc kiểm tra bộ khung, không cần để học/chạy bài.

## 5. Kiến thức nào liên quan Topic 4?

Đối chiếu [yêu cầu chung](../common/COMMON_REQUIREMENTS.md) và [đề Topic 4](../topic4_gateway_diagnostics/README.md).

| Yêu cầu | Kiến thức chuẩn bị | Bài |
|---|---|---|
| CMN-BLD-01..04 | C++17, warnings, tách library/demo/test, CMake | 01, 11 |
| CMN-COD-01 | Con trỏ không sở hữu, lifetime, RAII, smart pointer | 02–04 |
| CMN-COD-02, 04 | enum class, optional, lambda, move, constexpr, array, range-for | 04–08, 10 |
| CMN-COD-03; T4-REQ-01 | Interface trừu tượng, đa hình, nguồn file và fake | 09, 11 |
| CMN-COD-05; T4-REQ-04, 10 | Kiểm tra độ dài/trạng thái lỗi, log có ngữ cảnh | 02, 06–09 |
| CMN-COD-06; T4-REQ-05 | Queue an toàn luồng, đóng queue, bảo vệ snapshot | 10 |
| T4-REQ-02 | Bit extraction, scale/offset, enum, đếm mã lạ | 06, 08 |
| T4-REQ-03 | Bảng dữ liệu constexpr/array, một thuật toán dùng chung | 08 |
| T4-REQ-07..09 | Buffer vector<uint8_t>, chuỗi ASCII cố định, ghép byte | 05, 07 |
| T4-REQ-11 | Helper template big-endian cho kiểu nguyên | 07 |
| CMN-TST-01..04 | GoogleTest/CTest, lỗi/biên, fake, traceability | Test của các bài; đặc biệt 09, 11 |
| CMN-DOC, hướng dẫn trình bày | Giải thích kỹ thuật, đọc API, gắn yêu cầu với test | Câu hỏi từng bài, bài 11 |

Đây là bảng **chuẩn bị kiến thức**, không xác nhận đã triển khai các requirement Topic 4. CAN ID/DID, mã UDS/NRC, cấu trúc CAN và đơn vị tín hiệu là kiến thức giao thức cần đọc khi bắt đầu Topic 4; hiện chưa code các phần đó.

## 6. Những phần cần hiểu nhưng chưa cần dùng hết

- `shared_ptr`/`weak_ptr`: phần mở rộng ở bài 04; không thay `unique_ptr` một cách máy móc.
- `atomic`: lựa chọn cho một trạng thái đơn giản, không thay được cơ chế bảo vệ cả queue; xem bài 10.
- `chrono`: dùng cho thời hạn chờ trong test; không dùng `sleep` để “sửa” race condition.
- `variant`: phù hợp khi cần giữ giá trị bool/enum/số riêng biệt; bài 08 dùng giá trị số để giữ bài nhỏ.
- `std::function`: wrapper callable có kiểu cố định; so sánh với lambda và template trong bài 10.
- Sanitizer, clang-tidy/cppcheck, coverage: mở rộng ở bài 11, không bắt buộc cài cho bộ luyện ban đầu.

Đề chỉ yêu cầu **ít nhất ba** tính năng modern C++ trong danh sách, không yêu cầu dùng tất cả. Chưa học template metaprogramming nâng cao, allocator, lock-free queue hay multiple inheritance. SocketCAN, DTC, MISRA/AUTOSAR để sau.

## 7. Checklist sẵn sàng học Topic 4

- [ ] Giải thích được con trỏ/tham chiếu đang mượn hay sở hữu dữ liệu, và dữ liệu sống đến bao giờ.
- [ ] Dùng unique_ptr/RAII, không tự new/delete; hiểu move khác copy.
- [ ] Không đọc ngoài buffer; xử lý size/index và nullptr trước khi dereference.
- [ ] Tách bit và endian không dựa vào layout bộ nhớ của máy.
- [ ] Phân biệt buffer byte với C-string; không tự thêm byte kết thúc vào chuỗi cố định.
- [ ] Dùng interface qua tham chiếu/con trỏ, có virtual destructor và test qua fake.
- [ ] Bảo vệ mọi dữ liệu dùng chung, đóng queue và join các worker.
- [ ] Build/test bằng CMake + GoogleTest trên Linux/WSL trước khi nộp bài thật.
- [ ] Có thể giải thích vì sao chọn kỹ thuật, không chỉ mô tả cú pháp.

Môi trường luyện Windows không thay thế yêu cầu nộp trên Linux/WSL. Bộ khung không cài công cụ, không tải dependency tự động.

Xem [kết quả kiểm tra và giới hạn môi trường](VERIFICATION.md) để phân biệt phần đã chạy với phần chưa thể xác nhận.
