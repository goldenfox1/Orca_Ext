{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//====================  TD3MeshData ====================================================

constructor TD3MeshData.Create;
begin
  inherited ;
end;

destructor TD3MeshData.Destroy;
begin
  if Assigned(FOnDestroyHandle) then
    FOnDestroyHandle(Self);
  Handle:=0;
  inherited;
end;

procedure TD3MeshData.Assign(Source:TPersistent);
begin
  if Source is TD3MeshData then
  begin
    SetLength(MeshVertices,Length(TD3MeshData(Source).MeshVertices));
    system.Move(TD3MeshData(Source).MeshVertices[0],MeshVertices[0],SizeOf(MeshVertices[0]) * Length(MeshVertices));
    SetLength(MeshIndices,Length(TD3MeshData(Source).MeshIndices));
    system.Move(TD3MeshData(Source).MeshIndices[0],MeshIndices[0],SizeOf(MeshIndices[0]) * Length(MeshIndices));
    FNeedUpdate:=true;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end else
    inherited
end;

procedure TD3MeshData.CalcNormals;
var
  i,j:integer;
  vn:TD3Vector;
begin
  for i:=0 to Length(MeshIndices) - 1 do
    with MeshVertices[MeshIndices[i]] do
    begin
      nx:=0;
      ny:=0;
      nz:=0;
    end;

  for i:=0 to ((Length(MeshIndices) - 3) div 3) do
  begin
    j:=i * 3;
    if false then
      vn:=d3CalcPlaneNormal(d3Vector(MeshVertices[MeshIndices[j + 0]].x,MeshVertices[MeshIndices[j + 0]].y,MeshVertices[MeshIndices[j + 0]].z),
        d3Vector(MeshVertices[MeshIndices[j + 1]].x,MeshVertices[MeshIndices[j + 1]].y,MeshVertices[MeshIndices[j + 1]].z),
        d3Vector(MeshVertices[MeshIndices[j + 2]].x,MeshVertices[MeshIndices[j + 2]].y,MeshVertices[MeshIndices[j + 2]].z))
    else
      vn:=d3CalcPlaneNormal(d3Vector(MeshVertices[MeshIndices[j + 2]].x,MeshVertices[MeshIndices[j + 2]].y,MeshVertices[MeshIndices[j + 2]].z),
        d3Vector(MeshVertices[MeshIndices[j + 1]].x,MeshVertices[MeshIndices[j + 1]].y,MeshVertices[MeshIndices[j + 1]].z),
        d3Vector(MeshVertices[MeshIndices[j + 0]].x,MeshVertices[MeshIndices[j + 0]].y,MeshVertices[MeshIndices[j + 0]].z));

    with MeshVertices[MeshIndices[j + 0]] do
    begin
      nx:=nx + vn.x;
      ny:=ny + vn.y;
      nz:=nz + vn.z;
    end;
    with MeshVertices[MeshIndices[j + 1]] do
    begin
      nx:=nx + vn.x;
      ny:=ny + vn.y;
      nz:=nz + vn.z;
    end;
    with MeshVertices[MeshIndices[j + 2]] do
    begin
      nx:=nx + vn.x;
      ny:=ny + vn.y;
      nz:=nz + vn.z;
    end;
  end;
end;

procedure TD3MeshData.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('Mesh',ReadMesh,WriteMesh,Length(MeshVertices) > 0);
end;

procedure TD3MeshData.ReadMesh(Stream:TStream);
var
  L:cardinal;
begin
  Stream.Read(L,SizeOf(L));
  SetLength(MeshVertices,L);
  Stream.Read(MeshVertices[0],L * SizeOf(MeshVertices[0]));
  Stream.Read(L,SizeOf(L));
  SetLength(MeshIndices,L);
  Stream.Read(MeshIndices[0],L * SizeOf(MeshIndices[0]));
end;

procedure TD3MeshData.WriteMesh(Stream:TStream);
var
  L:cardinal;
begin
  L:=Length(MeshVertices);
  Stream.Write(L,SizeOf(L));
  Stream.Write(MeshVertices[0],L * SizeOf(MeshVertices[0]));
  L:=Length(MeshIndices);
  Stream.Write(L,SizeOf(L));
  Stream.Write(MeshIndices[0],L * SizeOf(MeshIndices[0]));
end;

function TD3MeshData.GetNormals:AnsiString;
var
  s:ansistring;
  i,pos:integer;
begin
  SetLength(Result,High(MeshVertices) * (3 * 12 + 4));
  pos:=0;
  for i:=0 to High(MeshVertices) do
  begin
    s:=d3FloatToStr(MeshVertices[i].nx) + ' ' + d3FloatToStr(MeshVertices[i].ny) + ' ' + d3FloatToStr(MeshVertices[i].nz) + '  ';
    system.Move(PChar(s)^,PByteArray(Result)[pos],length(s));
    pos:=pos + length(s);
  end;
  SetLength(Result,Pos);
end;

function TD3MeshData.GetPoints:AnsiString;
var
  s:ansistring;
  i,pos:integer;
begin
  SetLength(Result,High(MeshVertices) * (3 * 12 + 4));
  pos:=0;
  for i:=0 to High(MeshVertices) do
  begin
    s:=d3FloatToStr(MeshVertices[i].x) + ' ' + d3FloatToStr(MeshVertices[i].y) + ' ' + d3FloatToStr(MeshVertices[i].z) + '  ';
    system.Move(PChar(s)^,PByteArray(Result)[pos],length(s));
    pos:=pos + length(s);
  end;
  SetLength(Result,pos);
end;

function TD3MeshData.GetTexCoordinates:AnsiString;
var
  s:ansistring;
  i,pos:integer;
begin
  SetLength(Result,High(MeshVertices) * (2 * 12 + 4));
  pos:=0;
  for i:=0 to High(MeshVertices) do
  begin
    s:=d3FloatToStr(MeshVertices[i].tu) + ' ' + d3FloatToStr(MeshVertices[i].tv) + '  ';
    system.Move(PChar(s)^,PByteArray(Result)[pos],length(s));
    pos:=pos + length(s);
  end;
  SetLength(Result,pos);
end;

function TD3MeshData.GetTriangleIndices:AnsiString;
var
  s:ansistring;
  i,pos:integer;
begin
  SetLength(Result,High(MeshIndices) * (7));
  pos:=0;
  for i:=0 to High(MeshIndices) do
  begin
    s:=d3FloatToStr(MeshIndices[i]) + ' ';
    if (i + 1) mod 3=0 then
      s:=s + ' ';
    system.Move(PChar(s)^,PByteArray(Result)[pos],length(s));
    pos:=pos + length(s);
  end;
  SetLength(Result,pos);
end;

procedure TD3MeshData.SetNormals(const Value:AnsiString);
var
  Pos,Count:integer;
begin
  Pos:=1; Count:=0;
  while Pos < Length(Value) do
  begin
    try
      Count:=Count + 1;
      if Length(MeshVertices) < Count then
        SetLength(MeshVertices,Count);
      MeshVertices[Count - 1].nx:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
      MeshVertices[Count - 1].ny:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
      MeshVertices[Count - 1].nz:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
    except
    end;
  end;
  if Length(MeshVertices) > Count then
    SetLength(MeshVertices,Count);
  if Assigned(FOnChanged) then
    FOnChanged(Self);
  FNeedUpdate:=true;
end;

procedure TD3MeshData.SetPoints(const Value:AnsiString);
var
  Pos,Count:integer;
begin
  Count:=0; Pos:=1;
  while Pos < Length(Value) do
  begin
    try
      Count:=Count + 1;
      if Length(MeshVertices) < Count then
        SetLength(MeshVertices,Count);
      MeshVertices[Count - 1].x:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
      MeshVertices[Count - 1].y:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
      MeshVertices[Count - 1].z:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
    except
    end;
  end;
  if Length(MeshVertices) > Count then
    SetLength(MeshVertices,Count);
  if Assigned(FOnChanged) then
    FOnChanged(Self);
  FNeedUpdate:=true;
end;

procedure TD3MeshData.SetTexCoordinates(const Value:AnsiString);
var
  Pos,Count:integer;
begin
  Count:=0; Pos:=1;
  while Pos < Length(Value) do
  begin
    try
      Count:=Count + 1;
      if Length(MeshVertices) < Count then
        SetLength(MeshVertices,Count);
      MeshVertices[Count - 1].tu:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
      MeshVertices[Count - 1].tv:=d3StrToFloat(d3GetToken(Pos,Value,' ,'));
    except
    end;
  end;
  if Length(MeshVertices) > Count then
    SetLength(MeshVertices,Count);
  if Assigned(FOnChanged) then
    FOnChanged(Self);
  FNeedUpdate:=true;
end;

procedure TD3MeshData.SetTriangleIndices(const Value:AnsiString);
var
  Pos,Count:integer;
begin
  Count:=0; Pos:=1;
  while Pos < Length(Value) do
  begin
    try
      Count:=Count + 1;
      if Length(MeshIndices) < Count then
        SetLength(MeshIndices,Count);
      MeshIndices[Count - 1]:=StrToInt(d3GetToken(Pos,Value,' '));
    except
    end;
  end;
  if Length(MeshIndices) > Count then
    SetLength(MeshIndices,Count);
  if Assigned(FOnChanged) then
    FOnChanged(Self);
  FNeedUpdate:=true;
end;
           
           
