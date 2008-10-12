/*
 * OgreMaxViewer - An Ogre 3D-based viewer for .scene and .mesh files
 * Copyright 2008 Derek Nedelman
 *
 * This code is available under the OgreMax Free License:
 *   -You may use this code for any purpose, commercial or non-commercial.
 *   -If distributing derived works (that use this source code) in binary or source code form, 
 *    you must give the following credit in your work's end-user documentation: 
 *        "Portions of this work provided by OgreMax (www.ogremax.com)"
 *
 * Derek Nedelman assumes no responsibility for any harm caused by using this code.
 * 
 * OgreMaxViewer was written by Derek Nedelman and released at www.ogremax.com 
 */

//Includes---------------------------------------------------------------------
#include "OgreMaxUtilities.hpp"
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgrePlatform.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include <wtypes.h>
    #undef min
#endif

using namespace Ogre;
using namespace OgreMax;
using namespace OgreMax::Types;


//Implementation---------------------------------------------------------------
void OgreMaxUtilities::LoadXmlDocument(const String& fileName, TiXmlDocument& document, const String& resourceGroupName)
{
    //Open the file
    DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(fileName, resourceGroupName);
    if (stream.isNull())
    {
        StringUtil::StrStreamType errorMessage;
        errorMessage << "Could not open XML file: " << fileName;

        OGRE_EXCEPT
            (
            Exception::ERR_FILE_NOT_FOUND,
		    errorMessage.str(), 
		    "OgreMaxUtilities::LoadXmlDocument"
            );
    }

    //Get the file contents
    String data = stream->getAsString();
    
    //Parse the XML document
    document.Parse(data.c_str());
    stream.setNull();
    if (document.Error())
    {
        StringUtil::StrStreamType errorMessage;
        errorMessage << "There was an error with the XML file: " << fileName;

        OGRE_EXCEPT
            (
            Exception::ERR_INVALID_STATE,
		    errorMessage.str(), 
		    "OgreMaxUtilities::LoadXmlDocument"
            );
    }
}

void OgreMaxUtilities::LoadXYZ(const TiXmlElement* objectElement, Vector3& xyz)
{
    xyz.x = GetRealAttribute(objectElement, "x", 0);
    xyz.y = GetRealAttribute(objectElement, "y", 0);
    xyz.z = GetRealAttribute(objectElement, "z", 0);
}

Vector3 OgreMaxUtilities::LoadXYZ(const TiXmlElement* objectElement)
{
    Vector3 xyz;
    LoadXYZ(objectElement, xyz);
    return xyz;
}

ColourValue OgreMaxUtilities::LoadColor(const TiXmlElement* objectElement)
{   
    ColourValue color;
    color.r = GetRealAttribute(objectElement, "r", 0);
    color.g = GetRealAttribute(objectElement, "g", 0);
    color.b = GetRealAttribute(objectElement, "b", 0);
    color.a = GetRealAttribute(objectElement, "a", 1);
    return color;
}

Plane OgreMaxUtilities::LoadPlane(const TiXmlElement* objectElement)
{   
    Plane plane;
    plane.normal.x = GetRealAttribute(objectElement, "x", 0);
    plane.normal.y = GetRealAttribute(objectElement, "y", 0);
    plane.normal.z = GetRealAttribute(objectElement, "z", 0);
    plane.d = GetRealAttribute(objectElement, "d", 0);
    return plane;
}

Quaternion OgreMaxUtilities::LoadRotation(const TiXmlElement* objectElement)
{
    Quaternion rotation = Quaternion::IDENTITY;

    if (objectElement->Attribute("qx") != 0)
    {
        //The rotation is specified as a quaternion
        rotation.x = GetRealAttribute(objectElement, "qx", 0);
        rotation.y = GetRealAttribute(objectElement, "qy", 0);
        rotation.z = GetRealAttribute(objectElement, "qz", 0);
        rotation.w = GetRealAttribute(objectElement, "qw", 0);
    }
    else if (objectElement->Attribute("axisX") != 0)
    {
        //The rotation is specified as an axis and angle
        Real angle = GetRealAttribute(objectElement, "angle", 0);

        Vector3 axis;
        axis.x = GetRealAttribute(objectElement, "axisX", 0);
        axis.y = GetRealAttribute(objectElement, "axisY", 0);
        axis.z = GetRealAttribute(objectElement, "axisZ", 0);

        //Convert the angle and axis into the rotation quaternion
        rotation.FromAngleAxis(Radian(angle), axis);
    }
    else if (objectElement->Attribute("angleX") != 0)
    {
        //Assume the rotation is specified as three Euler angles
        Vector3 euler;
        euler.x = GetRealAttribute(objectElement, "angleX", 0);
        euler.y = GetRealAttribute(objectElement, "angleY", 0);
        euler.z = GetRealAttribute(objectElement, "angleZ", 0);
        String order = GetStringAttribute(objectElement, "order");

        //Convert Euler angles to a matrix
        Matrix3 rotationMatrix;
        if (order.length() < 2)
            rotationMatrix.FromEulerAnglesXYZ(Radian(euler.x), Radian(euler.y), Radian(euler.z));
        else
        {
            if (order[0] == 'x')
            {
                if (order[1] == 'y')
                    rotationMatrix.FromEulerAnglesXYZ(Radian(euler.x), Radian(euler.y), Radian(euler.z));
                else
                    rotationMatrix.FromEulerAnglesXZY(Radian(euler.x), Radian(euler.y), Radian(euler.z));
            }
            else if (order[0] == 'y')
            {
                if (order[1] == 'x')
                    rotationMatrix.FromEulerAnglesYXZ(Radian(euler.x), Radian(euler.y), Radian(euler.z));
                else
                    rotationMatrix.FromEulerAnglesYZX(Radian(euler.x), Radian(euler.y), Radian(euler.z));
            }
            else
            {
                if (order[1] == 'x')
                    rotationMatrix.FromEulerAnglesZXY(Radian(euler.x), Radian(euler.y), Radian(euler.z));
                else
                    rotationMatrix.FromEulerAnglesZYX(Radian(euler.x), Radian(euler.y), Radian(euler.z));
            }
        }

        //Convert the matrix into the rotation quaternion
        rotation.FromRotationMatrix(rotationMatrix);
    }
    
    return rotation;
}

FloatRect OgreMaxUtilities::LoadFloatRectangle(const TiXmlElement* objectElement)
{
    FloatRect rect;
    rect.left = GetRealAttribute(objectElement, "left", 0);
    rect.top = GetRealAttribute(objectElement, "top", 0);
    rect.right = GetRealAttribute(objectElement, "right", 0);
    rect.bottom = GetRealAttribute(objectElement, "bottom", 0);
    return rect;
}

void OgreMaxUtilities::LoadBufferUsage(const TiXmlElement* objectElement, HardwareBuffer::Usage& usage, bool& shadowed)
{
    String usageText = GetStringAttribute(objectElement, "usage");
    usage = usageText.empty() ? HardwareBuffer::HBU_STATIC_WRITE_ONLY : ParseHardwareBufferUsage(usageText);
	
    shadowed = GetBoolAttribute(objectElement, "useShadow", true);
}

void OgreMaxUtilities::LoadBoundingVolume(const TiXmlElement* objectElement, BoundingVolume& volume)
{
    String type = OgreMaxUtilities::GetStringAttribute(objectElement, "type");
    volume.type = ParseBoundingVolumeType(type);
    volume.sphereRadius = OgreMaxUtilities::GetRealAttribute(objectElement, "radius", 0);
    int faceCount = OgreMaxUtilities::GetIntAttribute(objectElement, "faceCount", 0);
    if (faceCount > 0)
        volume.meshFaces.resize(faceCount);

    //Parse child elements
    String elementName;
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(objectElement, childElement))
    {
        elementName = childElement->Value();

        if (elementName == "size")
            volume.boxSize = LoadXYZ(childElement);
        else if (elementName == "faces")
            LoadBoundingVolumeFaces(childElement, faceCount, volume.meshFaces);        
    }
}

void OgreMaxUtilities::LoadBoundingVolumeFaces(const TiXmlElement* objectElement, int faceCount, std::vector<BoundingVolume::Face>& faces)
{
    //Parse child elements, treating them all as faces
    int faceIndex = 0;
    const TiXmlElement* childElement = 0;
    while ((childElement = IterateChildElements(objectElement, childElement)) && faceIndex < faceCount)
    {
        BoundingVolume::Face& face = faces[faceIndex++];

        //Load the vertices
        int vertexIndex = 0;
        const TiXmlElement* vertexElement = 0;
        while ((vertexElement = IterateChildElements(childElement, vertexElement)) && vertexIndex < 3)
            LoadXYZ(vertexElement, face.vertex[vertexIndex++]);
    }
}

bool OgreMaxUtilities::ParseSceneManager(const String& sceneManager, SceneType& sceneType)
{
    sceneType = (SceneType)0;

    String sceneManagerLower = sceneManager;
    StringUtil::toLowerCase(sceneManagerLower);

    if (sceneManagerLower == "generic")
        sceneType = ST_GENERIC;
    else if (sceneManagerLower == "exteriorclose")
        sceneType = ST_EXTERIOR_CLOSE;
    else if (sceneManagerLower == "exteriorfar")
        sceneType = ST_EXTERIOR_FAR;
    else if (sceneManagerLower == "exteriorrealfar")
        sceneType = ST_EXTERIOR_REAL_FAR;
    else if (sceneManagerLower == "interior")
		sceneType = ST_INTERIOR;

    return sceneType != (SceneType)0;
}

bool OgreMaxUtilities::ParseBool(const String& value)
{
    String valueLower = value;
    StringUtil::toLowerCase(valueLower);
    if (valueLower.empty() || valueLower == "false" || value == "no" || value == "0")
        return false;
    else
        return true;
}    

Light::LightTypes OgreMaxUtilities::ParseLightType(const String& type)
{
    String typeLower = type;
    StringUtil::toLowerCase(typeLower);

    if (typeLower == "point")
        return Light::LT_POINT;
    else if (typeLower == "directional")
        return Light::LT_DIRECTIONAL;
    else if (typeLower == "spot")
        return Light::LT_SPOTLIGHT;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid light type specified: " << type;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseLightType"
        );
}

ProjectionType OgreMaxUtilities::ParseProjectionType(const String& type)
{
    String typeLower = type;
    StringUtil::toLowerCase(typeLower);

    if (typeLower == "perspective")
        return PT_PERSPECTIVE;
    else if (typeLower == "orthographic")
        return PT_ORTHOGRAPHIC;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid projection type specified: " << type;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseProjectionType"
        );
}

BillboardType OgreMaxUtilities::ParseBillboardType(const String& type)
{
    String typeLower = type;
    StringUtil::toLowerCase(typeLower);

    if (typeLower == "point")
        return BBT_POINT;
    else if (typeLower == "orientedcommon")
        return BBT_ORIENTED_COMMON;
    else if (typeLower == "orientedself")
        return BBT_ORIENTED_SELF;
    else if (typeLower == "perpendicularcommon")
        return BBT_PERPENDICULAR_COMMON;
    else if (typeLower == "perpendicularself")
        return BBT_PERPENDICULAR_SELF;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid billboard type specified: " << type;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseBillboardType"
        );
}

BillboardOrigin OgreMaxUtilities::ParseBillboardOrigin(const String& origin)
{
    String originLower = origin;
    StringUtil::toLowerCase(originLower);

    if (originLower == "topleft")
        return BBO_TOP_LEFT;
    else if (originLower == "topcenter")
        return BBO_TOP_CENTER;
    else if (originLower == "topright")
        return BBO_TOP_RIGHT;
    else if (originLower == "centerleft")
        return BBO_CENTER_LEFT;
    else if (originLower == "center")
        return BBO_CENTER;
    else if (originLower == "centerright")
        return BBO_CENTER_RIGHT;
    else if (originLower == "bottomleft")
        return BBO_BOTTOM_LEFT;
    else if (originLower == "bottomcenter")
        return BBO_BOTTOM_CENTER;
    else if (originLower == "bottomright")
        return BBO_BOTTOM_RIGHT;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid billboard origin specified: " << origin;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseBillboardOrigin"
        );
}

BillboardRotationType OgreMaxUtilities::ParseBillboardRotationType(const String& rotationType)
{
    String rotationTypeLower = rotationType;
    StringUtil::toLowerCase(rotationTypeLower);

    if (rotationTypeLower == "vertex")
        return BBR_VERTEX;
    else if (rotationTypeLower == "texcoord")
        return BBR_TEXCOORD;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid billboard rotation type specified: " << rotationType;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseBillboardRotationType"
        );
}

FogMode OgreMaxUtilities::ParseFogMode(const String& mode)
{
    String modeLower = mode;
    StringUtil::toLowerCase(modeLower);

    if (modeLower == "exp")
        return FOG_EXP;
    else if (modeLower == "exp2")
        return FOG_EXP2;
    else if (modeLower == "linear")
        return FOG_LINEAR;
    else
        return FOG_NONE;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid fog mode specified: " << mode;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseFogMode"
        );
}

HardwareBuffer::Usage OgreMaxUtilities::ParseHardwareBufferUsage(const String& usage)
{
    String usageLower = usage;
    StringUtil::toLowerCase(usageLower);

    if (usageLower == "static")
        return HardwareBuffer::HBU_STATIC;
    else if (usageLower == "dynamic")
        return HardwareBuffer::HBU_DYNAMIC;
    else if (usageLower == "writeonly")
        return HardwareBuffer::HBU_WRITE_ONLY;
    else if (usageLower == "staticwriteonly")
        return HardwareBuffer::HBU_STATIC_WRITE_ONLY;
    else if (usageLower == "dynamicwriteonly")
        return HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid hardware buffer usage specified: " << usage;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseHardwareBufferUsage"
        );
}

Node::TransformSpace OgreMaxUtilities::ParseTransformSpace(const String& space)
{
    String spaceLower = space;
    StringUtil::toLowerCase(spaceLower);

    if (spaceLower == "local")
        return Node::TS_LOCAL;
    else if (spaceLower == "parent")
        return Node::TS_PARENT;
    else if (spaceLower == "world")
        return Node::TS_WORLD;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid transform space specified: " << space;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseTransformSpace"
        );
}

BoundingVolume::Type OgreMaxUtilities::ParseBoundingVolumeType(const String& type)
{
    String typeLower = type;
    StringUtil::toLowerCase(typeLower);

    if (typeLower == "sphere")
        return BoundingVolume::SPHERE;
    else if (typeLower == "box")
        return BoundingVolume::BOX;
    else if (typeLower == "mesh")
        return BoundingVolume::MESH;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid bounding volume type specified: " << type;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseBoundingVolumeType"
        );
}

void OgreMaxUtilities::LoadCustomParameters(const TiXmlElement* objectElement, std::vector<CustomParameter>& customParameters)
{
    customParameters.resize(GetElementCount(objectElement, "customParameter"));
    
    size_t customParameterIndex = 0;
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(objectElement, childElement))
    {
        customParameters[customParameterIndex].id = (size_t)GetIntAttribute(childElement, "id", 0);
        customParameters[customParameterIndex].value = GetVector4Attributes(childElement);

        customParameterIndex++;
    }
}

void OgreMaxUtilities::LoadUserDataReference(const TiXmlElement* objectElement, String& userDataReference)
{
    userDataReference = GetStringAttribute(objectElement, "id");
}

void OgreMaxUtilities::LoadSubentities(const TiXmlElement* objectElement, std::vector<EntityParameters::Subentity>& subentities)
{
    subentities.resize(GetElementCount(objectElement, "subentity"));
    
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(objectElement, childElement))
    {
        int index = GetIntAttribute(childElement, "index", 0);
        subentities[index].materialName = GetStringAttribute(childElement, "materialName");
    }
}

void OgreMaxUtilities::LoadNoteTracks(const TiXmlElement* objectElement, std::vector<NoteTrack>& noteTracks)
{
    //Allocate note tracks
    size_t noteTrackCount = GetChildElementCount(objectElement, "noteTrack");
    noteTracks.resize(noteTrackCount);

    //Parse child elements
    size_t noteTrackIndex = 0;
    String elementName;
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(objectElement, childElement))
    {
        elementName = childElement->Value();

        if (elementName == "noteTrack")
            LoadNoteTrack(childElement, noteTracks[noteTrackIndex++]);
    }
}

void OgreMaxUtilities::LoadNoteTrack(const TiXmlElement* objectElement, NoteTrack& noteTrack)
{
    //Track name
    noteTrack.name = OgreMaxUtilities::GetStringAttribute(objectElement, "name");

    //Allocate notes
    size_t noteCount = OgreMaxUtilities::GetChildElementCount(objectElement, "note");
    noteTrack.notes.resize(noteCount);
    
    //Load notes
    size_t noteIndex = 0;
    String elementName;
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(objectElement, childElement))
    {
        elementName = childElement->Value();

        if (elementName == "note")
            LoadNote(childElement, noteTrack.notes[noteIndex++]);
    }
}

void OgreMaxUtilities::LoadNote(const TiXmlElement* objectElement, Note& note)
{
    //Note time
    note.time = OgreMaxUtilities::GetRealAttribute(objectElement, "time", 0);

    //Note text    
    GetChildText(objectElement, note.text);
}

ShadowTechnique OgreMaxUtilities::ParseShadowTechnique(const String& technique)
{
    String techniqueLower = technique;
    StringUtil::toLowerCase(techniqueLower);

    if (techniqueLower == "none")
        return SHADOWTYPE_NONE;
    else if (techniqueLower == "stencilmodulative")
        return SHADOWTYPE_STENCIL_MODULATIVE;
    else if (techniqueLower == "stenciladditive")
        return SHADOWTYPE_STENCIL_ADDITIVE;
    else if (techniqueLower == "texturemodulative")
        return SHADOWTYPE_TEXTURE_MODULATIVE;
    else if (techniqueLower == "textureadditive")
        return SHADOWTYPE_TEXTURE_ADDITIVE;
    else if (techniqueLower == "texturemodulativeintegrated")
        return SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
    else if (techniqueLower == "textureadditiveintegrated")
        return SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;    

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid shadow technique specified: " << technique;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseShadowTechnique"
        );
}

UpAxis OgreMaxUtilities::ParseUpAxis(const String& upAxis)
{
    String upAxisLower = upAxis;
    StringUtil::toLowerCase(upAxisLower);

    if (upAxisLower == "y")
        return UP_AXIS_Y;
    else if (upAxisLower == "z")
        return UP_AXIS_Z;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid up axis specified: " << upAxis;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseUpAxis"
        );
}

Animation::InterpolationMode OgreMaxUtilities::ParseAnimationInterpolationMode(const String& mode)
{
    String modeLower = mode;
    StringUtil::toLowerCase(modeLower);

    if (modeLower == "linear")
        return Animation::IM_LINEAR;
    else if (modeLower == "spline")
        return Animation::IM_SPLINE;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid animation interpolation mode specified: " << mode;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseAnimationInterpolationMode"
        );
}

Animation::RotationInterpolationMode OgreMaxUtilities::ParseAnimationRotationInterpolationMode(const String& mode)
{
    String modeLower = mode;
    StringUtil::toLowerCase(modeLower);

    if (modeLower == "linear")
        return Animation::RIM_LINEAR;
    else if (modeLower == "spherical")
        return Animation::RIM_SPHERICAL;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid animation rotation interpolation mode specified: " << mode;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseAnimationRotationInterpolationMode"
        );
}

NodeVisibility OgreMaxUtilities::ParseNodeVisibility(const String& visibility)
{
    String visibilityLower = visibility;
    StringUtil::toLowerCase(visibilityLower);

    if (visibilityLower == "visible")
        return NODE_VISIBLE;
    else if (visibilityLower == "hidden")
        return NODE_HIDDEN;
    else if (visibilityLower == "treevisible")
        return NODE_TREE_VISIBLE;
    else if (visibilityLower == "treehidden")
        return NODE_TREE_HIDDEN;
    else
        return NODE_VISIBILITY_DEFAULT;
}

ObjectVisibility OgreMaxUtilities::ParseObjectVisibility(const String& visibility)
{
    if (visibility.empty())
        return OBJECT_VISIBILITY_DEFAULT;
    else
        return ParseBool(visibility) ? OBJECT_VISIBLE : OBJECT_HIDDEN;
}

uint8 OgreMaxUtilities::ParseRenderQueue(const String& renderQueue)
{
    static bool initialized = false;
    static std::map<String, uint8> nameToNumber;
    if (!initialized)
    {
        nameToNumber["background"] = RENDER_QUEUE_BACKGROUND;
        nameToNumber["skiesearly"] = RENDER_QUEUE_SKIES_EARLY;
        nameToNumber["queue1"] = RENDER_QUEUE_1;
        nameToNumber["queue2"] = RENDER_QUEUE_2;
		nameToNumber["worldgeometry1"] = RENDER_QUEUE_WORLD_GEOMETRY_1;
        nameToNumber["queue3"] = RENDER_QUEUE_3;
        nameToNumber["queue4"] = RENDER_QUEUE_4;
		nameToNumber["main"] = RENDER_QUEUE_MAIN;
        nameToNumber["queue6"] = RENDER_QUEUE_6;
        nameToNumber["queue7"] = RENDER_QUEUE_7;
		nameToNumber["worldgeometry2"] = RENDER_QUEUE_WORLD_GEOMETRY_2;
        nameToNumber["queue8"] = RENDER_QUEUE_8;
        nameToNumber["queue9"] = RENDER_QUEUE_9;
        nameToNumber["skieslate"] = RENDER_QUEUE_SKIES_LATE;
        nameToNumber["overlay"] = RENDER_QUEUE_OVERLAY;
		nameToNumber["max"] = RENDER_QUEUE_MAX;

        initialized = true;
    }

    if (renderQueue.empty())
        return RENDER_QUEUE_MAIN;
    else if (AllDigits(renderQueue))
        return (uint8)StringConverter::parseUnsignedInt(renderQueue);
    else
    {
        //The render queue name, lowercase
        String renderQueueLower; 

        //Get the offset that comes after the +, if any
        uint8 offset = 0;
        size_t plusFoundAt = renderQueue.find('+');
        if (plusFoundAt != String::npos)
        {
            //Parse the number
            String offsetText = renderQueue.substr(plusFoundAt + 1);
            StringUtil::trim(offsetText);

            offset = (uint8)StringConverter::parseUnsignedInt(offsetText);

            //Remove the "+offset" substring from the render queue name
            renderQueueLower = renderQueue.substr(0, plusFoundAt);
            StringUtil::trim(renderQueueLower);
        }
        else
            renderQueueLower = renderQueue;
        StringUtil::toLowerCase(renderQueueLower);

        //Look up the render queue and return it
        std::map<String, uint8>::iterator item = nameToNumber.find(renderQueueLower);
        if (item != nameToNumber.end())
        {
            //Don't let the render queue exceed the maximum
            return std::min((uint8)(item->second + offset), (uint8)RENDER_QUEUE_MAX);
        }
        else
        {
            StringUtil::StrStreamType errorMessage;
            errorMessage << "Invalid render queue specified: " << renderQueue;

            OGRE_EXCEPT
                (
                Exception::ERR_INVALIDPARAMS,
	            errorMessage.str(), 
	            "OgreMaxUtilities::ParseRenderQueue"
                );
        }
    }
}

PixelFormat OgreMaxUtilities::ParsePixelFormat(const String& pixelFormat)
{
    static bool initialized = false;
    static std::map<String, PixelFormat> nameToFormat;
    if (!initialized)
    {
        nameToFormat["PF_L8"] = PF_L8;
        nameToFormat["PF_L16"] = PF_L16;
        nameToFormat["PF_A8"] = PF_A8;
        nameToFormat["PF_A4L4"] = PF_A4L4;
        nameToFormat["PF_BYTE_LA"] = PF_BYTE_LA;
        nameToFormat["PF_R5G6B5"] = PF_R5G6B5;
        nameToFormat["PF_B5G6R5"] = PF_B5G6R5;
        nameToFormat["PF_R3G3B2"] = PF_R3G3B2;
        nameToFormat["PF_A4R4G4B4"] = PF_A4R4G4B4;
        nameToFormat["PF_A1R5G5B5"] = PF_A1R5G5B5;
        nameToFormat["PF_R8G8B8"] = PF_R8G8B8;
        nameToFormat["PF_B8G8R8"] = PF_B8G8R8;
        nameToFormat["PF_A8R8G8B8"] = PF_A8R8G8B8;
        nameToFormat["PF_A8B8G8R8"] = PF_A8B8G8R8;
        nameToFormat["PF_B8G8R8A8"] = PF_B8G8R8A8;
        nameToFormat["PF_R8G8B8A8"] = PF_R8G8B8A8;
        nameToFormat["PF_X8R8G8B8"] = PF_X8R8G8B8;
        nameToFormat["PF_X8B8G8R8"] = PF_X8B8G8R8;
        nameToFormat["PF_A2R10G10B10"] = PF_A2R10G10B10;
        nameToFormat["PF_A2B10G10R10"] = PF_A2B10G10R10;
        nameToFormat["PF_FLOAT16_R"] = PF_FLOAT16_R;
        nameToFormat["PF_FLOAT16_RGB"] = PF_FLOAT16_RGB;
        nameToFormat["PF_FLOAT16_RGBA"] = PF_FLOAT16_RGBA;
        nameToFormat["PF_FLOAT32_R"] = PF_FLOAT32_R;
        nameToFormat["PF_FLOAT32_RGB"] = PF_FLOAT32_RGB;
        nameToFormat["PF_FLOAT32_RGBA"] = PF_FLOAT32_RGBA;
        nameToFormat["PF_FLOAT16_GR"] = PF_FLOAT16_GR;
        nameToFormat["PF_FLOAT32_GR"] = PF_FLOAT32_GR;
        nameToFormat["PF_DEPTH"] = PF_DEPTH;
        nameToFormat["PF_SHORT_RGBA"] = PF_SHORT_RGBA;
        nameToFormat["PF_SHORT_GR"] = PF_SHORT_GR;
        nameToFormat["PF_SHORT_RGB"] = PF_SHORT_RGB;        

        initialized = true;
    }

    std::map<String, PixelFormat>::iterator format = nameToFormat.find(pixelFormat);
    if (format != nameToFormat.end())
        return format->second;
    
    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid pixel format specified: " << pixelFormat;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParsePixelFormat"
        );
}

TextureType OgreMaxUtilities::ParseTextureType(const String& textureType)
{
    String textureTypeLower = textureType;
    StringUtil::toLowerCase(textureTypeLower);

    if (textureTypeLower == "1d")
        return TEX_TYPE_1D;
    else if (textureTypeLower == "2d")
        return TEX_TYPE_2D;
    else if (textureTypeLower == "3d")
        return TEX_TYPE_3D;
    else if (textureTypeLower == "cubic")
        return TEX_TYPE_CUBE_MAP;

    StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid texture type specified: " << textureType;

    OGRE_EXCEPT
        (
        Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(), 
	    "OgreMaxUtilities::ParseTextureType"
        );
}

void OgreMaxUtilities::LoadClipping(const TiXmlElement* objectElement, Real& nearClip, Real& farClip)
{
    nearClip = GetRealAttribute(objectElement, "near", 1);
    farClip = GetRealAttribute(objectElement, "far", 10000);
}

void OgreMaxUtilities::GetChildText(const TiXmlElement* xmlElement, String& text)
{
    //Get the first element 
    const TiXmlNode* childNode = xmlElement->FirstChild();
    while (childNode != 0)
    {
        if (childNode->Type() == TiXmlNode::TEXT)
        {
            const TiXmlText* textNode = childNode->ToText();
            if (textNode != 0)
            {
                text = textNode->Value();
                break;
            }
        }
        childNode = xmlElement->NextSibling();
    }    
}

String OgreMaxUtilities::GetStringAttribute(const TiXmlElement* xmlElement, const char* name)
{
    const char* value = xmlElement->Attribute(name);
    if (value != 0)
        return value;
    else
        return StringUtil::BLANK;    
}

String OgreMaxUtilities::GetStringAttribute(const TiXmlElement* xmlElement, const char* name, const char* defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : value;
}

Real OgreMaxUtilities::GetRealAttribute(const TiXmlElement* xmlElement, const char* name, Real defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : StringConverter::parseReal(value);    
}

int OgreMaxUtilities::GetIntAttribute(const TiXmlElement* xmlElement, const char* name, int defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : StringConverter::parseInt(value);
}

int OgreMaxUtilities::GetUIntAttribute(const TiXmlElement* xmlElement, const char* name, unsigned int defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : StringConverter::parseUnsignedInt(value);
}

bool OgreMaxUtilities::GetBoolAttribute(const TiXmlElement* xmlElement, const char* name, bool defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : ParseBool(value);
}

Plane OgreMaxUtilities::GetPlaneAttributes(const TiXmlElement* xmlElement, Real defaultX, Real defaultY, Real defaultZ, Real defaultD)
{
    Plane plane;
    plane.normal.x = GetRealAttribute(xmlElement, "planeX", defaultX);
    plane.normal.y = GetRealAttribute(xmlElement, "planeY", defaultY);
    plane.normal.z = GetRealAttribute(xmlElement, "planeZ", defaultZ);
    plane.d = GetRealAttribute(xmlElement, "planeD", defaultD);
    return plane;
}

Vector4 OgreMaxUtilities::GetVector4Attributes(const TiXmlElement* xmlElement)
{
    Vector4 v4;
    v4.x = GetRealAttribute(xmlElement, "x", 0);
    v4.y = GetRealAttribute(xmlElement, "y", 0);
    v4.z = GetRealAttribute(xmlElement, "z", 0);
    v4.w = GetRealAttribute(xmlElement, "w", 0);
    return v4;
}

PixelFormat OgreMaxUtilities::GetPixelFormatAttribute(const TiXmlElement* xmlElement, const char* name, PixelFormat defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : ParsePixelFormat(value);
}

TextureType OgreMaxUtilities::GetTextureTypeAttribute(const TiXmlElement* xmlElement, const char* name, TextureType defaultValue)
{
    String value = GetStringAttribute(xmlElement, name);
    return value.empty() ? defaultValue : ParseTextureType(value);
}

NodeVisibility OgreMaxUtilities::GetNodeVisibilityAttribute(const TiXmlElement* xmlElement, const char* name)
{
    String visibility = OgreMaxUtilities::GetStringAttribute(xmlElement, name);
    return ParseNodeVisibility(visibility);
}

ObjectVisibility OgreMaxUtilities::GetObjectVisibilityAttribute(const TiXmlElement* xmlElement, const char* name)
{
    String visibility = OgreMaxUtilities::GetStringAttribute(xmlElement, name);
    return ParseObjectVisibility(visibility);
}

size_t OgreMaxUtilities::GetElementCount(const TiXmlElement* xmlElement, const String& elementName)
{
    size_t count = 0;

    //Check name
    if (elementName == xmlElement->Value())
        count++;

    //Recurse into children
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(xmlElement, childElement))
        count += GetElementCount(childElement, elementName);

    return count;
}

size_t OgreMaxUtilities::GetChildElementCount(const TiXmlElement* xmlElement, const String& elementName)
{
    size_t count = 0;

    //Check children
    const TiXmlElement* childElement = 0;
    while (childElement = IterateChildElements(xmlElement, childElement))
    {
        if (elementName == childElement->Value())
            count++;
    }

    return count;
}

const TiXmlElement* OgreMaxUtilities::IterateChildElements(const TiXmlElement* xmlElement, const TiXmlElement* childElement)
{
    if (xmlElement != 0)
    {
        if (childElement == 0)
            childElement = xmlElement->FirstChildElement();
        else
            childElement = childElement->NextSiblingElement();
        
        return childElement;
    }

    return 0;
}

bool OgreMaxUtilities::AllDigits(const String& text)
{
    for (size_t i = 0; i < text.length(); i++)
    {
        if (!isdigit(text[i]))
            return false;
    }

    return true;
}

bool OgreMaxUtilities::IsPowerOfTwo(unsigned int value)
{
    return (value & (value - 1)) == 0;
}

unsigned int OgreMaxUtilities::NextLargestPowerOfTwo(unsigned int value)
{
    //From Ice/IceUtils.c in the ODE physics library
    value |= (value >> 1);
    value |= (value >> 2);
    value |= (value >> 4);
    value |= (value >> 8);
    value |= (value >> 16);
    return value + 1;
}

unsigned int OgreMaxUtilities::NextSmallestPowerOfTwo(unsigned int value)
{    
    if (!IsPowerOfTwo(value))
    {
        //Not a power of 2. Round value up to the next power of 2
        value = NextLargestPowerOfTwo(value);
    }
 
    //The value is a power of 2. Shift downward to get the next smallest power of 2
    return value >> 1;        
}

void OgreMaxUtilities::SetNodeVisibility(SceneNode* node, NodeVisibility visibility)
{
    switch (visibility)
    {
        case NODE_VISIBLE: node->setVisible(true, false); break;
        case NODE_HIDDEN: node->setVisible(false, false); break;
        case NODE_TREE_VISIBLE: node->setVisible(true, true); break;
        case NODE_TREE_HIDDEN: node->setVisible(false, true); break;
    }
}

void OgreMaxUtilities::SetCustomParameters(Renderable* renderable, const std::vector<CustomParameter>& customParameters)
{
    for (size_t customParameterIndex = 0; customParameterIndex < customParameters.size(); customParameterIndex++)
        renderable->setCustomParameter(customParameters[customParameterIndex].id, customParameters[customParameterIndex].value);
}

void OgreMaxUtilities::SetCustomParameters(Entity* entity, const std::vector<CustomParameter>& customParameters)
{
    for (unsigned int subentityIndex = 0; subentityIndex < entity->getNumSubEntities(); subentityIndex++)
        SetCustomParameters(entity->getSubEntity(subentityIndex), customParameters);
}

void OgreMaxUtilities::SetIdentityInitialState(SceneNode* node)
{
    //Get the current state 
    Vector3 position = node->getPosition();
    Quaternion orientation = node->getOrientation();
    Vector3 scale = node->getScale();

    //Set the initial state to be at identity
    node->setPosition(Vector3::ZERO);
    node->setOrientation(Quaternion::IDENTITY);
    node->setScale(Vector3::UNIT_SCALE);
    node->setInitialState();
    
    //Set the current state so the node is in the correct position if the node has 
    //animations that are initially disabled
    node->setPosition(position);
    node->setOrientation(orientation);
    node->setScale(scale);
}

void OgreMaxUtilities::CreateMovablePlaneName(String& createdName, const String& baseName)
{
    createdName = baseName;
    createdName += "_movable";
}

Entity* OgreMaxUtilities::CreateEntity
    (
    SceneManager* sceneManager, 
    const String& entityName, 
    const String& meshFile, 
    std::vector<EntityParameters::Subentity>& subentities
    )
{
    Entity* entity = sceneManager->createEntity(entityName, meshFile);

    //Set subentity materials
    size_t subentityCount = std::min(subentities.size(), (size_t)entity->getNumSubEntities());
    for (size_t subentityIndex = 0; subentityIndex < subentityCount; subentityIndex++)
    {
        SubEntity* subentity = entity->getSubEntity((unsigned int)subentityIndex);
        subentity->setMaterialName(subentities[subentityIndex].materialName);
    }

    return entity;
}

bool OgreMaxUtilities::IsSeparator(char c)
{
    return c == '\\' || c == '/';
}

void OgreMaxUtilities::EnsureTrailingPathSeparator(String& path)
{
    if (path.length() > 0)
    {
        char lastChar = path[path.size() - 1];
        if (!IsSeparator(lastChar))
            path += "/";
    }            
}

void OgreMaxUtilities::MakeFullPath(String& path)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    DWORD maxLength = MAX_PATH + 1;
    
    std::vector<char> fullPath;
    fullPath.resize(maxLength);
    
    char* filePath;
    DWORD result = GetFullPathName(path.c_str(), maxLength, &fullPath[0], &filePath);
    if (result > maxLength)
    {     
        fullPath.resize(result);
        result = GetFullPathName(path.c_str(), result, &fullPath[0], &filePath);
    }
    
    path = &fullPath[0];
#endif
}

bool OgreMaxUtilities::IsAbsolutePath(const String& path)
{
    if (path.empty())
        return false;    
    else if (path.length() > 1)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        return path[1] == ':' || (path[0] == '\\' && path[1] == '\\');
#else
        return path[0] == '/';
#endif
    }
    else
        return false;
}

String OgreMaxUtilities::JoinPath(const String& path1, const String& path2)
{
    //If path2 is an absolute path, it supercedes path1
    if (IsAbsolutePath(path2))
        return path2;

    //Path2 is not absolute
    String joined(path1);
    EnsureTrailingPathSeparator(joined);
    
    //Skip leading separators in path2
    size_t charIndex = 0;
    while (charIndex < path2.length() && IsSeparator(path2[charIndex]))
        charIndex++;

    //If not at the end of path2, append it
    if (charIndex < path2.length())
        joined += path2.substr(charIndex, path2.length() - charIndex);

    //Remove relative components
    MakeFullPath(joined);
    
    return joined;
}

bool OgreMaxUtilities::IsFileReadable(const String& path)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    return GetFileAttributes(path.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    return access(path.c_str(), R_OK) == 0;
#endif
}

void OgreMaxUtilities::RemoveFile(const String& path)
{
    remove(path.c_str());
}

bool OgreMaxUtilities::SetDefaultLighting(SceneManager* sceneManager, UpAxis upAxis)
{
    static const String defaultLightName("__defaultDirectionalLight");

    bool defaultLightingNeeded = !sceneManager->hasLight(defaultLightName);
    if (defaultLightingNeeded)
    {
        //Create a directional light
        Light* light = sceneManager->createLight(defaultLightName);
        light->setType(Light::LT_DIRECTIONAL);
        light->setDiffuseColour(ColourValue((Real).9, (Real).9, (Real).9));    
        light->setSpecularColour(ColourValue((Real).2, (Real).2, (Real).2));            

        //When the viewer faces down the forward axis, the light is angled to the lower right of the view
        Vector3 upDirection = (upAxis == UP_AXIS_Y) ? Vector3::UNIT_Y : Vector3::UNIT_Z;
        Vector3 position = upDirection + Vector3::NEGATIVE_UNIT_X;
        light->setPosition(position);
        light->setDirection(-position);

        //Set the ambient light if necessary
        if (sceneManager->getAmbientLight() == ColourValue::Black)
            sceneManager->setAmbientLight(ColourValue((Real).2, (Real).2, (Real).2));
    }

    return defaultLightingNeeded;
}

bool OgreMaxUtilities::IsInternalResourceGroup(const String& resourceGroupName)
{
    return 
        resourceGroupName == ResourceGroupManager::BOOTSTRAP_RESOURCE_GROUP_NAME ||
        resourceGroupName == ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME ||
        resourceGroupName == ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME;
}

void OgreMaxUtilities::ResetResourceGroups(const String& defaultResourceGroup)
{
    StringVector resourceGroups = ResourceGroupManager::getSingleton().getResourceGroups();
    for (size_t index = 0; index < resourceGroups.size(); index++)
    {
        if (!IsInternalResourceGroup(resourceGroups[index]))
            ResourceGroupManager::getSingleton().destroyResourceGroup(resourceGroups[index]);        
    }

    //Create the default resource group if necessary
    if (!defaultResourceGroup.empty())
        ResourceGroupManager::getSingleton().createResourceGroup(defaultResourceGroup);    
}
