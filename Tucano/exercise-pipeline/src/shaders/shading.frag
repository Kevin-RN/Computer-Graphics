#version 430

// output fragment color
out vec4 out_Color;
in vec4 color;

void main(void)
{        

	//green color
	//out_Color = vec4(0.3, 0.7, 0.3, 1.0);
	
	//orange color (ex 1 pipelines)
	//out_Color = vec4(1.0, 0.5, 0.0, 1.0);
	
	//the linearly interpolated color (ex 4 pipelines)
	out_Color = color;

}
