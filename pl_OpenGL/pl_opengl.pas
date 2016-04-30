{ This file was automatically created by Typhon IDE.
  Do not edit!
  This source is only used to compile and install the package.
 }

unit pl_opengl;

interface

uses
  AllOpenGLRegister, ctOpenGLES1x, ctOpenGLES1xCanvas, ctOpenGLES2x, 
  ctOpenGLES2xCanvas, dglOpenGL, dglShader, OpenGL_Particles, OpenGL_Textures, 
  OpenGLCanvas, OpenGLPanel, LazarusPackageIntf;

implementation

procedure Register;
begin
  RegisterUnit('AllOpenGLRegister', @AllOpenGLRegister.Register);
end;

initialization
  RegisterPackage('pl_opengl', @Register);
end.
