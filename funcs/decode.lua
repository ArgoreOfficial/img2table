
-- convert uint32_t to 4 component color
function uint32_col(_v:number) : color
	local r = bit32.band(_v, 0x000000FF) -- weird endianess stuff
	local g = bit32.band(_v, 0x0000FF00) 
	local b = bit32.band(_v, 0x00FF0000) 
	local a = bit32.band(_v, 0xFF000000) 
	
	a = bit32.rshift(a, 24)
	b = bit32.rshift(b, 16)
	g = bit32.rshift(g, 8)
	
	return ColorRGBA(r,g,b,a)
end

function get_px(_px:number) : vec2
	return vec2(
		(_px % img.width) + 1,
		(math.floor(_px / img.width)) + 1
	)
end

function img:toPixelData() : PixelData
	local pd = PixelData.new(img.width, img.height, color.clear)
	
	local px = 0
	local i = 1
	local d = img.data

	repeat
		if typeof( d[i] ) == "table" then
			local t = d[i]
			local v = t[1]
			for n = 1, t[2], img.vals_per_px do
				local p = get_px(px)
				pd:SetPixel(p.X, p.Y, ColorRGBA(v,v,v,v))
				px += 1
			end
			i += 1
		else
			local p = get_px(px)
			local c:color
			
			if img.vals_per_px == 4 then -- 4 component colors {r,g,b,a}
				c = ColorRGBA(d[i], d[i+1], d[i+2], d[i+3])
			elseif img.vals_per_px == 1 then
				c = uint32_col(d[i]) -- compact uint32_t
			end
			
			pd:SetPixel(p.X, p.Y, c)
			
			px += 1
			i  += img.vals_per_px
		end
	until(i > #img.data)
	
	return pd
end