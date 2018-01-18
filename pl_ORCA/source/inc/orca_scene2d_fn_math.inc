{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

const
  //Для использования в качестве описательных индексов. to be used as descriptive indices
  X=0;
  Y=1;
  Z=2;
  W=3;

//Вычисляет значение A1 * A2 / A3
function d2MulDiv(A1, A2, A3:single):single;
begin
  Result:=A1 * A2 / A3;
end;

//Вычисляет сумму A1 + A2 или А1 + 1 если A2 не задано
procedure d2IncFloat(var A1:single; A2:single = 1.0);
begin
  A1:= A1 + A2;
end;

//Вычисляет разность A1 - A2 или А1 - 1 если A2 не задано
procedure d2DecFloat(var A1:single; A2:single = 1.0);
begin
  A1:= A1 - A2;
end;

//Возвращает наибольшее из A1 и A2
function d2MaxFloat(A1,A2:single):single;
begin
  if A1 > A2 then
    Result:=A1
  else
    Result:=A2;
end;

//Возвращает наименьшее из A1 и A2
function d2MinFloat(A1,A2:single):single;
begin
  if A1 < A2 then
    Result:=A1
  else
    Result:=A2;
end;

//Если х внутри диапазона [mi,ma], то возвращает х. Если х вне диапазона [mi,ma],
//то возвращает ближайшую границу: если левее, то возвращает mi, правее - ma
function d2MinMax(x,mi,ma:single):single;
begin
  if (x < mi) then Result:=mi
    else if (x > ma) then Result:=ma else Result:=x;
end;      

//Вычисляет синус Sin и косинус Cos угла Theta
procedure d2SinCos(const Theta:single; var Sin,Cos:single);
var
  s,c :Extended;
begin
  Math.SinCos(Theta,s,c);
  Sin:=s;
  Cos:=c;
end;

function d2RadToDeg(const Degrees:single):single;
begin
  Result:=Degrees * c180divPI;
end;

function d2DegToRad(const Degrees:single):single;
begin
   Result:=Degrees * cPIdiv180;
end;

//Нормализует угол angle  ???
function d2NormalizeAngle(const angle:single) :single;
begin
  Result:=angle - Int(angle * cInv360) * c360;
  if Result < -c180 then
    Result:=Result + c360;
end;

//Возвращает точку (объект TD2Point) формируя его из X и Y
function d2Point(const X,Y:single):TD2Point;
begin
  Result.X:=X;
  Result.Y:=Y;
end;

//Возвращает точку (объект TD2Point) находящуюся левее другой.
//Если позиции по Y равны, то возвращает точку которая выше другой
function d2MinPoint(P1,P2:TD2Point):TD2Point;
begin
  Result:=P1;
  if (P2.y < P1.y) or ((P2.y=P1.y) and (P2.x < P1.x)) then
    Result:=P2;
end;

//Масштабирует точку P - умножает P.X и P.Y на dx и dy соответственно
function d2ScalePoint(P:TD2Point; dx,dy:single):TD2Point;
begin
  Result.X:=P.X * dx;
  Result.Y:=P.Y * dy;
end;

//Возвращает прямоугольник (объект TD2Rect), заданный параметрами ALeft,ATop,ARight,ABottom
function d2Rect(const ALeft,ATop,ARight,ABottom:single):TD2Rect;
begin
  Result.Left:=ALeft;
  Result.Top:=ATop;
  Result.Right:=ARight;
  Result.Bottom:=ABottom;
end;

//Возвращает прямоугольник (объект TD2Rect), описанный вокруг всех точек массива Pts
function d2NormalizeRect(const Pts:array of TD2Point):TD2Rect;
var
  i:integer;
begin
  Result.Left:=$F000;
  Result.Top:=$F000;
  Result.Right:=-$F000;
  Result.Bottom:=-$F000;
  for i:=0 to High(Pts) do
  begin
    if Pts[i].X < Result.Left then
      Result.Left:=Pts[i].X;
    if Pts[i].Y < Result.Top then
      Result.Top:=Pts[i].Y;
    if Pts[i].X > Result.Right then
      Result.Right:=Pts[i].X;
    if Pts[i].Y > Result.Bottom then
      Result.Bottom:=Pts[i].Y;
  end;
end;

//Возвращает нормализованный прямоугольник ARect, т.е. разворачивает его так, чтобы соблюдались условия Left <= Right и Top <= Bottom
function d2NormalizeRect2(const ARect:TD2Rect):TD2Rect;
begin
  with ARect do
    Result:=d2NormalizeRect([d2Point(Left,Top),d2Point(Right,Top),d2Point(Right,Bottom),d2Point(Left,Bottom)]);
end;

//Возвращает сдвинутый прямоугольник R по X и Y на величины Dx и Dy соответственно
procedure d2OffsetRect(var R:TD2Rect; const Dx,Dy:single);
begin
  R.Left:=R.Left + Dx;
  R.Right:=R.Right + Dx;
  R.Top:=R.Top + Dy;
  R.Bottom:=R.Bottom + Dy;
end;

//Масштабирует прямоугольник R - умножает R.Left и R.Right на Dx, а R.Top и R.Bottom на Dy
procedure d2MultiplyRect(var R:TD2Rect; const Dx,Dy:single);
begin
  R.Left:=R.Left * Dx;
  R.Right:=R.Right * Dx;
  R.Top:=R.Top * Dy;
  R.Bottom:=R.Bottom * Dy;
end;

//Изменяет размеры прямоугольника R одновременно влево и вправо на величину Dx,
//а вверх и вниз - на Dy соответственно.При этом центр прямоугольника остается на месте
procedure d2InflateRect(var R:TD2Rect; const Dx,Dy:single);
begin
  R.Left:=R.Left - Dx;
  R.Right:=R.Right + Dx;
  R.Top:=R.Top - Dy;
  R.Bottom:=R.Bottom + Dy;
end;

//Возвращает прямоугольник (объект TD2Rect) описанный вокруг прямоугольников ARect1 и ARect2
function d2UnionRect(const ARect1,ARect2:TD2Rect):TD2Rect;
begin
  Result.Left:=ARect1.Left;
  if ARect2.Left < Result.Left then
    Result.Left:=ARect2.Left;

  Result.Top:=ARect1.Top;
  if ARect2.Top < Result.Top then
    Result.Top:=ARect2.Top;

  Result.Right:=ARect1.Right;
  if ARect2.Right > Result.Right then
    Result.Right:=ARect2.Right;

  Result.Bottom:=ARect1.Bottom;
  if ARect2.Bottom > Result.Bottom then
    Result.Bottom:=ARect2.Bottom;
end;

//Возращает ширину прямоугольника R
function d2RectWidth(const R:TD2Rect):single;
begin
  Result:=R.Right - R.Left;
end;

//Возращает высоту прямоугольника R
function d2RectHeight(const R:TD2Rect):single;
begin
  Result:=R.Bottom - R.Top;
end;

//Возвращает прямоугольник (объект TD2Rect), равный R и отцентрованный относительно границ, заданных Bounds
function d2RectCenter(var R:TD2Rect; Bounds:TD2Rect):TD2Rect;
begin
  d2OffsetRect(R,-R.Left,-R.Top);
  d2OffsetRect(R,round((d2RectWidth(Bounds) - d2RectWidth(R)) / 2),round((d2RectHeight(Bounds) - d2RectHeight(R)) / 2));
  d2OffsetRect(R,Bounds.Left,Bounds.Top);
  Result:=R;
end;

//Центрует прямоугольник R относительно BoundsRect, при этом пропорционально
//его уменьшает если он не помещается в границы (впиисывает R в BoundsRect)
//Возвращает наибольшее из отношений ширин и высот R к BoundsRect.
//Если возращаемая величина меньше 1 то R не уменьшился.
function d2FitRect(var R:TD2Rect; BoundsRect:TD2Rect):single;
var
  ratio:single;
begin
  Result:=1;
  if d2RectWidth(BoundsRect) * d2RectHeight(BoundsRect)=0 then Exit; //если площадь границы 0 то выходим

  if (d2RectWidth(R) / d2RectWidth(BoundsRect)) > (d2RectHeight(R) / d2RectHeight(BoundsRect)) then
    ratio:=d2RectWidth(R) / d2RectWidth(BoundsRect)
  else
    ratio:=d2RectHeight(R) / d2RectHeight(BoundsRect);

  if ratio < 1 then
  begin
    R:=d2Rect(0,0,d2RectWidth(R),d2RectHeight(R));
  end
  else
  begin
    R:=d2Rect(0,0,round(d2RectWidth(R) / ratio),round(d2RectHeight(R) / ratio));
  end;

  Result:=ratio;
  d2RectCenter(R,BoundsRect);
end;

//True - если ширина или выота прямоугольника Rect меньше или равна 0
function d2IsRectEmpty(Rect:TD2Rect):boolean;
begin
  Result:=(d2RectWidth(Rect) <= 0) or (d2RectHeight(Rect) <= 0);
end;

//True - если прямоугольники Rect1 и Rect2 пересекаются
function d2IntersectRect(const Rect1,Rect2:TD2Rect):boolean;
begin
  Result:=(Rect1.Left <= Rect2.Right) and (Rect1.Right >= Rect2.Left) and
          (Rect1.Top <= Rect2.Bottom) and (Rect1.Bottom >= Rect2.Top);
end;

//True - если прямоугольники SrcRect1 и SrcRect2 пересекаются
//в DestRect - прямоугольник на пересечении SrcRect1 и SrcRect2 или нулевой прямоугольник если пересечения нет
function d2IntersectRect(var DestRect:TD2Rect; const SrcRect1,SrcRect2:TD2Rect):boolean;
begin
  Result:=(SrcRect2.Left < SrcRect1.Right)
      and (SrcRect2.Right > SrcRect1.Left)
      and (SrcRect2.Top < SrcRect1.Bottom)
      and (SrcRect2.Bottom > SrcRect1.Top);

  if Result then begin
    DestRect.Left:=Max(SrcRect1.Left,SrcRect2.Left);
    DestRect.Top:=Max(SrcRect1.Top,SrcRect2.Top);
    DestRect.Right:=Min(SrcRect1.Right,SrcRect2.Right);
    DestRect.Bottom:=Min(SrcRect1.Bottom,SrcRect2.Bottom);
  end
  else
  begin
    DestRect.Left:=0;
    DestRect.Top:=0;
    DestRect.Right:=0;
    DestRect.Bottom:=0;
  end;
end;

//True - если точка P находится внутри прямоугольника Rect
function d2PtInRect(const P:TD2Point; const Rect:TD2Rect):boolean;
begin
  Result:=(P.X >= Rect.Left) and (P.X <= Rect.Right) and (P.Y >= Rect.Top) and (P.Y <= Rect.Bottom);
end;

//True - если точка заданная координатами PX,PY находится внутри прямоугольника,
//ограниченного RLeft,RRight,RTop,RBottom
function d2PtInRect(const PX,PY,RLeft,RRight,RTop,RBottom:single):boolean;
begin
  Result:=(PX >= RLeft) and (PX <= RRight) and (PY >= RTop) and (PY <= RBottom);
end;

//Возвращает стоковое представление прямоугольника R вида '(L,T,R,B)', где L, T, R, B
//строковые представления значений R.Left, R.Top, R.Right, R.Bottom соответственно
function d2RectToString(R:TD2Rect):Ansistring;
begin
  Result:='(' + d2FloatToStr(R.Left) + ',' + d2FloatToStr(R.Top) + ',' + d2FloatToStr(R.Right) + ',' +
    d2FloatToStr(R.Bottom) + ')';
end;

//Возвращает прямоугольник (объект TD2Rect), заданный строкой S вида '(L,T,R,B)', где L, T, R, B
//строковые представления значений R.Left, R.Top, R.Right, R.Bottom соответственно.
//В случае некоректной строки возвращает нулевой прямоугольник (все значения равны 0)
function d2StringToRect(S:Ansistring):TD2Rect;
begin
  try
    d2GetToken(S,',()');
    Result.Left:=d2StrToFloat(d2GetToken(S,',()'));
    Result.Top:=d2StrToFloat(d2GetToken(S,',()'));
    Result.Right:=d2StrToFloat(d2GetToken(S,',()'));
    Result.Bottom:=d2StrToFloat(d2GetToken(S,',()'));
  except
    Result:=d2Rect(0,0,0,0);
  end;
end;

//Возвращает точку (объект TD2Point), заданную значениями v.X и v.Y вектора v
function d2PointFromVector(const v:TD2Vector):TD2Point;
begin
  Result.x:=v.x;
  Result.y:=v.y;
end;

//Возвращает стоковое представление точки P вида '(X,Y)', где X и Y
//строковые представления значений P.X и P.Y соответственно
function d2PointToString(P:TD2Point):Ansistring;
begin
  Result:='(' + d2FloatToStr(P.X) + ',' + d2FloatToStr(P.Y) + ')';
end;

//Возвращает точку (объект TD2Point), заданный строкой S '(X,Y)', где X и Y
//строковые представления значений P.X и P.Y соответственно.
//В случае некоректной строки возвращает нулевую точку (все значения равны 0)
function d2StringToPoint(S:Ansistring):TD2Point;
begin
  try
    d2GetToken(S,',()');
    Result.X:=d2StrToFloat(d2GetToken(S,',()'));
    Result.Y:=d2StrToFloat(d2GetToken(S,',()'));
  except
    Result:=d2Point(0,0);
  end;
end;

//Возвращает матриу 3х3 произведение матриц M1(3х3) * M2(3х3)
function d2MatrixMultiply(const M1,M2:TD2Matrix):TD2Matrix;
begin
  Result.m11:=M1.m11 * M2.m11 + M1.m12 * M2.m21 + M1.m13 * M2.m31;
  Result.m12:=M1.m11 * M2.m12 + M1.m12 * M2.m22 + M1.m13 * M2.m32;
  Result.m13:=M1.m11 * M2.m13 + M1.m12 * M2.m23 + M1.m13 * M2.m33;
  Result.m21:=M1.m21 * M2.m11 + M1.m22 * M2.m21 + M1.m23 * M2.m31;
  Result.m22:=M1.m21 * M2.m12 + M1.m22 * M2.m22 + M1.m23 * M2.m32;
  Result.m23:=M1.m21 * M2.m13 + M1.m22 * M2.m23 + M1.m23 * M2.m33;
  Result.m31:=M1.m31 * M2.m11 + M1.m32 * M2.m21 + M1.m33 * M2.m31;
  Result.m32:=M1.m31 * M2.m12 + M1.m32 * M2.m22 + M1.m33 * M2.m32;
  Result.m33:=M1.m31 * M2.m13 + M1.m32 * M2.m23 + M1.m33 * M2.m33;
end;

//Возвращает детерминант матрицы M(3х3)
function d2MatrixDeterminant(const M:TD2Matrix):single;
begin
  Result:=M.M[X].V[X] * (M.M[Y].V[Y] * M.M[Z].V[Z] - M.M[Z].V[Y] * M.M[Y].V[Z])
          - M.M[X].V[Y] * (M.M[Y].V[X] * M.M[Z].V[Z] - M.M[Z].V[X] * M.M[Y].V[Z])
          + M.M[X].V[Z] * (M.M[Y].V[X] * M.M[Z].V[Y] - M.M[Z].V[X] * M.M[Y].V[Y]);
end;

procedure d2AdjointMatrix(var M:TD2Matrix);
var
   a1,a2,a3,
   b1,b2,b3,
   c1,c2,c3:single;
begin
   a1:= M.M[X].V[X]; a2:= M.M[X].V[Y]; a3:= M.M[X].V[Z];
   b1:= M.M[Y].V[X]; b2:= M.M[Y].V[Y]; b3:= M.M[Y].V[Z];
   c1:= M.M[Z].V[X]; c2:= M.M[Z].V[Y]; c3:= M.M[Z].V[Z];
   M.M[X].V[X]:= (b2*c3-c2*b3);
   M.M[Y].V[X]:=-(b1*c3-c1*b3);
   M.M[Z].V[X]:= (b1*c2-c1*b2);

   M.M[X].V[Y]:=-(a2*c3-c2*a3);
   M.M[Y].V[Y]:= (a1*c3-c1*a3);
   M.M[Z].V[Y]:=-(a1*c2-c1*a2);

   M.M[X].V[Z]:= (a2*b3-b2*a3);
   M.M[Y].V[Z]:=-(a1*b3-b1*a3);
   M.M[Z].V[Z]:= (a1*b2-b1*a2);
end;


procedure d2ScaleMatrix(var M:TD2Matrix; const factor:single);
var
   i :integer;
begin
   for i:=0 to 2 do begin
      M.M[I].V[0]:=M.M[I].V[0] * Factor;
      M.M[I].V[1]:=M.M[I].V[1] * Factor;
      M.M[I].V[2]:=M.M[I].V[2] * Factor;
   end;
end;

//Инвертирует матрицу M(3х3)
procedure d2InvertMatrix(var M:TD2Matrix);
var
   det :single;
begin
  det:=d2MatrixDeterminant(M);
  if Abs(Det) < EPSILON then
     M:=IdentityMatrix
  else
  begin
    d2AdjointMatrix(M);
    d2ScaleMatrix(M,1/det);
  end;
end;

//Возвращает вектор (объект TD2Vector), заданный значениями x,y,w
function d2Vector(const x,y:single; const w:single=1.0):TD2Vector;
begin
  Result.X:=x;
  Result.Y:=y;
  Result.W:=w;
end;

//Возвращает вектор (объект TD2Vector), заданный точкой P и значением w
function d2Vector(const P:TD2Point; const w:single=1.0):TD2Vector;
begin
  Result.X:=P.x;
  Result.Y:=P.y;
  Result.W:=w;
end;

function d2VectorTransform(const V:TD2Vector; const M:TD2Matrix):TD2Vector;
begin
  Result.V[X]:=V.V[X] * M.M[X].V[X] + V.V[Y] * M.M[Y].V[X] + V.V[Z] * M.M[Z].V[X];
  Result.V[Y]:=V.V[X] * M.M[X].V[Y] + V.V[Y] * M.M[Y].V[Y] + V.V[Z] * M.M[Z].V[Y];
  Result.V[2]:=1.0;
end;

function d2VectorAdd(const v1:TD2Vector; const v2:TD2Vector):TD2Vector;
begin
   Result.v[0]:=v1.v[0] + v2.v[0];
   Result.v[1]:=v1.v[1] + v2.v[1];
   Result.W:=1.0;
end;

function d2VectorSubtract(const v1:TD2Vector; const v2:TD2Vector):TD2Vector;
begin
   Result.v[0]:=v1.v[0] - v2.v[0];
   Result.v[1]:=v1.v[1] - v2.v[1];
   Result.W:=1.0;
end;

function d2VectorNorm(const v :TD2Vector) :single;
begin
   Result:=v.V[0]*v.V[0]+v.V[1]*v.V[1];
end;

function RSqrt(v :single) :single;
var
  R:double;
begin
  R:=Abs(V);
  if (R > 0) then
    Result:=1 / Sqrt(R)
  else
    Result:=1;
end;

function d2VectorNormalize(const v:TD2Vector):TD2Vector;
var
   invLen :single;
begin
   invLen:=RSqrt(Abs(d2VectorNorm(v)));
   Result.v[0]:=v.v[0]*invLen;
   Result.v[1]:=v.v[1]*invLen;
   Result.v[2]:=0.0;
end;

function d2VectorScale(const v:TD2Vector; factor :single):TD2Vector;
begin
   Result.v[0]:=v.v[0] * factor;
   Result.v[1]:=v.v[1] * factor;
   Result.W:=1;
end;

function d2VectorLength(const v :TD2Vector) :single;
begin
   Result:=Sqrt(d2Vectornorm(v));
end;

function d2VectorDotProduct(const V1,V2 :TD2Vector):single;
begin
   Result:=V1.V[0]*V2.V[0]+V1.V[1]*V2.V[1];
end;

function d2VectorAngleCosine(const V1,V2:TD2Vector):single;
begin
  if (d2VectorLength(V1) <> 0) and (d2VectorLength(V2) <> 0) then
  begin
    Result:=d2VectorDotProduct(V1,V2) / (d2VectorLength(V1) * d2VectorLength(V2));
    if Result > 1 then
      Result:=1;
  end
  else
    Result:=0;
end;

function d2VectorCrossProductZ(const V1,V2:TD2Vector):single;
begin
  // 3D Cross with Z=0
  Result:=v1.X*v2.Y - v1.Y*v2.X;
end;

function d2VectorCombine2(const V1,V2:TD2Vector; const F1,F2:single):TD2Vector;
begin
   Result.V[X]:=(F1 * V1.V[X]) + (F2 * V2.V[X]);
   Result.V[Y]:=(F1 * V1.V[Y]) + (F2 * V2.V[Y]);
   Result.W:=1.0;
end;

function d2VectorReflect(const V,N:TD2Vector):TD2Vector;
begin
  Result:=d2VectorCombine2(V,N,1,-2 * d2VectorDotProduct(V,N));
end;

function d2VectorAngle(const V,N:TD2Vector):single;
begin
  if d2VectorCrossProductZ(V,N) < 0 then
    Result:=d2RadToDeg(ArcCos(d2VectorAngleCosine(V,N)))
  else
    Result:=-d2RadToDeg(ArcCos(d2VectorAngleCosine(V,N)));
end;

function d2CreateRotationMatrix(angle:single):TD2Matrix;
var
  cosine,sine:single;
begin
  d2SinCos(angle,sine,cosine);

  Result.m11:=cosine;
  Result.m12:=sine;
  Result.m13:=0;
  Result.m21:=-sine;
  Result.m22:=cosine;
  Result.m23:=0;

  Result.m31:=0;
  Result.m32:=0;
  Result.m33:=1;
end;

function d2InterpolateSingle(const start,stop,t:single):single;
begin
  Result:=start + (stop - start) * t;
end;

function d2InterpolateRotation(start,stop,t :single) :single;
begin
  Result:=d2InterpolateSingle(start,stop,t);
end;

function d2InterpolateColor(start,stop:TD2Color; t :single):TD2Color;
begin
  TD2ColorRec(Result).A:=TD2ColorRec(start).A + Trunc((TD2ColorRec(stop).A - TD2ColorRec(start).A) * t);
  TD2ColorRec(Result).R:=TD2ColorRec(start).R + Trunc((TD2ColorRec(stop).R - TD2ColorRec(start).R) * t);
  TD2ColorRec(Result).G:=TD2ColorRec(start).G + Trunc((TD2ColorRec(stop).G - TD2ColorRec(start).G) * t);
  TD2ColorRec(Result).B:=TD2ColorRec(start).B + Trunc((TD2ColorRec(stop).B - TD2ColorRec(start).B) * t);
end;

function d2AppendColor(start,stop:TD2Color):TD2Color;
begin
  if TD2ColorRec(start).A + TD2ColorRec(stop).A < $FF then
    TD2ColorRec(Result).A:=TD2ColorRec(start).A + TD2ColorRec(stop).A
  else
    TD2ColorRec(Result).A:=$FF;
  if TD2ColorRec(start).R + TD2ColorRec(stop).R < $FF then
    TD2ColorRec(Result).R:=TD2ColorRec(start).R + TD2ColorRec(stop).R
  else
    TD2ColorRec(Result).R:=$FF;
  if TD2ColorRec(start).G + TD2ColorRec(stop).G < $FF then
    TD2ColorRec(Result).G:=TD2ColorRec(start).G + TD2ColorRec(stop).G
  else
    TD2ColorRec(Result).G:=$FF;
  if TD2ColorRec(start).B + TD2ColorRec(stop).B < $FF then
    TD2ColorRec(Result).B:=TD2ColorRec(start).B + TD2ColorRec(stop).B
  else
    TD2ColorRec(Result).B:=$FF;
end;

function d2SubtractColor(start,stop:TD2Color):TD2Color;
begin
  if TD2ColorRec(start).A - TD2ColorRec(stop).A < $FF then
    TD2ColorRec(Result).A:=TD2ColorRec(start).A - TD2ColorRec(stop).A
  else
    TD2ColorRec(Result).A:=$FF;
  if TD2ColorRec(start).R - TD2ColorRec(stop).R < $FF then
    TD2ColorRec(Result).R:=TD2ColorRec(start).R - TD2ColorRec(stop).R
  else
    TD2ColorRec(Result).R:=$FF;
  if TD2ColorRec(start).G - TD2ColorRec(stop).G < $FF then
    TD2ColorRec(Result).G:=TD2ColorRec(start).G - TD2ColorRec(stop).G
  else
    TD2ColorRec(Result).G:=$FF;
  if TD2ColorRec(start).B - TD2ColorRec(stop).B < $FF then
    TD2ColorRec(Result).B:=TD2ColorRec(start).B - TD2ColorRec(stop).B
  else
    TD2ColorRec(Result).B:=$FF;
end;

{ Interpolation ===============================================================}

{ interpolations }

function d2InterpolateBack(T,B,C,D,S:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      if S=0 then S:= 1.70158;
      T:= T / D;
      Result:= C * T * T * ((S + 1) * T - S) + B;
    end;
    d2AnimationOut:begin
      if S=0 then S:= 1.70158;
      T:= T / D - 1;
      Result:= C * (T * T * ((S + 1) * T + S) + 1) + B;
    end;
    d2AnimationInOut:begin
      if S=0 then S:= 1.70158;
      T:= T / (D / 2);
      if T < 1 then
      begin
        S:= S * 1.525;
        Result:= C / 2 * (T * T * ((S + 1) * T - S)) + B;
      end
      else
      begin
        T:= T - 2;
        S:= S * 1.525;
        Result:= C / 2 * (T * T * ((S + 1) * T + S) + 2) + B;
      end;
    end;
  end;
end;

function d2InterpolateBounce(T,B,C,D:Double; aType:TD2AnimationType):Double;
  function _EaseOut(T,B,C,D:Double):Double;
  begin
    T:= T / D;
    if T < 1 / 2.75 then
    begin
      Result:= C * (7.5625 * T * T) + B;
    end
    else if T < 2 / 2.72 then
    begin
      T:= T - (1.5 / 2.75);
      Result:= C * (7.5625 * T * T + 0.75) + B;
    end
    else if T < 2.5 / 2.75 then
    begin
      T:= T - (2.25 / 2.75);
      Result:= C * (7.5625 * T * T + 0.9375) + B;
    end
    else
    begin
      T:= T - (2.625 / 2.75);
      Result:= C * (7.5625 * T * T + 0.984375) + B;
    end;
  end;
  function _EaseIn(T,B,C,D:Double):Double;
  begin
    Result:= C - _EaseOut(D - T,0,C,D) + B;
  end;
begin
  case aType of
    d2AnimationIn:begin
      Result:= _EaseIn(T,B,C,D);
    end;
    d2AnimationOut:begin
      Result:= _EaseOut(T,B,C,D);
    end;
    d2AnimationInOut:begin
      if T < D / 2 then
        Result:= _EaseIn(T * 2,0,C,D) * 0.5 + B
      else
        Result:= _EaseOut(T * 2 - D,0,C,D) * 0.5 + C * 0.5 + B;
    end;
  end;
end;

function d2InterpolateCirc(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      T:= T / D;
      Result:= -C * (Sqrt(1 - T * T) - 1) + B;
    end;
    d2AnimationOut:begin
      T:= T / D - 1;
      Result:= C * Sqrt(1 - T * T) + B;
    end;
    d2AnimationInOut:begin
      T:= T / (D / 2);
      if T < 1 then
        Result:= -C / 2 * (Sqrt(1 - T * T) - 1) + B
      else
      begin
        T:= T - 2;
        Result:= C / 2 * (Sqrt(1 - T * T) + 1) + B;
      end;
    end;
  end;
end;

function d2InterpolateCubic(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      T:= T / D;
      Result:= C * T * T * T + B;
    end;
    d2AnimationOut:begin
      T:= T / D - 1;
      Result:= C * (T * T * T + 1) + B;
    end;
    d2AnimationInOut:begin
      T:= T / (D / 2);
      if T < 1 then
        Result:= C / 2 * T * T * T + B
      else
      begin
        T:= T - 2;
        Result:= C / 2 * (T * T * T + 2) + B;
      end;
    end;
  end;
end;

function d2InterpolateElastic(T,B,C,D,A,P:Double; aType:TD2AnimationType):Double;
var
  S:Double;
begin
  case aType of
    d2AnimationIn:begin
      if T=0 then
      begin
        Result:= B;
        Exit;
      end;
      T:= T / D;
      if T=1 then
      begin
        Result:= B + C;
        Exit;
      end;
      if P=0 then P:= D * 0.3;
      if (A=0) or (A < Abs(C)) then
      begin
        A:= C;
        S:= P / 4;
      end
      else
      begin
        S:= P / (2 * Pi) * ArcSin(C / A);
      end;
      T:= T - 1;
      Result:= -(A * Power(2,10 * T) * Sin((T * D - S) * (2 * Pi)/P)) + B;
    end;
    d2AnimationOut:begin
      if T=0 then
      begin
        Result:= B;
        Exit;
      end;
      T:= T / D;
      if T=1 then
      begin
        Result:= B + C;
        Exit;
      end;
      if P=0 then P:= D * 0.3;
      if (A=0) or (A < Abs(C)) then
      begin
        A:= C;
        S:= P / 4;
      end
      else
      begin
        S:= P / (2 * Pi) * ArcSin(C / A);
      end;
      Result:= A * Power(2,-10 * T) * Sin((T * D - S) * (2 * Pi) / P) + C + B;
    end;
    d2AnimationInOut:begin
      if T=0 then
      begin
        Result:= B;
        Exit;
      end;
      T:= T / (D / 2);
      if T=2 then
      begin
        Result:= B + C;
        Exit;
      end;
      if P=0 then P:= D * (0.3 * 1.5);
      if (A=0) or (A < Abs(C)) then
      begin
        A:= C;
        S:= P / 4;
      end
      else
      begin
        S:= P / (2 * Pi) * ArcSin(C / A);
      end;

      if T < 1 then
      begin
        T:= T - 1;
        Result:= -0.5 * (A * Power(2,10 * T) * Sin((T * D -S) * (2 * Pi) / P)) + B;
      end
      else
      begin
        T:= T - 1;
        Result:= A * Power(2,-10 * T) * Sin((T * D - S) * (2 * Pi) / P) * 0.5 + C + B;
      end;
    end;
  end;
end;

function d2InterpolateExpo(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      If T=0 Then
        Result:=B
      else
        Result:=C * Power(2,10 * (T / D - 1)) + B;
    end;
    d2AnimationOut:begin
      If T=D then
        Result:=  B + C
      else
        Result:=  C * (-Power(2,-10 * T / D) + 1) + B;
    end;
    d2AnimationInOut:begin
      if T=0 then
      begin
        Result:= B;
        Exit;
      end;
      if T=D then
      begin
        Result:= B + C;
        Exit;
      end;
      T:= T / (D / 2);
      if T < 1 then
        Result:= C / 2 * Power(2,10 * (T - 1)) + B
      else
      begin
        T:= T - 1;
        Result:= C / 2 * (-Power(2,-10 * T) + 2) + B;
      end;
    end;
  end;
end;

function d2InterpolateLinear(T,B,C,D:Double):Double;
begin
  Result:= C * T / D + B;
end;

function d2InterpolateQuad(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      T:= T / D;
      Result:= C * T * T + B;
    end;
    d2AnimationOut:begin
      T:= T / D;
      Result:= -C * T * (T - 2) + B;
    end;
    d2AnimationInOut:begin
      T:= T / (D / 2);

      if T < 1 then
        Result:= C / 2 * T * T + B
      else
      begin
        T:= T - 1;
        Result:= -C / 2 * (T * (T - 2) - 1) + B;
      end;
    end;
  end;
end;

function d2InterpolateQuart(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      T:= T / D;
      Result:= C * T * T * T * T + B;
    end;
    d2AnimationOut:begin
      T:= T / D - 1;
      Result:= -C * (T * T * T * T - 1) + B;
    end;
    d2AnimationInOut:begin
      T:= T / (D / 2);
      if T < 1 then
        Result:= C / 2 * T * T * T * T +B
      else
      begin
        T:= T - 2;
        Result:= -C / 2 * (T * T * T * T - 2) + B;
      end;
    end;
  end;
end;

function d2InterpolateQuint(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      T:= T / D;
      Result:= C * T * T * T * T * T + B;
    end;
    d2AnimationOut:begin
      T:= T / D - 1;
      Result:= C * (T * T * T * T * T + 1) + B;
    end;
    d2AnimationInOut:begin
      T:= T / (D / 2);
      if T < 1 then
        Result:= C / 2 * T * T * T * T * T + B
      else
      begin
        T:= T - 2;
        Result:= C / 2 * (T * T * T * T * T + 2) + B;
      end;
    end;
  end;
end;

function d2InterpolateSine(T,B,C,D:Double; aType:TD2AnimationType):Double;
begin
  case aType of
    d2AnimationIn:begin
      Result:= -C * Cos(T / D * (Pi/2)) + C + B;
    end;
    d2AnimationOut:begin
      Result:= C * Sin(T / D * (Pi/2)) + B;
    end;
    d2AnimationInOut:begin
      Result:= -C / 2 * (Cos(Pi * T / D) - 1) + B;
    end;
  end;
end;
    
