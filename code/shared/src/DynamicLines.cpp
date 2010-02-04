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

#include "DynamicLines.h"
#include <Ogre.h>
#include <cassert>
#include <cmath>

using namespace Ogre;

enum {
  POSITION_BINDING,
  TEXCOORD_BINDING
};

DynamicLines::DynamicLines(OperationType opType)
{
  initialize(opType,false);
  setMaterial("BaseWhiteNoLighting");
  mDirty = true;
}

DynamicLines::~DynamicLines()
{
}

void DynamicLines::setOperationType(OperationType opType)
{
  mRenderOp.operationType = opType;
}

RenderOperation::OperationType DynamicLines::getOperationType() const
{
  return mRenderOp.operationType;
}

void DynamicLines::addPoint(const Vector3 &p)
{
   mPoints.push_back(p);
   mDirty = true;
}
void DynamicLines::addPoint(Real x, Real y, Real z)
{
   mPoints.push_back(Vector3(x,y,z));
   mDirty = true;
}
const Vector3& DynamicLines::getPoint(unsigned short index) const
{
   assert(index < mPoints.size() && "Point index is out of bounds!!");
   return mPoints[index];
}
unsigned short DynamicLines::getNumPoints(void) const
{
  return (unsigned short)mPoints.size();
}
void DynamicLines::setPoint(unsigned short index, const Vector3 &value)
{
  assert(index < mPoints.size() && "Point index is out of bounds!!");

  mPoints[index] = value;
  mDirty = true;
}
void DynamicLines::clear()
{
  mPoints.clear();
  mDirty = true;
}

void DynamicLines::update()
{
  if (mDirty) fillHardwareBuffers();
}

void DynamicLines::createVertexDeclaration()
{
  VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
  decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
}

void DynamicLines::fillHardwareBuffers()
{
  int size = mPoints.size();

  prepareHardwareBuffers(size,0);

  if (!size) { 
    mBox.setExtents(Vector3::ZERO,Vector3::ZERO);
    mDirty=false;
    return;
  }
  
  Vector3 vaabMin = mPoints[0];
  Vector3 vaabMax = mPoints[0];

  HardwareVertexBufferSharedPtr vbuf =
    mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

  Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
  {
   for(int i = 0; i < size; i++)
   {
      *prPos++ = mPoints[i].x;
      *prPos++ = mPoints[i].y;
      *prPos++ = mPoints[i].z;

      if(mPoints[i].x < vaabMin.x)
         vaabMin.x = mPoints[i].x;
      if(mPoints[i].y < vaabMin.y)
         vaabMin.y = mPoints[i].y;
      if(mPoints[i].z < vaabMin.z)
         vaabMin.z = mPoints[i].z;

      if(mPoints[i].x > vaabMax.x)
         vaabMax.x = mPoints[i].x;
      if(mPoints[i].y > vaabMax.y)
         vaabMax.y = mPoints[i].y;
      if(mPoints[i].z > vaabMax.z)
         vaabMax.z = mPoints[i].z;
   }
  }
  vbuf->unlock();

  mBox.setExtents(vaabMin, vaabMax);

  mDirty = false;
}

/*
void DynamicLines::getWorldTransforms(Matrix4 *xform) const
{
   // return identity matrix to prevent parent transforms
   *xform = Matrix4::IDENTITY;
}
*/
/*
const Quaternion &DynamicLines::getWorldOrientation(void) const
{
   return Quaternion::IDENTITY;
}

const Vector3 &DynamicLines::getWorldPosition(void) const
{
   return Vector3::ZERO;
}
*/

