
--------------------------------
-- @module Sprite3D
-- @extend Node,BlendProtocol
-- @parent_module cc

--------------------------------
-- 
-- @function [parent=#Sprite3D] isForceDepthWrite 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Sprite3D] setCullFaceEnabled 
-- @param self
-- @param #bool enable
        
--------------------------------
-- @overload self, cc.Texture2D         
-- @overload self, string         
-- @function [parent=#Sprite3D] setTexture
-- @param self
-- @param #string texFile

--------------------------------
-- 
-- @function [parent=#Sprite3D] getLightMask 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- remove all attach nodes
-- @function [parent=#Sprite3D] removeAllAttachNode 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#Sprite3D] setBlendFunc 
-- @param self
-- @param #cc.BlendFunc blendFunc
        
--------------------------------
-- get mesh
-- @function [parent=#Sprite3D] getMesh 
-- @param self
-- @return Mesh#Mesh ret (return value: cc.Mesh)
        
--------------------------------
-- 
-- @function [parent=#Sprite3D] setCullFace 
-- @param self
-- @param #unsigned int cullFace
        
--------------------------------
--  light mask getter & setter, light works only when _lightmask & light's flag is true, default value of _lightmask is 0xffff 
-- @function [parent=#Sprite3D] setLightMask 
-- @param self
-- @param #unsigned int mask
        
--------------------------------
-- 
-- @function [parent=#Sprite3D] getBlendFunc 
-- @param self
-- @return BlendFunc#BlendFunc ret (return value: cc.BlendFunc)
        
--------------------------------
--  get mesh count 
-- @function [parent=#Sprite3D] getMeshCount 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- remove attach node
-- @function [parent=#Sprite3D] removeAttachNode 
-- @param self
-- @param #string boneName
        
--------------------------------
-- 
-- @function [parent=#Sprite3D] getSkeleton 
-- @param self
-- @return Skeleton3D#Skeleton3D ret (return value: cc.Skeleton3D)
        
--------------------------------
-- get Mesh by index
-- @function [parent=#Sprite3D] getMeshByIndex 
-- @param self
-- @param #int index
-- @return Mesh#Mesh ret (return value: cc.Mesh)
        
--------------------------------
-- Force to write to depth buffer, this is useful if you want to achieve effects like fading.
-- @function [parent=#Sprite3D] setForceDepthWrite 
-- @param self
-- @param #bool value
        
--------------------------------
-- get Mesh by Name, it returns the first one if there are more than one mesh with the same name 
-- @function [parent=#Sprite3D] getMeshByName 
-- @param self
-- @param #string name
-- @return Mesh#Mesh ret (return value: cc.Mesh)
        
--------------------------------
-- get AttachNode by bone name, return nullptr if not exist
-- @function [parent=#Sprite3D] getAttachNode 
-- @param self
-- @param #string boneName
-- @return AttachNode#AttachNode ret (return value: cc.AttachNode)
        
--------------------------------
-- @overload self, string         
-- @overload self         
-- @overload self, string, string         
-- @function [parent=#Sprite3D] create
-- @param self
-- @param #string modelPath
-- @param #string texturePath
-- @return Sprite3D#Sprite3D ret (return value: cc.Sprite3D)

--------------------------------
-- Returns 2d bounding-box<br>
-- Note: the bouding-box is just get from the AABB which as Z=0, so that is not very accurate.
-- @function [parent=#Sprite3D] getBoundingBox 
-- @param self
-- @return rect_table#rect_table ret (return value: rect_table)
        
--------------------------------
--  set GLProgramState, you should bind attributes by yourself 
-- @function [parent=#Sprite3D] setGLProgramState 
-- @param self
-- @param #cc.GLProgramState glProgramState
        
--------------------------------
-- Executes an action, and returns the action that is executed. For Sprite3D special logic are needed to take care of Fading.<br>
-- This node becomes the action's target. Refer to Action::getTarget()<br>
-- warning Actions don't retain their target.<br>
-- return An Action pointer
-- @function [parent=#Sprite3D] runAction 
-- @param self
-- @param #cc.Action action
-- @return Action#Action ret (return value: cc.Action)
        
--------------------------------
--  just rember bind attributes 
-- @function [parent=#Sprite3D] setGLProgram 
-- @param self
-- @param #cc.GLProgram glprogram
        
return nil
