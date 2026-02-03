#include <iostream>
#include <vector>
#include <string>

//
// =======================
// BUILD MODE
// =======================
#define DEBUG_BUILD 1   // 1 = Debug, 0 = Release (Production)

//
// =======================
// WORLD (SOURCE OF TRUTH)
// =======================
struct Player {
    int  hp   = 10;
    bool dead = false;
};

//
// =======================
// SNAPSHOT (READ-ONLY)
// =======================
struct PlayerSnapshot {
    int  hp;
    bool dead;
};

//
// =======================
// DECISION (INTENT / RESULT)
// =======================
struct Decision {
    int  nextHp = 0;
    bool die    = false;
};

//
// =======================
// TRACE SYSTEM (DEBUG ONLY)
// =======================
struct TraceEvent {
    int         frame;
    std::string phase;
    std::string message;
};

struct TraceLog {
    std::vector<TraceEvent> events;

    void add(int frame,
             const std::string& phase,
             const std::string& msg) {
#if DEBUG_BUILD
        events.push_back({frame, phase, msg});
#endif
    }

    void dump() const {
#if DEBUG_BUILD
        std::cout << "---- TRACE ----\n";
        for (const auto& e : events) {
            std::cout << "[Frame " << e.frame << "] "
                      << "[" << e.phase << "] "
                      << e.message << "\n";
        }
        std::cout << "---------------\n";
#endif
    }

    void clear() {
#if DEBUG_BUILD
        events.clear();
#endif
    }
};

//
// =======================
// SAFE LOGGING (PRODUCTION)
// =======================
enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

void SafeLog(LogLevel level,
             int frame,
             const std::string& phase,
             const std::string& reason) {
#if DEBUG_BUILD
    std::cout << "[DEBUG] Frame=" << frame
              << " Phase=" << phase
              << " Reason=" << reason << "\n";
#else
    if (level != LogLevel::INFO) {
        std::cout << "[WARN] Frame=" << frame
                  << " Phase=" << phase
                  << " Reason=" << reason << "\n";
    }
#endif
}

//
// =======================
// RESOLVE + VALIDATE
// =======================
bool ResolveAndValidate(
    int frame,
    const PlayerSnapshot& snap,
    Decision& out,
    TraceLog& trace
) {
    trace.add(frame, "Resolve", "Compute damage");

    int damage = 20;               // cố tình tạo logic bug
    out.nextHp = snap.hp - damage;

    trace.add(frame, "Resolve",
        "Decision nextHp = " + std::to_string(out.nextHp));

    // Invariant: HP không âm trước commit
    if (out.nextHp < 0) {
        trace.add(frame, "Validate",
            "Invariant violated: HP < 0 before commit");

        SafeLog(
            LogLevel::ERROR,
            frame,
            "Validate",
            "HP < 0 before commit"
        );

        return false; // Abort frame
    }

    if (out.nextHp == 0) {
        out.die = true;
    }

    return true;
}

//
// =======================
// COMMIT (WRITE WORLD – ONLY PLACE)
// =======================
void Commit(
    int frame,
    Player& world,
    const Decision& d,
    TraceLog& trace
) {
    trace.add(frame, "Commit", "Apply decision to world");

    world.hp   = d.nextHp;
    world.dead = d.die;
}

//
// =======================
// SIDE-EFFECT (READ DECISION ONLY)
// =======================
void PlaySideEffect(
    int frame,
    const Decision& d,
    TraceLog& trace
) {
    if (d.die) {
        trace.add(frame, "SideEffect", "Play death effect");
    }
}

//
// =======================
// FRAME REPLAY (DEBUG ONLY)
// =======================
void ReplayFrame(
    int frame,
    const PlayerSnapshot& snap,
    const Decision& d,
    TraceLog& trace
) {
#if DEBUG_BUILD
    trace.add(frame, "Replay", "Dry-run replay start");

    int replayHp = d.nextHp;

    trace.add(frame, "Replay",
        "If committed, HP would be " + std::to_string(replayHp));

    trace.add(frame, "Replay", "Dry-run replay end");
#endif
}

//
// =======================
// MAIN FRAME LOOP
// =======================
int main() {
    Player world;
    TraceLog trace;

    for (int frame = 1; frame <= 3; ++frame) {

        trace.clear();
        trace.add(frame, "Frame", "Begin frame");

        // SNAPSHOT
        PlayerSnapshot snap{world.hp, world.dead};

        Decision decision;

        // RESOLVE + VALIDATE
        bool ok = ResolveAndValidate(frame, snap, decision, trace);

        if (!ok) {
            trace.add(frame, "Abort", "Frame aborted, no commit");

            // Debug only: replay to understand bug
            ReplayFrame(frame, snap, decision, trace);

            trace.dump();
            continue; // Recovery → frame sau
        }

        // COMMIT
        Commit(frame, world, decision, trace);

        // SIDE-EFFECT
        PlaySideEffect(frame, decision, trace);

        trace.add(frame, "Frame", "End frame");
        trace.dump();
    }

    return 0;
}