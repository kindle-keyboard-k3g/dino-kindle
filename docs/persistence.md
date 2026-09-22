# Persistence Subsystem

## 1. Overview & Flash Storage Constraints

The Kindle Keyboard utilizes internal eMMC flash memory. On Kindle OS 3.x, the primary storage for user files and custom applications is the FAT32 partition mounted at `/mnt/us` (USB storage).

Embedded flash storage presents two critical hazards for game score persistence:
1. **Flash Wear & Sector Degradation**: Repeated small writes to the exact same file blocks can exhaust write endurance on NAND flash.
2. **Abrupt Power Loss**: Kindles are battery-operated devices subject to sudden low-battery cutoffs or hard reboots (holding power slider for 20 seconds). Non-atomic file writes can corrupt the filesystem or leave truncated 0-byte high score files.

Dino Kindle addresses these concerns via an atomic write-sync-rename persistence strategy implemented in `persistence::FileHighScoreStore`.

---

## 2. Directory Resolution (`StoragePath`)

`persistence::StoragePath` provides heuristic location discovery to ensure the high score file is saved to persistent, non-volatile storage across both native Kindle hardware and host desktop environments:

```text
Check 1: Directory exists at /mnt/us/dino?
         Yes -> Use /mnt/us/dino/highscore.dat (Kindle Native USB partition)
         No  -> Proceed to Check 2

Check 2: Environment variable $HOME is defined?
         Yes -> Use $HOME/.dino/highscore.dat (Host Linux / macOS user dir)
         No  -> Proceed to Check 3

Check 3: Fallback
         -> Use ./highscore.dat (Current working directory)
```

### Path Creation
`StoragePath` ensures the target parent directory exists before attempting any file writes.

---

## 3. Atomic Write-Sync-Rename Pattern

`FileHighScoreStore::save` guarantees that high score persistence is completely crash-safe and power-fail-safe using the POSIX atomic rename pattern:

```text
[ New High Score ]
        |
        v
1. Open Temporary File: "highscore.dat.tmp.PID"
        |
        v
2. Write ASCII/Binary Score String
        |
        v
3. Flush User-Space Buffers (fflush)
        |
        v
4. Commit Dirty Pages to Flash NAND (fsync)
        |
        v
5. Atomic Filesystem Swap (rename -> "highscore.dat")
```

### 3.1 Step Breakdown
1. **Unique Temporary File**: A temporary sibling file is opened in the exact same directory and filesystem (`highscore.dat.tmp`). Because it resides on the same filesystem mount, the subsequent `rename` call is a pure inode metadata pointer update rather than a cross-device file copy.
2. **Buffer Flush (`fflush`)**: Flushes application-level C runtime buffers into kernel page cache.
3. **Hardware Storage Sync (`fsync`)**: Forces the kernel to flush dirty blocks directly to the underlying eMMC NAND storage controller, ensuring data is written to physical media.
4. **POSIX Atomic Rename (`rename`)**: Replaces the destination `highscore.dat` atomically. If power is severed at any point prior to rename, the existing high score file remains completely intact and uncorrupted. If power is lost after rename, the new score is securely stored.

---

## 4. File Format & Corruption Recovery

- **Format**: Plain ASCII integer followed by newline (e.g., `12480\n`).
- **Load Logic**: If the score file does not exist, or contains invalid characters, or is empty, `FileHighScoreStore::load()` gracefully defaults to returning `ScoreValue(0)` without crashing.
- **Fail-Fast & Guard Clauses**: Validates file descriptors immediately upon opening. If writing or syncing fails, the temporary file is unlinked (`unlink`) immediately to prevent abandoned `.tmp` files from accumulating on flash.
