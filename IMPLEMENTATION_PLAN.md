# C++ practice scaffold implementation plan

> Execution: inline using executing-plans. Approved spec: the user's 11-lesson plan, reproduced as the curriculum in README.md.

**Goal:** Deliver independent Vietnamese C++17 exercises, TODO stubs and meaningful tests, without implementing Topic 4.

**Architecture:** Lessons 01–10 share only a small test/TODO support library. Each has its own API, implementation, demo and tests. Lesson 11 supplies a small measurement library for a CMake/GoogleTest packaging exercise.

**Tech stack:** C++17, GCC, PowerShell/Bash; CMake and GoogleTest only in lesson 11. No automatic dependency installation.

## Constraints and decisions

- All deliverables remain inside `practice/`; do not change common or topic folders.
- No application `new`/`delete`, no supplied exercise solutions. Provided infrastructure, fakes and lesson 11's measurement library are explicitly marked as supplied code.
- Compile with `-std=c++17 -Wall -Wextra -Werror -pthread`.
- Pending TODO tests intentionally return 2; failures return 1; completed tests return 0. Build checks must never call pending tests successful solutions.
- Non-Git folder: implement in place, no worktree/commit/branch creation.
- Windows GCC is available; CMake/GoogleTest are absent. Verify lesson 11 library/demo directly, report GoogleTest/CTest as not executed.

## Tasks

- [x] 1. Define support runner acceptance tests; verify missing infrastructure fails, then implement and verify PASS/FAIL/TODO/exception behavior.
- [x] 2. Write lesson 01–10 behavioral tests and API contracts; add compiling TODO stubs and demos. Include fixed expected bytes, invalid ranges, ownership and concurrent shutdown cases.
- [x] 3. Provide lesson 11's library, demo, at least eight GoogleTests (including errors and fake), and TODO CMake packaging steps.
- [x] 4. Write Vietnamese READMEs, data fixtures, runnable PowerShell/Bash commands, knowledge-to-requirement map, hints, extension notes and reflection questions.
- [x] 5. Compile/run all available checks, document pending vs passing results and unavailable tools; request one independent review and address material findings.

## Review focus

- A mistaken implementation returning a default must produce FAIL, not TODO.
- No dangling pointers are executed; ownership counts are measured in test-only helpers.
- Signed big-endian extrema, including INT64_MIN, have literal expected byte sequences.
- Consumer exceptions must not escape a thread; hangs are bounded by the external runner.
- Close drains existing queue entries, wakes waiters and refuses future pushes; snapshot access uses its own mutex.

## Progress

- Baseline: `practice` was empty. No existing source files or tests; no Git repository. User explicitly approved implementation of the plan.
- Task 1 complete: missing runner header reproduced; support then verified exit 0/PASS, 2/TODO, 1/FAIL and 1/unexpected exception.
- Tasks 2–4 complete: test-first contracts and TODO stubs, demos, 11 GoogleTests, CMake packaging exercise and all Vietnamese lesson guides created.
- Initial verification found an ambiguous optional/vector brace initializer in a test; replaced it with has-value/empty assertions. Rebuild confirmed the fix.
- PowerShell execution policy blocks direct .ps1 invocation in this environment. Verified a process-local invocation with -ExecutionPolicy Bypass; README documents it without changing machine/user policy.
- Independent review complete: no Critical/Important findings. Addressed all four notes: update contract edge cases, printable ASCII bounds, numeric parser fixtures, and snapshot thread startup cleanup.
- Thread cleanup regression: reproduced a blocked join before the fix using a bounded subprocess; cleanup now releases the gate before joining on unwind, self-test PASS. All other behavior remains TODO for the learner.
- Final full scaffold check: all 20 lesson01–10 executables build and return TODO exit 2; 87 pending behavioral tests, zero FAIL. Provided library smoke 4/4 PASS; infrastructure runner and unwind regression PASS.
- PowerShell all/check passed and normal test/demo return 2 was verified. Bash syntax passed; runtime was not validated under Linux/WSL (MSYS2 sandbox path permission prevented its runtime check). CMake/GTest remain unavailable. See VERIFICATION.md for evidence and limits.
- Delivery: 11 Vietnamese lesson guides, central requirement map, code stubs, tests, fixtures and runners, all under practice. No Topic 4 implementation, no dependency installs, no Git operations.
