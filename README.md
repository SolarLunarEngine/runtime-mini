# runtime-mini

A minimal, learning-focused frame-based runtime prototype.
This project explores how to structure game/runtime logic to avoid common state and timing bugs.

> 🇻🇳 Một runtime tối giản dùng cho mục đích học tập.  
> Dự án này tập trung vào cách tổ chức frame loop để tránh các bug về state, thứ tự xử lý và commit.

---

## Frame Loop Overview

Each frame is executed in strict phases.
Violating these rules is considered a bug.

> 🇻🇳 Mỗi frame được chia thành các phase rõ ràng.  
> Vi phạm thứ tự hoặc luật của các phase này được xem là bug hệ thống.

Frame Loop
│
├─ Snapshot (freeze world)
├─ Resolve (read-only)
├─ Validate (invariants)
├─ Priority / Interrupt
├─ Abort (if violated, no commit)
├─ Commit (atomic, once)
├─ Side-effect (read decision only)
└─ Cleanup
---

## Snapshot (Read-only World)

Freeze the world state at the beginning of a frame.
All systems must read from this snapshot, never from the live world.

> 🇻🇳 Đóng băng trạng thái world ở đầu frame.  
> Mọi system chỉ được đọc snapshot, **không được đọc hoặc ghi world trực tiếp**.

---

## Resolve (Decision Making)

Compute all gameplay or system decisions based on the snapshot.
No world mutation is allowed here.

> 🇻🇳 Tính toán toàn bộ quyết định (damage, death, state change…).  
> Giai đoạn này **chỉ ra quyết định**, tuyệt đối không ghi world.

---

## Validate (Invariants)

Check whether decisions violate runtime invariants.
Examples:
- HP must not be negative
- Commit must be called once per frame

If validation fails, the frame is aborted.

> 🇻🇳 Kiểm tra các luật bất biến của hệ thống.  
> Nếu vi phạm, frame bị huỷ (abort) và **không được commit**.

---

## Priority / Interrupt

When multiple intents conflict, priority rules must be applied.
Interrupts must always happen **before commit**.

> 🇻🇳 Khi có nhiều intent xung đột, luôn phải có luật ưu tiên.  
> Interrupt **luôn chạy trước commit**, nếu không sẽ gây bug nghiêm trọng.

---

## Abort (Fail-Safe)

When a frame is aborted:
- No world changes occur
- The runtime logs the reason
- The system continues to the next frame

> 🇻🇳 Khi abort:
> - Không thay đổi world  
> - Ghi log + trace lỗi  
> - Không được làm sập hệ thống

---

## Commit (Write World – Once)

Commit is the **only place** where the world can be mutated.
It must happen **exactly once per frame**.

> 🇻🇳 Commit là nơi duy nhất được ghi world  
> Mỗi frame **chỉ được commit 1 lần**, không hơn.

---

## Side-effect (Read Decision Only)

Trigger effects (VFX, sound, logs) based on committed decisions.
Side-effects must never touch the world state.

> 🇻🇳 Side-effect chỉ để “kể lại kết quả đã xảy ra”.  
> Tuyệt đối **không được ghi hoặc sửa world** trong phase này.

---

## Cleanup

Clear all temporary data, traces, and per-frame state.

> 🇻🇳 Dọn dẹp toàn bộ dữ liệu tạm sau khi frame kết thúc.

---

## Debug vs Release Behavior

- Debug build enables full tracing and frame replay
- Release build logs minimal warnings and prioritizes stability

> 🇻🇳  
> - Debug: ưu tiên trace, bắt bug, replay frame  
> - Release: log tối thiểu, không crash, tiếp tục chạy

---

## Purpose of This Project

This project is **not** a production engine.
It is a learning artifact used to:
- Practice frame-based thinking
- Understand runtime bugs
- Design safer system boundaries

> 🇻🇳 Đây không phải engine hoàn chỉnh.  
> Đây là dự án học tập để rèn tư duy runtime, frame loop và debug hệ thống.

---

## Disclaimer

This code is written for learning and experimentation purposes.
Design choices favor clarity and correctness over performance.

> 🇻🇳 Code ưu tiên dễ hiểu và đúng tư duy hơn là tối ưu hiệu năng.


