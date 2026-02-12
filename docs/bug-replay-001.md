# Bug Replay 001 – Cross-frame State Leak

## Scenario
A damage value persists across frames and is applied one frame late.

## Observed Behavior
- Frame 1: HP should reach 0 → entity should die
- Frame 2: Damage is applied → entity dies late
- Dead flag and HP are inconsistent within the same frame

## Root Causes
1. Pending state survives across frames
2. Commit uses data not produced in the same frame
3. World invariants are violated (HP > 0 but dead = true)

## Violated Invariants
- Each frame must consume only its own decisions
- Commit must never read stale or cross-frame data
- World state must be self-consistent after commit

## Proposed Fix
- Clear pending state at end of frame
- Ensure commit only applies decisions generated in the same frame
- Validate invariants before and after commit

## Notes
This bug is intentionally constructed for learning and replay purposes.

---

## Related Invariant

This bug violates:
- Invariant 2 — Each frame may perform at most one Commit.