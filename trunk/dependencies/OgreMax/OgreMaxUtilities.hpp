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

#ifndef OgreMax_OgreMaxUtilities_INCLUDED
#define OgreMax_OgreMaxUtilities_INCLUDED


//Includes---------------------------------------------------------------------
#include <OgreSceneManager.h>
#include "OgreMaxPlatform.hpp"
#include "ProgressCalculator.hpp"
#include "OgreMaxTypes.hpp"


//Classes----------------------------------------------------------------------
namespace OgreMax
{   
    /** A collection of parsing, loading and other miscellaneous functions */
    class _OgreMaxExport OgreMaxUtilities
    {
    public:
        static void LoadXmlDocument(const Ogre::String& fileName, TiXmlDocument& document, const Ogre::String& resourceGroupName);

        static void LoadXYZ(const TiXmlElement* objectElement, Ogre::Vector3& xyz);
        static Ogre::Vector3 LoadXYZ(const TiXmlElement* objectElement);
        static Ogre::ColourValue LoadColor(const TiXmlElement* objectElement);
        static Ogre::Plane LoadPlane(const TiXmlElement* objectElement);
        static Ogre::Quaternion LoadRotation(const TiXmlElement* objectElement);
        static Ogre::FloatRect LoadFloatRectangle(const TiXmlElement* objectElement);
        static void LoadBufferUsage(const TiXmlElement* objectElement, Ogre::HardwareBuffer::Usage& usage, bool& shadowed);
        static void LoadBoundingVolume(const TiXmlElement* objectElement, Types::BoundingVolume& volume);
        static void LoadBoundingVolumeFaces(const TiXmlElement* objectElement, int faceCount, std::vector<Types::BoundingVolume::Face>& faces);

        static bool ParseSceneManager(const Ogre::String& sceneManager, Ogre::SceneType& sceneType);
        static bool ParseBool(const Ogre::String& value);
        static Ogre::Light::LightTypes ParseLightType(const Ogre::String& type);
        static Ogre::ProjectionType ParseProjectionType(const Ogre::String& type);
        static Ogre::BillboardType ParseBillboardType(const Ogre::String& type);
        static Ogre::BillboardOrigin ParseBillboardOrigin(const Ogre::String& origin);
        static Ogre::BillboardRotationType ParseBillboardRotationType(const Ogre::String& rotationType);
        static Ogre::FogMode ParseFogMode(const Ogre::String& mode);        
        static Ogre::HardwareBuffer::Usage ParseHardwareBufferUsage(const Ogre::String& usage);
        static Ogre::Node::TransformSpace ParseTransformSpace(const Ogre::String& space);
        static Types::BoundingVolume::Type ParseBoundingVolumeType(const Ogre::String& type);

        static void LoadCustomParameters(const TiXmlElement* objectElement, std::vector<Types::CustomParameter>& customParameters);
        static void LoadUserDataReference(const TiXmlElement* objectElement, Ogre::String& userDataReference);
        
        static void LoadSubentities(const TiXmlElement* objectElement, std::vector<Types::EntityParameters::Subentity>& subentities);
        
        static void LoadNoteTracks(const TiXmlElement* objectElement, std::vector<Types::NoteTrack>& noteTracks);
        static void LoadNoteTrack(const TiXmlElement* objectElement, Types::NoteTrack& noteTrack);
        static void LoadNote(const TiXmlElement* objectElement, Types::Note& note);

        static Ogre::ShadowTechnique ParseShadowTechnique(const Ogre::String& technique);
        static Types::UpAxis ParseUpAxis(const Ogre::String& upAxis);
        static Ogre::Animation::InterpolationMode ParseAnimationInterpolationMode(const Ogre::String& mode);
        static Ogre::Animation::RotationInterpolationMode ParseAnimationRotationInterpolationMode(const Ogre::String& mode);
        static Types::NodeVisibility ParseNodeVisibility(const Ogre::String& visibility);
        static Types::ObjectVisibility ParseObjectVisibility(const Ogre::String& visibility);
        static Ogre::uint8 ParseRenderQueue(const Ogre::String& renderQueue);
        static Ogre::PixelFormat ParsePixelFormat(const Ogre::String& pixelFormat);
        static Ogre::TextureType ParseTextureType(const Ogre::String& textureType);

        static void LoadClipping(const TiXmlElement* objectElement, Ogre::Real& nearClip, Ogre::Real& farClip);

        static void GetChildText(const TiXmlElement* xmlElement, Ogre::String& text);
        static Ogre::String GetStringAttribute(const TiXmlElement* xmlElement, const char* name);
        static Ogre::String GetStringAttribute(const TiXmlElement* xmlElement, const char* name, const char* defaultValue);
        static Ogre::Real GetRealAttribute(const TiXmlElement* xmlElement, const char* name, Ogre::Real defaultValue);
        static int GetIntAttribute(const TiXmlElement* xmlElement, const char* name, int defaultValue);
        static int GetUIntAttribute(const TiXmlElement* xmlElement, const char* name, unsigned int defaultValue);
        static bool GetBoolAttribute(const TiXmlElement* xmlElement, const char* name, bool defaultValue);
        static Ogre::Plane GetPlaneAttributes(const TiXmlElement* xmlElement, Ogre::Real defaultX, Ogre::Real defaultY, Ogre::Real defaultZ, Ogre::Real defaultD);
        static Ogre::Vector4 GetVector4Attributes(const TiXmlElement* xmlElement);
        static Ogre::PixelFormat GetPixelFormatAttribute(const TiXmlElement* xmlElement, const char* name, Ogre::PixelFormat defaultValue);
        static Ogre::TextureType GetTextureTypeAttribute(const TiXmlElement* xmlElement, const char* name, Ogre::TextureType defaultValue);
        static Types::NodeVisibility GetNodeVisibilityAttribute(const TiXmlElement* xmlElement, const char* name);
        static Types::ObjectVisibility GetObjectVisibilityAttribute(const TiXmlElement* xmlElement, const char* name);

        /** 
         * Recursively searches the XML element tree, counting elements with the specified name
         * @param element [in] - The start element. This element is included in the count
         * @param elementName [in] - The element name to search for
         * @return Returns the number of elements with the specified name
         */
        static size_t GetElementCount(const TiXmlElement* xmlElement, const Ogre::String& elementName);

        static size_t GetChildElementCount(const TiXmlElement* xmlElement, const Ogre::String& elementName);

        /**
         * Iterates over the specified element's child elements
         * @param element [in] - The element whose child elements are iterated. This may be null, 
         * in which case no iteration occurs
         * @param childElement [in] - The start child element. This will be null at the start of an iteration
         * @return The next child element will be returned, null if there are no child elements
         */
        static const TiXmlElement* IterateChildElements(const TiXmlElement* xmlElement, const TiXmlElement* childElement);

        static bool AllDigits(const Ogre::String& text);
        static bool IsPowerOfTwo(unsigned int value);
        static unsigned int NextLargestPowerOfTwo(unsigned int value);
        static unsigned int NextSmallestPowerOfTwo(unsigned int value);
        
        static void SetNodeVisibility(Ogre::SceneNode* node, Types::NodeVisibility visibility);
        template <class T> 
        static void SetObjectVisibility(T object, Types::ObjectVisibility visibility)
        {
            switch (visibility)
            {
                case Types::OBJECT_VISIBLE: object->setVisible(true); break;
                case Types::OBJECT_HIDDEN: object->setVisible(false); break;
            }
        }
        static void SetCustomParameters(Ogre::Renderable* renderable, const std::vector<Types::CustomParameter>& customParameters);
        static void SetCustomParameters(Ogre::Entity* entity, const std::vector<Types::CustomParameter>& customParameters);

        /** 
         * Sets the node's initial state (position/rotation/scale) to identity while
         * preserving the current state. This is used when setting up node animations
         */
        static void SetIdentityInitialState(Ogre::SceneNode* node);

        static void CreateMovablePlaneName(Ogre::String& createdName, const Ogre::String& baseName);

        static Ogre::Entity* CreateEntity
            (
            Ogre::SceneManager* sceneManager, 
            const Ogre::String& entityName, 
            const Ogre::String& meshFile, 
            std::vector<Types::EntityParameters::Subentity>& subentities
            );

        /** Determines if the specified character is a path separator */
        static bool IsSeparator(char c);

        /** 
         * Ensures that the specified path has a trailing path separator character
         * @param path [in/out] - The path
         */
        static void EnsureTrailingPathSeparator(Ogre::String& path);

        /**
         * Removes all relative fragments, yielding a full file path. Used only under Windows
         * @param path [in/out] - The path
         */
        static void MakeFullPath(Ogre::String& path);

        /** Determines if the specified path is an absolute path */
        static bool IsAbsolutePath(const Ogre::String& path);

        /** Joins two files paths together, returning the result */
        static Ogre::String JoinPath(const Ogre::String& path1, const Ogre::String& path2);

        /** Determines if the specifiled file is readable */
        static bool IsFileReadable(const Ogre::String& path);

        /** Deletes the specified file */
        static void RemoveFile(const Ogre::String& path);

        /**
         * Sets the default lighting in the scene manager if it hasn't already been set
         * @param sceneManager [in] - The scene manager for which the default lighting is set
         * @param upAxis [in] - The scene up axis
         * @return Returns true if the default lighting was successfully set, false if it had already been set
         */
        static bool SetDefaultLighting(Ogre::SceneManager* sceneManager, Types::UpAxis upAxis);

        /** Determines if the specified resource group is an Ogre 'internal' resource group */
        static bool IsInternalResourceGroup(const Ogre::String& resourceGroupName);

        /** 
         * Destroy all resource groups except the ones internally needed or created by Ogre 
         * and optionally creates a new default resource group
         * @param defaultResourceGroup [in] - Optional resource group to create after destroying non-internal
         * resource groups. If empty, nothing is done
         */
        static void ResetResourceGroups(const Ogre::String& defaultResourceGroup = Ogre::StringUtil::BLANK);
    };

}

#endif
