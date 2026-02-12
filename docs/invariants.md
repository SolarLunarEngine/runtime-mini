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