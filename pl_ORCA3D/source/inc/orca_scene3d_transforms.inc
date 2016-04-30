{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//====================  TD3Transform =====================================

constructor TD3Transform.Create;
begin
  inherited ;
  FMatrix:=IdentityMatrix;
  FPosition:=TD3Position.Create(d3Point(0,0,0));
  FPosition.OnChange:=MatrixChanged;
  FScale:=TD3Position.Create(d3Point(1,1,1));
  FScale.OnChange:=MatrixChanged;
  FSkew:=TD3Position.Create(d3Point(0,0,0));
  FSkew.OnChange:=MatrixChanged;
  FRotateCenter:=TD3Position.Create(d3Point(0.5,0.5,0.5));
  FRotateCenter.OnChange:=MatrixChanged;
end;

destructor TD3Transform.Destroy;
begin
  FRotateCenter.Free;
  FScale.Free;
  FSkew.Free;
  FPosition.Free;
  inherited ;
end;

procedure TD3Transform.Assign(Source:TPersistent);
begin
  if Source is TD3Transform then
  begin
    FPosition.FX:=TD3Transform(Source).Position.FX;
    FPosition.FY:=TD3Transform(Source).Position.FY;
    FPosition.FZ:=TD3Transform(Source).Position.FZ;
    FScale.FX:=TD3Transform(Source).Scale.FX;
    FScale.FY:=TD3Transform(Source).Scale.FY;
    FScale.FZ:=TD3Transform(Source).Scale.FZ;
    FSkew.FX:=TD3Transform(Source).Skew.FX;
    FSkew.FY:=TD3Transform(Source).Skew.FY;
    FSkew.FZ:=TD3Transform(Source).Skew.FZ;
    FRotateCenter.FX:=TD3Transform(Source).RotateCenter.FX;
    FRotateCenter.FY:=TD3Transform(Source).RotateCenter.FY;
    FRotateCenter.FZ:=TD3Transform(Source).RotateCenter.FZ;
    MatrixChanged(Self);
  end
  else
    inherited
end;

procedure TD3Transform.MatrixChanged(Sender:TObject);
var
  RotMatrix:TD3Matrix;
  M1,M2:TD3Matrix;
begin
  FMatrix:=IdentityMatrix;
  FMatrix.m41:=FPosition.X;
  FMatrix.m42:=FPosition.Y;
  FMatrix.m43:=FPosition.Z;
  FMatrix.m14:=FSkew.X;
  FMatrix.m24:=FSkew.Y;
  FMatrix.m34:=FSkew.Z;
  FMatrix.m11:=FScale.X;
  FMatrix.m22:=FScale.Y;
  FMatrix.m33:=FScale.Z;
  if FRotateAngle <> 0 then
  begin
  
{    M1:=IdentityMatrix;
    M1.m31:=-FRotateCenter.X * FWidth;
    M1.m32:=-FRotateCenter.Y * FHeight;
    M2:=IdentityMatrix;
    M2.m31:=FRotateCenter.X * FWidth;
    M2.m32:=FRotateCenter.Y * FHeight;
    RotMatrix:=d2MatrixMultiply(M1,d2MatrixMultiply(d2CreateRotationMatrix(d2DegToRad(FRotateAngle)),M2));
    FMatrix:=d2MatrixMultiply(RotMatrix,FMatrix); }
//    FMatrix:=d3MatrixMultiply(d3CreateRotationMatrix(d2DegToRad(FRotateAngle)),FMatrix);
  end;
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;          
         
           
