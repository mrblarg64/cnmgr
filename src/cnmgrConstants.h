//char cnmgrDefaultFragShader[]="#version 330\n\nout vec4 color\n\nvoid main(void)\n{\ncolor=vec4(0.0,1.0,0.0,0.0);\n}\n";
//char cnmgrDefaultVertexShader[]="#version 330\n\nuniform mat4 uberMatrix;\n\nvoid main(void)\n{\n	gl_Position=uberMatrix;\n}";