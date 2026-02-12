# Runtime Invariants

## 1. World Mutation
World state may only be mutated inside Commit.

*Trạng thái của world chỉ được phép thay đổi bên trong Commit.*

---

## 2. Single Commit Per Frame
Each frame may perform at most one Commit.

*Mỗi frame chỉ được thực hiện tối đa một lần Commit.*

---

## 3. Side-Effect Isolation
Side-effects must not mutate world state.

*Side-effect không được phép thay đổi trạng thái world.*

---

## 4. Terminal Finality
Entities in terminal state must not participate in subsequent frame processing.

*Entity ở trạng thái terminal không được tham gia xử lý ở các frame sau.*

---

# Guard Mapping / Ánh xạ Guard

## Invariant 1 — World Mutation
World state may only be mutated inside Commit.

Protected by:
- Commit is the only function that mutates world state.
- No other function writes to world.

*Được bảo vệ bởi:*
- Commit là nơi duy nhất thay đổi world.
- Không có hàm nào khác được phép ghi vào world.

---

## Invariant 2 — Single Commit Per Frame
Each frame may perform at most one Commit.

Protected by:
- g_committedThisFrame debug guard.
- Assert against nested or double commit.

*Được bảo vệ bởi:*
- Guard g_committedThisFrame.
- Assert chặn nested hoặc double commit.

---

## Invariant 3 — Side-Effect Isolation
Side-effects must not mutate world state.

Protected by:
- No world mutation allowed inside PlayEffect.
- Runtime structure separates commit and side-effect phase.

*Được bảo vệ bởi:*
- PlayEffect không được phép thay đổi world.
- Cấu trúc runtime tách commit và side-effect rõ ràng.

---

## Invariant 4 — Terminal Finality
Entities in terminal state must not participate in subsequent frame processing.

Protected by:
- Frame boundary check for terminal entities.
- Commit guard preventing commit after terminal.
- Side-effect skip if entity is terminal.

*Được bảo vệ bởi:*
- Kiểm tra terminal tại frame boundary.
- Guard trong commit chặn commit sau khi terminal.
- Side-effect bỏ qua entity đã terminal.