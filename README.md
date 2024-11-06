# img2table
converts images to a lua table with the structure  
made for RetroGadgets so there's a `toPixelData` helper function too  

simply drag and drop an image file onto the exe and it will create a .lua file

# RetroGadgets usage
*this uses RenderBuffers, not SpriteSheets, so RG might crash if your file is too large*  

run the exe with your image, and add the lua file to your gadget as usual
```lua
local img = require( "img.lua" )

local vc = gdt.VideoChip0
local rb = vc.RenderBuffers[1]
local pd = img:toPixelData() -- create pixel data object

-- resize buffer
vc:SetRenderBufferSize(1, img.width, img.height)

-- blit data onto buffer
vc:RenderOnBuffer(1)
vc:BlitPixelData(vec2(0, 0), pd)
vc:RenderOnScreen()

-- render
vc:DrawRenderBuffer(vec2(0, 0), rb, img.width, img.height)
```

# building
install [xmake](https://xmake.io/#/)  

start `build.bat` to generate a visual studio solution  
or run `xmake` in root directory to build directly
