{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Bounds ===============================================

constructor TD2Bounds.Create(const ADefaultValue:TD2Rect);
begin
  inherited Create;
  FDefaultValue:=ADefaultValue;
  Rect:=FDefaultValue;
end;

procedure TD2Bounds.Assign(Source:TPersistent);
begin
  if Source is TD2Bounds then
  begin
    Rect:=TD2Bounds(Source).Rect;
  end
  else
    inherited
end;

function TD2Bounds.GetRect:TD2Rect;
begin
  Result:=d2Rect(FLeft,FTop,FRight,FBottom);
end;

function TD2Bounds.MarginRect(const R:TD2Rect):TD2Rect;
begin
  Result:=d2Rect(R.Left + FLeft,R.Top + FTop,R.Right - FRight,R.Bottom - FBottom);
end;

function TD2Bounds.PaddinRect(const R:TD2Rect):TD2Rect;
begin
  Result:=d2Rect(R.Left - FLeft,R.Top - FTop,R.Right + FRight,R.Bottom + FBottom);
end;

function TD2Bounds.Width:single;
begin
  Result:=d2RectWidth(Rect);
end;

function TD2Bounds.Height:single;
begin
  Result:=d2RectHeight(Rect);
end;

function TD2Bounds.MarginEmpty:boolean;
begin
  Result:=(FLeft=0) and (FTop=0) and (FRight=0) and (FBottom=0);
end;

function TD2Bounds.Empty:boolean;
begin
  Result:=d2IsRectEmpty(Rect)
end;

procedure TD2Bounds.SetBottom(const Value:single);
begin
  if FBottom <> Value then
  begin
    FBottom:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Bounds.SetLeft(const Value:single);
begin
  if FLeft <> Value then
  begin
    FLeft:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Bounds.SetRight(const Value:single);
begin
  if FRight <> Value then
  begin
    FRight:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Bounds.SetTop(const Value:single);
begin
  if FTop <> Value then
  begin
    FTop:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Bounds.SetRect(const Value:TD2Rect);
begin
  if (FLeft <> Value.Left) or (FTop <> Value.Top) or (FRight <> Value.Right) or (FBottom <> Value.Bottom) then
  begin
    FLeft:=Value.Left;
    FTop:=Value.Top;
    FRight:=Value.Right;
    FBottom:=Value.Bottom;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Bounds.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('Rect',ReadRect,WriteRect,(FLeft <> DefaultValue.Left) or (FTop <> DefaultValue.Top) or
    (FRight <> DefaultValue.Right) or (FBottom <> DefaultValue.Bottom));
end;

procedure TD2Bounds.ReadRect(Reader:TReader);
begin
  Rect:=d2StringToRect(Reader.ReadString);
end;

procedure TD2Bounds.WriteRect(Writer:TWriter);
begin
  Writer.WriteString(d2RectToString(Rect));
end;

//=============== TD2Position ======================================================

constructor TD2Position.Create(const ADefaultValue:TD2Point);
begin
  inherited Create;
  FDefaultValue:=ADefaultValue;
  FX:=FDefaultValue.X;
  FY:=FDefaultValue.Y;
end;

procedure TD2Position.Assign(Source:TPersistent);
begin
  if Source is TD2Position then
  begin
    Point:=TD2Position(Source).Point;
  end
  else
    inherited
end;

function TD2Position.Empty:boolean;
begin
  Result:=(FX=0) and (FY=0);
end;

procedure TD2Position.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('Point',ReadPoint,WritePoint,(FX <> DefaultValue.X) or (FY <> DefaultValue.Y));
end;

procedure TD2Position.ReadPoint(Reader:TReader);
begin
  Point:=d2StringToPoint(Reader.ReadString);
end;

procedure TD2Position.WritePoint(Writer:TWriter);
begin
  Writer.WriteString(d2PointToString(Point));
end;

function TD2Position.GetPoint:TD2Point;
begin
  Result:=d2Point(Fx,Fy);
end;

procedure TD2Position.SetPoint(const Value:TD2Point);
begin
  Fx:=Value.X;
  Fy:=Value.Y;
  if Assigned(OnChange) then
    OnChange(Self);
end;

procedure TD2Position.SetX(const Value:single);
begin
  if FX <> Value then
  begin
    FX:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

procedure TD2Position.SetY(const Value:single);
begin
  if FY <> Value then
  begin
    FY:=Value;
    if Assigned(OnChange) then
      OnChange(Self);
  end;
end;

//=============== TD2Transform ================================================================}

constructor TD2Transform.Create;
begin
  inherited ;
  FMatrix:=IdentityMatrix;
  FPosition:=TD2Position.Create(d2Point(0,0));
  FPosition.OnChange:=MatrixChanged;
  FScale:=TD2Position.Create(d2Point(1,1));
  FScale.OnChange:=MatrixChanged;
  FSkew:=TD2Position.Create(d2Point(0,0));
  FSkew.OnChange:=MatrixChanged;
  FRotateCenter:=TD2Position.Create(d2Point(0.5,0.5));
  FRotateCenter.OnChange:=MatrixChanged;
end;

destructor TD2Transform.Destroy;
begin
  FRotateCenter.Free;
  FScale.Free;
  FSkew.Free;
  FPosition.Free;
  inherited ;
end;

procedure TD2Transform.Assign(Source:TPersistent);
begin
  if Source is TD2Transform then
  begin
    FPosition.FX:=TD2Transform(Source).Position.FX;
    FPosition.FY:=TD2Transform(Source).Position.FY;
    FScale.FX:=TD2Transform(Source).Scale.FX;
    FScale.FY:=TD2Transform(Source).Scale.FY;
    FSkew.FX:=TD2Transform(Source).Skew.FX;
    FSkew.FY:=TD2Transform(Source).Skew.FY;
    FRotateCenter.FX:=TD2Transform(Source).RotateCenter.FX;
    FRotateCenter.FY:=TD2Transform(Source).RotateCenter.FY;
    MatrixChanged(Self);
  end
  else
    inherited
end;

procedure TD2Transform.MatrixChanged(Sender:TObject);
begin
  FMatrix:=IdentityMatrix;
  FMatrix.m31:=FPosition.X;
  FMatrix.m32:=FPosition.Y;
  FMatrix.m13:=FSkew.X;
  FMatrix.m23:=FSkew.Y;
  FMatrix.m11:=FScale.X;
  FMatrix.m22:=FScale.Y;
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
    FMatrix:=d2MatrixMultiply(d2CreateRotationMatrix(d2DegToRad(FRotateAngle)),FMatrix);
  end;
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;

procedure TD2Transform.SetRotateAngle(const Value:single);
begin
  if FRotateAngle <> Value then
  begin
    FRotateAngle:=Value;
  end;
end;


function TD2Position.GetVector:TD2Vector;
begin
  Result:=d2Vector(FX,FY);
end;

procedure TD2Position.SetVector(const Value:TD2Vector);
begin
  Point:=d2Point(Value.X,Value.Y);
end;

procedure TD2Position.Reflect(const Normal:TD2Vector);
begin
  Vector:=d2VectorReflect(Vector,Normal);
end;


//=============== TD2GradientPoint =======================================

procedure TD2GradientPoint.Assign(Source:TPersistent);
begin
  if Source is TD2GradientPoint then
  begin
    FColor:=TD2GradientPoint(Source).FColor;
    FOffset:=TD2GradientPoint(Source).FOffset;
  end
  else
    inherited;
end;

constructor TD2GradientPoint.Create(ACollection:TCollection);
begin
  inherited Create(ACollection);
end;

function TD2GradientPoint.GetColor:string;
begin
  Result:=d2ColorToStr(FColor);
end;

procedure TD2GradientPoint.SetColor(const Value:string);
begin
  FColor:=d2StrToColor(Value);
end;

//=============== TD2GradientPoints ====================================

function TD2GradientPoints.GetPoint(Index:integer):TD2GradientPoint;
begin
  Result:=TD2GradientPoint(Items[Index]);
end;

//=============== TD2Gradient =======================================

constructor TD2Gradient.Create;
begin
  inherited ;
  FStartPosition:=TD2Position.Create(d2Point(0,0));
  FStartPosition.OnChange:=PositionChanged;
  FStopPosition:=TD2Position.Create(d2Point(0,1));
  FStopPosition.OnChange:=PositionChanged;
  FRadialTransform:=TD2Transform.Create;
  FRadialTransform.OnChanged:=PositionChanged;
  FPoints:=TD2GradientPoints.Create(TD2GradientPoint);
  with TD2GradientPoint(FPoints.Add) do
  begin
    IntColor:=$FF000000;
  end;
  with TD2GradientPoint(FPoints.Add) do
  begin
    IntColor:=$FFFFFFFF;
    Offset:=1;
  end;
end;

procedure TD2Gradient.Assign(Source:TPersistent);
var
  SaveChanged:TNotifyEvent;
begin
  if Source is TD2Gradient then
  begin
    SaveChanged:=FOnChanged;
    FOnChanged:=nil;
    FPoints.Clear;
    FPoints.Assign(TD2Gradient(Source).FPoints);
    FStyle:=TD2Gradient(Source).Style;
    if FStyle=d2LinearGradient then
    begin
      FStopPosition.Assign(TD2Gradient(Source).StopPosition);
      FStartPosition.Assign(TD2Gradient(Source).StartPosition);
    end
    else
    begin
      FRadialTransform.Assign(TD2Gradient(Source).RadialTransform);
    end;
    FOnChanged:=SaveChanged;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
    inherited;
end;

destructor TD2Gradient.Destroy;
begin
  FStartPosition.Free;
  FStopPosition.Free;
  FRadialTransform.Free;
  FPoints.Free;
  inherited;
end;

procedure TD2Gradient.Change;
begin
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;

function TD2Gradient.InterpolateColor(Offset:single):TD2Color;
var
  i:integer;
begin
  if FPoints.Count > 1 then
  begin
    if Offset < 0 then
      Offset:=0;
    if Offset > 1 then
      Offset:=1;
    if Offset < FPoints[0].Offset then
    begin
      Result:=Points[0].IntColor;
      Exit;
    end;
    if Offset > FPoints[FPoints.Count - 1].Offset then
    begin
      Result:=FPoints[FPoints.Count - 1].IntColor;
      Exit;
    end;
    for i:=0 to FPoints.Count - 2 do
    begin
      if (Offset < Points[i].Offset) then Continue;
      if Points[i + 1].Offset - Points[i].Offset <= 0 then
        Result:=Points[i].IntColor
      else
        if (i=FPoints.Count - 2) and (Offset > Points[Points.Count - 1].Offset) then // last
          Result:=Points[Points.Count - 1].IntColor
        else
          Result:=d2InterpolateColor(Points[i].IntColor,Points[i + 1].IntColor,(Offset - Points[i].Offset) / (Points[i + 1].Offset - Points[i].Offset));
    end;
  end
  else
    Result:=0;
end;

procedure TD2Gradient.PositionChanged(Sender:TObject);
begin
  if Assigned(FOnChanged) then
    FOnChanged(Self);
end;

function TD2Gradient.isLinearStored:boolean;
begin
  Result:=FStyle=d2LinearGradient;
end;

function TD2Gradient.isRadialStored:boolean;
begin
  Result:=FStyle=d2RadialGradient;
end;

procedure TD2Gradient.SetRadialTransform(const Value:TD2Transform);
begin
  FRadialTransform.Assign(Value);
end;

procedure TD2Gradient.SetStartPosition(const Value:TD2Position);
begin
  FStartPosition.Assign(Value);
end;

procedure TD2Gradient.SetStopPosition(const Value:TD2Position);
begin
  FStopPosition.Assign(Value);
end;

procedure TD2Gradient.SetColor(const Value:string);
begin
  if FPoints.Count > 0 then
    Points[0].Color:=Value;
end;

procedure TD2Gradient.SetColor1(const Value:string);
begin
  if FPoints.Count > 1 then
    Points[1].Color:=Value;
end;

procedure TD2Gradient.SetStyle(const Value:TD2GradientStyle);
begin
  if FStyle <> Value then
  begin
    FStyle:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;
           
                 
