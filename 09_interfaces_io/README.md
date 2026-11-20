# Bài 09 — Interface, đọc file, fake và lỗi có ngữ cảnh

## Mục tiêu và lý thuyết

Interface thuần ảo định nghĩa điều một nguồn dữ liệu làm được, không quy định nó lấy dữ liệu từ file hay nơi khác. `collect(IRecordSource&, ostream&)` dùng dependency injection: test truyền fake, demo truyền FileSource, không cần sửa collector.

Gọi qua reference/pointer giữ đa hình. Copy derived vào một base có thể làm mất phần derived (object slicing); interface thuần ảo không tạo được object base trực tiếp. Destructor base phải virtual khi có thể hủy derived qua owner base. `override` giúp compiler phát hiện viết sai chữ ký.

Stream có trạng thái good/fail/eof/bad. Một dòng đọc xong nhưng parse sai khác với không còn dòng, và cả hai khác lỗi mở file. Đừng dùng `while (!stream.eof())` rồi giả sử lần đọc kế tiếp thành công.

## Đề và hợp đồng API

`ReadResult` có status, optional record và detail. Quy tắc: `ok` luôn có record; các status khác không có record. `end` detail rỗng; `malformed`/`io_error` detail không rỗng.

**FileSource**

- Constructor thử mở file, không ném lỗi cho trường hợp không mở được; `next()` báo io_error có đường dẫn/nguyên nhân. Exception hệ thống như hết bộ nhớ không cần biến thành status.
- Mỗi lần next đọc một dòng; dòng hợp lệ chứa đúng hai token thập phân `sensor_id value`, có thể có whitespace trước/sau.
- ID > 0, value hữu hạn thuộc [0,100]. Dòng trống, token dư, số vượt phạm vi, NaN/inf hoặc thiếu token → malformed.
- Malformed phải chứa số dòng tính từ 1 và lý do, bỏ dòng đó để lần gọi tiếp đọc dòng sau.
- EOF lặp lại → end. Lỗi I/O thật → io_error, không giả làm EOF.
- Dùng RAII/ifstream; parse từng dòng bằng stringstream để token sai không phá toàn bộ stream file.

**collect(source,log)**

- Đọc đến end hoặc io_error. Với ok: tăng accepted và cộng value vào total.
- Malformed: tăng rejected, log detail rồi tiếp tục.
- Io_error: đặt io_failed=true, log detail rồi dừng ngay; không đọc các record phía sau.
- Không log một lỗi cho EOF bình thường; không dùng dynamic_cast để phụ thuộc implementation nguồn.

File `records.txt` có `1 10`, `BAD`, `2 20` → accepted=2, rejected=1, total=30. `invalid.txt` chứa năm dòng sai, trong đó có một dòng trống.

`numeric_edges.txt` bổ sung thiếu token, số vượt phạm vi, số âm và hai giá trị biên 0/100 có whitespace xung quanh.

## Chạy

```powershell
.\practice\run.ps1 -Lesson 09 -Mode test
.\practice\run.ps1 -Lesson 09 -Mode demo
```

Linux/WSL: `bash practice/run.sh 09 test`. Demo in thống kê trên stdout và lỗi dòng 2 trên stderr.

## Gợi ý theo mức

1. Viết nguồn file trước, kiểm tra từng status; sau đó viết collector chỉ biết interface.
2. Khi đã đọc được cả dòng, dùng stream khác để parse; kiểm tra còn token ngoài hai trường hay không.
3. Fake được cung cấp trong test.cpp. Kết quả cần kiểm tra là tổng/thống kê/log của collector thật, không phải “fake trả lại đúng dữ liệu đã cài”.

## Checklist và câu hỏi

- [ ] Không lẫn malformed/EOF/io_error; lỗi một dòng không ngăn đọc dòng sau.
- [ ] Log có dòng/nguyên nhân; test thật qua file và test collector qua fake đều PASS.
- [ ] Nếu đổi FileSource thành nguồn mạng, collector có phải đổi không?
- [ ] Vì sao destructor base cần virtual và vì sao tham số collector không truyền source theo giá trị?
- [ ] File đọc đồng thời từ hai thread có an toàn chỉ vì nó được bọc trong class không?

Liên quan: T4-REQ-01; CMN-COD-03, 05 và CMN-TST-03. Chưa parse CAN frame.
