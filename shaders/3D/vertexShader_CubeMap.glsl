#version 330 core
layout (location = 0) in vec3 aPos;   // Attributo Posizione

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoords;

void main(){	
     TexCoords = aPos;//vec3(aPos.x,-aPos.yz) ;  // in caso le coordinate vengono flipped
     mat4 static_View = mat4(mat3(view));  //MAtrice di vista a cui si toglie il contributo della traslazione
     vec4 pos = projection * static_View * vec4(aPos, 1.0);

     gl_Position = pos.xyww;
}


