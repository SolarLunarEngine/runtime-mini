# Bug Replay 003 – Side-effect Mutates World
# Bug Replay 003 – Side-effect chỉnh sửa world sau commit

## Scenario
After a commit decides the final game state, a side-effect function
modifies the world state directly.

## Kịch bản
Sau khi commit đã quyết định trạng thái cuối cùng của game,
một hàm side-effect vẫn trực tiếp chỉnh sửa world state.

---

## Observed Behavior
- Commit sets the final state of the entity
- Side-effect function modifies world data
- Output appears correct in simple cases

## Hành vi quan sát được
- Commit đã thiết lập trạng thái cuối
- Side-effect vẫn ghi vào world
- Kết quả in ra trông có vẻ đúng ở ví dụ đơn giản

---

## Root Causes
1. Side-effect is allowed to mutate world state
2. World mutation is not restricted to commit phase
3. No guard prevents post-commit modification

## Nguyên nhân gốc
1. Side-effect được phép chỉnh sửa world
2. Việc ghi world không bị giới hạn ở phase commit
3. Không có guard chặn chỉnh sửa sau commit

---

## Violated Invariants
- World state must only be mutated during commit
- Side-effects must be read-only
- Commit must be the single source of truth

## Các bất biến (invariant) bị phá vỡ
- World chỉ được thay đổi trong commit
- Side-effect phải là read-only
- Commit là nguồn sự thật duy nhất

---

## Why This Is Dangerous
This bug does not fail immediately.
As the system grows, it causes silent state corruption,
breaking determinism and stability.

## Vì sao bug này nguy hiểm
Bug này không gây lỗi ngay.
Khi hệ thống lớn dần, nó âm thầm làm sai state,
phá vỡ tính ổn định và khả năng kiểm soát hệ thống.

---

## Proposed Fix
- Restrict all world mutations to the commit phase
- Enforce read-only side-effects
- Add guards or assertions to prevent post-commit mutation

## Hướng sửa đề xuất
- Giới hạn mọi thay đổi world chỉ ở phase commit
- Bắt buộc side-effect chỉ được đọc dữ liệu
- Thêm guard/assert để chặn chỉnh sửa sau commit

---

## Notes
This bug highlights the importance of separating decision,
commit, and side-effect responsibilities.

## Ghi chú
Bug này nhấn mạnh tầm quan trọng của việc tách biệt rõ
decision, commit và side-effect.