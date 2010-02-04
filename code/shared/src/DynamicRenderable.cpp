/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Original Public Domain Code: http://www.ogre3d.org/wiki/index.php/DynamicLineDrawing
    Copyright © 2010  Andrew Fenn
    
    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DynamicRenderable.h"
#include <OgreCamera.h>
#include <OgreHardwareBufferManager.h>

using namespace Ogre;

DynamicRenderable::DynamicRenderable()
{
}

DynamicRenderable::~DynamicRenderable()
{
  delete mRenderOp.vertexData;
  delete mRenderOp.indexData;
}

void DynamicRenderable::initialize(RenderOperation::OperationType operationType,
                                   bool useIndices)
{
  // Initialize render operation
  mRenderOp.operationType = operationType;
  mRenderOp.useIndexes = useIndices;
  mRenderOp.vertexData = new VertexData;
  if (mRenderOp.useIndexes)
    mRenderOp.indexData = new IndexData;

  // Reset buffer capacities
  mVertexBufferCapacity = 0;
  mIndexBufferCapacity = 0;

  // Create vertex declaration
  createVertexDeclaration();
}

void DynamicRenderable::prepareHardwareBuffers(size_t vertexCount, 
                                               size_t indexCount)
{
  // Prepare vertex buffer
  size_t newVertCapacity = mVertexBufferCapacity;
  if ((vertexCount > mVertexBufferCapacity) ||
      (!mVertexBufferCapacity))
  {
    // vertexCount exceeds current capacity!
    // It is necessary to reallocate the buffer.

    // Check if this is the first call
    if (!newVertCapacity)
      newVertCapacity = 1;

    // Make capacity the next power of two
    while (newVertCapacity < vertexCount)
      newVertCapacity <<= 1;
  }
  else if (vertexCount < mVertexBufferCapacity>>1) {
    // Make capacity the previous power of two
    while (vertexCount < newVertCapacity>>1)
      newVertCapacity >>= 1;
  }
  if (newVertCapacity != mVertexBufferCapacity) 
  {
    mVertexBufferCapacity = newVertCapacity;
    // Create new vertex buffer
    HardwareVertexBufferSharedPtr vbuf =
      HardwareBufferManager::getSingleton().createVertexBuffer(
        mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
        mVertexBufferCapacity,
        HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?

    // Bind buffer
    mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
  }
  // Update vertex count in the render operation
  mRenderOp.vertexData->vertexCount = vertexCount;

  if (mRenderOp.useIndexes)
  {
    OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");

    size_t newIndexCapacity = mIndexBufferCapacity;
    // Prepare index buffer
    if ((indexCount > newIndexCapacity) ||
        (!newIndexCapacity))
    {
      // indexCount exceeds current capacity!
      // It is necessary to reallocate the buffer.

      // Check if this is the first call
      if (!newIndexCapacity)
        newIndexCapacity = 1;

      // Make capacity the next power of two
      while (newIndexCapacity < indexCount)
        newIndexCapacity <<= 1;

    }
    else if (indexCount < newIndexCapacity>>1) 
    {
      // Make capacity the previous power of two
      while (indexCount < newIndexCapacity>>1)
        newIndexCapacity >>= 1;
    }

    if (newIndexCapacity != mIndexBufferCapacity)
    {
      mIndexBufferCapacity = newIndexCapacity;
      // Create new index buffer
      mRenderOp.indexData->indexBuffer =
        HardwareBufferManager::getSingleton().createIndexBuffer(
          HardwareIndexBuffer::IT_16BIT,
          mIndexBufferCapacity,
          HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
    }

    // Update index count in the render operation
    mRenderOp.indexData->indexCount = indexCount;
  }
}

Real DynamicRenderable::getBoundingRadius(void) const
{
  return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}

Real DynamicRenderable::getSquaredViewDepth(const Camera* cam) const
{
   Vector3 vMin, vMax, vMid, vDist;
   vMin = mBox.getMinimum();
   vMax = mBox.getMaximum();
   vMid = ((vMax - vMin) * 0.5) + vMin;
   vDist = cam->getDerivedPosition() - vMid;

   return vDist.squaredLength();
}

