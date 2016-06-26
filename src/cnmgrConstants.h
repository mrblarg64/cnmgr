#ifndef CNMGR_CONSTANTS_H

#define CNMGR_CONSTANTS_H
char cnmgrBuiltInFragShader[]="#version 330\n out vec3 color;\n\nvoid main(void)\n{\ncolor=vec3(1.0,1.0,1.0);\n}\n";
char cnmgrBuiltInVertexShader[]="#version 330\n in vec3 vertex;\n uniform mat4 uberMatrix;\n void main(void)\n{\n gl_Position=uberMatrix*vec4(vertex,1.0);\n}\n";
#endif