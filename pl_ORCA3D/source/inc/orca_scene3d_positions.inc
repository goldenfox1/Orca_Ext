{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//================== TD3Position ===================================

constructor TD3Position.Create(const ADefaultValue:TD3Point);
begin
  inherited Create;
  FDefaultValue:=ADefaultValue;
  FX:=FDefaultValue.X;
  FY:=FDefaultValue.Y;
  FZ:=FDefaultValue.Z;
  FSave:=d3Vector(Fx,Fy,Fz);
end;

procedure TD3Position.Assign(Source:TPersistent);
begin
  if Source is TD3Position then
  begin
    Point:=TD3Position(Source).Point;
    FSave:=Vector;
  end
  else
    inherited
end;

procedure TD3Position.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('Point',ReadPoint,WritePoint,(FX <> DefaultValue.X) or (FY <> DefaultValue.Y) or (FZ <> DefaultValue.Z));
end;

procedure TD3Position.ReadPoint(Reader:TReader);
begin
  Point:=d3StringToPoint(Reader.ReadString);
end;

procedure TD3Position.WritePoint(Writer:TWriter);
begin
  Writer.WriteString(d3PointToString(Point));
end;

function TD3Position.GetVector:TD3Vector;
begin
  Result:=d3Vector(Fx,Fy,Fz);
end;

procedure TD3Position.SetVector(const Value:TD3Vector);
begin
  SetPoint(d3Point(Value));
end;

function TD3Position.GetPoint:TD3Point;
begin
  Result:=d3Point(Fx,Fy,Fz);
end;

procedure TD3Position.SetVectorNoChange(const P:TD3Vector);
begin
  FSave:=d3Vector(Fx,Fy,Fz);
  Fx:=P.X;
  Fy:=P.Y;
  Fz:=P.Z;
end;

procedure TD3Position.SetPointNoChange(const P:TD3Point);
begin
  FSave:=d3Vector(Fx,Fy,Fz);
  Fx:=P.X;
  Fy:=P.Y;
  Fz:=P.Z;
end;

procedure TD3Position.SetPoint(const Value:TD3Point);
begin
  FSave:=d3Vector(Fx,Fy,Fz);
  Fx:=Value.X;
  Fy:=Value.Y;
  Fz:=Value.Z;
  if Assigned(OnChange) then
    OnChange(Self);
end;

procedure TD3Position.SetX(const Value:single);
begin
  if FX <> Value then
  begin
    FSave.X:=FX;
    FX:=Value;
    if Assigned(OnChangeX) then
      OnChangeX(Self)
    else
      if Assigned(OnChange) then
        OnChange(Self);
  end;
end;

procedure TD3Position.SetY(const Value:single);
begin
  if FY <> Value then
  begin
    FSave.Y:=FY;
    FY:=Value;
    if Assigned(OnChangeY) then
      OnChangeY(Self)
    else
      if Assigned(OnChange) then
        OnChange(Self);
  end;
end;

procedure TD3Position.SetZ(const Value:single);
begin
  if FZ <> Value then
  begin
    FSave.Z:=FZ;
    FZ:=Value;
    if Assigned(OnChangeZ) then
      OnChangeZ(Self)
    else
      if Assigned(OnChange) then
        OnChange(Self);
  end;
end;

function TD3Position.Empty:boolean;
begin
  Result:=(FX=0) and (FY=0) and (FZ=0);
end;

      
