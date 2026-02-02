RUNTIME MIN
FRAME LOOP
|
|- Snapshot ( đóng băng world )
- a copy of the world; all subsequent systems view the world through it.
- Một bản sao của thế giới; tất cả các hệ thống tiếp theo đều nhìn thế giới thông qua nó.

|-Resolve (read-only)
- This is where all gameplay / system decisions are computed..
- Đây là nơi tất cả các quyết định về lối chơi/hệ thống được tính toán.

|- Validate (invariant)
- Check if the conclusion is reasonable, check for correctness/incorrectness, logical validity/illogicality.
-  kiểm tra xem kết luận có hợp lý không , check đúng / sai , hợp lý / vô /lý của logic

|- Priority / Interrupt
- In any system, if you have to consider and choose how to resolve something, there's always a priority rule.
- Trong 1 hệ thống nếu phải xét và chọn lựa giải quyết 1 điều gì đó thì luôn có luật ưu tiên .

- Interrupts must always be executed before commits. If executed after commits, it will always be a bug.
- interrupt luôn phải thực hiện trước commit .. nếu làm sau commit thì luôn là bug.
|- [Abort?]
 - Log + Trace ( không commit)
"When an abort occurs, you must always be notified:
- What caused the error?
- What violation occurred?
- The runtime must continue running after an abort , aborting a frame must not crash the system.."

- Huỷ bỏ khi xảy ra thì phải luôn được thông báo
- Lỗi do đâu
- Vi phạm gì
- Chương trình phải tiếp tục chạy sau khi bị hủy bỏ, việc hủy bỏ một khung hình không được làm sập hệ thống.
|- Commit (atomic – 1 lần)
- This is the only place to make world changes and commit only once.
- Đây là nơi duy nhất thực hiện thay đổi world  và chỉ commit 1 lần duy nhất 
|- Side-effect (read Decision only)
- This is where the commit results are reflected; read the results after the decision has been made.
- Absolutely do not edit or change any status data.

- Đây là nơi phản ánh kết quả sau commit , đọc kết quả khi đã phán quyết .
- Tuyệt đối không chỉnh sửa hay thay đổi bất cứ dữ liệu thông tin trạng thái nào
|-Cleanup
- This is the final processing location within the frame.
- All temporary states , actions and transient data must be cleared after use.

"- Là nơi xử lý cuối cùng ở frame.
- Tất cả trạng thái tạm thời, hành động và dữ liệu tạm thời phải được xóa sau khi sử dụng."
