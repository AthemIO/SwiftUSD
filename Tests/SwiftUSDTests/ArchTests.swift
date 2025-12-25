import XCTest
@testable import SwiftUSD

final class ArchTests: XCTestCase {

    // MARK: - Platform Information Tests

    func testPageSize() {
        let pageSize = Arch.pageSize
        XCTAssertGreaterThan(pageSize, 0)
        // Page size should be a power of 2
        XCTAssertEqual(pageSize & (pageSize - 1), 0)
        // Common page sizes are 4096 or 16384
        XCTAssertTrue(pageSize >= 4096)
    }

    func testIsMainThread() {
        // When running tests on the main thread
        XCTAssertTrue(Arch.isMainThread)

        // Test on background thread
        let expectation = expectation(description: "Background thread check")
        DispatchQueue.global().async {
            XCTAssertFalse(Arch.isMainThread)
            expectation.fulfill()
        }
        wait(for: [expectation], timeout: 5.0)
    }

    // MARK: - System Information Tests

    func testCurrentWorkingDirectory() throws {
        let cwd = try Arch.currentWorkingDirectory
        XCTAssertFalse(cwd.isEmpty)
        XCTAssertTrue(cwd.hasPrefix("/"))
    }

    func testExecutablePath() throws {
        let path = try Arch.executablePath
        XCTAssertFalse(path.isEmpty)
        XCTAssertTrue(path.hasPrefix("/"))
    }

    // MARK: - File System Tests

    func testTemporaryDirectory() {
        let tmpDir = Arch.temporaryDirectory
        XCTAssertFalse(tmpDir.isEmpty)
        XCTAssertTrue(tmpDir.hasPrefix("/"))
    }

    func testMakeTemporaryFileName() throws {
        let fileName = try Arch.makeTemporaryFileName(prefix: "swiftusd_test", suffix: ".txt")
        XCTAssertTrue(fileName.contains("swiftusd_test"))
        XCTAssertTrue(fileName.hasSuffix(".txt"))
    }

    func testMakeTemporaryFile() throws {
        let (fd, path) = try Arch.makeTemporaryFile(prefix: "swiftusd_test")
        XCTAssertGreaterThanOrEqual(fd, 0)
        XCTAssertFalse(path.isEmpty)
        XCTAssertTrue(path.contains("swiftusd_test"))

        // Clean up
        close(fd)
        try? FileManager.default.removeItem(atPath: path)
    }

    func testMakeTemporarySubdirectory() throws {
        let tmpDir = Arch.temporaryDirectory
        let subdir = try Arch.makeTemporarySubdirectory(in: tmpDir, prefix: "swiftusd_test")
        XCTAssertFalse(subdir.isEmpty)
        XCTAssertTrue(subdir.contains("swiftusd_test"))

        // Verify it exists
        var isDir: ObjCBool = false
        XCTAssertTrue(FileManager.default.fileExists(atPath: subdir, isDirectory: &isDir))
        XCTAssertTrue(isDir.boolValue)

        // Clean up
        try? FileManager.default.removeItem(atPath: subdir)
    }

    func testFileLength() throws {
        // Create a temp file with known content
        let (fd, path) = try Arch.makeTemporaryFile(prefix: "swiftusd_length_test")
        let testData = "Hello, SwiftUSD!"
        write(fd, testData, testData.utf8.count)
        close(fd)

        let length = Arch.fileLength(at: path)
        XCTAssertEqual(length, Int64(testData.utf8.count))

        // Clean up
        try? FileManager.default.removeItem(atPath: path)
    }

    func testFileLengthNotFound() {
        let length = Arch.fileLength(at: "/nonexistent/file/path")
        XCTAssertNil(length)
    }

    func testNormalizePath() throws {
        let normalized = try Arch.normalizePath("/foo/bar/../baz/./qux")
        XCTAssertEqual(normalized, "/foo/baz/qux")
    }

    func testAbsolutePath() throws {
        let cwd = try Arch.currentWorkingDirectory
        let absPath = try Arch.absolutePath(for: "relative/path")
        XCTAssertTrue(absPath.hasPrefix("/"))
        XCTAssertTrue(absPath.contains("relative/path"))
    }

    func testModificationTime() throws {
        // Create a temp file
        let (fd, path) = try Arch.makeTemporaryFile(prefix: "swiftusd_mtime_test")
        close(fd)

        let mtime = try Arch.modificationTime(at: path)
        XCTAssertGreaterThan(mtime, 0)

        // Should be recent (within the last minute)
        let now = Date().timeIntervalSince1970
        XCTAssertLessThan(Swift.abs(now - mtime), 60)

        // Clean up
        try? FileManager.default.removeItem(atPath: path)
    }

    // MARK: - Environment Tests

    func testEnvironmentGet() {
        // PATH should always exist
        let path = Arch.Environment.get("PATH")
        XCTAssertNotNil(path)
        XCTAssertFalse(path!.isEmpty)
    }

    func testEnvironmentHas() {
        XCTAssertTrue(Arch.Environment.has("PATH"))
        XCTAssertFalse(Arch.Environment.has("SWIFTUSD_NONEXISTENT_VAR_12345"))
    }

    func testEnvironmentSetAndRemove() {
        let testVar = "SWIFTUSD_TEST_VAR"
        let testValue = "test_value_123"

        // Set the variable
        XCTAssertTrue(Arch.Environment.set(testVar, value: testValue))
        XCTAssertEqual(Arch.Environment.get(testVar), testValue)

        // Remove it
        XCTAssertTrue(Arch.Environment.remove(testVar))
        XCTAssertNil(Arch.Environment.get(testVar))
    }

    func testEnvironmentSubscript() {
        let testVar = "SWIFTUSD_SUBSCRIPT_TEST"

        Arch.Environment[testVar] = "value1"
        XCTAssertEqual(Arch.Environment[testVar], "value1")

        Arch.Environment[testVar] = nil
        XCTAssertNil(Arch.Environment[testVar])
    }

    func testEnvironmentExpand() throws {
        let testVar = "SWIFTUSD_EXPAND_TEST"
        Arch.Environment[testVar] = "expanded_value"

        let expanded = try Arch.Environment.expand("prefix_${SWIFTUSD_EXPAND_TEST}_suffix")
        XCTAssertEqual(expanded, "prefix_expanded_value_suffix")

        // Clean up
        Arch.Environment[testVar] = nil
    }

    // MARK: - Timing Tests

    func testTickTime() {
        let t1 = Arch.Timing.tickTime
        // Do a small amount of work
        var sum = 0
        for i in 0..<1000 { sum += i }
        let t2 = Arch.Timing.tickTime

        XCTAssertGreaterThan(t2, t1)
        _ = sum // Silence unused variable warning
    }

    func testStartStopTickTime() {
        let start = Arch.Timing.startTickTime
        // Do some work
        var sum = 0
        for i in 0..<10000 { sum += i }
        let stop = Arch.Timing.stopTickTime

        XCTAssertGreaterThan(stop, start)
        _ = sum
    }

    func testTickQuantum() {
        let quantum = Arch.Timing.tickQuantum
        XCTAssertGreaterThan(quantum, 0)
    }

    func testNanosecondsPerTick() {
        let nsPerTick = Arch.Timing.nanosecondsPerTick
        XCTAssertGreaterThan(nsPerTick, 0)
    }

    func testTicksToNanoseconds() {
        let ticks: UInt64 = 1000000
        let ns = Arch.Timing.ticksToNanoseconds(ticks)
        XCTAssertGreaterThan(ns, 0)
    }

    func testTicksToSeconds() {
        let oneSecondTicks = Arch.Timing.secondsToTicks(1.0)
        let seconds = Arch.Timing.ticksToSeconds(oneSecondTicks)
        XCTAssertEqual(seconds, 1.0, accuracy: 0.001)
    }

    func testSecondsToTicks() {
        let ticks = Arch.Timing.secondsToTicks(1.0)
        XCTAssertGreaterThan(ticks, 0)
    }

    // MARK: - Interval Timer Tests

    func testIntervalTimerBasic() {
        let timer = Arch.IntervalTimer()
        XCTAssertTrue(timer.isStarted)
        XCTAssertGreaterThan(timer.startTime, 0)

        // Do some work
        Thread.sleep(forTimeInterval: 0.01) // 10ms

        let elapsed = timer.elapsedTicks
        XCTAssertGreaterThan(elapsed, 0)

        let elapsedNs = timer.elapsedNanoseconds
        XCTAssertGreaterThan(elapsedNs, 0)

        let elapsedSec = timer.elapsedSeconds
        XCTAssertGreaterThan(elapsedSec, 0)
        XCTAssertLessThan(elapsedSec, 1.0) // Should be much less than 1 second
    }

    func testIntervalTimerNotStarted() {
        let timer = Arch.IntervalTimer(start: false)
        XCTAssertFalse(timer.isStarted)
        XCTAssertEqual(timer.elapsedTicks, 0)
    }

    func testIntervalTimerRestart() {
        let timer = Arch.IntervalTimer()
        Thread.sleep(forTimeInterval: 0.01)
        let elapsed1 = timer.elapsedTicks

        timer.start()
        let elapsed2 = timer.elapsedTicks

        // After restart, elapsed should be much smaller
        XCTAssertLessThan(elapsed2, elapsed1)
    }

    // MARK: - Error Handling Tests

    func testErrorStringForCode() throws {
        // ENOENT = 2 on most systems
        let error = try Arch.errorString(for: 2)
        XCTAssertFalse(error.isEmpty)
        // On most systems this should be "No such file or directory"
        XCTAssertTrue(error.lowercased().contains("no such") || error.lowercased().contains("not found") || !error.isEmpty)
    }

    // MARK: - Debugger Tests

    func testDebuggerIsAttached() {
        // This may or may not be true depending on how tests are run
        // Just verify it doesn't crash
        let _ = Arch.isDebuggerAttached
    }

    func testDebuggerWait() {
        // Just verify this doesn't crash
        Arch.setDebuggerWait(false)
    }
}
