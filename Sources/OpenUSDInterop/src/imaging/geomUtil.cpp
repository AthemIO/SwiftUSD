// geomUtil.cpp - GeomUtil C wrapper implementation
// Mirrors: pxr/imaging/geomUtil/*.h (Mesh generation utilities)

#include "../../include/imaging/geomUtil.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define GEOMUTIL_USE_USD 1
    #include <pxr/imaging/geomUtil/sphereMeshGenerator.h>
    #include <pxr/imaging/geomUtil/cuboidMeshGenerator.h>
    #include <pxr/imaging/geomUtil/cylinderMeshGenerator.h>
    #include <pxr/imaging/geomUtil/coneMeshGenerator.h>
    #include <pxr/imaging/geomUtil/capsuleMeshGenerator.h>
    #include <pxr/imaging/geomUtil/planeMeshGenerator.h>
    #include <pxr/imaging/geomUtil/diskMeshGenerator.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/matrix4d.h>
    #include <pxr/base/vt/array.h>
    #include <pxr/imaging/pxOsd/meshTopology.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define GEOMUTIL_USE_USD 0
#endif

#include <atomic>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================================
// MARK: - GeomUtilMeshData Wrapper
// ============================================================================

struct GeomUtilMeshDataOpaque {
    std::atomic<int> refCount;
    std::vector<float> points;      // 3 floats per point
    std::vector<float> normals;     // 3 floats per normal
    std::vector<int> faceVertexCounts;
    std::vector<int> faceVertexIndices;
    GeomUtilNormalsInterpolation normalsInterpolation;

    GeomUtilMeshDataOpaque() : refCount(1), normalsInterpolation(GEOMUTIL_NORMALS_VERTEX) {}
};

// ============================================================================
// MARK: - Helper Functions
// ============================================================================

#if GEOMUTIL_USE_USD
static GfMatrix4d ToGfMatrix4d(const float* m) {
    if (!m) {
        return GfMatrix4d(1.0); // Identity
    }
    return GfMatrix4d(
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    );
}

static void ExtractTopology(
    const PxOsdMeshTopology& topology,
    std::vector<int>& faceVertexCounts,
    std::vector<int>& faceVertexIndices)
{
    const VtIntArray& counts = topology.GetFaceVertexCounts();
    const VtIntArray& indices = topology.GetFaceVertexIndices();

    faceVertexCounts.resize(counts.size());
    for (size_t i = 0; i < counts.size(); ++i) {
        faceVertexCounts[i] = counts[i];
    }

    faceVertexIndices.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        faceVertexIndices[i] = indices[i];
    }
}

static void ExtractPoints(const VtVec3fArray& vtPoints, std::vector<float>& points) {
    points.resize(vtPoints.size() * 3);
    for (size_t i = 0; i < vtPoints.size(); ++i) {
        points[i * 3 + 0] = vtPoints[i][0];
        points[i * 3 + 1] = vtPoints[i][1];
        points[i * 3 + 2] = vtPoints[i][2];
    }
}
#endif

// Standalone helper: generate a simple quad face indices
static void GenerateQuadFaceIndices(
    std::vector<int>& faceVertexCounts,
    std::vector<int>& faceVertexIndices,
    int v0, int v1, int v2, int v3)
{
    faceVertexCounts.push_back(4);
    faceVertexIndices.push_back(v0);
    faceVertexIndices.push_back(v1);
    faceVertexIndices.push_back(v2);
    faceVertexIndices.push_back(v3);
}

// Standalone helper: generate a simple triangle face indices
static void GenerateTriFaceIndices(
    std::vector<int>& faceVertexCounts,
    std::vector<int>& faceVertexIndices,
    int v0, int v1, int v2)
{
    faceVertexCounts.push_back(3);
    faceVertexIndices.push_back(v0);
    faceVertexIndices.push_back(v1);
    faceVertexIndices.push_back(v2);
}

// Standalone helper: apply transform to a point
static void TransformPoint(float& x, float& y, float& z, const float* transform) {
    if (!transform) return;
    float nx = transform[0]*x + transform[4]*y + transform[8]*z + transform[12];
    float ny = transform[1]*x + transform[5]*y + transform[9]*z + transform[13];
    float nz = transform[2]*x + transform[6]*y + transform[10]*z + transform[14];
    x = nx;
    y = ny;
    z = nz;
}

// Standalone helper: apply transform to a normal (no translation, inverse transpose for correct normal transform)
static void TransformNormal(float& x, float& y, float& z, const float* transform) {
    if (!transform) return;
    // For proper normals, we should use the inverse transpose, but for uniform scaling
    // and rigid transforms, just applying the rotation part works
    float nx = transform[0]*x + transform[4]*y + transform[8]*z;
    float ny = transform[1]*x + transform[5]*y + transform[9]*z;
    float nz = transform[2]*x + transform[6]*y + transform[10]*z;
    // Normalize
    float len = std::sqrt(nx*nx + ny*ny + nz*nz);
    if (len > 0.0001f) {
        x = nx / len;
        y = ny / len;
        z = nz / len;
    } else {
        x = nx;
        y = ny;
        z = nz;
    }
}

extern "C" {

// ============================================================================
// MARK: - GeomUtilMeshData Lifecycle
// ============================================================================

GeomUtilMeshDataRef GeomUtilMeshData_Retain(GeomUtilMeshDataRef mesh) {
    if (mesh) {
        mesh->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return mesh;
}

void GeomUtilMeshData_Release(GeomUtilMeshDataRef mesh) {
    if (mesh) {
        if (mesh->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete mesh;
        }
    }
}

// ============================================================================
// MARK: - GeomUtilMeshData Accessors
// ============================================================================

size_t GeomUtilMeshData_GetPointCount(GeomUtilMeshDataRef mesh) {
    if (!mesh) return 0;
    return mesh->points.size() / 3;
}

size_t GeomUtilMeshData_GetPoints(GeomUtilMeshDataRef mesh, float* outPoints, size_t maxCount) {
    if (!mesh || !outPoints || maxCount == 0) return 0;
    size_t pointCount = mesh->points.size() / 3;
    size_t toCopy = (maxCount < pointCount) ? maxCount : pointCount;
    for (size_t i = 0; i < toCopy * 3; ++i) {
        outPoints[i] = mesh->points[i];
    }
    return toCopy;
}

size_t GeomUtilMeshData_GetNormalCount(GeomUtilMeshDataRef mesh) {
    if (!mesh) return 0;
    return mesh->normals.size() / 3;
}

size_t GeomUtilMeshData_GetNormals(GeomUtilMeshDataRef mesh, float* outNormals, size_t maxCount) {
    if (!mesh || !outNormals || maxCount == 0) return 0;
    size_t normalCount = mesh->normals.size() / 3;
    size_t toCopy = (maxCount < normalCount) ? maxCount : normalCount;
    for (size_t i = 0; i < toCopy * 3; ++i) {
        outNormals[i] = mesh->normals[i];
    }
    return toCopy;
}

GeomUtilNormalsInterpolation GeomUtilMeshData_GetNormalsInterpolation(GeomUtilMeshDataRef mesh) {
    if (!mesh) return GEOMUTIL_NORMALS_VERTEX;
    return mesh->normalsInterpolation;
}

size_t GeomUtilMeshData_GetFaceCount(GeomUtilMeshDataRef mesh) {
    if (!mesh) return 0;
    return mesh->faceVertexCounts.size();
}

size_t GeomUtilMeshData_GetFaceVertexCounts(GeomUtilMeshDataRef mesh, int* outCounts, size_t maxCount) {
    if (!mesh || !outCounts || maxCount == 0) return 0;
    size_t faceCount = mesh->faceVertexCounts.size();
    size_t toCopy = (maxCount < faceCount) ? maxCount : faceCount;
    for (size_t i = 0; i < toCopy; ++i) {
        outCounts[i] = mesh->faceVertexCounts[i];
    }
    return toCopy;
}

size_t GeomUtilMeshData_GetFaceVertexIndexCount(GeomUtilMeshDataRef mesh) {
    if (!mesh) return 0;
    return mesh->faceVertexIndices.size();
}

size_t GeomUtilMeshData_GetFaceVertexIndices(GeomUtilMeshDataRef mesh, int* outIndices, size_t maxCount) {
    if (!mesh || !outIndices || maxCount == 0) return 0;
    size_t indexCount = mesh->faceVertexIndices.size();
    size_t toCopy = (maxCount < indexCount) ? maxCount : indexCount;
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = mesh->faceVertexIndices[i];
    }
    return toCopy;
}

// ============================================================================
// MARK: - Sphere Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeSpherePointCount(size_t numRadial, size_t numAxial, bool closedSweep) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilSphereMeshGenerator::ComputeNumPoints(numRadial, numAxial, closedSweep);
    } catch (...) {
        return 0;
    }
#else
    // Standalone: poles + rings
    // For a closed sweep: 2 poles + (numAxial - 1) rings * numRadial vertices
    if (numRadial < 3) numRadial = 3;
    if (numAxial < 2) numAxial = 2;
    if (closedSweep) {
        return 2 + (numAxial - 1) * numRadial;
    } else {
        return 2 + (numAxial - 1) * (numRadial + 1);
    }
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateSphereMesh(
    size_t numRadial,
    size_t numAxial,
    float radius,
    float sweepDegrees,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_VERTEX;

        if (numRadial < 3) numRadial = 3;
        if (numAxial < 2) numAxial = 2;
        bool closedSweep = (sweepDegrees >= 360.0f);

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilSphereMeshGenerator::GenerateTopology(
            numRadial, numAxial, closedSweep);
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilSphereMeshGenerator::ComputeNumPoints(numRadial, numAxial, closedSweep);
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        if (closedSweep) {
            GeomUtilSphereMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, numAxial, radius, framePtr);
        } else {
            GeomUtilSphereMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, numAxial, radius, sweepDegrees, framePtr);
        }
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals
        size_t numNormals = GeomUtilSphereMeshGenerator::ComputeNumPoints(numRadial, numAxial, closedSweep);
        VtVec3fArray vtNormals(numNormals);
        if (closedSweep) {
            GeomUtilSphereMeshGenerator::GenerateNormals(
                vtNormals.begin(), numRadial, numAxial, framePtr);
        } else {
            GeomUtilSphereMeshGenerator::GenerateNormals(
                vtNormals.begin(), numRadial, numAxial, sweepDegrees, framePtr);
        }
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone sphere generation
        float sweepRad = sweepDegrees * static_cast<float>(M_PI) / 180.0f;
        if (sweepRad > 2.0f * static_cast<float>(M_PI)) {
            sweepRad = 2.0f * static_cast<float>(M_PI);
        }

        // Generate points
        // Bottom pole
        {
            float px = 0, py = 0, pz = -radius;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Rings
        size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);
        for (size_t axial = 1; axial < numAxial; ++axial) {
            float phi = static_cast<float>(M_PI) * axial / numAxial;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (size_t radial = 0; radial < vertsPerRing; ++radial) {
                float theta = sweepRad * radial / numRadial;
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                float px = radius * sinPhi * cosTheta;
                float py = radius * sinPhi * sinTheta;
                float pz = -radius * cosPhi;
                TransformPoint(px, py, pz, transform);
                mesh->points.push_back(px);
                mesh->points.push_back(py);
                mesh->points.push_back(pz);
            }
        }

        // Top pole
        {
            float px = 0, py = 0, pz = radius;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Generate normals (same as points but normalized, radius = 1)
        // Bottom pole normal
        {
            float nx = 0, ny = 0, nz = -1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Ring normals
        for (size_t axial = 1; axial < numAxial; ++axial) {
            float phi = static_cast<float>(M_PI) * axial / numAxial;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (size_t radial = 0; radial < vertsPerRing; ++radial) {
                float theta = sweepRad * radial / numRadial;
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                float nx = sinPhi * cosTheta;
                float ny = sinPhi * sinTheta;
                float nz = -cosPhi;
                TransformNormal(nx, ny, nz, transform);
                mesh->normals.push_back(nx);
                mesh->normals.push_back(ny);
                mesh->normals.push_back(nz);
            }
        }

        // Top pole normal
        {
            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Generate topology
        int bottomPole = 0;
        int topPole = static_cast<int>(mesh->points.size() / 3 - 1);

        // Bottom cap triangles
        for (size_t r = 0; r < numRadial; ++r) {
            int v0 = bottomPole;
            int v1 = 1 + static_cast<int>(r);
            int v2 = 1 + static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue; // Skip last triangle for open sweep
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, v0, v1, v2);
        }

        // Middle quads
        for (size_t axial = 1; axial < numAxial - 1; ++axial) {
            int ringStart = 1 + static_cast<int>((axial - 1) * vertsPerRing);
            int nextRingStart = 1 + static_cast<int>(axial * vertsPerRing);

            for (size_t r = 0; r < numRadial; ++r) {
                int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
                if (!closedSweep && r == numRadial - 1) continue;

                int v0 = ringStart + static_cast<int>(r);
                int v1 = ringStart + r_next;
                int v2 = nextRingStart + r_next;
                int v3 = nextRingStart + static_cast<int>(r);
                GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, v0, v1, v2, v3);
            }
        }

        // Top cap triangles
        int lastRingStart = 1 + static_cast<int>((numAxial - 2) * vertsPerRing);
        for (size_t r = 0; r < numRadial; ++r) {
            int v0 = lastRingStart + static_cast<int>(r);
            int v1 = topPole;
            int v2 = lastRingStart + static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, v0, v1, v2);
        }
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Cuboid Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeCuboidPointCount(void) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilCuboidMeshGenerator::ComputeNumPoints();
    } catch (...) {
        return 0;
    }
#else
    // Cuboid has 8 vertices
    return 8;
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateCuboidMesh(
    float xLength,
    float yLength,
    float zLength,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_UNIFORM;

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilCuboidMeshGenerator::GenerateTopology();
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilCuboidMeshGenerator::ComputeNumPoints();
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        GeomUtilCuboidMeshGenerator::GeneratePoints(
            vtPoints.begin(), xLength, yLength, zLength, framePtr);
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals (6 face normals for uniform interpolation)
        size_t numNormals = GeomUtilCuboidMeshGenerator::ComputeNumNormals();
        VtVec3fArray vtNormals(numNormals);
        GeomUtilCuboidMeshGenerator::GenerateNormals(vtNormals.begin(), framePtr);
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone cuboid generation
        float hx = xLength * 0.5f;
        float hy = yLength * 0.5f;
        float hz = zLength * 0.5f;

        // 8 vertices of the cuboid
        float verts[8][3] = {
            {-hx, -hy, -hz}, // 0: bottom-front-left
            { hx, -hy, -hz}, // 1: bottom-front-right
            { hx,  hy, -hz}, // 2: bottom-back-right
            {-hx,  hy, -hz}, // 3: bottom-back-left
            {-hx, -hy,  hz}, // 4: top-front-left
            { hx, -hy,  hz}, // 5: top-front-right
            { hx,  hy,  hz}, // 6: top-back-right
            {-hx,  hy,  hz}  // 7: top-back-left
        };

        for (int i = 0; i < 8; ++i) {
            float px = verts[i][0], py = verts[i][1], pz = verts[i][2];
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // 6 face normals (uniform interpolation)
        float normals[6][3] = {
            { 0,  0, -1}, // bottom
            { 0,  0,  1}, // top
            { 0, -1,  0}, // front
            { 1,  0,  0}, // right
            { 0,  1,  0}, // back
            {-1,  0,  0}  // left
        };

        for (int i = 0; i < 6; ++i) {
            float nx = normals[i][0], ny = normals[i][1], nz = normals[i][2];
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // 6 quad faces
        // Bottom face (0,3,2,1)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 0, 3, 2, 1);
        // Top face (4,5,6,7)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 4, 5, 6, 7);
        // Front face (0,1,5,4)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 0, 1, 5, 4);
        // Right face (1,2,6,5)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 1, 2, 6, 5);
        // Back face (2,3,7,6)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 2, 3, 7, 6);
        // Left face (3,0,4,7)
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 3, 0, 4, 7);
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Cylinder Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeCylinderPointCount(size_t numRadial, bool closedSweep) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilCylinderMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
    } catch (...) {
        return 0;
    }
#else
    if (numRadial < 3) numRadial = 3;
    // Two rings + 2 center points for caps
    size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);
    return 2 * vertsPerRing + 2;
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateCylinderMesh(
    size_t numRadial,
    float bottomRadius,
    float topRadius,
    float height,
    float sweepDegrees,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_VERTEX;

        if (numRadial < 3) numRadial = 3;
        bool closedSweep = (sweepDegrees >= 360.0f);

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilCylinderMeshGenerator::GenerateTopology(
            numRadial, closedSweep);
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilCylinderMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        GeomUtilCylinderMeshGenerator::GeneratePoints(
            vtPoints.begin(), numRadial, bottomRadius, topRadius, height, sweepDegrees, framePtr);
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals
        size_t numNormals = GeomUtilCylinderMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
        VtVec3fArray vtNormals(numNormals);
        GeomUtilCylinderMeshGenerator::GenerateNormals(
            vtNormals.begin(), numRadial, bottomRadius, topRadius, height, sweepDegrees, framePtr);
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone cylinder generation
        float sweepRad = sweepDegrees * static_cast<float>(M_PI) / 180.0f;
        if (sweepRad > 2.0f * static_cast<float>(M_PI)) {
            sweepRad = 2.0f * static_cast<float>(M_PI);
        }

        float halfHeight = height * 0.5f;
        size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);

        // Bottom center
        {
            float px = 0, py = 0, pz = -halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Bottom ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = bottomRadius * cosTheta;
            float py = bottomRadius * sinTheta;
            float pz = -halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Top ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = topRadius * cosTheta;
            float py = topRadius * sinTheta;
            float pz = halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Top center
        {
            float px = 0, py = 0, pz = halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Normals
        // Bottom center normal
        {
            float nx = 0, ny = 0, nz = -1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Side normals for bottom ring
        float slopeAngle = std::atan2(bottomRadius - topRadius, height);
        float cosSlope = std::cos(slopeAngle);
        float sinSlope = std::sin(slopeAngle);

        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float nx = cosSlope * cosTheta;
            float ny = cosSlope * sinTheta;
            float nz = sinSlope;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Side normals for top ring (same as bottom)
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float nx = cosSlope * cosTheta;
            float ny = cosSlope * sinTheta;
            float nz = sinSlope;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Top center normal
        {
            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Topology
        int bottomCenter = 0;
        int bottomRingStart = 1;
        int topRingStart = 1 + static_cast<int>(vertsPerRing);
        int topCenter = static_cast<int>(mesh->points.size() / 3 - 1);

        // Bottom cap
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                bottomCenter, bottomRingStart + r_next, bottomRingStart + static_cast<int>(r));
        }

        // Side quads
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;

            int v0 = bottomRingStart + static_cast<int>(r);
            int v1 = bottomRingStart + r_next;
            int v2 = topRingStart + r_next;
            int v3 = topRingStart + static_cast<int>(r);
            GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, v0, v1, v2, v3);
        }

        // Top cap
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                topCenter, topRingStart + static_cast<int>(r), topRingStart + r_next);
        }
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Cone Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeConePointCount(size_t numRadial, bool closedSweep) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilConeMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
    } catch (...) {
        return 0;
    }
#else
    if (numRadial < 3) numRadial = 3;
    // Base ring + apex + base center
    size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);
    return vertsPerRing + 2;
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateConeMesh(
    size_t numRadial,
    float radius,
    float height,
    float sweepDegrees,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_VERTEX;

        if (numRadial < 3) numRadial = 3;
        bool closedSweep = (sweepDegrees >= 360.0f);

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilConeMeshGenerator::GenerateTopology(
            numRadial, closedSweep);
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilConeMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        if (closedSweep) {
            GeomUtilConeMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, radius, height, framePtr);
        } else {
            GeomUtilConeMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, radius, height, sweepDegrees, framePtr);
        }
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals
        size_t numNormals = GeomUtilConeMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
        VtVec3fArray vtNormals(numNormals);
        if (closedSweep) {
            GeomUtilConeMeshGenerator::GenerateNormals(
                vtNormals.begin(), numRadial, radius, height, framePtr);
        } else {
            GeomUtilConeMeshGenerator::GenerateNormals(
                vtNormals.begin(), numRadial, radius, height, sweepDegrees, framePtr);
        }
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone cone generation
        float sweepRad = sweepDegrees * static_cast<float>(M_PI) / 180.0f;
        if (sweepRad > 2.0f * static_cast<float>(M_PI)) {
            sweepRad = 2.0f * static_cast<float>(M_PI);
        }

        float halfHeight = height * 0.5f;
        size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);

        // Base center
        {
            float px = 0, py = 0, pz = -halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Base ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = radius * cosTheta;
            float py = radius * sinTheta;
            float pz = -halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Apex
        {
            float px = 0, py = 0, pz = halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Normals
        // Base center normal
        {
            float nx = 0, ny = 0, nz = -1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Side normals for base ring
        float slopeAngle = std::atan2(radius, height);
        float cosSlope = std::cos(slopeAngle);
        float sinSlope = std::sin(slopeAngle);

        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float nx = cosSlope * cosTheta;
            float ny = cosSlope * sinTheta;
            float nz = sinSlope;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Apex normal (average of all side normals = pointing up)
        {
            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Topology
        int baseCenter = 0;
        int baseRingStart = 1;
        int apex = static_cast<int>(mesh->points.size() / 3 - 1);

        // Base cap
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                baseCenter, baseRingStart + r_next, baseRingStart + static_cast<int>(r));
        }

        // Side triangles
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                baseRingStart + static_cast<int>(r), baseRingStart + r_next, apex);
        }
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Capsule Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeCapsulePointCount(size_t numRadial, size_t numCapAxial, bool closedSweep) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilCapsuleMeshGenerator::ComputeNumPoints(numRadial, numCapAxial, closedSweep);
    } catch (...) {
        return 0;
    }
#else
    if (numRadial < 3) numRadial = 3;
    if (numCapAxial < 1) numCapAxial = 1;
    // 2 poles + 2 * numCapAxial rings for caps + 2 rings for cylinder
    size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);
    return 2 + (2 * numCapAxial + 2) * vertsPerRing;
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateCapsuleMesh(
    size_t numRadial,
    size_t numCapAxial,
    float bottomRadius,
    float topRadius,
    float height,
    float sweepDegrees,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_VERTEX;

        if (numRadial < 3) numRadial = 3;
        if (numCapAxial < 1) numCapAxial = 1;
        bool closedSweep = (sweepDegrees >= 360.0f);

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilCapsuleMeshGenerator::GenerateTopology(
            numRadial, numCapAxial, closedSweep);
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilCapsuleMeshGenerator::ComputeNumPoints(
            numRadial, numCapAxial, closedSweep);
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        GeomUtilCapsuleMeshGenerator::GeneratePoints(
            vtPoints.begin(), numRadial, numCapAxial,
            bottomRadius, topRadius, height, sweepDegrees, framePtr);
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals
        size_t numNormals = GeomUtilCapsuleMeshGenerator::ComputeNumPoints(
            numRadial, numCapAxial, closedSweep);
        VtVec3fArray vtNormals(numNormals);
        GeomUtilCapsuleMeshGenerator::GenerateNormals(
            vtNormals.begin(), numRadial, numCapAxial,
            bottomRadius, topRadius, height, sweepDegrees, framePtr);
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone capsule generation - simplified version
        // A capsule is a cylinder with hemispherical caps
        float sweepRad = sweepDegrees * static_cast<float>(M_PI) / 180.0f;
        if (sweepRad > 2.0f * static_cast<float>(M_PI)) {
            sweepRad = 2.0f * static_cast<float>(M_PI);
        }

        float halfHeight = height * 0.5f;
        size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);

        // Bottom pole
        {
            float px = 0, py = 0, pz = -halfHeight - bottomRadius;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);

            float nx = 0, ny = 0, nz = -1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Bottom hemisphere rings
        for (size_t axial = 1; axial <= numCapAxial; ++axial) {
            float phi = static_cast<float>(M_PI) * 0.5f * axial / numCapAxial;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (size_t r = 0; r < vertsPerRing; ++r) {
                float theta = sweepRad * r / numRadial;
                float cosTheta = std::cos(theta);
                float sinTheta = std::sin(theta);

                float px = bottomRadius * sinPhi * cosTheta;
                float py = bottomRadius * sinPhi * sinTheta;
                float pz = -halfHeight - bottomRadius * cosPhi;
                TransformPoint(px, py, pz, transform);
                mesh->points.push_back(px);
                mesh->points.push_back(py);
                mesh->points.push_back(pz);

                float nx = sinPhi * cosTheta;
                float ny = sinPhi * sinTheta;
                float nz = -cosPhi;
                TransformNormal(nx, ny, nz, transform);
                mesh->normals.push_back(nx);
                mesh->normals.push_back(ny);
                mesh->normals.push_back(nz);
            }
        }

        // Cylinder bottom ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = bottomRadius * cosTheta;
            float py = bottomRadius * sinTheta;
            float pz = -halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);

            float nx = cosTheta;
            float ny = sinTheta;
            float nz = 0;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Cylinder top ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = topRadius * cosTheta;
            float py = topRadius * sinTheta;
            float pz = halfHeight;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);

            float nx = cosTheta;
            float ny = sinTheta;
            float nz = 0;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Top hemisphere rings
        for (size_t axial = numCapAxial; axial >= 1; --axial) {
            float phi = static_cast<float>(M_PI) * 0.5f * axial / numCapAxial;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (size_t r = 0; r < vertsPerRing; ++r) {
                float theta = sweepRad * r / numRadial;
                float cosTheta = std::cos(theta);
                float sinTheta = std::sin(theta);

                float px = topRadius * sinPhi * cosTheta;
                float py = topRadius * sinPhi * sinTheta;
                float pz = halfHeight + topRadius * cosPhi;
                TransformPoint(px, py, pz, transform);
                mesh->points.push_back(px);
                mesh->points.push_back(py);
                mesh->points.push_back(pz);

                float nx = sinPhi * cosTheta;
                float ny = sinPhi * sinTheta;
                float nz = cosPhi;
                TransformNormal(nx, ny, nz, transform);
                mesh->normals.push_back(nx);
                mesh->normals.push_back(ny);
                mesh->normals.push_back(nz);
            }
        }

        // Top pole
        {
            float px = 0, py = 0, pz = halfHeight + topRadius;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);

            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Generate topology
        int bottomPole = 0;
        int topPole = static_cast<int>(mesh->points.size() / 3 - 1);
        size_t totalRings = 2 * numCapAxial + 2;

        // Bottom cap triangles
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                bottomPole, 1 + static_cast<int>(r), 1 + r_next);
        }

        // Ring quads
        for (size_t ring = 0; ring < totalRings - 1; ++ring) {
            int ringStart = 1 + static_cast<int>(ring * vertsPerRing);
            int nextRingStart = 1 + static_cast<int>((ring + 1) * vertsPerRing);

            for (size_t r = 0; r < numRadial; ++r) {
                int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
                if (!closedSweep && r == numRadial - 1) continue;

                int v0 = ringStart + static_cast<int>(r);
                int v1 = ringStart + r_next;
                int v2 = nextRingStart + r_next;
                int v3 = nextRingStart + static_cast<int>(r);
                GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, v0, v1, v2, v3);
            }
        }

        // Top cap triangles
        int lastRingStart = 1 + static_cast<int>((totalRings - 1) * vertsPerRing);
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                lastRingStart + static_cast<int>(r), topPole, lastRingStart + r_next);
        }
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Plane Mesh Generation
// ============================================================================

size_t GeomUtil_ComputePlanePointCount(void) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilPlaneMeshGenerator::ComputeNumPoints();
    } catch (...) {
        return 0;
    }
#else
    return 4; // Simple quad
#endif
}

GeomUtilMeshDataRef GeomUtil_GeneratePlaneMesh(
    float xLength,
    float yLength,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_CONSTANT;

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilPlaneMeshGenerator::GenerateTopology();
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilPlaneMeshGenerator::ComputeNumPoints();
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        GeomUtilPlaneMeshGenerator::GeneratePoints(
            vtPoints.begin(), xLength, yLength, framePtr);
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals (single normal for constant interpolation)
        VtVec3fArray vtNormals(1);
        GeomUtilPlaneMeshGenerator::GenerateNormals(vtNormals.begin(), framePtr);
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone plane generation
        float hx = xLength * 0.5f;
        float hy = yLength * 0.5f;

        // 4 vertices of the plane
        float verts[4][3] = {
            {-hx, -hy, 0},
            { hx, -hy, 0},
            { hx,  hy, 0},
            {-hx,  hy, 0}
        };

        for (int i = 0; i < 4; ++i) {
            float px = verts[i][0], py = verts[i][1], pz = verts[i][2];
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Single normal (constant interpolation)
        {
            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Single quad face
        GenerateQuadFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices, 0, 1, 2, 3);
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - Disk Mesh Generation
// ============================================================================

size_t GeomUtil_ComputeDiskPointCount(size_t numRadial, bool closedSweep) {
#if GEOMUTIL_USE_USD
    try {
        return GeomUtilDiskMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
    } catch (...) {
        return 0;
    }
#else
    if (numRadial < 3) numRadial = 3;
    // Center + ring
    size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);
    return 1 + vertsPerRing;
#endif
}

GeomUtilMeshDataRef GeomUtil_GenerateDiskMesh(
    size_t numRadial,
    float radius,
    float sweepDegrees,
    const float* transform)
{
    try {
        auto* mesh = new GeomUtilMeshDataOpaque();
        mesh->normalsInterpolation = GEOMUTIL_NORMALS_CONSTANT;

        if (numRadial < 3) numRadial = 3;
        bool closedSweep = (sweepDegrees >= 360.0f);

#if GEOMUTIL_USE_USD
        // Generate topology
        PxOsdMeshTopology topology = GeomUtilDiskMeshGenerator::GenerateTopology(
            numRadial, closedSweep);
        ExtractTopology(topology, mesh->faceVertexCounts, mesh->faceVertexIndices);

        // Generate points
        size_t numPoints = GeomUtilDiskMeshGenerator::ComputeNumPoints(numRadial, closedSweep);
        VtVec3fArray vtPoints(numPoints);
        const GfMatrix4d* framePtr = nullptr;
        GfMatrix4d frame;
        if (transform) {
            frame = ToGfMatrix4d(transform);
            framePtr = &frame;
        }

        if (closedSweep) {
            GeomUtilDiskMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, radius, framePtr);
        } else {
            GeomUtilDiskMeshGenerator::GeneratePoints(
                vtPoints.begin(), numRadial, radius, sweepDegrees, framePtr);
        }
        ExtractPoints(vtPoints, mesh->points);

        // Generate normals (single normal for constant interpolation)
        VtVec3fArray vtNormals(1);
        GeomUtilDiskMeshGenerator::GenerateNormals(vtNormals.begin(), framePtr);
        ExtractPoints(vtNormals, mesh->normals);
#else
        // Standalone disk generation
        float sweepRad = sweepDegrees * static_cast<float>(M_PI) / 180.0f;
        if (sweepRad > 2.0f * static_cast<float>(M_PI)) {
            sweepRad = 2.0f * static_cast<float>(M_PI);
        }

        size_t vertsPerRing = closedSweep ? numRadial : (numRadial + 1);

        // Center
        {
            float px = 0, py = 0, pz = 0;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Ring
        for (size_t r = 0; r < vertsPerRing; ++r) {
            float theta = sweepRad * r / numRadial;
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            float px = radius * cosTheta;
            float py = radius * sinTheta;
            float pz = 0;
            TransformPoint(px, py, pz, transform);
            mesh->points.push_back(px);
            mesh->points.push_back(py);
            mesh->points.push_back(pz);
        }

        // Single normal (constant interpolation)
        {
            float nx = 0, ny = 0, nz = 1;
            TransformNormal(nx, ny, nz, transform);
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }

        // Triangle fan
        for (size_t r = 0; r < numRadial; ++r) {
            int r_next = static_cast<int>((r + 1) % (closedSweep ? numRadial : (numRadial + 1)));
            if (!closedSweep && r == numRadial - 1) continue;
            GenerateTriFaceIndices(mesh->faceVertexCounts, mesh->faceVertexIndices,
                0, 1 + static_cast<int>(r), 1 + r_next);
        }
#endif

        return mesh;
    } catch (...) {
        return nullptr;
    }
}

} // extern "C"
