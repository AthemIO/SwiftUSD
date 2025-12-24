// VtTests.swift - Tests for Vt module
// Tests VtValue, VtArray, and VtDictionary wrappers

import XCTest
@testable import SwiftUSD

final class VtTests: XCTestCase {

    // MARK: - VtValue Tests

    func testValueEmpty() throws {
        let value = try Value()
        XCTAssertTrue(value.isEmpty)
        XCTAssertEqual(value.type, .unknown)
    }

    func testValueBool() throws {
        let value = try Value(true)
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.type, .bool)
        XCTAssertTrue(value.isHolding(.bool))
        XCTAssertTrue(value.getBool())

        let valueFalse = try Value(false)
        XCTAssertFalse(valueFalse.getBool())
    }

    func testValueInt() throws {
        let value = try Value(Int32(42))
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.type, .int)
        XCTAssertTrue(value.isHolding(.int))
        XCTAssertEqual(value.getInt(), 42)
    }

    func testValueInt64() throws {
        let largeValue: Int64 = 9_000_000_000_000
        let value = try Value(largeValue)
        XCTAssertEqual(value.type, .int64)
        XCTAssertEqual(value.getInt64(), largeValue)
    }

    func testValueFloat() throws {
        let value = try Value(Float(3.14))
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.type, .float)
        XCTAssertTrue(value.isHolding(.float))
        XCTAssertEqual(value.getFloat(), 3.14, accuracy: 0.001)
    }

    func testValueDouble() throws {
        let value = try Value(Double(2.71828))
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.type, .double)
        XCTAssertTrue(value.isHolding(.double))
        XCTAssertEqual(value.getDouble(), 2.71828, accuracy: 0.00001)
    }

    func testValueString() throws {
        let value = try Value("hello world")
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.type, .string)
        XCTAssertTrue(value.isHolding(.string))
        XCTAssertEqual(value.getString(), "hello world")
    }

    func testValueSetters() throws {
        let value = try Value()
        XCTAssertTrue(value.isEmpty)

        value.set(Int32(123))
        XCTAssertFalse(value.isEmpty)
        XCTAssertEqual(value.getInt(), 123)

        value.set(Float(4.56))
        XCTAssertEqual(value.type, .float)
        XCTAssertEqual(value.getFloat(), 4.56, accuracy: 0.01)

        value.set("test")
        XCTAssertEqual(value.getString(), "test")

        value.clear()
        XCTAssertTrue(value.isEmpty)
    }

    func testValueEquality() throws {
        let v1 = try Value(Int32(42))
        let v2 = try Value(Int32(42))
        let v3 = try Value(Int32(43))

        XCTAssertEqual(v1, v2)
        XCTAssertNotEqual(v1, v3)
    }

    func testValueCopy() throws {
        let original = try Value(Int32(100))
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
        XCTAssertEqual(copy.getInt(), 100)

        // Modify original, copy should be unaffected
        original.set(Int32(200))
        XCTAssertEqual(copy.getInt(), 100)
        XCTAssertEqual(original.getInt(), 200)
    }

    func testValueCasting() throws {
        let intValue = try Value(Int32(42))

        XCTAssertTrue(intValue.canCast(to: .float))
        try intValue.cast(to: .float)
        XCTAssertEqual(intValue.type, .float)
        XCTAssertEqual(intValue.getFloat(), 42.0, accuracy: 0.001)
    }

    func testValueHashable() throws {
        let v1 = try Value(Int32(42))
        let v2 = try Value(Int32(42))

        var set = Set<Value>()
        set.insert(v1)
        XCTAssertTrue(set.contains(v2))
    }

    // MARK: - VtArrayInt Tests

    func testArrayIntEmpty() throws {
        let array = try ArrayInt()
        XCTAssertTrue(array.isEmpty)
        XCTAssertEqual(array.count, 0)
    }

    func testArrayIntFromElements() throws {
        let array = try ArrayInt([1, 2, 3, 4, 5])
        XCTAssertFalse(array.isEmpty)
        XCTAssertEqual(array.count, 5)
        XCTAssertEqual(array[0], 1)
        XCTAssertEqual(array[4], 5)
    }

    func testArrayIntWithSize() throws {
        let array = try ArrayInt(size: 10)
        XCTAssertEqual(array.count, 10)
        // All elements should be initialized to 0
        for i in 0..<10 {
            XCTAssertEqual(array[i], 0)
        }
    }

    func testArrayIntModification() throws {
        let array = try ArrayInt()

        array.append(10)
        array.append(20)
        array.append(30)
        XCTAssertEqual(array.count, 3)
        XCTAssertEqual(array.elements, [10, 20, 30])

        array[1] = 25
        XCTAssertEqual(array[1], 25)

        array.removeLast()
        XCTAssertEqual(array.count, 2)

        array.removeAll()
        XCTAssertTrue(array.isEmpty)
    }

    func testArrayIntResize() throws {
        let array = try ArrayInt([1, 2, 3])
        array.resize(5)
        XCTAssertEqual(array.count, 5)

        array.resize(2)
        XCTAssertEqual(array.count, 2)
        XCTAssertEqual(array.elements, [1, 2])
    }

    func testArrayIntEquality() throws {
        let a1 = try ArrayInt([1, 2, 3])
        let a2 = try ArrayInt([1, 2, 3])
        let a3 = try ArrayInt([1, 2, 4])

        XCTAssertEqual(a1, a2)
        XCTAssertNotEqual(a1, a3)
    }

    func testArrayIntCopy() throws {
        let original = try ArrayInt([10, 20, 30])
        let copy = try original.copy()

        XCTAssertEqual(original, copy)

        original[0] = 100
        XCTAssertEqual(copy[0], 10)  // Copy should be unaffected
    }

    func testArrayIntCollection() throws {
        let array = try ArrayInt([5, 10, 15, 20])

        var sum: Int32 = 0
        for element in array {
            sum += element
        }
        XCTAssertEqual(sum, 50)
    }

    func testArrayIntLiteral() throws {
        let array: ArrayInt = [1, 2, 3, 4]
        XCTAssertEqual(array.count, 4)
        XCTAssertEqual(array.elements, [1, 2, 3, 4])
    }

    // MARK: - VtArrayFloat Tests

    func testArrayFloatBasics() throws {
        let array = try ArrayFloat([1.0, 2.5, 3.7])
        XCTAssertEqual(array.count, 3)
        XCTAssertEqual(array[0], 1.0, accuracy: 0.001)
        XCTAssertEqual(array[1], 2.5, accuracy: 0.001)
        XCTAssertEqual(array[2], 3.7, accuracy: 0.001)
    }

    func testArrayFloatModification() throws {
        let array = try ArrayFloat()
        array.append(1.5)
        array.append(2.5)
        XCTAssertEqual(array.count, 2)

        array[0] = 10.5
        XCTAssertEqual(array[0], 10.5, accuracy: 0.001)
    }

    // MARK: - VtArrayDouble Tests

    func testArrayDoubleBasics() throws {
        let array = try ArrayDouble([1.0, 2.0, 3.0])
        XCTAssertEqual(array.count, 3)
        XCTAssertEqual(array[0], 1.0, accuracy: 0.0001)
        XCTAssertEqual(array[1], 2.0, accuracy: 0.0001)
    }

    // MARK: - VtArrayVec3f Tests

    func testArrayVec3fEmpty() throws {
        let array = try ArrayVec3f()
        XCTAssertTrue(array.isEmpty)
        XCTAssertEqual(array.count, 0)
    }

    func testArrayVec3fFromTuples() throws {
        let array = try ArrayVec3f([
            (1.0, 2.0, 3.0),
            (4.0, 5.0, 6.0),
            (7.0, 8.0, 9.0)
        ])
        XCTAssertEqual(array.count, 3)

        let elem0 = array.getElement(at: 0)
        XCTAssertEqual(elem0.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(elem0.y, 2.0, accuracy: 0.001)
        XCTAssertEqual(elem0.z, 3.0, accuracy: 0.001)

        let elem2 = array.getElement(at: 2)
        XCTAssertEqual(elem2.x, 7.0, accuracy: 0.001)
    }

    func testArrayVec3fModification() throws {
        let array = try ArrayVec3f(size: 2)
        XCTAssertEqual(array.count, 2)

        array.setElement(x: 1.0, y: 2.0, z: 3.0, at: 0)
        array.setElement(x: 4.0, y: 5.0, z: 6.0, at: 1)

        let elem0 = array.getElement(at: 0)
        XCTAssertEqual(elem0.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(elem0.y, 2.0, accuracy: 0.001)
        XCTAssertEqual(elem0.z, 3.0, accuracy: 0.001)

        array.append(x: 10.0, y: 11.0, z: 12.0)
        XCTAssertEqual(array.count, 3)
    }

    func testArrayVec3fFlatData() throws {
        let array = try ArrayVec3f(flatData: [1, 2, 3, 4, 5, 6], count: 2)
        XCTAssertEqual(array.count, 2)

        let flatData = array.flatData
        XCTAssertEqual(flatData.count, 6)
        XCTAssertEqual(flatData, [1, 2, 3, 4, 5, 6])
    }

    // MARK: - VtArrayVec3d Tests

    func testArrayVec3dBasics() throws {
        let array = try ArrayVec3d([
            (1.0, 2.0, 3.0),
            (4.0, 5.0, 6.0)
        ])
        XCTAssertEqual(array.count, 2)

        let elem0 = array.getElement(at: 0)
        XCTAssertEqual(elem0.x, 1.0, accuracy: 0.0001)
        XCTAssertEqual(elem0.y, 2.0, accuracy: 0.0001)
        XCTAssertEqual(elem0.z, 3.0, accuracy: 0.0001)
    }

    // MARK: - VtDict Tests

    func testDictEmpty() throws {
        let dict = try VtDict()
        XCTAssertTrue(dict.isEmpty)
        XCTAssertEqual(dict.count, 0)
    }

    func testDictSetAndGet() throws {
        let dict = try VtDict()

        let intValue = try Value(Int32(42))
        dict.setValue(intValue, forKey: "count")

        let stringValue = try Value("hello")
        dict.setValue(stringValue, forKey: "message")

        XCTAssertEqual(dict.count, 2)
        XCTAssertTrue(dict.contains(key: "count"))
        XCTAssertTrue(dict.contains(key: "message"))
        XCTAssertFalse(dict.contains(key: "nonexistent"))

        if let retrieved = dict.getValue(forKey: "count") {
            XCTAssertEqual(retrieved.getInt(), 42)
        } else {
            XCTFail("Should have found 'count' key")
        }

        if let retrieved = dict.getValue(forKey: "message") {
            XCTAssertEqual(retrieved.getString(), "hello")
        } else {
            XCTFail("Should have found 'message' key")
        }
    }

    func testDictSubscript() throws {
        let dict = try VtDict()

        dict["name"] = try Value("test")
        dict["value"] = try Value(Int32(100))

        XCTAssertEqual(dict["name"]?.getString(), "test")
        XCTAssertEqual(dict["value"]?.getInt(), 100)

        // Remove via subscript
        dict["name"] = nil
        XCTAssertFalse(dict.contains(key: "name"))
    }

    func testDictKeys() throws {
        let dict = try VtDict()
        dict.setValue(try Value(Int32(1)), forKey: "a")
        dict.setValue(try Value(Int32(2)), forKey: "b")
        dict.setValue(try Value(Int32(3)), forKey: "c")

        let keys = dict.keys
        XCTAssertEqual(keys.count, 3)
        XCTAssertTrue(keys.contains("a"))
        XCTAssertTrue(keys.contains("b"))
        XCTAssertTrue(keys.contains("c"))
    }

    func testDictRemove() throws {
        let dict = try VtDict()
        dict.setValue(try Value(Int32(1)), forKey: "keep")
        dict.setValue(try Value(Int32(2)), forKey: "remove")

        XCTAssertEqual(dict.count, 2)

        XCTAssertTrue(dict.removeValue(forKey: "remove"))
        XCTAssertEqual(dict.count, 1)
        XCTAssertFalse(dict.contains(key: "remove"))
        XCTAssertTrue(dict.contains(key: "keep"))

        XCTAssertFalse(dict.removeValue(forKey: "nonexistent"))
    }

    func testDictClear() throws {
        let dict = try VtDict()
        dict.setValue(try Value(Int32(1)), forKey: "a")
        dict.setValue(try Value(Int32(2)), forKey: "b")

        dict.removeAll()
        XCTAssertTrue(dict.isEmpty)
        XCTAssertEqual(dict.count, 0)
    }

    func testDictEquality() throws {
        let dict1 = try VtDict()
        dict1.setValue(try Value(Int32(42)), forKey: "value")

        let dict2 = try VtDict()
        dict2.setValue(try Value(Int32(42)), forKey: "value")

        let dict3 = try VtDict()
        dict3.setValue(try Value(Int32(99)), forKey: "value")

        XCTAssertEqual(dict1, dict2)
        XCTAssertNotEqual(dict1, dict3)
    }

    func testDictCopy() throws {
        let original = try VtDict()
        original.setValue(try Value(Int32(100)), forKey: "number")

        let copy = try original.copy()
        XCTAssertEqual(original, copy)

        // Modify original
        original.setValue(try Value(Int32(200)), forKey: "number")

        // Copy should be unaffected
        XCTAssertEqual(copy.getValue(forKey: "number")?.getInt(), 100)
    }

    func testDictOver() throws {
        let strong = try VtDict()
        strong.setValue(try Value("strong"), forKey: "shared")
        strong.setValue(try Value(Int32(1)), forKey: "onlyStrong")

        let weak = try VtDict()
        weak.setValue(try Value("weak"), forKey: "shared")
        weak.setValue(try Value(Int32(2)), forKey: "onlyWeak")

        let result = try strong.over(weak)

        // Strong values should take precedence
        XCTAssertEqual(result.getValue(forKey: "shared")?.getString(), "strong")
        XCTAssertEqual(result.getValue(forKey: "onlyStrong")?.getInt(), 1)
        XCTAssertEqual(result.getValue(forKey: "onlyWeak")?.getInt(), 2)
    }

    // MARK: - Type Alias Tests

    func testTypeAliases() throws {
        // Verify type aliases work
        let vtValue: VtValue = try Value(Int32(42))
        XCTAssertEqual(vtValue.getInt(), 42)

        let vtArrayInt: VtArrayInt = try ArrayInt([1, 2, 3])
        XCTAssertEqual(vtArrayInt.count, 3)

        let vtDict: VtDictionary = try VtDict()
        XCTAssertTrue(vtDict.isEmpty)
    }
}
