__kernel void gradient(__write_only  image2d_t glTexture) 
{
	float x = get_global_id(0);
	float y = get_global_id(1);
	
	int size_x = get_global_size(0);
	int size_y = get_global_size(1);
	
	int2 coord = (int2)(x,y); 
	
	write_imagef(glTexture, coord, (float4)(1-(x/size_x),1-(y/size_y),0,1));
}
