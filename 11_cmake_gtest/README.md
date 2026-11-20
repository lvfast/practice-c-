# Bài 11 — CMake, GoogleTest và CTest

## Phần đã cho sẵn và phần tự làm

`include/measurements.hpp`, `src/measurements.cpp`, demo và **11 GoogleTest** là dữ liệu luyện được cung cấp sẵn. Đây là thư viện số đo nhỏ, không phải lời giải các bài 01–10 hay Topic 4.

Bạn tự hoàn thiện các `TODO 11.*` trong CMakeLists để thêm test executable và đăng ký test vào CTest, sau đó tự viết thêm ba test. Library/demo targets được làm mẫu để bạn đọc trước khi tạo target test.

## Lý thuyết tối thiểu

- CMake configure/generate khác compile/link. `cmake -S ... -B ...` tạo build tree; `cmake --build` gọi build tool.
- Library chứa logic dùng lại; demo chứa main; test executable dùng main từ GTest::gtest_main, không link main của demo vào test.
- PUBLIC include path truyền cho target sử dụng thư viện; PRIVATE chỉ dùng trong target hiện tại. Link vào measurements giúp test include `measurements.hpp` qua public interface.
- GoogleTest cung cấp ASSERT/EXPECT và test runner; CTest phát hiện/chạy executable đã đăng ký. Build được test executable chưa đồng nghĩa CTest đã biết các test.
- Test case có expected độc lập, bao phủ input thường/lỗi/biên; fake chỉ thay nguồn input, assertions kiểm tra collector thật.

## Điều kiện môi trường

Cần CMake >= 3.20, compiler C++17, build tool phù hợp generator và GoogleTest được cài/build tương thích **cùng compiler, kiến trúc và runtime**. Không trộn GoogleTest build bằng MSVC với GCC UCRT64.

Máy được kiểm tra lúc tạo khung có GCC MSYS2 UCRT64, chưa tìm thấy CMake/GoogleTest. Bộ luyện không tự cài hoặc tải thư viện. Chuẩn bị công cụ bằng phương thức quản lý gói của môi trường bạn chọn; dùng Ninja nếu đã có, hoặc generator khác phù hợp. Trong MSYS2, dùng compiler và các thư viện cùng UCRT64; trên Linux/WSL dùng toolchain Linux.

`find_package(GTest REQUIRED)` dùng thư viện đã chuẩn bị. Nếu chưa tìm thấy, configure phải dừng với lỗi thiếu GTest, không chuyển sang test giả. Với bản cài riêng, truyền `-DCMAKE_PREFIX_PATH=<prefix>` hoặc `-DGTest_DIR=<folder-chứa-GTestConfig.cmake>`.

## Đề bài

1. Đọc ba public API trong measurements.hpp, chạy demo baseline nếu đã có CMake.
2. Tạo executable **measurements_tests** từ `tests/measurements_test.cpp`.
3. Link target đó với **measurements** và **GTest::gtest_main**. Nếu config package của bạn không có imported target này, chuẩn bị GoogleTest tương thích thay vì đoán tên library thủ công.
4. Include module **GoogleTest**, dùng **gtest_discover_tests** để CTest khám phá executable.
5. Xóa warning TODO và test `TODO_register_google_tests` sau khi đã hoàn tất bước 2–4. Không chỉ xóa pending test rồi bỏ qua đăng ký test thật.
6. Viết thêm ba test: một case thường với expected tính tay, một lỗi/biên chưa có và một case collector qua fake. Chọn hành vi mới, không lặp lại test chỉ để tăng số lượng.
7. Thêm bảng traceability ngắn trong ghi chú cá nhân: yêu cầu thư viện → hàm → tên test. Chỉ trace thư viện luyện tập, không đánh dấu T4 đã hoàn thành.

Các API cho sẵn: sample hợp lệ nếu ID>0 và value hữu hạn [0,100]; average trả nullopt với input rỗng/không hữu hạn hoặc tổng trung gian không hữu hạn; summarize bỏ sample sai và đọc đến hết source. Không sửa quy tắc để ép test xanh.

Ví dụ demo: `count=2 mean=20`. Fake `{1,10}, {0,99}, {2,30}` → count=2, sum=40.

## Chạy trên Linux/WSL

Từ thư mục gốc, sau khi hoàn thiện TODO:

```bash
cmake -S practice/11_cmake_gtest -B practice/11_cmake_gtest/build -DCMAKE_BUILD_TYPE=Debug
cmake --build practice/11_cmake_gtest/build
./practice/11_cmake_gtest/build/measurements_demo
ctest --test-dir practice/11_cmake_gtest/build -N
ctest --test-dir practice/11_cmake_gtest/build --output-on-failure --no-tests=error
```

## Chạy trên Windows / PowerShell

Ví dụ dùng Ninja khi bạn **đã có** Ninja và GoogleTest tương thích GCC:

```powershell
cmake -S practice/11_cmake_gtest -B practice/11_cmake_gtest/build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++
cmake --build practice/11_cmake_gtest/build
.\practice\11_cmake_gtest\build\measurements_demo.exe
ctest --test-dir practice/11_cmake_gtest/build -N
ctest --test-dir practice/11_cmake_gtest/build --output-on-failure --no-tests=error
```

Nếu dùng generator nhiều cấu hình, build thêm `--config Debug`, CTest thêm `-C Debug`, executable thường nằm trong thư mục Debug. Dùng build directory khác nhau cho Windows và Linux hoặc khi đổi compiler/generator.

Trước khi làm TODO, CMake có thể build library/demo nếu dependencies đủ, nhưng CTest cố ý có test pending **FAIL**. `ctest -N` phải liệt kê ít nhất 11 test thật sau khi bạn hoàn thiện đăng ký (14 nếu đã thêm ba test), không chỉ một pending test.

Chỉ muốn chạy demo baseline mà chưa chuẩn bị GoogleTest: configure một build tree khác với `-DBUILD_TESTING=OFF`. Cách này không đáp ứng phần kiểm thử, không coi là hoàn thành bài.

## Gợi ý theo mức

1. Đối chiếu library/demo targets làm mẫu; test cần target riêng và đúng dependency.
2. Lỗi không tìm thấy header khác lỗi undefined reference và khác lỗi không tìm thấy GTest package.
3. Khi CTest báo “No tests were found”, kiểm tra đăng ký test và build tree đang dùng trước khi sửa logic thư viện.

## Mở rộng và checklist

Khi môi trường hỗ trợ, thử AddressSanitizer/UndefinedBehaviorSanitizer cho truy cập bộ nhớ, ThreadSanitizer riêng cho bài 10; không trộn ASan và TSan trong cùng một build. Sanitizer chỉ thấy đường chạy đã thực thi, không chứng minh mọi input đều an toàn. Có thể thử clang-tidy/cppcheck và gcov/lcov sau khi bài chính xong; đây là phần tùy chọn của common.

- [ ] Có ba target riêng, compile C++17 không warning; CTest khám phá test thật.
- [ ] 11 test cho sẵn và ba test tự viết PASS; có ít nhất ba lỗi/biên và một test collector qua fake.
- [ ] Giải thích lỗi compile/link/configure/test bằng một ví dụ cụ thể.
- [ ] Giải thích vì sao fake test không chứng minh FileSource đọc file đúng.
- [ ] Ghi ba kỹ thuật modern C++ và lý do chọn, một lỗi đã gặp và cách tìm ra nguyên nhân; phục vụ phần trình bày sau này.

Liên quan: CMN-BLD-01..04, CMN-TST-01..04, traceability và hướng dẫn trình bày. Khi làm bài thật, vẫn phải kiểm tra build từ sạch trên Linux/WSL.
