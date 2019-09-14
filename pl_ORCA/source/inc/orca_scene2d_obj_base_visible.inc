{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Visual ======================================================

constructor TD2Visual.Create;
begin
  inherited ;
end;

procedure TD2Visual.Assign(Source:TPersistent);
begin
  if Source is TD2Visual then
  begin
    VisualObject:=TD2Visual(Source).VisualObject;
  end
  else
    inherited;
end;

destructor TD2Visual.Destroy;
begin
  if FVisualObject <> nil then
  begin
    FVisualObject.RemoveFreeNotify(Self);
    FVisualObject:=nil;
  end;
  inherited;
end;

procedure TD2Visual.SetVisualObject(const Value:TD2VisualObject);
begin
  if FVisualObject <> Value then
  begin
    if FVisualObject <> nil then
      FVisualObject.RemoveFreeNotify(Self);
    FVisualObject:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
    if FVisualObject <> nil then
      FVisualObject.AddFreeNotify(Self);
  end;
end;
        
//=============== TD2VisualObject ========================================================

constructor TD2VisualObject.Create(AOwner:TComponent);
begin
  inherited;
  FDragClipChildren:=false;		//Added by GoldenFox
  FTabOrder:=-1;
  FEnabled:=true;
  FRecalcEnabled:=true;
  FOpacity:=1;
  FLocalMatrix:=IdentityMatrix;
  FPosition:=TD2Position.Create(d2Point(0,0));
  FPosition.OnChange:=MatrixChanged;
  FScale:=TD2Position.Create(d2Point(1,1));
  FScale.OnChange:=MatrixChanged;
  FSkew:=TD2Position.Create(d2Point(0,0));
  FSkew.OnChange:=MatrixChanged;
  FRotateCenter:=TD2Position.Create(d2Point(0.5,0.5));
  FRotateCenter.OnChange:=MatrixChanged;
  FMargins:=TD2Bounds.Create(d2Rect(0,0,0,0));
  FMargins.OnChange:=MarginsChanged;
  FPadding:=TD2Bounds.Create(d2Rect(0,0,0,0));
  FPadding.OnChange:=PaddingChanged;
  FWidth:=50;
  FLastWidth:=FWidth;
  FHeight:=50;
  FLastHeight:=FHeight;
  FVisible:=true;
  FHitTest:=true;
  FRecalcAbsolute:=true;
  FRecalcOpacity:=true;
  FUpdateEffects:=true;
  FRecalcUpdateRect:=true;
  FCanFocused:=false;
  FCanClipped:=true;
end;

destructor TD2VisualObject.Destroy;
//var i: integer;
begin
  //if FTabList <> nil then
  //  for i:=0 to FTabList.Count-1 do
  //    TD2Object(FTabList[i]).Free;
  FreeAndNil(FTabList);
  if FEffectBitmap <> nil then
    FreeAndNil(FEffectBitmap);
  FMargins.Free;
  FPadding.Free;
  FRotateCenter.Free;
  FScale.Free;
  FSkew.Free;
  FPosition.Free;
  inherited;
end;

procedure TD2VisualObject.Loaded;
begin
  inherited;
  FLastWidth:=FWidth;
  FLastHeight:=FHeight;
  MatrixChanged(Self);
  if (FChildren <> nil) and (FChildren.Count > 0) then
    Realign;
  FixupTabList;
end;

procedure TD2VisualObject.DeleteChildren;
begin
  inherited ;
  if FTabList <> nil then
    FreeAndNil(FTabList);
end;

procedure TD2VisualObject.Notification(AComponent:TComponent;
      Operation:TOperation);
begin
  if (Operation=opRemove) and (AComponent=FPopupMenu) then
    FPopupMenu:=nil;

  if (Operation=opRemove) and (AComponent=FPopup) then
    FPopup:=nil;
end;

{ matrix }

procedure TD2VisualObject.MatrixChanged(Sender:TObject);
var
  RotMatrix:TD2Matrix;
  M1,M2:TD2Matrix;
begin
  if (FScene <> nil) and (not FScene.GetDisableUpdate) and (not FInPaintTo) and (FUpdating=0) then
    Repaint;
  FLocalMatrix:=IdentityMatrix;
  FLocalMatrix.m31:=FPosition.X;
  FLocalMatrix.m32:=FPosition.Y;
  FLocalMatrix.m11:=FScale.X;
  FLocalMatrix.m22:=FScale.Y;
  if FRotateAngle <> 0 then
  begin
    M1:=IdentityMatrix;
    M1.m31:=-FRotateCenter.X * FWidth{ * FScale.X};
    M1.m32:=-FRotateCenter.Y * FHeight{ * FScale.Y};
    M2:=IdentityMatrix;
    M2.m31:=FRotateCenter.X * FWidth{ * FScale.X};
    M2.m32:=FRotateCenter.Y * FHeight{ * FScale.Y};
    RotMatrix:=d2MatrixMultiply(M1,d2MatrixMultiply(d2CreateRotationMatrix(d2DegToRad(FRotateAngle)),M2));
    FLocalMatrix:=d2MatrixMultiply(RotMatrix,FLocalMatrix);
  end;
  RecalcAbsolute;
  RecalcUpdateRect;
  UpdateEffects;
  if (FScene <> nil) and (not FScene.GetDisableUpdate) and (not FInPaintTo) and (FUpdating=0)then
    Repaint;
end;

procedure TD2VisualObject.RecalcUpdateRect;
var
  i:integer;
begin
  if (Parent <> nil) and (Parent.IsVisual) and not (Parent.Visual.ClipChildren) and
     ((Position.X < 0) or (Position.Y < 0) or
      (Position.X + Width < Parent.Visual.Width) or
      (Position.Y + Height < Parent.Visual.Height))
    then
      Parent.Visual.FRecalcUpdateRect:=true;

  FRecalcUpdateRect:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).IsVisual then Continue;
      TD2VisualObject(FChildren[i]).RecalcUpdateRect;
    end;
end;

function TD2VisualObject.GetUpdateRect:TD2Rect;
var
  R:TD2Rect;
  P:TD2Object;
  i:integer;
begin
  if FRecalcUpdateRect then
  begin
    FRecalcUpdateRect:=false;
    FUpdating:=FUpdating + 1;
    FUpdateRect:=AbsoluteRect;
    if (FScene <> nil) and not (FScene.GetDisableUpdate) then
    begin
      if not (csLoading in ComponentState) then
      begin
        P:=Parent;
        while (P <> nil) and (P.IsVisual) do
        begin
          if TD2VisualObject(P).ClipChildren then
            d2IntersectRect(FUpdateRect,FUpdateRect,TD2VisualObject(P).UpdateRect);
          P:=P.Parent;
        end;
        { focused }
        if CanFocused and IsFocused then
          d2InflateRect(FUpdateRect,5,5);
        if (FScene <> nil) and (Self=FScene.GetSelected) then
        begin
          d2InflateRect(FUpdateRect,(cnGripSize) + 1,(cnGripSize) + 1);
          FUpdateRect.Top:=FUpdateRect.Top - cnRotSize - cnGripSize;
        end;
        if (FScene <> nil) and (FScene.GetDesignPlaceObject=Self) then
        begin
          d2InflateRect(FUpdateRect,1,1);
          FUpdateRect.Top:=FUpdateRect.Top - 20;
          if d2RectWidth(FUpdateRect) < 160 then
            FUpdateRect.Right:=FUpdateRect.Left + 160;
        end;
        { Effects }
        if FHasEffect and not ClipChildren then
        begin
          R:=GetEffectsRect;
          with R do
            R:=d2NormalizeRect([LocaltoAbsolute(d2Point(Left,Top)),LocaltoAbsolute(d2Point(Right,Top)),
              LocaltoAbsolute(d2Point(Right,Bottom)),LocaltoAbsolute(d2Point(Left,Bottom))]);
          FUpdateRect:=d2UnionRect(FUpdateRect,R);
        end;
        { Children }
        if not ClipChildren and (FChildren <> nil) then
        begin
          for i:=0 to FChildren.Count - 1 do
          begin
            if not TD2Object(FChildren[i]).IsVisual then Continue;
            if not TD2VisualObject(FChildren[i]).Visible then Continue;
            R:=TD2VisualObject(FChildren[i]).UpdateRect;
            FUpdateRect:=d2UnionRect(FUpdateRect,R);
          end;
        end;
      end;
    end;
    FUpdating:=FUpdating - 1;
  end;
  Result:=FUpdateRect;
end;

function TD2VisualObject.GetChildrenRect:TD2Rect;
var
  i:integer;
begin
  Result:=AbsoluteRect;
  { children }
  if not ClipChildren and (FChildren <> nil) then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).IsVisual and (TD2VisualObject(FChildren[i]).Visible) then
        Result:=d2UnionRect(Result,TD2VisualObject(FChildren[i]).GetChildrenRect);
end;

function TD2VisualObject.GetAbsoluteWidth:single;
var
  V:TD2Vector;
begin
  V:=LocalToAbsoluteVector(d2Vector(Width,Height));
  Result:=V.X;
end;

function TD2VisualObject.GetAbsoluteHeight:single;
var
  V:TD2Vector;
begin
  V:=LocalToAbsoluteVector(d2Vector(Width,Height));
  Result:=V.Y;
end;

function TD2VisualObject.GetAbsoluteScale:TD2Point;
var
  P:TD2Object;
begin
  Result:=Scale.Point;
  P:=Parent;
  while P <> nil do
  begin
    if P.IsVisual then
    begin
      Result.X:=Result.X * P.Visual.Scale.X;
      Result.Y:=Result.Y * P.Visual.Scale.Y;
    end;
    P:=P.Parent;
  end;
end;

function TD2VisualObject.GetChildrenMatrix:TD2Matrix;
begin
  Result:=IdentityMatrix;
end;

function TD2VisualObject.GetAbsoluteMatrix:TD2Matrix;
begin
  if FRecalcAbsolute then
  begin
    if (FParent <> nil) and (FParent.IsVisual) then
    begin
      FAbsoluteMatrix:=d2MatrixMultiply(d2MatrixMultiply(FLocalMatrix,FParent.Visual.GetChildrenMatrix),FParent.Visual.AbsoluteMatrix);
    end
    else
      FAbsoluteMatrix:=FLocalMatrix;

    Result:=FAbsoluteMatrix;
    FRecalcAbsolute:=false;
    if (FScene <> nil) and (not FScene.GetDisableUpdate) and (not FInPaintTo) and (FUpdating=0) then
      Repaint;
  end
  else
  begin
    Result:=FAbsoluteMatrix;
  end;
end;

function TD2VisualObject.GetInvertAbsoluteMatrix:TD2Matrix;
begin
  Result:=AbsoluteMatrix;
  d2InvertMatrix(Result);
end;

procedure TD2VisualObject.RecalcAbsoluteNow;
var
  i:integer;
  Child:TD2VisualObject;
begin
  AbsoluteMatrix; // recalc
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).isVisual then Continue;
      Child:=TD2VisualObject(FChildren[i]);
      TD2VisualObject(Child).RecalcAbsoluteNow;
    end;
end;

procedure TD2VisualObject.RecalcAbsolute;
var
  i:integer;
  Child:TD2VisualObject;
begin
  FRecalcAbsolute:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).isVisual then Continue;
      Child:=TD2VisualObject(FChildren[i]);
      TD2VisualObject(Child).RecalcAbsolute;
    end;
end;

function TD2VisualObject.AbsoluteToLocalVector(P:TD2Vector):TD2Vector;
begin
  P.W:=0;
  Result:=d2VectorTransform(P,InvertAbsoluteMatrix);
end;

function TD2VisualObject.LocalToAbsoluteVector(P:TD2Vector):TD2Vector;
begin
  P.W:=0;
  Result:=d2VectorTransform(P,AbsoluteMatrix);
end;

function TD2VisualObject.AbsoluteToLocal(P:TD2Point):TD2Point;
var
  V:TD2Vector;
begin
  V.X:=P.X;
  V.Y:=P.Y;
  V.W:=1;
  V:=d2VectorTransform(V,InvertAbsoluteMatrix);
  Result.X:=V.X;
  Result.Y:=V.Y;
end;

function TD2VisualObject.LocalToAbsolute(P:TD2Point):TD2Point;
var
  V:TD2Vector;
begin
  V.X:=P.X;
  V.Y:=P.Y;
  V.W:=1;
  V:=d2VectorTransform(V,AbsoluteMatrix);
  Result:=d2Point(V.X,V.Y);
end;

{ Opacity }

function TD2VisualObject.GetAbsoluteOpacity:single;
begin
  if FRecalcOpacity then
  begin
    if (FParent <> nil) and (FParent.IsVisual) then
      FAbsoluteOpacity:=FOpacity * FParent.Visual.AbsoluteOpacity
    else
      FAbsoluteOpacity:=FOpacity;

    if not AbsoluteEnabled and (FScene <> nil) and ((FScene.GetRoot <> Self) and (FScene.GetRoot <> Parent)) then
      FAbsoluteOpacity:=FAbsoluteOpacity * 0.8;

    Result:=FAbsoluteOpacity;

    FRecalcOpacity:=false;
  end
  else
  begin
    Result:=FAbsoluteOpacity;
  end;
end;

procedure TD2VisualObject.RecalcOpacity;
var
  i:integer;
  Child:TD2VisualObject;
begin
  FRecalcOpacity:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).isVisual then Continue;
      Child:=TD2VisualObject(FChildren[i]);
      TD2VisualObject(Child).RecalcOpacity;
    end;
end;

{ methods }

procedure TD2VisualObject.CreateCaret;
var
  A:TD2FloatAnimation;
begin
  if FCaret=nil then
  begin
    FCaret:=TD2Rectangle.Create(Self);
    FCaret.Parent:=Self;
    FCaret.Width:=3;
    FCaret.Height:=20;
    FCaret.Stored:=false;
    FCaret.HitTest:=false;
    FCaret.Locked:=true;
    TD2Rectangle(FCaret).Fill.Color:=vcBlue;
    TD2Rectangle(FCaret).Stroke.Style:=d2BrushNone;
    if Assigned(FScene) and (FScene.GetAnimatedCaret) then
    begin
      A:=TD2FloatAnimation.Create(Self);
      A.BindingName:='caret';
      A.Parent:=FCaret;
      A.StartValue:=1;
      A.Duration:=0.1;
      A.PropertyName:='Opacity';
      A.StopValue:=0;
      A.AutoReverse:=true;
      A.Loop:=true;
      A.Enabled:=false;
    end;
    FCaret.Visible:=false;
  end;
end;

procedure TD2VisualObject.ShowCaretProc;
begin
  if FCaret=nil then
    CreateCaret;

  FCaret.Visible:=true;
  if (FCaret.FindBinding('caret') <> nil) and not TD2FloatAnimation(FCaret.FindBinding('caret')).Running then
    TD2FloatAnimation(FCaret.FindBinding('caret')).Start;
end;

procedure TD2VisualObject.SetCaretPos(const APoint:TD2Point);
begin
  if FCaret=nil then
    CreateCaret;
  FCaret.Position.Point:=d2Point(round(APoint.x),round(APoint.y));
end;

procedure TD2VisualObject.SetCaretSize(const ASize:TD2Point);
begin
  if FCaret=nil then
    CreateCaret;
  FCaret.Width:=ASize.X;
  FCaret.Height:=ASize.Y;
end;

procedure TD2VisualObject.SetCaretColor(const AColor:TD2Color);
begin
  if FCaret=nil then
    CreateCaret;
  TD2Rectangle(FCaret).Fill.SolidColor:=AColor;
end;

procedure TD2VisualObject.HideCaret;
begin
  if FCaret <> nil then
    FCaret.Visible:=false;
end;

function TD2VisualObject.PointInObject(X,Y:single):boolean;
var
  P:TD2Point;
begin
  Result:=false;
  P:=AbsoluteToLocal(d2Point(X,Y));
  if (P.X > 0) and (P.X < Width) and
     (P.Y > 0) and (P.Y < Height) then
  begin
    Result:=true;
  end;
end;

function TD2VisualObject.CheckHitTest(const AHitTest:boolean):boolean;
begin
  Result:=FHitTest;
  if ((Scene <> nil) and (Scene.GetDesignTime)) then
    Result:=true;
  if (((Scene <> nil) and (Scene.GetDesignTime))) and FLocked then
    Result:=false;
  if (((Scene <> nil) and (Scene.GetDesignTime))) and FDesignHide then
    Result:=false;
end;

function TD2VisualObject.FindTarget(Shift: TShiftState; const APoint:TD2Point; const Data:TD2DragObject):TD2VisualObject;
var
  i:integer;
  Obj,NewObj:TD2VisualObject;
  Accept:boolean;
begin
  if not Visible and not (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    Result:=nil;
    Exit;
  end;
  if (Self is TD2Control) and not TD2Control(Self).AbsoluteEnabled and not (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    Result:=nil;
    Exit;
  end;

  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
    begin
      if not TD2Object(FChildren[i]).IsVisual then Continue;
      Obj:=TD2VisualObject(FChildren[i]);
      if (not Obj.Visible) and not (((FScene <> nil) and FScene.GetDesignTime)) then Continue;
      if FDesignHide and (((FScene <> nil) and FScene.GetDesignTime)) then Continue;
      //if ClipChildren and not PointInObject(APoint.X, APoint.Y) then Continue;		//Delete by GoldenFox
	  if DragClipChildren and not PointInObject(APoint.X, APoint.Y) then Continue;		//Added by GoldenFox

      NewObj:=Obj.FindTarget(Shift,APoint,Data);
      if NewObj <> nil then
      begin
        Result:=NewObj;
        Exit;
      end;
    end;

  Result:=nil;
  Accept:=false;
  DragOver(Data,Shift,APoint,Accept);
  if PointInObject(APoint.X,APoint.Y) and CheckHitTest(HitTest) and (Accept) then
    Result:=Self;
end;

function TD2VisualObject.ObjectByPoint(X,Y:single):TD2VisualObject;
var
  i:integer;
  Obj,NewObj:TD2VisualObject;
begin
  if not Visible and not (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    Result:=nil;
    Exit;
  end;
  if (Self is TD2Control) and not TD2Control(Self).AbsoluteEnabled and not (Assigned(FScene) and (FScene.GetDesignTime)) then
  begin
    Result:=nil;
    Exit;
  end;
  if ClipChildren and not PointInObject(X,Y) then
  begin
    Result:=nil;
    Exit;
  end;
  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
    begin
      if not TD2Object(FChildren[i]).IsVisual then Continue;
      Obj:=TD2VisualObject(FChildren[i]);
      if (not Obj.Visible) and not (((FScene <> nil) and FScene.GetDesignTime)) then Continue;
      if FDesignHide and (((FScene <> nil) and FScene.GetDesignTime)) then Continue;

      NewObj:=Obj.ObjectByPoint(X,Y);
      if NewObj <> nil then
      begin
        Result:=NewObj;
        Exit;
      end;
    end;

  Result:=nil;
  if PointInObject(X,Y) and CheckHitTest(HitTest) then
    Result:=Self;
end;

function TD2VisualObject.GetCanvas:TD2Canvas;
begin
  if FTempCanvas <> nil then
    Result:=FTempCanvas
  else
    if FScene <> nil then
      Result:=FScene.GetCanvas
    else
      Result:=nil;
end;

procedure TD2VisualObject.BeforePaint;
begin
end;

procedure TD2VisualObject.ApplyResource;
begin
end;

procedure TD2VisualObject.Paint;
begin
end;

procedure TD2VisualObject.AfterPaint;
begin
end;

procedure TD2VisualObject.SetInPaintTo(value:boolean);
var
  i:integer;
begin
  FInPaintTo:=value;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).isVisual then
        TD2VisualObject(FChildren[i]).SetInPaintTo(value);
end;

procedure TD2VisualObject.PaintTo(const ACanvas:TD2Canvas; const ARect:TD2Rect; const AParent:TD2Object=nil);
var
  SaveIndex:integer;
  SaveTempCanvas:TD2Canvas;
  SaveDisableAlign:boolean;
  SavePos:TD2Point;
  SaveScale:TD2Point;
  SaveParent:TD2Object;
  SaveRotate:single;
begin
  if FScene=nil then Exit;
  if Width * Height=0 then Exit;

  FScene.SetDisableUpdate(true);
  SaveDisableAlign:=FDisableAlign;
  FDisableAlign:=true;
  SetInPaintTo(true);
  try
    SaveTempCanvas:=TempCanvas;
    TempCanvas:=ACanvas;
    SaveIndex:=TempCanvas.SaveCanvas;
    { save }
    SavePos:=Position.Point;
    SaveScale:=Scale.Point;
    SaveParent:=FParent;
    SaveRotate:=RotateAngle;
    FParent:=AParent;
    FPosition.FX:=ARect.Left;
    FPosition.FY:=ARect.Top;
    FScale.FX:=d2RectWidth(ARect) / Width;
    FScale.FY:=d2RectHeight(ARect) / Height;
    FRotateAngle:=0;
    MatrixChanged(Self);

    { paint }
    TempCanvas.SetMatrix(AbsoluteMatrix);
    BeforePaint;
    Paint;
    AfterPaint;
    PaintChildren;

    { restore }
    FRotateAngle:=SaveRotate;
    FPosition.FX:=SavePos.X;
    FPosition.FY:=SavePos.Y;
    FScale.FX:=SaveScale.X;
    FScale.FY:=SaveScale.Y;
    FParent:=SaveParent;
    MatrixChanged(Self);
    RecalcUpdateRect;
    RecalcAbsoluteNow;
    RecalcOpacity;
    RecalcEnabled;
  finally
    SetInPaintTo(false);
    FDisableAlign:=SaveDisableAlign;
    TempCanvas.RestoreCanvas(SaveIndex);
    TempCanvas:=SaveTempCanvas;
    FScene.SetDisableUpdate(false);
  end;
end;

procedure TD2VisualObject.UpdateEffects;
var
  P:TD2Object;
begin
  if FHasEffect then
  begin
    FUpdateEffects:=true;
  end;
  P:=Parent;
  while P <> nil do
  begin
    if P.IsVisual then
      P.Visual.UpdateEffects;
    P:=P.Parent;
  end;
end;

procedure TD2VisualObject.ApplyEffect;
var
  i,State,State2:integer;
  M:TD2Matrix;
  R:TD2Rect;
  Effect:TD2Effect;
  EffectRect:TD2Rect;
begin
  if FChildren=nil then Exit;
  if FScene=nil then Exit;
  if FDisableEffect then Exit;
  if not FHasEffect then Exit;

  State:=Canvas.SaveCanvas;
  if not FUpdateEffects then
  begin
    if FEffectBitmap <> nil then
    begin
      Canvas.SetMatrix(AbsoluteMatrix);
      for i:=0 to FChildren.Count - 1 do
      if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) then
      begin
        Effect:=TD2Effect(FChildren[i]);
        EffectRect:=Effect.GetRect(d2Rect(0,0,Width,Height));
        Canvas.DrawBitmap(FEffectBitmap,d2Rect(0,0,FEffectBitmap.Width,FEffectBitmap.Height),EffectRect,AbsoluteOpacity,RotateAngle=0);
      end;
    end;
  end
  else
  begin
    for i:=0 to FChildren.Count - 1 do
    if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) then
    begin
      Effect:=TD2Effect(FChildren[i]);
      EffectRect:=Effect.GetRect(d2Rect(0,0,Width,Height));
      with GetAbsoluteScale do
        d2MultiplyRect(EffectRect,X,Y);
      if FEffectBitmap=nil then
      begin
        { create }
        FEffectBitmap:=TD2Bitmap.Create(trunc(d2RectWidth(EffectRect)),trunc(d2RectHeight(EffectRect)));
      end
      else
        if (FEffectBitmap.Width <> trunc(d2RectWidth(EffectRect))) or
           (FEffectBitmap.Height <> trunc(d2RectHeight(EffectRect))) then
        begin
          { resize }
          FEffectBitmap.SetSize(trunc(d2RectWidth(EffectRect)),trunc(d2RectHeight(EffectRect)));
        end;
      { Paint Self }
      State2:=FEffectBitmap.Canvas.SaveCanvas;
      M:=IdentityMatrix;
      FEffectBitmap.Canvas.BeginScene;
      FEffectBitmap.Canvas.Clear(0);
      FEffectBitmap.Canvas.SetMatrix(M);
      R:=d2Rect(Effect.GetOffset.X,Effect.GetOffset.Y,(Effect.GetOffset.X + Width),(Effect.GetOffset.Y + Height));
      with GetAbsoluteScale do
        d2MultiplyRect(R,X,Y);

      PaintTo(FEffectBitmap.Canvas,R);

      FEffectBitmap.Canvas.EndScene;
      FEffectBitmap.Canvas.RestoreCanvas(State2);
      { apply effects }
      with GetAbsoluteScale do
      begin
        Effect.ProcessEffect(FEffectBitmap.Canvas,FEffectBitmap,X);
        { draw effectBitmap }
        d2MultiplyRect(EffectRect,1 / X,1 / Y);
      end;
      Canvas.SetMatrix(AbsoluteMatrix);
      Canvas.DrawBitmap(FEffectBitmap,d2Rect(0,0,FEffectBitmap.Width,FEffectBitmap.Height),EffectRect,AbsoluteOpacity,RotateAngle=0);
    end;
    FUpdateEffects:=false;
  end;
  Canvas.RestoreCanvas(State);
end;

procedure TD2VisualObject.PaintChildren;
var
  i,j:integer;
  R:TD2Rect;
  State,State2,State3:cardinal;
  ClipParentObject:TD2VisualObject;
  AllowPaint:boolean;
begin
  if FScene=nil then Exit;
  if FChildren <> nil then
  begin
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).IsVisual and
         ((TD2VisualObject(FChildren[i]).Visible) or
          (not TD2VisualObject(FChildren[i]).Visible and (Assigned(FScene) and (FScene.GetDesignTime)) and not TD2VisualObject(FChildren[i]).Locked)) then
        with TD2VisualObject(FChildren[i]) do
        begin
          if (((Scene <> nil) and (Scene.GetDesignTime))) and FDesignHide then Continue;
          if (d2RectWidth(UpdateRect)=0) or (d2RectHeight(UpdateRect)=0) then Continue;

          FScene:=Self.FScene;
          if Self.ClipChildren and not d2IntersectRect(Self.UpdateRect,UpdateRect) then Continue;

          AllowPaint:=false;
          if (Assigned(FScene) and (FScene.GetDesignTime)) or FInPaintTo then
            AllowPaint:=true;
          if not AllowPaint then
          begin
            R:=d2UnionRect(GetChildrenRect,UpdateRect);
            for j:=0 to FScene.GetUpdateRectsCount - 1 do
              if d2IntersectRect(FScene.GetUpdateRect(j),R) then
              begin
                AllowPaint:=true;
                Break;
              end;
          end;

          if AllowPaint then
          begin
            if Self.FClipChildren and CanClipped then
            begin
              State:=Canvas.SaveCanvas;
              Canvas.SetMatrix(Self.AbsoluteMatrix);
              Canvas.IntersectClipRect(Self.ClipRect);
            end
            else
              State:=InvalideCanvasState;
            Canvas.SetMatrix(AbsoluteMatrix);
            BeforePaint;
            if FHasEffect and not HasAfterPaintEffect then
              ApplyEffect;
            Canvas.SetMatrix(AbsoluteMatrix);
            if not HasDisablePaintEffect then
            begin
              ClipParentObject:=HasClipParent;
              if ClipParentObject <> nil then
              begin
                State3:=Canvas.SaveCanvas;

                Canvas.SetMatrix(ClipParentObject.AbsoluteMatrix);
                Canvas.ExcludeClipRect(ClipParentObject.LocalRect);
                Canvas.SetMatrix(AbsoluteMatrix);

                if ClipChildren then
                begin
                  // Clip self
                  State2:=Canvas.SaveCanvas;
                  Canvas.SetMatrix(AbsoluteMatrix);
                  Canvas.IntersectClipRect(ClipRect);
                  if Assigned(FOnBeforePaint) then
                  begin
                    FOnBeforePaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                    Canvas.SetMatrix(AbsoluteMatrix);
                  end;
                  Paint;
                  Canvas.RestoreCanvas(State2);
                end
                else
                begin
                  if Assigned(FOnBeforePaint) then
                  begin
                    FOnBeforePaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                    Canvas.SetMatrix(AbsoluteMatrix);
                  end;
                  Paint;
                end;
                Canvas.RestoreCanvas(State3);
                PaintChildren;
                if Assigned(FOnPaint) then
                begin
                  Canvas.SetMatrix(AbsoluteMatrix);
                  FOnPaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                end;
              end
              else
              begin
                if ClipChildren then
                begin
                  // Clip self
                  State2:=Canvas.SaveCanvas;
                  Canvas.SetMatrix(AbsoluteMatrix);
                  Canvas.IntersectClipRect(ClipRect);
                  if Assigned(FOnBeforePaint) then
                  begin
                    FOnBeforePaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                    Canvas.SetMatrix(AbsoluteMatrix);
                  end;
                  Paint;
                  Canvas.RestoreCanvas(State2);
                end
                else
                begin
                  if Assigned(FOnBeforePaint) then
                  begin
                    FOnBeforePaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                    Canvas.SetMatrix(AbsoluteMatrix);
                  end;
                  Paint;
                end;
                PaintChildren;
                if Assigned(FOnPaint) then
                begin
                  Canvas.SetMatrix(AbsoluteMatrix);
                  FOnPaint(TD2VisualObject(Self.FChildren[i]),Canvas,LocalRect);
                end;
              end;
            end;
            AfterPaint;
            // design selection
            if (GvarD2Designer <> nil) and (Assigned(Self.Scene)) and (Self.GetOwner <> nil) and
               (Self.FChildren[i] <> Scene.GetSelected) and (GvarD2Designer.IsSelected(Self.Scene.GetOwner,TD2VisualObject(Self.FChildren[i]))) then
            begin
              Canvas.SetMatrix(AbsoluteMatrix);
              Canvas.Stroke.Style:=d2BrushSolid;
              Canvas.Stroke.SolidColor:=$B200CC5A;
              Canvas.StrokeCap:=d2CapFlat;
              Canvas.StrokeJoin:=d2JoinMiter;
              Canvas.StrokeDash:=d2DashDash;
              Canvas.StrokeThickness:=1;
              R:=d2Rect(0,0,Width,Height);
              d2InflateRect(R,-1,-1);
              Canvas.DrawRect(R,1,1,AllCorners,1);
              Canvas.StrokeDash:=d2DashSolid;
            end;
            // drag highlight
            if IsDragOver and not DragDisableHighlight then
            begin
              Canvas.SetMatrix(AbsoluteMatrix);
              Canvas.Stroke.Style:=d2BrushSolid;
              Canvas.Stroke.SolidColor:=$B2005ACC;
              Canvas.StrokeCap:=d2CapFlat;
              Canvas.StrokeJoin:=d2JoinMiter;
              Canvas.StrokeDash:=d2DashSolid;
              Canvas.StrokeThickness:=3;
              R:=d2Rect(0,0,Width,Height);
              d2InflateRect(R,-1,-1);
              Canvas.DrawRect(R,1,1,AllCorners,1);
              Canvas.StrokeDash:=d2DashSolid;
            end;
            if State <> InvalideCanvasState then
            begin
              Canvas.RestoreCanvas(State);
            end;
          end;
          if HasAfterPaintEffect then
          begin
            Canvas.SetMatrix(AbsoluteMatrix);
            ApplyEffect;
          end;
          {$IFDEF BOUNDS}
          State3:=Canvas.SaveCanvas;
          Canvas.ResetClipRect;
          Canvas.SetMatrix(IdentityMatrix);
          Canvas.Stroke.Style:=d2BrushSolid;
          Canvas.Stroke.Color:='#FF00FF00';
          Canvas.StrokeThickness:=1;
          R:=AbsoluteRect;
          Canvas.DrawRect(R,0,0,Allcorners,0.5);
          Canvas.RestoreCanvas(State3);
          {$ENDIF}
        end;
  end;
  {$IFDEF DRAWFOCUSED}
  if Canfocused and IsFocused then
  begin
    State3:=Canvas.SaveCanvas;
    Canvas.ResetClipRect;
    Canvas.SetMatrix(AbsoluteMatrix);
    R:=localRect;
    Canvas.Stroke.Style:=d2Brushsolid;
    Canvas.Stroke.Color:=vcRed;
    Canvas.StrokeThickness:=1;
    Canvas.DrawRect(R,1,1,Allcorners,AbsoluteOpacity);
    Canvas.RestoreCanvas(State3);
  end;
  {$ENDIF}
end;

function TD2VisualObject.CheckParentVisible:boolean;
var
  P:TD2Object;
begin
  P:=Self;
  Result:=false;
  while P <> nil do
  begin
    if P.IsVisual and not TD2VisualObject(P).Visible then Exit;
    P:=P.Parent;
  end;
  Result:=true;
end;

procedure TD2VisualObject.InvalidateRect(ARect: TD2Rect);
begin
  if not Visible and (FScene <> nil) and (not FScene.GetDesignTime) then Exit;
  if FScene=nil then Exit;
  if (((Scene <> nil) and (Scene.GetDesignTime))) and FDesignHide then Exit;;
  if FScene.GetDisableUpdate then Exit;
  if not (((Scene <> nil) and (Scene.GetDesignTime))) and not CheckParentVisible then Exit;;
  ARect.TopLeft:=LocalToAbsolute(ARect.TopLeft);
  ARect.BottomRight:=LocalToAbsolute(ARect.BottomRight);
  FScene.AddUpdateRect(ARect);
end;

procedure TD2VisualObject.Repaint;
begin
  if not Visible and (FScene <> nil) and (not FScene.GetDesignTime) then Exit;
  if FScene=nil then Exit;
  if (((Scene <> nil) and (Scene.GetDesignTime))) and FDesignHide then Exit;;
  if FScene.GetDisableUpdate then Exit;
  if not (((Scene <> nil) and (Scene.GetDesignTime))) and not CheckParentVisible then Exit;;
  if FUpdating > 0 then Exit;
  if HasDisablePaintEffect then
    FUpdateEffects:=true;
  FScene.AddUpdateRect(UpdateRect);
end;


procedure TD2VisualObject.Lock;
var
  i:integer;
begin
  Locked:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).isVisual then
        TD2VisualObject(FChildren[i]).Lock;
end;

{ bounds }

function TD2VisualObject.GetLocalRect:TD2Rect;
begin
  Result:=d2Rect(0,0,FWidth,FHeight);
end;

function TD2VisualObject.GetAbsoluteRect:TD2Rect;
begin
  Result:=d2NormalizeRect([LocalToAbsolute(d2Point(0,0)),LocalToAbsolute(d2Point(Width,0)),
    LocalToAbsolute(d2Point(Width,Height)),LocalToAbsolute(d2Point(0,Height))]);
end;

function TD2VisualObject.GetClipRect:TD2Rect;
begin
  Result:=d2Rect(0,0,Width,Height);
end;

function TD2VisualObject.GetBoundsRect:TD2Rect;
begin
  Result:=d2Rect(0,0,Width,Height);
end;

function TD2VisualObject.GetParentedRect:TD2Rect;
begin
  Result:=d2Rect(0,0,Width,Height);
  d2OffsetRect(Result,Position.X,Position.Y);
end;

procedure TD2VisualObject.SetBoundsRect(const Value:TD2Rect);
var
  P:TD2Point;
begin
  Repaint;
  FWidth:=Value.Right - Value.Left;
  FHeight:=Value.Bottom - Value.Top;
  P:=LocalToAbsolute(d2Point(Value.Left + FRotateCenter.X * FWidth,Value.Top + FRotateCenter.Y * FHeight));
  if (Parent <> nil) and (Parent.isVisual) then
    P:=TD2VisualObject(Parent).AbsoluteToLocal(P);
  FPosition.FX:=P.X - FScale.X * FRotateCenter.X * FWidth;
  FPosition.FY:=P.Y - FScale.Y * FRotateCenter.Y * FHeight;
  if (FWidth < 0) then
  begin
    FWidth:=Abs(FWidth);
    FScale.X:=-FScale.X;
  end;
  if (FHeight < 0) then
  begin
    FHeight:=Abs(FHeight);
    FScale.Y:=-FScale.Y;
  end;
  MatrixChanged(Self);
  Realign;
end;

{ }

procedure TD2VisualObject.PaddingChanged(Sender:TObject);
begin
  if (FParent <> nil) and (FParent.isVisual) then
    TD2VisualObject(FParent).Realign;
end;

procedure TD2VisualObject.MarginsChanged(Sender:TObject);
begin
  Realign;
end;

procedure TD2VisualObject.DesignInsert;
begin

end;

procedure TD2VisualObject.DesignSelect;
begin

end;

procedure TD2VisualObject.DesignClick;
begin

end;

type

  TD2AlignInfo=record
    AlignList:TList;
    ControlIndex: Integer;
    Align: TD2Align;
    Scratch: Integer;
  end;

procedure TD2VisualObject.BeginUpdate;
var
  i:integer;
begin
  FUpdating:=FUpdating + 1;
  for i:=0 to ChildrenCount - 1 do
    if TD2Object(FChildren[i]).IsVisual then
      TD2VisualObject(FChildren[i]).BeginUpdate;
end;

procedure TD2VisualObject.EndUpdate;
var
  i:integer;
begin
  FUpdating:=FUpdating - 1;
  for i:=0 to ChildrenCount - 1 do
    if TD2Object(FChildren[i]).IsVisual then
      TD2VisualObject(FChildren[i]).EndUpdate;
  if FUpdating=0 then
    Realign;
end;

procedure TD2VisualObject.RecalcNeedAlign;
var
  i:integer;
begin
  FNeedAlign:=false;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if not (TD2Object(FChildren[i]).IsVisual) then Continue;
      if TD2VisualObject(FChildren[i]).Align <> vaNone then
      begin
        FNeedAlign:=true;
        Break;
      end;
    end;
end;

procedure TD2VisualObject.Realign;
var
  //i: integer;		//Delete by GoldenFox
  R:TD2Rect;
  AlignList:TList;

  function InsertBefore(C1,C2:TD2VisualObject; AAlign:TD2Align):boolean;
  begin
    Result:=False;
    case AAlign of
      vaTop,vaMostTop:Result:=C1.Position.Y < C2.Position.Y;
      vaBottom,vaMostBottom:Result:=(C1.Position.Y + C1.Height) >= (C2.Position.Y + C2.Height);
      vaLeft,vaMostLeft:Result:=C1.Position.X < C2.Position.X;
      vaRight,vaMostRight:Result:=(C1.Position.X + C1.Width) >= (C2.Position.X + C2.Width);
    end;
  end;

  procedure DoPosition(Control:TD2VisualObject; AAlign:TD2Align; AlignInfo:TD2AlignInfo);
  var
    NewLeft,NewTop,NewWidth,NewHeight:single;
    cR,mR:TD2Rect;
    fitScale:single;
  begin
    with R do
    begin
      NewWidth:=Right - Left;
      if (NewWidth < 0) or (AAlign in [vaLeft,vaRight,vaVertical,vaMostLeft,vaMostRight,vaTopLeft,vaTopRight,vaBottomLeft,vaBottomRight]) then
        NewWidth:=Control.Width + Control.Padding.Left + Control.Padding.Right;
      NewHeight:=Bottom - Top;
      if (NewHeight < 0) or (AAlign in [vaTop,vaBottom,vaMostTop,vaMostBottom,vaHorizontal,vaTopLeft,vaTopRight,vaBottomLeft,vaBottomRight]) then
        NewHeight:=Control.Height + Control.Padding.Top + Control.Padding.Bottom;
      NewLeft:=Left;
      NewTop:=Top;
      if (AAlign in [vaVertical]) then
        NewLeft:=Control.Position.X + Control.Padding.Left;
      if (AAlign in [vaHorizontal]) then
        NewTop:=Control.Position.Y + Control.Padding.Top;
      case AAlign of
        vaTop,vaMostTop:
          Top:=Top + NewHeight;
        vaBottom,vaMostBottom:
          begin
            Bottom:=Bottom - NewHeight;
            NewTop:=Bottom;
          end;
        vaLeft,vaMostLeft:
          Left:=Left + NewWidth;
        vaRight,vaMostRight:
          begin
            Right:=Right - NewWidth;
            NewLeft:=Right;
          end;
        vaContents:
          begin
            NewLeft:=0;
            NewTop:=0;
            NewWidth:=Width;
            NewHeight:=Height;
            Control.FPosition.FX:=NewLeft + Control.Padding.Left;
            Control.FPosition.FY:=NewTop + Control.Padding.Top;
            Control.FWidth:=NewWidth - Control.Padding.Left - Control.Padding.Right;
            Control.FHeight:=NewHeight - Control.Padding.Top - Control.Padding.Bottom;
            Control.MatrixChanged(Self);
            Control.Realign;
            Exit;
          end;
        vaFit,vaFitLeft,vaFitRight:
          begin
            mR:=d2Rect(Margins.Left,Margins.Top,Width - Margins.Right,Height - Margins.Bottom);
            cR:=d2Rect(Control.FPosition.FX - Control.Padding.Left,Control.FPosition.FY - Control.Padding.Top,
              Control.FPosition.FX + Control.FWidth + Control.Padding.Right,
              Control.FPosition.FY + Control.FHeight + Control.Padding.Bottom);
            fitScale:=d2FitRect(cR,mR);
            if fitScale < 1 then
            begin
              cR.Left:=cR.Left / fitScale;
              cR.Right:=cR.Right / fitScale;
              cR.Top:=cR.Top / fitScale;
              cR.Bottom:=cR.Bottom / fitScale;
              d2RectCenter(cR,mR);
              if AAlign=vaFitLeft then
                d2OffsetRect(cR,mR.Left - cR.Left,0);
              if AAlign=vaFitRight then
                d2OffsetRect(cR,mR.Right - cR.Right,0);
              NewLeft:=cR.Left;
              NewTop:=cR.Top;
              NewWidth:=cR.Right - cR.Left;
              NewHeight:=cR.Bottom - cR.Top;
            end
            else
            begin
              if AAlign=vaFitLeft then
                d2OffsetRect(cR,mR.Left - cR.Left,0);
              if AAlign=vaFitRight then
                d2OffsetRect(cR,mR.Right - cR.Right,0);
              NewLeft:=cR.Left;
              NewTop:=cR.Top;
              NewWidth:=cR.Right - cR.Left;
              NewHeight:=cR.Bottom - cR.Top;
            end;
            Control.FPosition.FX:=NewLeft + Control.Padding.Left;
            Control.FPosition.FY:=NewTop + Control.Padding.Top;
            Control.FWidth:=NewWidth - Control.Padding.Left - Control.Padding.Right;
            Control.FHeight:=NewHeight - Control.Padding.Top - Control.Padding.Bottom;
            Control.MatrixChanged(Self);
            Control.Realign;
            Exit;
          end;
        vaCenter:
          begin
            NewLeft:=Left + Trunc((NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right)) / 2);
            NewWidth:=(Control.Width + Control.Padding.Left + Control.Padding.Right);
            NewTop:=Top + Trunc((NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom)) / 2);
            NewHeight:=(Control.Height + Control.Padding.Top + Control.Padding.Bottom);
          end;
        vaHorzCenter:
          begin
            NewLeft:=Left + Trunc((NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right)) / 2);
            NewWidth:=(Control.Width + Control.Padding.Left + Control.Padding.Right);
          end;
        vaVertCenter:
          begin
            NewTop:=Top + Trunc((NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom)) / 2);
            NewHeight:=(Control.Height + Control.Padding.Top + Control.Padding.Bottom);
          end;
        vaTopRight:
          begin
            Control.Position.X:=Control.Position.X + (FWidth - FLastWidth);
            Exit;
          end;
        vaBottomLeft:
          begin
            Control.Position.Y:=Control.FPosition.Y + (FHeight - FLastHeight);
            Exit;
          end;
        vaBottomRight:
          begin
            Control.Position.SetPoint(d2Point(Control.Position.X + (FWidth - FLastWidth),Control.FPosition.Y + (FHeight - FLastHeight)));
            Exit;
          end;
      end;
    end;

    if (AALign=vaScale) then
    begin
      if (FLastWidth > 0) and (FLastHeight > 0) and (FWidth > 0) and (FHeight > 0) then
      begin
        Control.FPosition.FX:=Control.FPosition.X * (FWidth / FLastWidth);
        Control.FPosition.FY:=Control.FPosition.Y * (FHeight / FLastHeight);
        Control.FWidth:=Control.FWidth * (FWidth / FLastWidth);
        Control.FHeight:=Control.FHeight * (FHeight / FLastHeight);
        Control.MatrixChanged(Self);
        Control.Realign;
      end;
      Exit;
    end
    else
    begin
      Control.FPosition.FX:=NewLeft + Control.Padding.Left;
      Control.FPosition.FY:=NewTop + Control.Padding.Top;
      if (Control.FWidth <> NewWidth - Control.Padding.Left - Control.Padding.Right) or
         (Control.FHeight <> NewHeight - Control.Padding.Top - Control.Padding.Bottom) then
      begin
        Control.FWidth:=NewWidth - Control.Padding.Left - Control.Padding.Right;
        Control.FHeight:=NewHeight - Control.Padding.Top - Control.Padding.Bottom;
        Control.Realign;
      end;
      Control.MatrixChanged(Self);
    end;

    { Adjust client rect if control didn't resize as we expected }
    if (Control.Width + Control.Padding.Left + Control.Padding.Right <> NewWidth) or
       (Control.Height + Control.Padding.Top + Control.Padding.Bottom <> NewHeight) then
      with R do
        case AAlign of
          vaTop:
            Top:=Top - (NewHeight - (Control.Height + Control.Padding.Left + Control.Padding.Right));
          vaBottom:
            Bottom:=Bottom + (NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom));
          vaLeft:
            Left:=Left - (NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right));
          vaRight:
            Right:=Right + (NewWidth - (Control.Width + Control.Padding.Top + Control.Padding.Bottom));
          vaClient:
            begin
              Right:=Right + NewWidth - (Control.Width + Control.Padding.Left + Control.Padding.Right);
              Bottom:=Bottom + NewHeight - (Control.Height + Control.Padding.Top + Control.Padding.Bottom);
            end;
        end;
  end;

  procedure DoAlign(AAlign:TD2Align);
  var
    I,J:integer;
    Control:TD2VisualObject;
    AlignInfo:TD2AlignInfo;
  begin
    AlignList.Clear;
    for I:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).isVisual then Continue;
      Control:=TD2VisualObject(FChildren[i]);
      if (Control.Align=AAlign) and (Control.Visible) then
      begin
        J:=0;
        while (J < AlignList.Count) and not InsertBefore(Control,TD2VisualObject(AlignList[J]),AAlign) do Inc(J);
        AlignList.Insert(J,Control);
      end;
    end;
    for I:=0 to AlignList.Count - 1 do
    begin
      AlignInfo.AlignList:=AlignList;
      AlignInfo.ControlIndex:=I;
      AlignInfo.Align:=AAlign;
      DoPosition(TD2VisualObject(AlignList[I]),AAlign,AlignInfo);
    end;
  end;

begin
  if csDestroying in ComponentState then Exit;
  if ((FWidth > -2) and (FWidth < 2)) or ((FHeight > -2) and (FHeight < 2)) then Exit;
  if FDisableDefaultAlign then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;
  if csLoading in ComponentState then
  begin
    FLastWidth:=FWidth;
    FLastHeight:=FHeight;
    Exit;
  end;
  if ((FLastWidth <> FWidth) or (FLastHeight <> FHeight)) and FHasEffect then
  begin
    UpdateEffects;
  end;
  if not FNeedAlign then Exit;
  if FChildren=nil then Exit;
  if FChildren.Count=0 then Exit;

  FDisableAlign:=true;
  try
    R:=d2Rect(0,0,FWidth,FHeight);
    R:=FMargins.MarginRect(R);
    AlignList:=TList.Create;
    try
      DoAlign(vaMostTop);
      DoAlign(vaMostBottom);
      DoAlign(vaMostLeft);
      DoAlign(vaMostRight);
      DoAlign(vaTop);
      DoAlign(vaBottom);
      DoAlign(vaLeft);
      DoAlign(vaRight);
      DoAlign(vaClient);
      DoAlign(vaHorizontal);
      DoAlign(vaVertical);
      DoAlign(vaContents);
      DoAlign(vaCenter);
      DoAlign(vaHorzCenter);
      DoAlign(vaVertCenter);
      DoAlign(vaScale);
      DoAlign(vaFit);
      DoAlign(vaFitLeft);
      DoAlign(vaFitRight);
      // Move anchored controls
      // DoAlign(vaTopLeft); nothing to move
      DoAlign(vaTopRight);
      DoAlign(vaBottomLeft);
      DoAlign(vaBottomRight);
    finally
      AlignList.Free;
    end;
    FLastWidth:=FWidth;
    FLastHeight:=FHeight;
    Repaint;
  finally
    FDisableAlign:=false;
  end;
end;

{ events }

procedure TD2VisualObject.KeyDown(var Key:Word; var KeyChar:System.WideChar; Shift:TShiftState);
var
  VP:TD2Point;
begin
  if (Key=VK_APPS) then
  begin
    VP:=LocalToAbsolute(d2Point(Width / 2,Height / 2));
    VP:=Scene.LocalToScreen(VP);
    ContextMenu(VP);
  end
  else
    if Assigned(FOnKeyDown) then
      FOnKeyDown(Self,Key,KeyChar,Shift);
end;

procedure TD2VisualObject.KeyUp(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if Assigned(FOnKeyUp) then
    FOnKeyUp(Self,Key,KeyChar,Shift);
end;

procedure TD2VisualObject.DialogKey(var Key:Word; Shift:TShiftState);
var
  i:integer;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).isVisual and (TD2VisualObject(FChildren[i]).Visible) and TD2VisualObject(FChildren[i]).Enabled then
      begin
        TD2VisualObject(FChildren[i]).DialogKey(Key,Shift);
        if Key=0 then Break;
      end;
end;

procedure TD2VisualObject.Capture;
begin
  if (FScene <> nil) then
  begin
    FScene.SetCaptured(Self);
  end;
end;

procedure TD2VisualObject.ReleaseCapture;
begin
  if (FScene <> nil) and (FScene.GetCaptured=Self) then
  begin
    FScene.SetCaptured(nil);
  end;
end;

procedure TD2VisualObject.MouseEnter;
begin
  FIsMouseOver:=true;
  StartTriggerAnimation(Self,'IsMouseOver');
  ApplyTriggerEffect(Self,'IsMouseOver');
  if Assigned(FOnMouseEnter) then FOnMouseEnter(Self);
  MouseEnterChildren;		//Added by GoldenFox
end;

procedure TD2VisualObject.MouseLeave;
begin
  FIsMouseOver:=false;
  StartTriggerAnimation(Self,'IsMouseOver');
  ApplyTriggerEffect(Self,'IsMouseOver');
  if Assigned(FOnMouseLeave) then FOnMouseLeave(Self);
  MouseLeaveChildren;		//Added by GoldenFox
end;

{**********************************************************************
                          This part Added by GoldenFox
 **********************************************************************}
procedure TD2VisualObject.MouseEnterChildren;
begin
  SetMouseOverChildren(Self, true);
end;

procedure TD2VisualObject.MouseLeaveChildren;
begin
  SetMouseOverChildren(Self, false);
end;

procedure TD2VisualObject.SetMouseOverChildren(Sender: TObject; Value: Boolean);
begin
  if (Parent<>nil) and (Parent.IsVisual)
    then Parent.Visual.SetMouseOverChildren(Sender, Value);
  FIsMouseOverChildren:=Value;
  FMouseChildren:=Sender;
  if FIsMouseOverChildren
    then if Assigned(FOnMouseEnterChildren) then FOnMouseEnterChildren(Self)
    else if Assigned(FOnMouseLeaveChildren) then FOnMouseLeaveChildren(Self);
end;
//======================= End part of Added by GoldenFox =======================

function TD2VisualObject.EnterFocusChildren(AObject:TD2VisualObject):boolean;
begin
  Result:=false;
end;

procedure TD2VisualObject.EnterFocus;
var
  P:TD2Object;
begin
  if not CanFocused then Exit;

  P:=Parent;
  while P <> nil do
  begin
    if P.IsVisual then
      if P.Visual.EnterFocusChildren(Self) then Break;
    P:=P.Parent;
  end;
  FIsFocused:=true;
  FRecalcUpdateRect:=true;
  Repaint;
  if Assigned(FOnEnterFocus) then FOnEnterFocus(Self);
  if DisableFocusEffect then Exit;
  if GlobalDisableFocusEffect then Exit;
  StartTriggerAnimation(Self,'IsFocused');
  ApplyTriggerEffect(Self,'IsFocused');
end;

procedure TD2VisualObject.SetNewScene(AScene:Id2Scene);
begin
  if (AScene=nil) and (FIsFocused) then
    KillFocus;
  inherited ;
end;

procedure TD2VisualObject.KillFocus;
begin
  if not CanFocused then Exit;

  FRecalcUpdateRect:=true;
  Repaint;
  FIsFocused:=false;
  if Assigned(FOnKillFocus) then FOnKillFocus(Self);
  if DisableFocusEffect then Exit;
  if GlobalDisableFocusEffect then Exit;
  StartTriggerAnimation(Self,'IsFocused');
  ApplyTriggerEffect(Self,'IsFocused');
end;

procedure TD2VisualObject.SetFocus;
var
  C:boolean;
begin
  if not CanFocused then Exit;
  if Assigned(FOnCanFocused) then
  begin
    C:=true;
    FOnCanFocused(Self,C);
    if not C then Exit;
  end;
  FScene.SetFocused(Self);
end;

procedure TD2VisualObject.ContextMenu(const ScreenPosition:TD2Point);
begin
  if FPopupMenu <> nil then
  begin
    FPopupMenu.PopupComponent:=Self;
    FPopupMenu.Popup(round(ScreenPosition.X),round(ScreenPosition.Y));
    Exit;
  end;
  if FPopup <> nil then
  begin
    FPopup.StaysOpen:=false;
    FPopup.PopupModal;
  end;
end;

procedure TD2VisualObject.Click;
begin
  if Assigned(FOnClick) then
    FOnClick(Self);
end;

procedure TD2VisualObject.DblClick;
begin
  if Assigned(FOnDblClick) then
    FOnDblClick(Self);
end;

function TD2VisualObject.MakeScreenshot:TD2Bitmap;
begin
  Result:=TD2Bitmap.Create(round(Width),round(Height));
  Result.Clear(0);

  Result.Canvas.BeginScene;
  PaintTo(Result.Canvas,d2Rect(0,0,Result.Width,Result.Height));
  Result.Canvas.EndScene;
end;

procedure TD2VisualObject.BeginAutoDrag;
var
  B,S:TD2Bitmap;
  R:TD2Rect;
begin
  S:=MakeScreenshot;
  if (S.Width > 512) or (S.Height > 512) then
  begin
    R:=d2Rect(0,0,S.Width,S.Height);
    d2FitRect(R,d2Rect(0,0,512,512));
    B:=TD2Bitmap.Create(round(d2RectWidth(R)),round(d2RectHeight(R)));
    B.Canvas.BeginScene;
    B.Canvas.DrawBitmap(S,d2Rect(0,0,S.Width,S.Height),d2Rect(0,0,B.Width,B.Height),0.7,true);
    B.Canvas.EndScene;
  end
  else
  begin
    B:=TD2Bitmap.Create(S.Width,S.Height);
    B.Canvas.BeginScene;
    B.Canvas.DrawBitmap(S,d2Rect(0,0,B.width,B.Height),d2Rect(0,0,B.width,B.Height),0.7,true);
    B.Canvas.EndScene;
  end;
  FScene.BeginVCLDrag(Self,B);
  B.Free;
  S.Free;
end;

procedure TD2VisualObject.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  VP:TD2Point;
begin
  if (ssDouble in Shift) and (((FScene <> nil) and (FScene.GetDesignTime))) then
    FScene.SetDesignRoot(Self);
  if (Assigned(FScene) and not (FScene.GetDesignTime)) and CanFocused and not FIsFocused and (FScene <> nil) and (FScene.GetFocused <> Self) then
    SetFocus;
  if Assigned(FOnMouseDown) then
    FOnMouseDown(Self,Button,Shift,X,Y);
  if (Button=mbRight) then
  begin
    VP:=LocalToAbsolute(d2Point(X,Y));
    VP:=Scene.LocalToScreen(VP);
    ContextMenu(VP);
    Exit;
  end;
  if FAutoCapture then
    Capture;
  if (ssDouble in Shift) then
  begin
    DblClick;
    FDoubleClick:=true;
  end
  else
  if Button=mbLeft then
  begin
    FPressed:=true;
  end;
end;

procedure TD2VisualObject.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  if Assigned(FOnMouseMove) then
    FOnMouseMove(Self,Shift,X,Y,Dx,Dy);
end;

procedure TD2VisualObject.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  ReleaseCapture;
  if Assigned(FOnMouseUp) then
    FOnMouseUp(Self,Button,Shift,X,Y);
  if FPressed and not (FDoubleClick) and d2PtInRect(d2Point(X,Y),LocalRect) then
  begin
    FPressed:=false;
    Click;
  end;
  FPressed:=false;
  FDoubleClick:=false;
end;

procedure TD2VisualObject.MouseWheel(Shift:TShiftState; WheelDelta:integer; var Handled:boolean);
begin
  if Assigned(FOnMouseWheel) then
    if FScene <> nil then
      FOnMouseWheel(Self,Shift,WheelDelta,FScene.GetMousePos,Handled)
    else
      FOnMouseWheel(Self,Shift,WheelDelta,d2Point(0,0),Handled);
end;

procedure TD2VisualObject.DragEnter(const Data:TD2DragObject; const Point:TD2Point);
begin
  FIsDragOver:=true;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  ApplyTriggerEffect(Self,'IsDragOver');
  if Assigned(OnDragEnter) then
    OnDragEnter(Self,Data,Point);
end;

procedure TD2VisualObject.DragLeave;
begin
  FIsDragOver:=false;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  ApplyTriggerEffect(Self,'IsDragOver');
  if Assigned(OnDragLeave) then
    OnDragLeave(Self);
end;

procedure TD2VisualObject.DragOver(const Data: TD2DragObject;
  Shift: TShiftState; const Point: TD2Point; var Accept: Boolean);
begin
  if Assigned(OnDragOver) then
    OnDragOver(Self,Data,Shift,Point,Accept);
end;

procedure TD2VisualObject.DragDrop(const Data: TD2DragObject;
  Shift: TShiftState; const Point: TD2Point);
begin
  FIsDragOver:=false;
  Repaint;
  StartTriggerAnimation(Self,'IsDragOver');
  ApplyTriggerEffect(Self,'IsDragOver');
  if Assigned(OnDragDrop) then
    OnDragDrop(Self,Data,Shift,Point);
end;

procedure TD2VisualObject.DragEnd;
begin
  // Call mouse up - for effects - inside control
  if DragMode=d2DragAutomatic then
    MouseUp(mbLeft,[ssLeft],$FFFF,$FFFF);
  if Assigned(OnDragEnd) then
    OnDragEnd(Self);
end;

{ controls }

procedure TD2VisualObject.SetEnabled(const Value:boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled:=Value;
    RecalcEnabled;
    RecalcOpacity;
    Repaint;
  end;
end;

function TD2VisualObject.GetAbsoluteEnabled:boolean;
begin
  if FRecalcEnabled then
  begin
    if (FParent <> nil) and (FParent.IsVisual) and (not FParent.Visual.AbsoluteEnabled) then
      FAbsoluteEnabled:=false
    else
      FAbsoluteEnabled:=FEnabled;

    Result:=FAbsoluteEnabled;
    FRecalcEnabled:=false;

    if not Result and Assigned(FScene) and CanFocused and IsFocused then
      FScene.SetFocused(nil);
  end
  else
  begin
    Result:=FAbsoluteEnabled;
  end;
end;

procedure TD2VisualObject.RecalcEnabled;
var
  i:integer;
begin
  FRecalcEnabled:=true;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).IsVisual then
        TD2VisualObject(FChildren[i]).RecalcEnabled;
end;

{ properties }

procedure TD2VisualObject.SetTempCanvas(const Value:TD2Canvas);
var
  i:integer;
begin
  FTempCanvas:=Value;
  if (FChildren <> nil) and (FChildren.Count > 0) then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).IsVisual then
        TD2VisualObject(FChildren[i]).TempCanvas:=Value;
end;

procedure TD2VisualObject.SetHitTest(const Value:boolean);
begin
  FHitTest:=Value;
end;

procedure TD2VisualObject.SetClipChildren(const Value:boolean);
begin
  if FClipChildren <> Value then
  begin
    FClipChildren:=Value;
    Repaint;
  end;
end;

procedure TD2VisualObject.SetAlign(const Value:TD2Align);
begin
  if FAlign <> Value then
  begin
    FAlign:=Value;
    if (FParent <> nil) and (FParent.IsVisual) then
    begin
      FParent.Visual.FNeedAlign:=true;
      if not (csLoading in ComponentState) then
        FParent.Visual.Realign;
    end;
  end;
end;

procedure TD2VisualObject.SetVisible(const Value:boolean);
begin
  if FVisible <> Value then
  begin
    if FVisible then
    begin
      if FScene <> nil then
        FScene.AddUpdateRect(UpdateRect);
    end;
    FVisible:=Value;
    if not (csLoading in ComponentState) then
    begin
      if (Parent <> nil) and (Parent.IsVisual) and (Align <> vaNone) then
        TD2VisualObject(Parent).Realign;
    end;
    if FVisible then
    begin
      RecalcUpdateRect;
      if FScene <> nil then
        FScene.AddUpdateRect(UpdateRect);
      StartTriggerAnimation(Self,'IsVisible');
    end
    else
      if CanFocused and FIsFocused then
        FScene.SetFocused(nil);
  end;
end;

procedure TD2VisualObject.SetPosition(const Value:TD2Position);
begin
  FPosition.Assign(Value);
end;

procedure TD2VisualObject.SetRotateAngle(const Value:single);
begin
  if FRotateAngle <> Value then
  begin
    FRotateAngle:=d2NormalizeAngle(Value);
    MatrixChanged(Self);
  end;
end;

procedure TD2VisualObject.SetBounds(X,Y,AWidth,AHeight:single);
var
  SizeChanged:boolean;
begin
  Repaint;
  SizeChanged:=false;
  if (FHeight <> AHeight) then
  begin
    FHeight:=AHeight;
    if (FHeight < 0) and (((FScene <> nil) and (FScene.GetDesignTime))) then
    begin
      FHeight:=Abs(FHeight);
      FScale.Y:=-FScale.Y;
    end;
    SizeChanged:=true;
  end;
  if (FWidth <> AWidth) then
  begin
    FWidth:=AWidth;
    if (FWidth < 0) and (((FScene <> nil) and (FScene.GetDesignTime))) then
    begin
      FWidth:=Abs(FWidth);
      FScale.X:=-FScale.X;
    end;
    SizeChanged:=true;
  end;

  if (X <> FPosition.X) or (Y <> FPosition.Y) then
  begin
    FPosition.FX:=X;
    FPosition.FY:=Y;
    MatrixChanged(Self);
  end;

  if not (csLoading in ComponentState) and (SizeChanged) then
  begin
    if (Parent <> nil) and (Parent.IsVisual) and (Align <> vaNone) then
    begin
      TD2VisualObject(Parent).Realign;
    end;
    if (FChildren <> nil) then
      Realign;
  end;
  if not (csLoading in ComponentState) then
  begin
    RecalcUpdateRect;
    Repaint;
  end;
end;

procedure TD2VisualObject.SetSizeWithoutChange(AWidth,AHeight:single);
begin
  FWidth:=AWidth;
  FLastWidth:=AWidth;
  FHeight:=AHeight;
  FLastHeight:=AHeight;
end;

procedure TD2VisualObject.SetHeight(const Value:single);
begin
  if FHeight <> Value then
  begin
    Repaint;
    FHeight:=Value;
    if (FHeight < 0) and (FScene <> nil) and (FScene.GetDesignTime) then
    begin
      FHeight:=Abs(FHeight);
      FScale.Y:=-FScale.Y;
    end;
    if not (csLoading in ComponentState) and (FScene <> nil) then
    begin
      UpdateEffects;
      RecalcUpdateRect;
      if (Parent <> nil) and (Parent.IsVisual) and ((Align <> vaNone) or (Parent is TD2ScrollContent)) then
      begin
        TD2VisualObject(Parent).Realign;
      end;
      if (FChildren <> nil) then
        Realign;
    end;
  end;
end;

procedure TD2VisualObject.SetWidth(const Value:single);
begin
  if FWidth <> Value then
  begin
    Repaint;
    FWidth:=Value;
    if (FWidth < 0) and (FScene <> nil) and (FScene.GetDesignTime) then
    begin
      FWidth:=Abs(FWidth);
      FScale.X:=-FScale.X;
    end;
    if not (csLoading in ComponentState) and (FScene <> nil) then
    begin
      UpdateEffects;
      RecalcUpdateRect;
      if (Parent <> nil) and (Parent.IsVisual) and ((Align <> vaNone) or (Parent is TD2ScrollContent)) then
      begin
        TD2VisualObject(Parent).Realign;
      end;
      if (FChildren <> nil) then
        Realign;
    end;
  end;
end;

function TD2VisualObject.isOpacityStored: Boolean;
begin
  Result:=FOpacity <> 1;
end;

procedure TD2VisualObject.SetOpacity(const Value:single);
begin
  if FOpacity <> Value then
  begin
    FOpacity:=Value;
    if FOpacity < 0 then FOpacity:=0;
    if FOpacity > 1 then FOpacity:=1;
    RecalcOpacity;
    Repaint;
  end;
end;

procedure TD2VisualObject.UpdateDesignHide(const Value:boolean);
var
  i:integer;
begin
  FDesignHide:=Value;
  for i:=0 to ChildrenCount - 1 do
  begin
    if Children[i].IsVisual then
      Children[i].Visual.UpdateDesignHide(Value);
  end;
end;

procedure TD2VisualObject.SetDesignHide(const Value:boolean);
begin
  if FDesignHide <> Value then
  begin
    FDesignHide:=Value;
    if (FScene <> nil) and FScene.GetDesignTime and (Parent <> nil) then
    begin
      if FDesignHide and (Scene.GetSelected=Self) and (GvarD2Designer <> nil) then
        GvarD2Designer.SelectObject(Owner,Parent.Visual,[]);
      Parent.Visual.Repaint;
    end;
  end;
end;

procedure TD2VisualObject.SetCursor(const Value:TCursor);
begin
  if FCursor <> Value then
  begin
    FCursor:=Value;
  end;
end;

procedure TD2VisualObject.FixupTabList;
var
  I,J:integer;
  List:TList;
  Control:TD2VisualObject;
begin
  if not Assigned(Scene) then Exit;
  if FTabList=nil then Exit;
  List:=TList.Create;
  try
    List.Count:=FTabList.Count;
    for i:=0 to FTabList.Count - 1 do
    begin
      Control:=TD2VisualObject(FTabList[i]);
      j:=Control.FTabOrder;
      if (j >= 0) and (j < FTabList.Count) then List[j]:=Control;
    end;
    for I:=0 to FTabList.Count - 1 do
    begin
      Control:=TD2VisualObject(List[I]);
      if Control <> nil then Control.UpdateTabOrder(I);
    end;
  finally
    List.Free;
  end;
end;

procedure TD2VisualObject.GetTabOrderList(List:TList; Children:boolean);
var
  i:integer;
  Control:TD2VisualObject;
begin
  if FTabList <> nil then
    for I:=0 to FTabList.Count - 1 do
    begin
      Control:=TD2VisualObject(FTabList[I]);
      List.Add(Control);
      if Children then
        Control.GetTabOrderList(List,Children);
    end;
end;

function TD2VisualObject.GetTabOrder:TTabOrder;
begin
  if (FParent <> nil) and (FParent.IsVisual) and (FParent.Visual.FTabList <> nil) then
    Result:=TD2VisualObject(FParent).FTabList.IndexOf(Self)
  else
    Result:=-1;
end;

procedure TD2VisualObject.UpdateTabOrder(Value:TTabOrder);
var
  CurIndex,Count:integer;
begin
  CurIndex:=GetTabOrder;
  if CurIndex >= 0 then
  begin
    Count:=TD2VisualObject(FParent).FTabList.Count;
    if Value < 0 then Value:=0;
    if Value >= Count then Value:=Count - 1;
    if Value <> CurIndex then
    begin
      TD2VisualObject(FParent).FTabList.Delete(CurIndex);
      TD2VisualObject(FParent).FTabList.Insert(Value,Self);
    end;
  end;
end;

procedure TD2VisualObject.SetTabOrder(const Value:TTabOrder);
begin
  if csLoading in ComponentState then
    FTabOrder:=Value
  else
    UpdateTabOrder(Value);
end;

function TD2VisualObject.GetHint: String;  // 7777
begin
  Result:=UTF8Encode(FHint);
end;

procedure TD2VisualObject.SetHint(const Value:String);  // 7777
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  if FHint <> ss then FHint:=ss;
end;

         
  
