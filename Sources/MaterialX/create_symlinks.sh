#!/bin/bash
cd "$(dirname "$0")"
BASEDIR=$(pwd)

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
# ==========================================

# MaterialXCore
for f in source/MaterialXCore/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXCore/${name}"
done

# MaterialXFormat
for f in source/MaterialXFormat/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXFormat/${name}"
done

# MaterialXGenShader
for f in source/MaterialXGenShader/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenShader/${name}"
done
for f in source/MaterialXGenShader/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenShader/Nodes/${name}"
done

# MaterialXGenGlsl
for f in source/MaterialXGenGlsl/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenGlsl/${name}"
done
for f in source/MaterialXGenGlsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenGlsl/Nodes/${name}"
done

# MaterialXGenMsl
for f in source/MaterialXGenMsl/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenMsl/${name}"
done
for f in source/MaterialXGenMsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenMsl/Nodes/${name}"
done

# MaterialXGenOsl
for f in source/MaterialXGenOsl/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenOsl/${name}"
done
for f in source/MaterialXGenOsl/Nodes/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXGenOsl/Nodes/${name}"
done

# MaterialXRender
for f in source/MaterialXRender/*.h; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXRender/${name}"
done
# MaterialXRender .inl files
for f in source/MaterialXRender/*.inl; do
  name=$(basename "$f")
  ln -sf "$BASEDIR/$f" "include/MaterialXRender/${name}"
done

# ==========================================
# MX* prefixed headers in include/MaterialX/ (for public API)
# ==========================================

# MaterialXCore (MXCore prefix)
for f in source/MaterialXCore/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXCore${name}.h"
done

# MaterialXFormat
for f in source/MaterialXFormat/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXFormat${name}.h"
done

# MaterialXGenShader
for f in source/MaterialXGenShader/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenShader${name}.h"
done
for f in source/MaterialXGenShader/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenShader${name}.h"
done

# MaterialXGenGlsl
for f in source/MaterialXGenGlsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenGlsl${name}.h"
done
for f in source/MaterialXGenGlsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenGlsl${name}.h"
done

# MaterialXGenMsl
for f in source/MaterialXGenMsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenMsl${name}.h"
done
for f in source/MaterialXGenMsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenMsl${name}.h"
done

# MaterialXGenOsl
for f in source/MaterialXGenOsl/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenOsl${name}.h"
done
for f in source/MaterialXGenOsl/Nodes/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXGenOsl${name}.h"
done

# MaterialXRender
for f in source/MaterialXRender/*.h; do
  name=$(basename "$f" .h)
  ln -sf "$BASEDIR/$f" "include/MaterialX/MXRender${name}.h"
done

# MXGenShader.h umbrella header
ln -sf "$BASEDIR/source/MaterialXGenShader/Shader.h" "include/MaterialX/MXGenShader.h"

echo "Created symlinks in include/"
find include -type l | wc -l | xargs echo "Total symlinks:"
