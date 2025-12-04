/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import CxxStdlib
import UsdMtlx

// MARK: - MxDocument Typealias

/// Swift-compatible handle for MaterialX::DocumentPtr.
/// This type wraps a std::shared_ptr<MaterialX::Document> and is automatically
/// reference-counted by Swift.
public typealias MxDocument = Pixar.MxDocumentHandle

/// Swift-compatible handle for MaterialX::ConstDocumentPtr.
/// This type wraps a std::shared_ptr<const MaterialX::Document>.
public typealias MxConstDocument = Pixar.MxConstDocumentHandle

/// Swift-compatible handle for MaterialX::ElementPtr.
public typealias MxElement = Pixar.MxElementHandle

/// Swift-compatible handle for MaterialX::ConstElementPtr.
public typealias MxConstElement = Pixar.MxConstElementHandle

/// Swift-compatible handle for MaterialX::NodeDefPtr.
public typealias MxNodeDef = Pixar.MxNodeDefHandle

// MARK: - UsdMtlx Document Extensions

public extension UsdMtlx
{
  /// Handle types for MaterialX shared_ptr wrappers.
  typealias Document = MxDocument
  typealias ConstDocument = MxConstDocument
  typealias Element = MxElement
  typealias ConstElement = MxConstElement
  typealias NodeDef = MxNodeDef

  // MARK: - Document Creation

  /// Read a MaterialX document from a file path.
  ///
  /// Unlike `getDocument()`, this function does not implement any
  /// caching or special behavior for MaterialX standard library documents.
  ///
  /// - Parameter resolvedPath: The resolved file path to the .mtlx file
  /// - Returns: A MaterialX document handle, or nil if reading failed
  ///
  /// ## Example
  /// ```swift
  /// if let doc = UsdMtlx.readDocument("/path/to/material.mtlx") {
  ///     print("Document name: \(doc.GetName())")
  /// }
  /// ```
  static func readDocument(_ resolvedPath: String) -> MxDocument?
  {
    Pixar.UsdMtlxSwiftReadDocument(std.string(resolvedPath))
  }

  /// Get a (possibly cached) MaterialX document by URI.
  ///
  /// Returns the cached document if available, or loads and caches it.
  /// Pass an empty string to get the standard library documents combined.
  ///
  /// - Parameter resolvedUri: The URI to the MaterialX document,
  ///   or empty string for the standard library
  /// - Returns: A const MaterialX document handle, or nil if reading failed
  ///
  /// ## Example
  /// ```swift
  /// // Get standard library documents
  /// if let stdlib = UsdMtlx.getDocument("") {
  ///     print("Standard library loaded")
  /// }
  ///
  /// // Get a specific document
  /// if let doc = UsdMtlx.getDocument("/path/to/material.mtlx") {
  ///     print("Document name: \(doc.GetName())")
  /// }
  /// ```
  static func getDocument(_ resolvedUri: String) -> MxConstDocument?
  {
    Pixar.UsdMtlxSwiftGetDocument(std.string(resolvedUri))
  }

  /// Get a MaterialX document from an XML string.
  ///
  /// Parses the provided MaterialX XML content and returns a document handle.
  ///
  /// - Parameter mtlxXml: MaterialX XML content as a string
  /// - Returns: A const MaterialX document handle, or nil if parsing failed
  ///
  /// ## Example
  /// ```swift
  /// let mtlxXml = """
  /// <?xml version="1.0"?>
  /// <materialx version="1.38">
  ///     <standard_surface name="my_material" type="surfaceshader">
  ///         <input name="base_color" type="color3" value="0.8, 0.2, 0.1"/>
  ///     </standard_surface>
  /// </materialx>
  /// """
  /// if let doc = UsdMtlx.getDocumentFromString(mtlxXml) {
  ///     print("Parsed document: \(doc.GetName())")
  /// }
  /// ```
  static func getDocumentFromString(_ mtlxXml: String) -> MxConstDocument?
  {
    Pixar.UsdMtlxSwiftGetDocumentFromString(std.string(mtlxXml))
  }

  /// Create a new empty MaterialX document.
  ///
  /// - Returns: A new empty MaterialX document handle
  static func createDocument() -> MxDocument?
  {
    Pixar.UsdMtlxSwiftCreateDocument()
  }

  // MARK: - Document to USD Conversion

  /// Translate a MaterialX document into a USD stage.
  ///
  /// Converts MaterialX materials, looks, and node graphs into USD scene description.
  ///
  /// - Parameters:
  ///   - mtlx: The MaterialX document to translate
  ///   - stage: The USD stage to populate
  ///   - internalPath: Namespace path for converted MaterialX objects (default: "/MaterialX")
  ///   - externalPath: Namespace path for look variants (default: "/ModelRoot")
  ///
  /// ## Example
  /// ```swift
  /// if let mtlx = UsdMtlx.getDocumentFromString(mtlxXml) {
  ///     let stage = Usd.Stage.createNew("output.usda")
  ///     UsdMtlx.read(mtlx, into: stage)
  ///     stage.save()
  /// }
  /// ```
  static func read(
    _ mtlx: MxConstDocument,
    into stage: Usd.StageRefPtr,
    internalPath: Sdf.Path = Sdf.Path("/MaterialX"),
    externalPath: Sdf.Path = Sdf.Path("/ModelRoot")
  )
  {
    Pixar.UsdMtlxSwiftRead(mtlx, stage, internalPath, externalPath)
  }

  /// Translate MaterialX node graphs into a USD stage.
  ///
  /// Converts only the node graphs from a MaterialX document, skipping
  /// materials and looks.
  ///
  /// - Parameters:
  ///   - mtlx: The MaterialX document containing node graphs
  ///   - stage: The USD stage to populate
  ///   - internalPath: Namespace path for converted objects (default: "/MaterialX")
  static func readNodeGraphs(
    _ mtlx: MxConstDocument,
    into stage: Usd.StageRefPtr,
    internalPath: Sdf.Path = Sdf.Path("/MaterialX")
  )
  {
    Pixar.UsdMtlxSwiftReadNodeGraphs(mtlx, stage, internalPath)
  }

  // MARK: - Element Utilities

  /// Get the source URI for a MaterialX element.
  ///
  /// Returns the URI of the element, or walks up the hierarchy to find
  /// the nearest element with a source URI.
  ///
  /// - Parameter element: The MaterialX element
  /// - Returns: The source URI, or empty string if none found
  static func getSourceURI(_ element: MxConstElement) -> String
  {
    String(Pixar.UsdMtlxSwiftGetSourceURI(element))
  }

  /// Get the value of a MaterialX element as a VtValue.
  ///
  /// - Parameters:
  ///   - element: The MaterialX element
  ///   - getDefaultValue: If true, gets the default value instead of the current value
  /// - Returns: The value as a VtValue, or empty VtValue if conversion failed
  static func getUsdValue(
    _ element: MxConstElement,
    getDefaultValue: Bool = false
  ) -> Vt.Value
  {
    Pixar.UsdMtlxSwiftGetUsdValue(element, getDefaultValue)
  }
}

// MARK: - MxDocument Extensions

public extension MxDocument
{
  /// Check if this document handle is valid (non-null).
  var isValid: Bool
  {
    IsValid()
  }

  /// The name of the document.
  var name: String
  {
    String(GetName())
  }

  /// Export the document to MaterialX XML string.
  var xmlString: String
  {
    String(ExportToXmlString())
  }
}

// MARK: - MxConstDocument Extensions

public extension MxConstDocument
{
  /// Check if this document handle is valid (non-null).
  var isValid: Bool
  {
    IsValid()
  }

  /// The name of the document.
  var name: String
  {
    String(GetName())
  }
}

// MARK: - MxElement Extensions

public extension MxElement
{
  /// Check if this element handle is valid (non-null).
  var isValid: Bool
  {
    IsValid()
  }

  /// The name of the element.
  var name: String
  {
    String(GetName())
  }

  /// The category of the element.
  var category: String
  {
    String(GetCategory())
  }
}

// MARK: - MxConstElement Extensions

public extension MxConstElement
{
  /// Check if this element handle is valid (non-null).
  var isValid: Bool
  {
    IsValid()
  }

  /// The name of the element.
  var name: String
  {
    String(GetName())
  }

  /// The category of the element.
  var category: String
  {
    String(GetCategory())
  }
}

// MARK: - MxNodeDef Extensions

public extension MxNodeDef
{
  /// Check if this node definition handle is valid (non-null).
  var isValid: Bool
  {
    IsValid()
  }

  /// The name of the node definition.
  var name: String
  {
    String(GetName())
  }

  /// The node string (node type) of the node definition.
  var nodeString: String
  {
    String(GetNodeString())
  }
}
