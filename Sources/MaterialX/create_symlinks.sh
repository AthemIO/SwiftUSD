#!/bin/bash
cd "$(dirname "$0")"

# Create directories for original MaterialX include structure
mkdir -p include/MaterialXCore
mkdir -p include/MaterialXFormat
mkdir -p include/MaterialXGenShader
mkdir -p include/MaterialXGenShader/Nodes
mkdir -p include/MaterialXGenGlsl
mkdir -p include/MaterialXGenGlsl/Nodes
mkdir -p include/MaterialXGenMsl
mkdir -p include/MaterialXGenMsl/Nodes
mkdir -p include/MaterialXGenOsl
mkdir -p include/MaterialXGenOsl/Nodes
mkdir -p include/MaterialXRender

# ==========================================
# Original MaterialX include paths (for internal includes)
# Using RELATIVE paths for cross-platform compatibility
# ==========================================

# MaterialXCore (relative path: ../../source/MaterialXCore/)
for f in source/MaterialXCore/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXCore/${name}"
done

# MaterialXFormat (relative path: ../../source/MaterialXFormat/)
for f in source/MaterialXFormat/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXFormat/${name}"
done

# MaterialXGenShader (relative path: ../../source/MaterialXGenShader/)
for f in source/MaterialXGenShader/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXGenShader/${name}"
done
# MaterialXGenShader/Nodes (relative path: ../../../source/MaterialXGenShader/Nodes/)
for f in source/MaterialXGenShader/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "../../../$f" "include/MaterialXGenShader/Nodes/${name}"
done

# MaterialXGenGlsl (relative path: ../../source/MaterialXGenGlsl/)
for f in source/MaterialXGenGlsl/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXGenGlsl/${name}"
done
# MaterialXGenGlsl/Nodes (relative path: ../../../source/MaterialXGenGlsl/Nodes/)
for f in source/MaterialXGenGlsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "../../../$f" "include/MaterialXGenGlsl/Nodes/${name}"
done

# MaterialXGenMsl (relative path: ../../source/MaterialXGenMsl/)
for f in source/MaterialXGenMsl/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXGenMsl/${name}"
done
# MaterialXGenMsl/Nodes (relative path: ../../../source/MaterialXGenMsl/Nodes/)
for f in source/MaterialXGenMsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "../../../$f" "include/MaterialXGenMsl/Nodes/${name}"
done

# MaterialXGenOsl (relative path: ../../source/MaterialXGenOsl/)
for f in source/MaterialXGenOsl/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXGenOsl/${name}"
done
# MaterialXGenOsl/Nodes (relative path: ../../../source/MaterialXGenOsl/Nodes/)
for f in source/MaterialXGenOsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "../../../$f" "include/MaterialXGenOsl/Nodes/${name}"
done

# MaterialXRender (relative path: ../../source/MaterialXRender/)
for f in source/MaterialXRender/*.h; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXRender/${name}"
done
# MaterialXRender .inl files
for f in source/MaterialXRender/*.inl; do
  name=$(basename "$f")
  ln -sf "../../$f" "include/MaterialXRender/${name}"
done

# ==========================================
# MX* prefixed headers in include/MaterialX/ (for public API)
# Using RELATIVE paths for cross-platform compatibility
# ==========================================

# MaterialXCore (MXCore prefix) - relative path: ../../source/MaterialXCore/
for f in source/MaterialXCore/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXCore${name}.h"
done

# MaterialXFormat - relative path: ../../source/MaterialXFormat/
for f in source/MaterialXFormat/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXFormat${name}.h"
done

# MaterialXGenShader - relative path: ../../source/MaterialXGenShader/
for f in source/MaterialXGenShader/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenShader${name}.h"
  # Also create shortcut aliases (MXGen<name> without "Shader" prefix duplication)
  # e.g., ShaderGenerator.h -> both MXGenShaderShaderGenerator.h AND MXGenShaderGenerator.h
  ln -sf "../../$f" "include/MaterialX/MXGen${name}.h"
done
for f in source/MaterialXGenShader/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenShader${name}.h"
done

# MaterialXGenGlsl - relative path: ../../source/MaterialXGenGlsl/
for f in source/MaterialXGenGlsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenGlsl${name}.h"
done
for f in source/MaterialXGenGlsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenGlsl${name}.h"
done

# MaterialXGenMsl - relative path: ../../source/MaterialXGenMsl/
for f in source/MaterialXGenMsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenMsl${name}.h"
done
for f in source/MaterialXGenMsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenMsl${name}.h"
done

# MaterialXGenOsl - relative path: ../../source/MaterialXGenOsl/
for f in source/MaterialXGenOsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenOsl${name}.h"
done
for f in source/MaterialXGenOsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXGenOsl${name}.h"
done

# MaterialXRender - relative path: ../../source/MaterialXRender/
for f in source/MaterialXRender/*.h; do
  name=$(basename "$f" .h)
  ln -sf "../../$f" "include/MaterialX/MXRender${name}.h"
done

# MXGenShader.h umbrella header - relative path: ../../source/MaterialXGenShader/
ln -sf "../../source/MaterialXGenShader/Shader.h" "include/MaterialX/MXGenShader.h"

echo "Created symlinks in include/"
find include -type l | wc -l | xargs echo "Total symlinks:"
