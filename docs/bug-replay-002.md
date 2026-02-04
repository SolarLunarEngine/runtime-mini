# Bug Replay 002 – Commit After Terminal State
# Bug Replay 002 – Commit sau trạng thái kết thúc (dead)

## Scenario
An entity receives damage and reaches a terminal state (dead),
but systems continue to process damage and effects in subsequent frames.

## Kịch bản
Thực thể nhận sát thương và đã đạt trạng thái kết thúc (dead),
nhưng các hệ thống vẫn tiếp tục xử lý sát thương và hiệu ứng ở các frame sau.

---

## Observed Behavior
- Frame 1: Damage is committed, entity becomes dead
- Death effect is played
- Frame 2: Damage is resolved again
- Commit and side-effects still run on a dead entity

## Hành vi quan sát được
- Frame 1: Sát thương được commit, thực thể chết
- Hiệu ứng chết được kích hoạt
- Frame 2: Sát thương vẫn tiếp tục được resolve
- Commit và side-effect vẫn chạy trên một thực thể đã chết

---

## Root Causes
1. Pending damage is not cleared after commit
2. Resolve phase does not check terminal state
3. Commit allows execution after entity reaches a terminal state
4. Side-effect reads world state to infer logic

## Nguyên nhân gốc
1. Trạng thái pending damage không được xoá sau khi commit
2. Giai đoạn resolve không kiểm tra trạng thái terminal (dead)
3. Commit vẫn cho phép thực thi sau khi entity đã chết
4. Side-effect đọc world state để suy luận logic (sai vai trò)

---

## Violated Invariants
- Terminal states must be final and one-way
- Pending intent must not survive across frames
- Side-effects must not make logical decisions

## Các bất biến (invariant) bị phá vỡ
- Trạng thái kết thúc (dead) phải là một chiều, không được quay lại
- Intent/pending chỉ được tồn tại trong một frame
- Side-effect không được phép quyết định logic

---

## Why This Is Dangerous
This bug does not crash immediately.
It silently applies invalid logic across frames,
making it difficult to detect in production.

## Vì sao bug này nguy hiểm
Bug này không làm crash ngay.
Nó âm thầm áp dụng logic sai qua nhiều frame,
khiến rất khó phát hiện trong môi trường production.

---

## Proposed Fix
- Clear pending state at frame boundary
- Prevent resolve from generating intent for dead entities
- Guard commit against terminal states
- Restrict side-effects to reading decisions only

## Hướng sửa đề xuất
- Xoá pending state ở ranh giới frame
- Ngăn resolve sinh intent cho entity đã chết
- Bảo vệ commit khỏi việc chạy trên terminal state
- Giới hạn side-effect chỉ được đọc decision, không đọc world

---

## Notes
This bug was identified through manual bug replay and frame-based reasoning.

## Ghi chú
Bug này được xác định thông qua việc replay bug thủ công
và phân tích theo từng frame của runtime.