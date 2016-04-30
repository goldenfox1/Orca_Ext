{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

 //==================================================================
{$IFNDEF KS_COMPILER6_UP}
type
  PInteger=^integer;

{$ENDIF}

function GetToken(var S:ansistring; Separators:ansistring;
  Stop:ansistring=''):ansistring;
var
  i,Len:integer;
  CopyS:ansistring;
begin
  Result:='';
  CopyS:=S;
  Len:=Length(CopyS);
  for i:=1 to Len do
  begin
    if Pos(CopyS[i],Stop) > 0 then
      Break;
    Delete(S,1,1);
    if Pos(CopyS[i],Separators) > 0 then
    begin
      Result:=Result;
      Break;
    end;
    Result:=Result + CopyS[i];
  end;
  Result:=Trim(Result);
  S:=Trim(S);
end;

function d3GetToken(var Pos:integer; const S:ansistring; Separators:ansistring;
  Stop:ansistring=''):ansistring;
var
  i,Len:integer;
begin
  Result:='';
  Len:=Length(S);
  for i:=Pos to Len do
  begin
    if System.Pos(S[i],Stop) > 0 then
      Break;
    if System.Pos(S[i],Separators) > 0 then
      Break;
    Result:=Result + S[i];
  end;
  { skip separators }
  Pos:=i;
  for i:=Pos to Len do
    if System.Pos(S[i],Separators) <= 0 then
      Break;
  Pos:=i;
end;

function d3FloatToStr(Value:single):ansistring;
var
  S:char;
  Buffer:array[0..63] of char;
begin
  S:=DecimalSeparator;
  try
    DecimalSeparator:='.';
    if Frac(Value) <> 0 then
    begin
      Result:=FloatToStrF(Value,ffGeneral,6,0);
    end
    else
      Result:=IntToStr(Trunc(Value))
  finally
    DecimalSeparator:=S;
  end;
end;

function d3StrToFloat(Value:ansistring):single;
var
  S:char;
begin
  S:=DecimalSeparator;
  try
    DecimalSeparator:='.';
    Result:=StrToFloat(Value);
  finally
    DecimalSeparator:=S;
  end;
end;

function d3Point(X,Y,Z:single):TD3Point;
begin
  Result.X:=X;
  Result.Y:=Y;
  Result.Z:=Z;
end;

function d3Point(const P:TD3Vector):TD3Point;
begin
  Result.X:=P.X;
  Result.Y:=P.Y;
  Result.Z:=P.Z;
end;

function d3PointToString(R:TD3Point):ansistring;
begin
  Result:='(' + d3FloatToStr(R.X) + ',' + d3FloatToStr(R.Y) + ',' +
    d3FloatToStr(R.Z) + ')';
end;

function d3StringToPoint(S:ansistring):TD3Point;
begin
  try
    GetToken(S,',()');
    Result.X:=d3StrToFloat(GetToken(S,',()'));
    Result.Y:=d3StrToFloat(GetToken(S,',()'));
    Result.Z:=d3StrToFloat(GetToken(S,',()'));
  except
    Result:=d3Point(0,0,0);
  end;
end;

{ Colros }

function d3Opacity(const C:TD3Color; const AOpacity:single):TD3Color;
begin
  Result:=C;
  if AOpacity < 1 then
    TD3ColorRec(Result).A:=Trunc(TD3ColorRec(C).A * AOpacity);
end;

function d3Color(R,G,B:byte; A:byte=$FF):TD3Color;
begin
  TD3ColorRec(Result).R:=R;
  TD3ColorRec(Result).G:=G;
  TD3ColorRec(Result).B:=B;
  TD3ColorRec(Result).A:=A;
end;

function d3ColorToStr(Value:TD3Color):ansistring;
begin
  Result:='#' + IntToHex(Value,8);
end;

function d3StrToColor(Value:ansistring):TD3Color;
begin
  if Value=#0 then
    Value:='$0'
  else
  if (Value <> '') and (Value[1]='#') then
    Value[1]:='$';
  try
    Result:=StrToInt(Value);
  except
  end;
end;

{ Vertexs }

function VertexSize(const AFormat:TD3VertexFormat):integer;
begin
  case AFormat of
    vfVertex:Result:=SizeOf(TD3Vertex);
    vfVertexNormal:Result:=SizeOf(TD3VertexNormal);
    vfTexVertexNormal:Result:=SizeOf(TD3TexVertexNormal);
    vfTexVertex:Result:=SizeOf(TD3TexVertex);
    vfColorVertex:Result:=SizeOf(TD3ColorVertex);
    vfColorVertexNormal:Result:=SizeOf(TD3ColorVertexNormal);
    vfColorTexVertex:Result:=SizeOf(TD3ColorTexVertex);
    vfColorTexVertexNormal:Result:=SizeOf(TD3ColorTexVertexNormal);
    else
      Result:=0;
  end;
end;

function Vertex(x,y,z:single):TD3Vertex;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
end;

function VertexNormal(x,y,z,nx,ny,nz:single):TD3VertexNormal;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.nx:=nx;
  Result.ny:=ny;
  Result.nz:=nz;
end;

function TexVertexNormal(x,y,z,nx,ny,nz,tu,tv:single):TD3TexVertexNormal;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.nx:=nx;
  Result.ny:=ny;
  Result.nz:=nz;
  Result.tu:=tu;
  Result.tv:=tv;
end;

function ColorVertex(x,y,z:single; color:TD3Color):TD3ColorVertex;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.color:=color;
end;

function ColorVertexNormal(x,y,z:single; nx,ny,nz:single;
  color:TD3Color):TD3ColorVertexNormal;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.nx:=nx;
  Result.ny:=ny;
  Result.nz:=nz;
  Result.color:=color;
end;

function TexVertex(x,y,z:single; tu,tv:single):TD3TexVertex;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.tu:=tu;
  Result.tv:=tv;
end;

function ColorTexVertex(x,y,z:single; color:TD3Color;
  tu,tv:single):TD3ColorTexVertex;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.color:=color;
  Result.tu:=tu;
  Result.tv:=tv;
end;

function ColorTexVertexNormal(x,y,z:single; nx,ny,nz:single;
  color:TD3Color; tu,tv:single):TD3ColorTexVertexNormal;
begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
  Result.nx:=nx;
  Result.ny:=ny;
  Result.nz:=nz;
  Result.color:=color;
  Result.tu:=tu;
  Result.tv:=tv;
end;

function VertexTod3Vector(V:TD3Vertex):TD3Vector;
begin
  Result.X:=V.X;
  Result.Y:=V.Y;
  Result.Z:=V.Z;
  Result.W:=1.0;
end;

function d3VectorToVertex(V:TD3Vector):TD3Vertex;
begin
  Result.X:=V.X;
  Result.Y:=V.Y;
  Result.Z:=V.Z;
end;

function MinInt(A1,A2:integer):integer;
begin
  if A1 < A2 then
    Result:=A1
  else
    Result:=A2;
end;

function MinFloat(A1,A2:single):single;
begin
  if A1 < A2 then
    Result:=A1
  else
    Result:=A2;
end;

function MaxInt(A1,A2:integer):integer;
begin
  if A1 > A2 then
    Result:=A1
  else
    Result:=A2;
end;

function MaxFloat(A1,A2:single):single;
begin
  if A1 > A2 then
    Result:=A1
  else
    Result:=A2;
end;

function Lerp(const start,stop,t:single):single;
begin
  Result:=start + (stop - start) * t;
end;

function ColorLerp(start,stop:TD3Color; t:single):TD3Color;
begin
  TD3ColorRec(Result).A:=TD3ColorRec(start).A + Trunc(
    (TD3ColorRec(stop).A - TD3ColorRec(start).A) * t);
  TD3ColorRec(Result).R:=TD3ColorRec(start).R + Trunc(
    (TD3ColorRec(stop).R - TD3ColorRec(start).R) * t);
  TD3ColorRec(Result).G:=TD3ColorRec(start).G + Trunc(
    (TD3ColorRec(stop).G - TD3ColorRec(start).G) * t);
  TD3ColorRec(Result).B:=TD3ColorRec(start).B + Trunc(
    (TD3ColorRec(stop).B - TD3ColorRec(start).B) * t);
end;

function d3InterpolateSingle(const start,stop,t:single):single;
begin
  Result:=start + (stop - start) * t;
end;

function d3InterpolateRotation(start,stop,t:single):single;
begin
  Result:=d3InterpolateSingle(start,stop,t);
end;

function d3InterpolateColor(start,stop:TD3Color; t:single):TD3Color;
begin
  TD3ColorRec(Result).A:=TD3ColorRec(start).A + Trunc(
    (TD3ColorRec(stop).A - TD3ColorRec(start).A) * t);
  TD3ColorRec(Result).R:=TD3ColorRec(start).R + Trunc(
    (TD3ColorRec(stop).R - TD3ColorRec(start).R) * t);
  TD3ColorRec(Result).G:=TD3ColorRec(start).G + Trunc(
    (TD3ColorRec(stop).G - TD3ColorRec(start).G) * t);
  TD3ColorRec(Result).B:=TD3ColorRec(start).B + Trunc(
    (TD3ColorRec(stop).B - TD3ColorRec(start).B) * t);
end;

function d3AppendColor(start,stop:TD3Color):TD3Color;
begin
  if TD3ColorRec(start).A + TD3ColorRec(stop).A < $FF then
    TD3ColorRec(Result).A:=TD3ColorRec(start).A + TD3ColorRec(stop).A
  else
    TD3ColorRec(Result).A:=$FF;
  if TD3ColorRec(start).R + TD3ColorRec(stop).R < $FF then
    TD3ColorRec(Result).R:=TD3ColorRec(start).R + TD3ColorRec(stop).R
  else
    TD3ColorRec(Result).R:=$FF;
  if TD3ColorRec(start).G + TD3ColorRec(stop).G < $FF then
    TD3ColorRec(Result).G:=TD3ColorRec(start).G + TD3ColorRec(stop).G
  else
    TD3ColorRec(Result).G:=$FF;
  if TD3ColorRec(start).B + TD3ColorRec(stop).B < $FF then
    TD3ColorRec(Result).B:=TD3ColorRec(start).B + TD3ColorRec(stop).B
  else
    TD3ColorRec(Result).B:=$FF;
end;

function d3SubtractColor(start,stop:TD3Color):TD3Color;
begin
  if TD3ColorRec(start).A - TD3ColorRec(stop).A < $FF then
    TD3ColorRec(Result).A:=TD3ColorRec(start).A - TD3ColorRec(stop).A
  else
    TD3ColorRec(Result).A:=$FF;
  if TD3ColorRec(start).R - TD3ColorRec(stop).R < $FF then
    TD3ColorRec(Result).R:=TD3ColorRec(start).R - TD3ColorRec(stop).R
  else
    TD3ColorRec(Result).R:=$FF;
  if TD3ColorRec(start).G - TD3ColorRec(stop).G < $FF then
    TD3ColorRec(Result).G:=TD3ColorRec(start).G - TD3ColorRec(stop).G
  else
    TD3ColorRec(Result).G:=$FF;
  if TD3ColorRec(start).B - TD3ColorRec(stop).B < $FF then
    TD3ColorRec(Result).B:=TD3ColorRec(start).B - TD3ColorRec(stop).B
  else
    TD3ColorRec(Result).B:=$FF;
end;

{ vectors }

function d3TexVector(const u,v:single):TD3TexVector;
begin
  Result.S:=u;
  Result.T:=v;
end;

function d3Vector(const x,y,z:single; const w:single=1.0):TD3Vector;
begin
  Result.X:=x;
  Result.Y:=y;
  Result.Z:=z;
  Result.W:=w;
end;

function d3Vector(const Point:TD3Point; const w:single=1.0):TD3Vector;
begin
  Result.X:=Point.X;
  Result.Y:=Point.Y;
  Result.Z:=Point.Z;
  Result.W:=w;
end;

function MidPoint(const p1,p2:TD3Vector):TD3Vector;
begin
  Result.x:=(p1.x + p2.x) / 2;
  Result.y:=(p1.y + p2.y) / 2;
  Result.z:=(p1.z + p2.z) / 2;
  Result.w:=1;
end;

function BSphere(center:TD3Vector; radius:single):TD3BSphere;
begin
  Result.center:=center;
  Result.Radius:=radius;
end;

function AABB(min,max:TD3Vector):TD3AABB;
begin
  Result.min:=min;
  Result.max:=max;
end;

procedure AABBInclude(var bb:TD3AABB; const p:TD3Vector);
begin
  if p.V[0] < bb.min.V[0] then
    bb.min.V[0]:=p.V[0];
  if p.V[0] > bb.max.V[0] then
    bb.max.V[0]:=p.V[0];
  if p.V[1] < bb.min.V[1] then
    bb.min.V[1]:=p.V[1];
  if p.V[1] > bb.max.V[1] then
    bb.max.V[1]:=p.V[1];
  if p.V[2] < bb.min.V[2] then
    bb.min.V[2]:=p.V[2];
  if p.V[2] > bb.max.V[2] then
    bb.max.V[2]:=p.V[2];
end;

procedure AABBTransform(var bb:TD3AABB; const m:TD3Matrix);
var
  oldMin,oldMax:TD3Vector;
begin
  oldMin:=bb.min;
  oldMax:=bb.max;
  bb.min:=d3VectorTransform(oldMin,m);
  bb.max:=bb.min;
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMin.V[0],oldMin.V[1],
    oldMax.V[2]),m));
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMin.V[0],oldMax.V[1],
    oldMin.V[2]),m));
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMin.V[0],oldMax.V[1],
    oldMax.V[2]),m));
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMax.V[0],oldMin.V[1],
    oldMin.V[2]),m));
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMax.V[0],oldMin.V[1],
    oldMax.V[2]),m));
  AABBInclude(bb,d3VectorTransform(d3Vector(oldMax.V[0],oldMax.V[1],
    oldMin.V[2]),m));
  AABBInclude(bb,d3VectorTransform(oldMax,m));
end;

function IntersecTD3AABBsAbsolute(const aabb1,aabb2:TD3AABB):boolean;
begin
  Result:=not ((AABB1.min.V[0] > AABB2.max.V[0]) or
    (AABB1.min.V[1] > AABB2.max.V[1]) or (AABB1.min.V[2] > AABB2.max.V[2]) or
    (AABB2.min.V[0] > AABB1.max.V[0]) or (AABB2.min.V[1] > AABB1.max.V[1]) or
    (AABB2.min.V[2] > AABB1.max.V[2]));
end;

procedure NormalizePlane(var plane:TD3Vector);
var
  n:single;
begin
  n:=RSqrt(abs(plane.V[0] * plane.V[0] + plane.V[1] * plane.V[1] +
    plane.V[2] * plane.V[2]));
  plane.v[0]:=plane.v[0] * n;
  plane.v[1]:=plane.v[1] * n;
  plane.v[2]:=plane.v[2] * n;
  plane.v[3]:=1;
end;

function PlaneEvaluatePoint(const plane:TD3Vector; const point:TD3Vector):single;
begin
  Result:=plane.V[0] * point.V[0] + plane.V[1] * point.V[1] +
    plane.V[2] * point.V[2] + plane.V[3];
end;

procedure ExtracTD3AABBCorners(const AABB:TD3AABB; var AABBCorners:TD3AABBCorners);
begin
  AABBCorners[0]:=d3Vector(AABB.min.v[0],AABB.min.v[1],AABB.min.v[2]);
  AABBCorners[1]:=d3Vector(AABB.min.v[0],AABB.min.v[1],AABB.max.v[2]);
  AABBCorners[2]:=d3Vector(AABB.min.v[0],AABB.max.v[1],AABB.min.v[2]);
  AABBCorners[3]:=d3Vector(AABB.min.v[0],AABB.max.v[1],AABB.max.v[2]);

  AABBCorners[4]:=d3Vector(AABB.max.v[0],AABB.min.v[1],AABB.min.v[2]);
  AABBCorners[5]:=d3Vector(AABB.max.v[0],AABB.min.v[1],AABB.max.v[2]);
  AABBCorners[6]:=d3Vector(AABB.max.v[0],AABB.max.v[1],AABB.min.v[2]);
  AABBCorners[7]:=d3Vector(AABB.max.v[0],AABB.max.v[1],AABB.max.v[2]);
end;

procedure SeTD3Vector(var V:TD3Vector; const x,y,z:single; const w:single=1.0);
begin
  V:=d3Vector(x,y,z,w);
end;

function d3VectorNorm(const v:TD3Vector):single;
begin
  Result:=v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
end;

procedure Normalized3Vector(var v:TD3Vector);
var
  invLen:single;
begin
  invLen:=RSqrt(Abs(d3VectorNorm(v)));
  v.v[0]:=v.v[0] * invLen;
  v.v[1]:=v.v[1] * invLen;
  v.v[2]:=v.v[2] * invLen;
  v.v[3]:=0.0;
end;

function d3VectorNormalize(const v:TD3Vector):TD3Vector;
var
  invLen:single;
begin
  invLen:=RSqrt(Abs(d3VectorNorm(v)));
  Result.v[0]:=v.v[0] * invLen;
  Result.v[1]:=v.v[1] * invLen;
  Result.v[2]:=v.v[2] * invLen;
  Result.v[3]:=0.0;
end;

function d3VectorAdd(const v1:TD3Vector; const v2:TD3Vector):TD3Vector;
begin
  Result.v[0]:=v1.v[0] + v2.v[0];
  Result.v[1]:=v1.v[1] + v2.v[1];
  Result.v[2]:=v1.v[2] + v2.v[2];
  Result.W:=1.0;
end;

procedure d3VectorCombine(const V1,V2:TD3Vector; const F2:single; var vr:TD3Vector);
begin
  vr.V[0]:=V1.V[0] + (F2 * V2.V[0]);
  vr.V[1]:=V1.V[1] + (F2 * V2.V[1]);
  vr.V[2]:=V1.V[2] + (F2 * V2.V[2]);
  vr.W:=1;
end;

function d3VectorCombine2(const V1,V2:TD3Vector; const F1,F2:single):TD3Vector;
begin
  Result.V[X]:=(F1 * V1.V[X]) + (F2 * V2.V[X]);
  Result.V[Y]:=(F1 * V1.V[Y]) + (F2 * V2.V[Y]);
  Result.V[Z]:=(F1 * V1.V[Z]) + (F2 * V2.V[Z]);
  Result.W:=1.0;
end;

function d3VectorReflect(const V,N:TD3Vector):TD3Vector;
begin
  Result:=d3VectorCombine2(V,N,1,-2 * d3VectorDotProduct(V,N));
end;

function d3VectorAddScale(const v1:TD3Vector; const v2:single):TD3Vector;
begin
  Result.v[0]:=v1.v[0] + v2;
  Result.v[1]:=v1.v[1] + v2;
  Result.v[2]:=v1.v[2] + v2;
  Result.v[3]:=1.0;
end;

procedure Addd3Vector(var v1:TD3Vector; const v2:TD3Vector);
begin
  v1:=d3VectorAdd(v1,v2);
end;

procedure Combined3Vector(var v1:TD3Vector; const v2:TD3Vector; f:single);
begin
  v1.V[0]:=v1.V[0] + v2.V[0] * f;
  v1.V[1]:=v1.V[1] + v2.V[1] * f;
  v1.V[2]:=v1.V[2] + v2.V[2] * f;
  v1.V[3]:=1.0;
end;

function d3VectorSubtract(const v1:TD3Vector; const v2:TD3Vector):TD3Vector;
begin
  Result.v[0]:=v1.v[0] - v2.v[0];
  Result.v[1]:=v1.v[1] - v2.v[1];
  Result.v[2]:=v1.v[2] - v2.v[2];
  Result.W:=1.0;
end;

function d3VectorLength(const v:TD3Vector):single;
var
  R:double;
begin
  R:=(v.x * v.x) + (v.y * v.y) + (v.z * v.z);
  Result:=Sqrt(R);
end;

function d3VectorScale(const v:TD3Vector; factor:single):TD3Vector;
begin
  Result.v[0]:=v.v[0] * factor;
  Result.v[1]:=v.v[1] * factor;
  Result.v[2]:=v.v[2] * factor;
  Result.v[3]:=1;
end;

function PointProject(const p,origin,direction:TD3Vector):single;
begin
  Result:=direction.V[0] * (p.V[0] - origin.V[0]) + direction.V[1] *
    (p.V[1] - origin.V[1]) + direction.V[2] * (p.V[2] - origin.V[2]);
end;

function VectorDistance2(const v1,v2:TD3Vector):single;
begin
  Result:=Sqr(v2.V[0] - v1.V[0]) + Sqr(v2.V[1] - v1.V[1]) + Sqr(v2.V[2] - v1.V[2]);
end;

function d3MatrixMultiply(const M1,M2:TD3Matrix):TD3Matrix;
begin
  Result.M[X].V[X]:=M1.M[X].V[X] * M2.M[X].V[X] + M1.M[X].V[Y] *
    M2.M[Y].V[X] + M1.M[X].V[Z] * M2.M[Z].V[X] + M1.M[X].V[W] * M2.M[W].V[X];
  Result.M[X].V[Y]:=M1.M[X].V[X] * M2.M[X].V[Y] + M1.M[X].V[Y] *
    M2.M[Y].V[Y] + M1.M[X].V[Z] * M2.M[Z].V[Y] + M1.M[X].V[W] * M2.M[W].V[Y];
  Result.M[X].V[Z]:=M1.M[X].V[X] * M2.M[X].V[Z] + M1.M[X].V[Y] *
    M2.M[Y].V[Z] + M1.M[X].V[Z] * M2.M[Z].V[Z] + M1.M[X].V[W] * M2.M[W].V[Z];
  Result.M[X].V[W]:=M1.M[X].V[X] * M2.M[X].V[W] + M1.M[X].V[Y] *
    M2.M[Y].V[W] + M1.M[X].V[Z] * M2.M[Z].V[W] + M1.M[X].V[W] * M2.M[W].V[W];
  Result.M[Y].V[X]:=M1.M[Y].V[X] * M2.M[X].V[X] + M1.M[Y].V[Y] *
    M2.M[Y].V[X] + M1.M[Y].V[Z] * M2.M[Z].V[X] + M1.M[Y].V[W] * M2.M[W].V[X];
  Result.M[Y].V[Y]:=M1.M[Y].V[X] * M2.M[X].V[Y] + M1.M[Y].V[Y] *
    M2.M[Y].V[Y] + M1.M[Y].V[Z] * M2.M[Z].V[Y] + M1.M[Y].V[W] * M2.M[W].V[Y];
  Result.M[Y].V[Z]:=M1.M[Y].V[X] * M2.M[X].V[Z] + M1.M[Y].V[Y] *
    M2.M[Y].V[Z] + M1.M[Y].V[Z] * M2.M[Z].V[Z] + M1.M[Y].V[W] * M2.M[W].V[Z];
  Result.M[Y].V[W]:=M1.M[Y].V[X] * M2.M[X].V[W] + M1.M[Y].V[Y] *
    M2.M[Y].V[W] + M1.M[Y].V[Z] * M2.M[Z].V[W] + M1.M[Y].V[W] * M2.M[W].V[W];
  Result.M[Z].V[X]:=M1.M[Z].V[X] * M2.M[X].V[X] + M1.M[Z].V[Y] *
    M2.M[Y].V[X] + M1.M[Z].V[Z] * M2.M[Z].V[X] + M1.M[Z].V[W] * M2.M[W].V[X];
  Result.M[Z].V[Y]:=M1.M[Z].V[X] * M2.M[X].V[Y] + M1.M[Z].V[Y] *
    M2.M[Y].V[Y] + M1.M[Z].V[Z] * M2.M[Z].V[Y] + M1.M[Z].V[W] * M2.M[W].V[Y];
  Result.M[Z].V[Z]:=M1.M[Z].V[X] * M2.M[X].V[Z] + M1.M[Z].V[Y] *
    M2.M[Y].V[Z] + M1.M[Z].V[Z] * M2.M[Z].V[Z] + M1.M[Z].V[W] * M2.M[W].V[Z];
  Result.M[Z].V[W]:=M1.M[Z].V[X] * M2.M[X].V[W] + M1.M[Z].V[Y] *
    M2.M[Y].V[W] + M1.M[Z].V[Z] * M2.M[Z].V[W] + M1.M[Z].V[W] * M2.M[W].V[W];
  Result.M[W].V[X]:=M1.M[W].V[X] * M2.M[X].V[X] + M1.M[W].V[Y] *
    M2.M[Y].V[X] + M1.M[W].V[Z] * M2.M[Z].V[X] + M1.M[W].V[W] * M2.M[W].V[X];
  Result.M[W].V[Y]:=M1.M[W].V[X] * M2.M[X].V[Y] + M1.M[W].V[Y] *
    M2.M[Y].V[Y] + M1.M[W].V[Z] * M2.M[Z].V[Y] + M1.M[W].V[W] * M2.M[W].V[Y];
  Result.M[W].V[Z]:=M1.M[W].V[X] * M2.M[X].V[Z] + M1.M[W].V[Y] *
    M2.M[Y].V[Z] + M1.M[W].V[Z] * M2.M[Z].V[Z] + M1.M[W].V[W] * M2.M[W].V[Z];
  Result.M[W].V[W]:=M1.M[W].V[X] * M2.M[X].V[W] + M1.M[W].V[Y] *
    M2.M[Y].V[W] + M1.M[W].V[Z] * M2.M[Z].V[W] + M1.M[W].V[W] * M2.M[W].V[W];
end;

function d3VectorCrossProduct(const V1,V2:TD3Vector):TD3Vector;
begin
  Result.X:=v1.Y * v2.Z - v1.Z * v2.Y;
  Result.Y:=v1.Z * v2.X - v1.X * v2.Z;
  Result.Z:=v1.X * v2.Y - v1.Y * v2.X;
  Result.W:=1.0;
end;

function d3VectorDotProduct(const V1,V2:TD3Vector):single;
begin
  Result:=V1.V[0] * V2.V[0] + V1.V[1] * V2.V[1] + V1.V[2] * V2.V[2];
end;

function d3CalcPlaneNormal(const p1,p2,p3:TD3Vector):TD3Vector;
var
  v1,v2:TD3Vector;
begin
  v1:=d3VectorSubtract(p2,p1);
  v2:=d3VectorSubtract(p3,p1);
  Result:=d3VectorCrossProduct(v1,v2);
  Result:=d3VectorNormalize(Result);
end;

function d3VectorAngleCosine(const V1,V2:TD3Vector):single;
begin
  Result:=d3VectorLength(V1) * d3VectorLength(V2);
  if Abs(Result) > Epsilon then
    Result:=d3VectorDotProduct(V1,V2) / Result
  else
    Result:=d3VectorDotProduct(V1,V2) / Epsilon;
end;

function d3VectorTransform(const V:TD3Vector; const M:TD3Matrix):TD3Vector;
begin
  Result.V[X]:=V.V[X] * M.M[X].V[X] + V.V[Y] * M.M[Y].V[X] +
    V.V[Z] * M.M[Z].V[X] + V.V[W] * M.M[W].V[X];
  Result.V[Y]:=V.V[X] * M.M[X].V[Y] + V.V[Y] * M.M[Y].V[Y] +
    V.V[Z] * M.M[Z].V[Y] + V.V[W] * M.M[W].V[Y];
  Result.V[Z]:=V.V[X] * M.M[X].V[Z] + V.V[Y] * M.M[Y].V[Z] +
    V.V[Z] * M.M[Z].V[Z] + V.V[W] * M.M[W].V[Z];
  Result.V[W]:=1;
end;

procedure RotateVector(var vector:TD3Vector; const axis:TD3Vector; angle:single);
var
  rotMatrix:TD3Matrix;
begin
  rotMatrix:=d3CreateRotationMatrix(axis,Angle);
  vector:=d3VectorTransform(vector,rotMatrix);
end;

function MatrixDetInternal(const a1,a2,a3,b1,b2,b3,c1,c2,c3:single):single;
begin
  Result:=a1 * (b2 * c3 - b3 * c2) - b1 * (a2 * c3 - a3 * c2) +
    c1 * (a2 * b3 - a3 * b2);
end;

function d3MatrixDeterminant(const M:TD3Matrix):single;
begin
  Result:=M.M[X].V[X] * MatrixDetInternal(M.M[Y].V[Y],M.M[Z].V[Y],
    M.M[W].V[Y],M.M[Y].V[Z],M.M[Z].V[Z],M.M[W].V[Z],M.M[Y].V[W],
    M.M[Z].V[W],M.M[W].V[W]) - M.M[X].V[Y] * MatrixDetInternal(
    M.M[Y].V[X],M.M[Z].V[X],M.M[W].V[X],M.M[Y].V[Z],M.M[Z].V[Z],
    M.M[W].V[Z],M.M[Y].V[W],M.M[Z].V[W],M.M[W].V[W]) + M.M[X].V[Z] *
    MatrixDetInternal(M.M[Y].V[X],M.M[Z].V[X],M.M[W].V[X],M.M[Y].V[Y],
    M.M[Z].V[Y],M.M[W].V[Y],M.M[Y].V[W],M.M[Z].V[W],M.M[W].V[W]) -
    M.M[X].V[W] * MatrixDetInternal(M.M[Y].V[X],M.M[Z].V[X],
    M.M[W].V[X],M.M[Y].V[Y],M.M[Z].V[Y],M.M[W].V[Y],M.M[Y].V[Z],
    M.M[Z].V[Z],M.M[W].V[Z]);
end;

procedure AdjoinTD3Matrix(var M:TD3Matrix);
var
  a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4:single;
begin
  a1:=M.M[X].V[X];
  b1:=M.M[X].V[Y];
  c1:=M.M[X].V[Z];
  d1:=M.M[X].V[W];
  a2:=M.M[Y].V[X];
  b2:=M.M[Y].V[Y];
  c2:=M.M[Y].V[Z];
  d2:=M.M[Y].V[W];
  a3:=M.M[Z].V[X];
  b3:=M.M[Z].V[Y];
  c3:=M.M[Z].V[Z];
  d3:=M.M[Z].V[W];
  a4:=M.M[W].V[X];
  b4:=M.M[W].V[Y];
  c4:=M.M[W].V[Z];
  d4:=M.M[W].V[W];

  // row column labeling reversed since we transpose rows & columns
  M.M[X].V[X]:=MatrixDetInternal(b2,b3,b4,c2,c3,c4,d2,d3,d4);
  M.M[Y].V[X]:=-MatrixDetInternal(a2,a3,a4,c2,c3,c4,d2,d3,d4);
  M.M[Z].V[X]:=MatrixDetInternal(a2,a3,a4,b2,b3,b4,d2,d3,d4);
  M.M[W].V[X]:=-MatrixDetInternal(a2,a3,a4,b2,b3,b4,c2,c3,c4);

  M.M[X].V[Y]:=-MatrixDetInternal(b1,b3,b4,c1,c3,c4,d1,d3,d4);
  M.M[Y].V[Y]:=MatrixDetInternal(a1,a3,a4,c1,c3,c4,d1,d3,d4);
  M.M[Z].V[Y]:=-MatrixDetInternal(a1,a3,a4,b1,b3,b4,d1,d3,d4);
  M.M[W].V[Y]:=MatrixDetInternal(a1,a3,a4,b1,b3,b4,c1,c3,c4);

  M.M[X].V[Z]:=MatrixDetInternal(b1,b2,b4,c1,c2,c4,d1,d2,d4);
  M.M[Y].V[Z]:=-MatrixDetInternal(a1,a2,a4,c1,c2,c4,d1,d2,d4);
  M.M[Z].V[Z]:=MatrixDetInternal(a1,a2,a4,b1,b2,b4,d1,d2,d4);
  M.M[W].V[Z]:=-MatrixDetInternal(a1,a2,a4,b1,b2,b4,c1,c2,c4);

  M.M[X].V[W]:=-MatrixDetInternal(b1,b2,b3,c1,c2,c3,d1,d2,d3);
  M.M[Y].V[W]:=MatrixDetInternal(a1,a2,a3,c1,c2,c3,d1,d2,d3);
  M.M[Z].V[W]:=-MatrixDetInternal(a1,a2,a3,b1,b2,b3,d1,d2,d3);
  M.M[W].V[W]:=MatrixDetInternal(a1,a2,a3,b1,b2,b3,c1,c2,c3);
end;

procedure Scaled3Matrix(var M:TD3Matrix; const factor:single);
var
  i:integer;
begin
  for i:=0 to 3 do
  begin
    M.M[I].V[0]:=M.M[I].V[0] * Factor;
    M.M[I].V[1]:=M.M[I].V[1] * Factor;
    M.M[I].V[2]:=M.M[I].V[2] * Factor;
    M.M[I].V[3]:=M.M[I].V[3] * Factor;
  end;
end;

procedure Transposed3Matrix(var M:TD3Matrix);
var
  f:single;
begin
  f:=M.M[0].V[1];
  M.M[0].V[1]:=M.M[1].V[0];
  M.M[1].V[0]:=f;
  f:=M.M[0].V[2];
  M.M[0].V[2]:=M.M[2].V[0];
  M.M[2].V[0]:=f;
  f:=M.M[0].V[3];
  M.M[0].V[3]:=M.M[3].V[0];
  M.M[3].V[0]:=f;
  f:=M.M[1].V[2];
  M.M[1].V[2]:=M.M[2].V[1];
  M.M[2].V[1]:=f;
  f:=M.M[1].V[3];
  M.M[1].V[3]:=M.M[3].V[1];
  M.M[3].V[1]:=f;
  f:=M.M[2].V[3];
  M.M[2].V[3]:=M.M[3].V[2];
  M.M[3].V[2]:=f;
end;

procedure InvertMatrix(var M:TD3Matrix);
var
  det:single;
begin
  det:=d3MatrixDeterminant(M);
  if Abs(Det) < EPSILON then
    M:=IdentityMatrix
  else
  begin
    AdjoinTD3Matrix(M);
    Scaled3Matrix(M,1 / det);
  end;
end;

function d3CreateRotationMatrix(const anAxis:TD3Vector; angle:single):TD3Matrix;
var
  axis:TD3Vector;
  cosine,sine,one_minus_cosine:single;
begin
  SinCos(NormalizeDegAngle(angle),sine,cosine);
  one_minus_cosine:=1 - cosine;
  axis:=d3VectorNormalize(anAxis);

  Result.M[X].V[X]:=(one_minus_cosine * axis.V[0] * axis.V[0]) + cosine;
  Result.M[X].V[Y]:=(one_minus_cosine * axis.V[0] * axis.V[1]) - (axis.V[2] * sine);
  Result.M[X].V[Z]:=(one_minus_cosine * axis.V[2] * axis.V[0]) + (axis.V[1] * sine);
  Result.M[X].V[W]:=0;

  Result.M[Y].V[X]:=(one_minus_cosine * axis.V[0] * axis.V[1]) + (axis.V[2] * sine);
  Result.M[Y].V[Y]:=(one_minus_cosine * axis.V[1] * axis.V[1]) + cosine;
  Result.M[Y].V[Z]:=(one_minus_cosine * axis.V[1] * axis.V[2]) - (axis.V[0] * sine);
  Result.M[Y].V[W]:=0;

  Result.M[Z].V[X]:=(one_minus_cosine * axis.V[2] * axis.V[0]) - (axis.V[1] * sine);
  Result.M[Z].V[Y]:=(one_minus_cosine * axis.V[1] * axis.V[2]) + (axis.V[0] * sine);
  Result.M[Z].V[Z]:=(one_minus_cosine * axis.V[2] * axis.V[2]) + cosine;
  Result.M[Z].V[W]:=0;

  Result.M[W].V[X]:=0;
  Result.M[W].V[Y]:=0;
  Result.M[W].V[Z]:=0;
  Result.M[W].V[W]:=1;
end;

function MatrixOrthoLH(w,h,zn,zf:single):TD3Matrix;
begin
  Result:=IdentityMatrix;
  Result.m11:=2 / w;
  Result.m22:=2 / h;
  Result.m33:=1 / (zf - zn);
  Result.m42:=zn / (zn - zf);
end;

function MatrixOrthoOffCenterLH(l,r,b,t,zn,zf:single):TD3Matrix;
begin
  Result:=IdentityMatrix;
  Result.m11:=2 / (r - l);
  Result.m22:=2 / (t - b);
  Result.m33:=1 / (zf - zn);
  Result.m41:=(l + r) / (l - r);
  Result.m42:=(t + b) / (b - t);
  Result.m43:=zn / (zn - zf);
end;

function MatrixOrthoOffCenterRH(l,r,b,t,zn,zf:single):TD3Matrix;
begin
  Result:=IdentityMatrix;
  Result.m11:=2 / (r - l);
  Result.m22:=2 / (t - b);
  Result.m33:=1 / (zn - zf);
  Result.m41:=(l + r) / (l - r);
  Result.m42:=(t + b) / (b - t);
  Result.m43:=zn / (zn - zf);
end;

function MatrixPerspectiveOffCenterLH(l,r,b,t,zn,zf:single):TD3Matrix;
begin
  Result:=IdentityMatrix;
  Result.m11:=(2 * zn) / (r - l);
  Result.m22:=(2 * zn) / (t - b);
{  Result.m31:=(l + r) / (r - l);
  Result.m32:=(t + b) / (t - b);}
  Result.m34:=-1;
  Result.m33:=1;
  Result.m43:=0; //(zn * zf) / (zn - zf);
end;

function MatrixPerspectiveFovRH(flovy,aspect,zn,zf:single):TD3Matrix;
var
  yScale,xScale,h,w:single;
begin
  {$IFDEF KS_COMPILER6_UP}
  yScale:=cot(flovy / 2);
  {$ELSE}
  yScale:=cotan(flovy / 2);
  {$ENDIF}
  xScale:=yScale / aspect;
  h:=cos(flovy / 2) / sin(flovy / 2);
  w:=h / aspect;
  Result:=IdentityMatrix;
  Result.m11:=xScale;
  Result.m22:=yScale;
  Result.m33:=(zf / (zn - zf));
  Result.m34:=-1;
  Result.m43:=zn * zf / (zn - zf);
  Result.m44:=0;
end;

function MatrixPerspectiveFovLH(flovy,aspect,zn,zf:single):TD3Matrix;
var
  yScale,xScale,h,w:single;
begin
  {$IFDEF KS_COMPILER6_UP}
  yScale:=cot(flovy / 2);
  {$ELSE}
  yScale:=cotan(flovy / 2);
  {$ENDIF}
  xScale:=yScale / aspect;
  h:=cos(flovy / 2) / sin(flovy / 2);
  w:=h / aspect;
  Result:=IdentityMatrix;
  Result.m11:=xScale;
  Result.m22:=yScale;
  Result.m33:=(zf / (zf - zn));
  Result.m34:=1;
  Result.m43:=-zn * zf / (zf - zn);
  Result.m44:=0;
end;

function MatrixLookAtRH(const Eye,At,Up:TD3Vector):TD3Matrix;
var
  zaxis,xaxis,yaxis:TD3Vector;
begin
  zaxis:=d3VectorNormalize(d3VectorSubtract(Eye,At));
  zaxis.V[3]:=0;
  xaxis:=d3VectorNormalize(d3VectorCrossProduct(Up,zaxis));
  xaxis.V[3]:=0;
  yaxis:=d3VectorCrossProduct(zaxis,xaxis);
  yaxis.V[3]:=0;

  Result:=IdentityMatrix;
  Result.m11:=xaxis.x;
  Result.m12:=yaxis.x;
  Result.m13:=zaxis.x;
  Result.m21:=xaxis.y;
  Result.m22:=yaxis.y;
  Result.m23:=zaxis.y;
  Result.m31:=xaxis.z;
  Result.m32:=yaxis.z;
  Result.m33:=zaxis.z;
  Result.m41:=-d3VectorDotProduct(xaxis,eye);
  Result.m42:=-d3VectorDotProduct(yaxis,eye);
  Result.m43:=-d3VectorDotProduct(zaxis,eye);
end;

function MatrixLookAtDirRH(const Pos,Dir,Up:TD3Vector):TD3Matrix;
var
  zaxis,xaxis,yaxis:TD3Vector;
begin
  zaxis:=d3VectorNormalize(d3VectorScale(Dir,-1));
  zaxis.V[3]:=0;
  xaxis:=d3VectorNormalize(d3VectorCrossProduct(Up,zaxis));
  xaxis.V[3]:=0;
  yaxis:=d3VectorCrossProduct(zaxis,xaxis);
  yaxis.V[3]:=0;

  Result:=IdentityMatrix;
  Result.m11:=xaxis.x;
  Result.m12:=yaxis.x;
  Result.m13:=zaxis.x;
  Result.m21:=xaxis.y;
  Result.m22:=yaxis.y;
  Result.m23:=zaxis.y;
  Result.m31:=xaxis.z;
  Result.m32:=yaxis.z;
  Result.m33:=zaxis.z;
  Result.m41:=-d3VectorDotProduct(xaxis,Pos);
  Result.m42:=-d3VectorDotProduct(yaxis,Pos);
  Result.m43:=-d3VectorDotProduct(zaxis,Pos);
end;

function MakeShadowMatrix(const planePoint,planeNormal,lightPos:TD3Vector):TD3Matrix;
var
  planeNormal3,dot:single;
begin
  planeNormal3:=-(planeNormal.V[0] * planePoint.V[0] + planeNormal.V[1] *
    planePoint.V[1] + planeNormal.V[2] * planePoint.V[2]);
  dot:=planeNormal.V[0] * lightPos.V[0] + planeNormal.V[1] *
    lightPos.V[1] + planeNormal.V[2] * lightPos.V[2] + planeNormal3 * lightPos.V[3];
  Result.M[0].V[0]:=dot - lightPos.V[0] * planeNormal.V[0];
  Result.M[1].V[0]:=-lightPos.V[0] * planeNormal.V[1];
  Result.M[2].V[0]:=-lightPos.V[0] * planeNormal.V[2];
  Result.M[3].V[0]:=-lightPos.V[0] * planeNormal3;

  Result.M[0].V[1]:=-lightPos.V[1] * planeNormal.V[0];
  Result.M[1].V[1]:=dot - lightPos.V[1] * planeNormal.V[1];
  Result.M[2].V[1]:=-lightPos.V[1] * planeNormal.V[2];
  Result.M[3].V[1]:=-lightPos.V[1] * planeNormal3;

  Result.M[0].V[2]:=-lightPos.V[2] * planeNormal.V[0];
  Result.M[1].V[2]:=-lightPos.V[2] * planeNormal.V[1];
  Result.M[2].V[2]:=dot - lightPos.V[2] * planeNormal.V[2];
  Result.M[3].V[2]:=-lightPos.V[2] * planeNormal3;

  Result.M[0].V[3]:=-lightPos.V[3] * planeNormal.V[0];
  Result.M[1].V[3]:=-lightPos.V[3] * planeNormal.V[1];
  Result.M[2].V[3]:=-lightPos.V[3] * planeNormal.V[2];
  Result.M[3].V[3]:=dot - lightPos.V[3] * planeNormal3;
end;

function MakeReflectionMatrix(const planePoint,planeNormal:TD3Vector):TD3Matrix;
var
  pv2:single;
begin
  pv2:=2 * d3VectorDotProduct(planePoint,planeNormal);
  Result.M[0].V[0]:=1 - 2 * Sqr(planeNormal.V[0]);
  Result.M[0].V[1]:=-2 * planeNormal.V[0] * planeNormal.V[1];
  Result.M[0].V[2]:=-2 * planeNormal.V[0] * planeNormal.V[2];
  Result.M[0].V[3]:=0;
  Result.M[1].V[0]:=-2 * planeNormal.V[1] * planeNormal.V[0];
  Result.M[1].V[1]:=1 - 2 * Sqr(planeNormal.V[1]);
  Result.M[1].V[2]:=-2 * planeNormal.V[1] * planeNormal.V[2];
  Result.M[1].V[3]:=0;
  Result.M[2].V[0]:=-2 * planeNormal.V[2] * planeNormal.V[0];
  Result.M[2].V[1]:=-2 * planeNormal.V[2] * planeNormal.V[1];
  Result.M[2].V[2]:=1 - 2 * Sqr(planeNormal.V[2]);
  Result.M[2].V[3]:=0;
  Result.M[3].V[0]:=pv2 * planeNormal.V[0];
  Result.M[3].V[1]:=pv2 * planeNormal.V[1];
  Result.M[3].V[2]:=pv2 * planeNormal.V[2];
  Result.M[3].V[3]:=1;
end;

function RSqrt(v:single):single;
var
  R:double;
begin
  R:=Abs(V);
  if (R > 0) then
    Result:=1 / Sqrt(R)
  else
    Result:=1;
end;

function ISqrt(i:integer):integer;
begin
   {$HINTS OFF}
  i:=Abs(i);
  if i > 0 then
    Result:=Round(Sqrt(i))
  else
    Result:=1;
   {$HINTS ON}
end;

function DegToRad(const Degrees:single):single;
begin
  Result:=Degrees * cPIdiv180;
end;

function RadToDeg(const Degrees:single):single;
begin
  Result:=Degrees * c180divPI;
end;

procedure SinCos(const Theta:single; var Sin,Cos:single);
var
  s,c:extended;
begin
  Math.SinCos(NormalizeDegAngle(Theta),s,c);
  {$HINTS OFF}
  Sin:=s;
  Cos:=c;
  {$HINTS ON}
end;

function ArcCos(const x:single):single;
begin
  {$HINTS OFF}
  if (x > 1) then
    Result:=Math.arccos(1)
  else
  if (x < -1) then
    Result:=Math.arccos(-1)
  else
    Result:=Math.ArcCos(x);
  {$HINTS ON}
end;

function ArcSin(const x:single):single;
begin
  {$HINTS OFF}
  if (x > 1) then
    Result:=Math.ArcSin(1)
  else
  if (x < -1) then
    Result:=Math.ArcSin(-1)
  else
    Result:=Math.ArcSin(x);
  {$HINTS ON}
end;

function ArcTan2(const a,b:single):single;
begin
  Result:=Math.ArcTan2(a,b);
end;

function NormalizeDegAngle(angle:single):single;
begin
  Result:=angle - Int(angle * cInv360) * c360;
  if Result < -c180 then
    Result:=Result + c360;
end;

function RoundUpToPowerOf2(Value:integer):integer;
begin
  Result:=1;
  while (Result < Value) do
    Result:=Result shl 1;
end;

function RoundDownToPowerOf2(Value:integer):integer;
begin
  if Value > 0 then
  begin
    Result:=1 shl 30;
    while Result > Value do
      Result:=Result shr 1;
  end
  else
    Result:=1;
end;

function IsPowerOf2(Value:integer):boolean;
begin
  Result:=(RoundUpToPowerOf2(Value)=Value);
end;

function RoundInt(v:single):single;
begin
  Result:=Int(v + cOneDotFive);
end;

function Power(const base,exponent:single):single;
begin
   {$HINTS OFF}
  if exponent=cZero then
    Result:=cOne
  else if (base=cZero) and (exponent > cZero) then
    Result:=cZero
  else if RoundInt(exponent)=exponent then
    Result:=Power(base,integer(Round(exponent)))
  else
    Result:=Exp(exponent * Ln(base));
   {$HINTS ON}
end;


function Power(Base:single; Exponent:integer):single;
begin
   {$HINTS OFF}
  Result:=Math.Power(Base,Exponent);
   {$HINTS ON}
end;

function VectorCombine(const V1,V2:TD3Vector; const F1,F2:single):TD3Vector;
begin
  Result.V[X]:=(F1 * V1.V[X]) + (F2 * V2.V[X]);
  Result.V[Y]:=(F1 * V1.V[Y]) + (F2 * V2.V[Y]);
  Result.V[Z]:=(F1 * V1.V[Z]) + (F2 * V2.V[Z]);
  Result.V[W]:=(F1 * V1.V[W]) + (F2 * V2.V[W]);
end;

function RayCastIntersectsSphere(const rayStart,rayVector:TD3Vector;
  const sphereCenter:TD3Vector; const sphereRadius:single):boolean;
var
  proj:single;
begin
  proj:=PointProject(sphereCenter,rayStart,rayVector);
  if proj <= 0 then
    proj:=0;
  Result:=(VectorDistance2(sphereCenter,VectorCombine(rayStart,
    rayVector,1,proj)) <= Sqr(sphereRadius));
end;

function RayCastSphereIntersect(const rayStart,rayVector:TD3Vector;
  const sphereCenter:TD3Vector; const sphereRadius:single;
  var i1,i2:TD3Vector):integer;
var
  proj,d2:single;
  dd2:double;
  id2:integer;
  projPoint:TD3Vector;
begin
  proj:=PointProject(sphereCenter,rayStart,rayVector);
  projPoint:=VectorCombine(rayStart,rayVector,1,proj);
  d2:=sphereRadius * sphereRadius - VectorDistance2(sphereCenter,projPoint);
  id2:=PInteger(@d2)^;
  if id2 >= 0 then
  begin
    if id2=0 then
    begin
      if PInteger(@proj)^ > 0 then
      begin
        i1:=VectorCombine(rayStart,rayVector,1,proj);
        Result:=1;
        Exit;
      end;
    end
    else if id2 > 0 then
    begin
      dd2:=Abs(d2);
      d2:=Sqrt(dd2);
      if proj >= d2 then
      begin
        i1:=VectorCombine(rayStart,rayVector,1,proj - d2);
        i2:=VectorCombine(rayStart,rayVector,1,proj + d2);
        Result:=2;
        Exit;
      end
      else if proj + d2 >= 0 then
      begin
        i1:=VectorCombine(rayStart,rayVector,1,proj + d2);
        Result:=1;
        Exit;
      end;
    end;
  end;
  Result:=0;
end;

function RayCastPlaneIntersect(const rayStart,rayVector:TD3Vector;
  const planePoint,planeNormal:TD3Vector; var intersectPoint:TD3Vector):boolean;
var
  sp:TD3Vector;
  t,d:single;
begin
  d:=d3VectorDotProduct(rayVector,planeNormal);
  Result:=((d > cnEPSILON2) or (d < -cnEPSILON2));
  if Result then
  begin
    sp:=d3VectorSubtract(planePoint,rayStart);
    d:=1 / d; // will keep one FPU unit busy during dot product calculation
    t:=d3VectorDotProduct(sp,planeNormal) * d;
    if t > 0 then
      intersectPoint:=VectorCombine(rayStart,rayVector,1,t)
    else
      Result:=False;
  end;
end;

function RayCastTriangleIntersect(const rayStart,rayVector:TD3Vector;
  const p1,p2,p3:TD3Vector; intersectPoint:PD3Vector=nil;
  intersectNormal:PD3Vector=nil):boolean;
var
  pvec:TD3Vector;
  v1,v2,qvec,tvec:TD3Vector;
  t,u,v,det,invDet:single;
begin
  v1:=d3VectorSubtract(p2,p1);
  v2:=d3VectorSubtract(p3,p1);
  pvec:=d3VectorCrossProduct(rayVector,v2);
  det:=d3VectorDotProduct(v1,pvec);
  if ((det < cnEPSILON2) and (det > -cnEPSILON2)) then
  begin
    Result:=False;
    Exit;
  end;
  invDet:=cOne / det;
  tvec:=d3VectorSubtract(rayStart,p1);
  u:=d3VectorDotProduct(tvec,pvec) * invDet;
  if (u < 0) or (u > 1) then
    Result:=False
  else
  begin
    qvec:=d3VectorCrossProduct(tvec,v1);
    v:=d3VectorDotProduct(rayVector,qvec) * invDet;
    Result:=(v >= 0) and (u + v <= 1);
    if Result then
    begin
      t:=d3VectorDotProduct(v2,qvec) * invDet;
      if t > 0 then
      begin
        if intersectPoint <> nil then
          d3VectorCombine(rayStart,rayVector,t,intersectPoint^);
        if intersectNormal <> nil then
          intersectNormal^:=d3VectorCrossProduct(v1,v2);
      end
      else
        Result:=False;
    end;
  end;
end;

procedure NegateVector(var v:TD3Vector);
begin
  v.V[0]:=-v.V[0];
  v.V[1]:=-v.V[1];
  v.V[2]:=-v.V[2];
end;


function QuaternionFromAngleAxis(const angle:single;
  const axis:TD3Vector):TD3Quaternion;
var
  f,s,c:single;
begin
  SinCos(DegToRad(angle * cOneDotFive),s,c);
  Result.RealPart:=c;
  f:=s / d3VectorLength(axis);
  Result.ImagPart.V[0]:=axis.V[0] * f;
  Result.ImagPart.V[1]:=axis.V[1] * f;
  Result.ImagPart.V[2]:=axis.V[2] * f;
end;

function QuaternionMultiply(const qL,qR:TD3Quaternion):TD3Quaternion;
var
  Temp:TD3Quaternion;
begin
  Temp.RealPart:=qL.RealPart * qR.RealPart - qL.ImagPart.V[X] *
    qR.ImagPart.V[X] - qL.ImagPart.V[Y] * qR.ImagPart.V[Y] -
    qL.ImagPart.V[Z] * qR.ImagPart.V[Z];
  Temp.ImagPart.V[X]:=qL.RealPart * qR.ImagPart.V[X] + qL.ImagPart.V[X] *
    qR.RealPart + qL.ImagPart.V[Y] * qR.ImagPart.V[Z] - qL.ImagPart.V[Z] *
    qR.ImagPart.V[Y];
  Temp.ImagPart.V[Y]:=qL.RealPart * qR.ImagPart.V[Y] + qL.ImagPart.V[Y] *
    qR.RealPart + qL.ImagPart.V[Z] * qR.ImagPart.V[X] - qL.ImagPart.V[X] *
    qR.ImagPart.V[Z];
  Temp.ImagPart.V[Z]:=qL.RealPart * qR.ImagPart.V[Z] + qL.ImagPart.V[Z] *
    qR.RealPart + qL.ImagPart.V[X] * qR.ImagPart.V[Y] - qL.ImagPart.V[Y] *
    qR.ImagPart.V[X];
  Result:=Temp;
end;

function QuaternionFromRollPitchYaw(const r,p,y:single):TD3Quaternion;
var
  qp,qy,qr:TD3Quaternion;
begin
  qr:=QuaternionFromAngleAxis(r,d3Vector(0,0,1));
  qp:=QuaternionFromAngleAxis(p,d3Vector(1,0,0));
  qy:=QuaternionFromAngleAxis(y,d3Vector(0,1,0));

  Result:=qy;
  Result:=QuaternionMultiply(qp,Result);
  Result:=QuaternionMultiply(qr,Result);
end;

function QuaternionMagnitude(const q:TD3Quaternion):single;
begin
  Result:=Sqrt(d3VectorNorm(q.ImagPart) + Sqr(q.RealPart));
end;

procedure NormalizeQuaternion(var q:TD3Quaternion);
var
  m,f:single;
begin
  m:=QuaternionMagnitude(q);
  if m > cnEPSILON2 then
  begin
    f:=1 / m;
    q.ImagPart:=d3VectorScale(q.ImagPart,f);
    q.RealPart:=q.RealPart * f;
  end
  else
    q:=IdentityQuaternion;
end;

function QuaternionToMatrix(quat:TD3Quaternion):TD3Matrix;
var
  w,x,y,z,xx,xy,xz,xw,yy,yz,yw,zz,zw:single;
begin
  NormalizeQuaternion(quat);
  w:=quat.RealPart;
  x:=quat.ImagPart.V[0];
  y:=quat.ImagPart.V[1];
  z:=quat.ImagPart.V[2];
  xx:=x * x;
  xy:=x * y;
  xz:=x * z;
  xw:=x * w;
  yy:=y * y;
  yz:=y * z;
  yw:=y * w;
  zz:=z * z;
  zw:=z * w;
  Result.M[0].V[0]:=1 - 2 * (yy + zz);
  Result.M[1].V[0]:=2 * (xy - zw);
  Result.M[2].V[0]:=2 * (xz + yw);
  Result.M[3].V[0]:=0;
  Result.M[0].V[1]:=2 * (xy + zw);
  Result.M[1].V[1]:=1 - 2 * (xx + zz);
  Result.M[2].V[1]:=2 * (yz - xw);
  Result.M[3].V[1]:=0;
  Result.M[0].V[2]:=2 * (xz - yw);
  Result.M[1].V[2]:=2 * (yz + xw);
  Result.M[2].V[2]:=1 - 2 * (xx + yy);
  Result.M[3].V[2]:=0;
  Result.M[0].V[3]:=0;
  Result.M[1].V[3]:=0;
  Result.M[2].V[3]:=0;
  Result.M[3].V[3]:=1;
end;

function d3QuaternionFromMatrix(const mat:TD3Matrix):TD3Quaternion;
var
  traceMat,s,invS:double;
begin
  tracemat:=1 + mat.M[0].V[0] + mat.M[1].V[1] + mat.M[2].V[2];
  if tracemat > cnEPSILON2 then
  begin
    s:=Sqrt(tracemat) * 2;
    invS:=1 / s;
    Result.ImagPart.V[0]:=(mat.M[1].V[2] - mat.M[2].V[1]) * invS;
    Result.ImagPart.V[1]:=(mat.M[2].V[0] - mat.M[0].V[2]) * invS;
    Result.ImagPart.V[2]:=(mat.M[0].V[1] - mat.M[1].V[0]) * invS;
    Result.RealPart:=0.25 * s;
  end
  else
  if (mat.M[0].V[0] > mat.M[1].V[1]) and (mat.M[0].V[0] > mat.M[2].V[2]) then
  begin  // Row 0:
    s:=Sqrt(MaxFloat(cnEPSILON2,cOne + mat.M[0].V[0] - mat.M[1].V[1] -
      mat.M[2].V[2])) * 2;
    invS:=1 / s;
    Result.ImagPart.V[0]:=0.25 * s;
    Result.ImagPart.V[1]:=(mat.M[0].V[1] + mat.M[1].V[0]) * invS;
    Result.ImagPart.V[2]:=(mat.M[2].V[0] + mat.M[0].V[2]) * invS;
    Result.RealPart:=(mat.M[1].V[2] - mat.M[2].V[1]) * invS;
  end
  else
  if (mat.M[1].V[1] > mat.M[2].V[2]) then
  begin
    s:=Sqrt(MaxFloat(cnEPSILON2,cOne + mat.M[1].V[1] - mat.M[0].V[0] -
      mat.M[2].V[2])) * 2;
    invS:=1 / s;
    Result.ImagPart.V[0]:=(mat.M[0].V[1] + mat.M[1].V[0]) * invS;
    Result.ImagPart.V[1]:=0.25 * s;
    Result.ImagPart.V[2]:=(mat.M[1].V[2] + mat.M[2].V[1]) * invS;
    Result.RealPart:=(mat.M[2].V[0] - mat.M[0].V[2]) * invS;
  end
  else
  begin
    s:=Sqrt(MaxFloat(cnEPSILON2,cOne + mat.M[2].V[2] - mat.M[0].V[0] -
      mat.M[1].V[1])) * 2;
    invS:=1 / s;
    Result.ImagPart.V[0]:=(mat.M[2].V[0] + mat.M[0].V[2]) * invS;
    Result.ImagPart.V[1]:=(mat.M[1].V[2] + mat.M[2].V[1]) * invS;
    Result.ImagPart.V[2]:=0.25 * s;
    Result.RealPart:=(mat.M[0].V[1] - mat.M[1].V[0]) * invS;
  end;
  NormalizeQuaternion(Result);
end;

function d3CreateYawPitchRollMatrix(const y,p,r:single):TD3Matrix;
var
  Q:TD3Quaternion;
begin
  Q:=QuaternionFromRollPitchYaw(r,p,y);
  Result:=QuaternionToMatrix(Q);
end;



                        
