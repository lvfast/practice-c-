"""Maintainer check for untouched scaffolds. This does NOT solve or grade lessons.

Uses only Python's standard library. Binaries/logs stay in practice/build.
After implementing a lesson, use run.ps1/run.sh test instead of this check.
"""
from pathlib import Path
import os
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"


def run(command, cwd=ROOT, expected=0):
    result = subprocess.run(command, cwd=cwd, text=True, capture_output=True, timeout=15)
    if result.returncode != expected:
        raise RuntimeError(f"exit {result.returncode}, expected {expected}: {command}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def main():
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if not compiler:
        raise RuntimeError("Missing g++; add compiler to PATH or set CXX")
    BUILD.mkdir(exist_ok=True)
    suffix = ".exe" if os.name == "nt" else ""

    def compile_cpp(output, *sources):
        binary = BUILD / (output + suffix)
        run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pthread",
             *(str(path) for path in sources), "-o", str(binary)])
        return str(binary)

    runner = compile_cpp("runner_self_test", ROOT / "support/self_test.cpp")
    for mode, status, marker in [("pass", 0, "[PASS]"), ("todo", 2, "[TODO]"),
                                 ("wrong", 1, "[FAIL]"), ("exception", 1, "[FAIL]")]:
        assert marker in run([runner, mode], expected=status)
    print("OK runner distinguishes PASS / TODO / FAIL / unexpected exception", flush=True)
    thread_check = compile_cpp("thread_unwind_self_test", ROOT / "support/thread_self_test.cpp")
    print(run([thread_check]).strip(), flush=True)
    count = 0
    for folder in sorted(ROOT.glob("[0-9][0-9]_*")):
        if folder.name.startswith("11_"):
            continue
        for entry, label in [("test.cpp", "tests"), ("main.cpp", "demo")]:
            binary = compile_cpp(folder.name + "_" + label, folder / "lesson.cpp", folder / entry)
            output = run([binary], cwd=folder, expected=2)
            if "[TODO]" not in output or "[FAIL]" in output:
                raise RuntimeError(f"Unexpected scaffold state in {folder.name} {label}:\n{output}")
            (BUILD / f"{folder.name}_{label}.log").write_text(output, encoding="utf-8")
            if label == "tests":
                print(folder.name, output.strip().splitlines()[-1], flush=True)
        count += 1
    assert count == 10, f"Expected ten independent lessons, got {count}"
    lesson11 = ROOT / "11_cmake_gtest"
    binary = compile_cpp("11_library_smoke", lesson11 / "src/measurements.cpp", lesson11 / "tests/library_smoke.cpp")
    print(run([binary], cwd=lesson11).strip(), flush=True)
    demo = compile_cpp("11_demo", lesson11 / "src/measurements.cpp", lesson11 / "src/main.cpp")
    assert "count=2 mean=20" in run([demo], cwd=lesson11)
    print("OK lesson 11 supplied library and demo (direct g++ build)", flush=True)
    print("NOT RUN: CMake/GoogleTest/CTest. Follow lesson 11 README; check this environment separately.")
    print("SCAFFOLD CHECK PASSED: compiling TODO exercises, not completed solutions.")


if __name__ == "__main__":
    try:
        main()
    except (RuntimeError, AssertionError, subprocess.TimeoutExpired) as error:
        print(f"CHECK FAILED: {error}", file=sys.stderr)
        sys.exit(1)
