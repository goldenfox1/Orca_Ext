{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2ImageControl =================================================

constructor TD2ImageControl.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=true;
  FEnableOpenDialog:=true;
  FBitmap:=TD2Bitmap.Create(1,1);
  FBitmap.OnChange:=DoBitmapChanged;
end;

destructor TD2ImageControl.Destroy;
begin
  FBitmap.Free;
  inherited;
end;

procedure TD2ImageControl.ApplyStyle;
var
  O:TD2Object;
begin
  inherited;
  O:=FindResource('image');
  if (O <> nil) and (O is TD2Image) then
  begin
    FImage:=TD2Image(O);
    FImage.Bitmap.Assign(FBitmap);
  end;
end;

procedure TD2ImageControl.FreeStyle;
begin
  inherited;
  FImage:=nil;
end;

procedure TD2ImageControl.Click;
var
  D:TOpenDialog;
begin
  inherited;
  if not FEnableOpenDialog then Exit;

  D:=TOpenDialog.Create(nil);
  D.Filter:=GvarD2DefaultFilterClass.GetFileTypes;
  if D.Execute then
  begin
    Bitmap.LoadFromFile(D.FileName);
  end;
  D.Free;
end;

//procedure TD2ImageControl.DragOver(const Data:TD2DragObject;	//Deleted by GoldenFox
procedure TD2ImageControl.DragOver(const Data:TD2DragObject; Shift: TShiftState;	//Added by GoldenFox
  const Point:TD2Point; var Accept:boolean);
begin
  inherited;
  // accept correct image file or TD2Image
  Accept :=
    ((Length(Data.Files) > 0) and FileExists(Data.Files[0]) and (Pos(ExtractFileExt(Data.Files[0]),GvarD2DefaultFilterClass.GetFileTypes) > 0))
    or
    (Data.Source is TD2Image);
end;

//procedure TD2ImageControl.DragDrop(const Data:TD2DragObject;	//Deleted by GoldenFox
procedure TD2ImageControl.DragDrop(const Data:TD2DragObject; Shift: TShiftState;	//Added by GoldenFox
  const Point:TD2Point);
begin
  inherited;
  if Data.Source is TD2Image then
  begin
    Bitmap.Assign(TD2Image(Data.Source).Bitmap);
  end
  else
  if Length(Data.Files) > 0 then
  begin
    Bitmap.LoadFromFile(Data.Files[0]);
  end;
end;

procedure TD2ImageControl.SetBitmap(const Value:TD2Bitmap);
begin
  FBitmap.Assign(Value);
end;

procedure TD2ImageControl.DoBitmapChanged(Sender:TObject);
var
  R:TD2Rect;
begin
  if FImage <> nil then
  begin
    { create thumbnail }
    R:=d2Rect(0,0,FBitmap.Width,FBitmap.Height);
    d2FitRect(R,d2Rect(0,0,FImage.Width,FImage.Height));
    FImage.Bitmap.SetSize(round(d2RectWidth(R)),round(d2RectHeight(R)));
    if FImage.Bitmap.Canvas.BeginScene then
    begin
      FImage.Bitmap.Canvas.Clear(0);
      FImage.Bitmap.Canvas.DrawBitmap(FBitmap,d2Rect(0,0,FBitmap.Width,FBitmap.Height),
        d2Rect(0,0,FImage.Bitmap.Width,FImage.Bitmap.Height),1);
      FImage.Bitmap.Canvas.EndScene;
    end;
    FImage.Repaint;

    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

function TD2ImageControl.GetData:Variant;
begin
  Result:=ObjectToVariant(Bitmap);
end;

procedure TD2ImageControl.SetData(const Value:Variant);
begin
  if VarIsNull(Value) then
    Bitmap.SetSize(1,1)
  else
  if VarIsObject(Value) then
  begin
    if VariantToObject(Value) is TPersistent then
      Bitmap.Assign(TPersistent(VariantToObject(Value)));
  end
  else
    if VarIsStr(Value) then
      Bitmap.LoadFromFile(Value)
end;
      

//=============== TD2ImageViewer =====================================================

constructor TD2ImageViewer.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='framedscrollboxstyle';
  MouseTracking:=true;
  MouseScaling:=true;
  Cursor:=crHandPoint;
  FScale:=1;
  FBack:=TD2Rectangle.Create(Self);
  FBack.HitTest:=false;
  FBack.Parent:=Self;
  FBack.Locked:=true;
  FBack.Stroke.Style:=d2BrushNone;
  FBack.Stored:=false;
  FBack.Visible:=false;
  FImage:=TD2Image.Create(Self);
  FImage.HitTest:=false;
  FImage.Parent:=Self;
  FImage.Locked:=true;
  FImage.Stored:=false;
  FImage.WrapMode:=d2ImageStretch;
  FImage.Bitmap.OnChange:=DoBitmapChange;
end;

destructor TD2ImageViewer.Destroy;
begin
  inherited;
end;

function TD2ImageViewer.GetBitmap:TD2Bitmap;
begin
  Result:=FImage.Bitmap;
end;

function TD2ImageViewer.GetContentBounds:TD2Rect;
begin
  FImage.SetBounds(0,0,Bitmap.Width * BitmapScale,Bitmap.Height * BitmapScale);

  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    if FImage.Width < ContentLayout.Width then
      FImage.Position.X:=round((ContentLayout.Width - FImage.Width) / 2);
    if FImage.Height < ContentLayout.Height then
      FImage.Position.Y:=round((ContentLayout.Height - FImage.Height) / 2);
  end;
  FBack.SetBounds(FImage.Position.X,FImage.Position.Y,FImage.Width,FImage.Height);

  Result:=d2UnionRect(d2Rect(0,0,0,0),FImage.ParentedRect);
end;

procedure TD2ImageViewer.MouseWheel(Shift:TShiftState;
  WheelDelta:integer; var Handled:boolean);
begin
  if MouseScaling then
  begin
    BitmapScale:=BitmapScale + (WheelDelta / 120) * 0.04;
    Handled:=true;
  end;
  inherited;
end;

procedure TD2ImageViewer.BestFit;
var
  R:TD2Rect;
  s:single;
  NeedRealign:boolean;
begin
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    NeedRealign:=false;
    if (VScrollBar <> nil) and (VScrollBar.Enabled) then
      NeedRealign:=true;
    if (VScrollBar <> nil) and (VScrollBar.Enabled) then
      NeedRealign:=true;
    R:=d2Rect(0,0,Bitmap.Width,Bitmap.Height);
    s:=d2FitRect(R,ContentLayout.LocalRect);
    if s >= 1 then
      BitmapScale:=1 / s
    else
      BitmapScale:=1;
    if NeedRealign then
    begin
      R:=d2Rect(0,0,Bitmap.Width,Bitmap.Height);
      s:=d2FitRect(R,ContentLayout.LocalRect);
      if s >= 1 then
        BitmapScale:=1 / s
      else
        BitmapScale:=1;
    end;
  end
end;

procedure TD2ImageViewer.SetBitmap(const Value:TD2Bitmap);
begin
  FImage.Bitmap:=Value;
end;

procedure TD2ImageViewer.SetScale(const Value:single);
begin
  if FScale <> Value then
  begin
    FScale:=Value;
    if FScale < 0.01 then
      FScale:=0.01;
    if FScale > 10 then
      FScale:=10;
    Realign;
    Centre;
  end;
end;

function TD2ImageViewer.GetBackgroundFill:TD2Brush;
begin
  Result:=FBack.Fill;
end;

procedure TD2ImageViewer.SetBackgroundFill(const Value:TD2Brush);
begin
  FBack.Fill:=Value;
end;

procedure TD2ImageViewer.DoBitmapChange(Sender:TObject);
begin
  Realign;
end;

procedure TD2ImageViewer.SetShowBackground(const Value:boolean);
begin
  if FShowBackground <> Value then
  begin
    FShowBackground:=Value;
    FBack.Visible:=FShowBackground;
  end;
end;

function TD2ImageViewer.GetData:Variant;
begin
  Result:=ObjectToVariant(Bitmap);
end;

procedure TD2ImageViewer.SetData(const Value:Variant);
begin
  if VarIsNull(Value) then
    Bitmap.SetSize(1,1)
  else
  if VarIsObject(Value) then
  begin
    if VariantToObject(Value) is TPersistent then
      Bitmap.Assign(TPersistent(VariantToObject(Value)));
  end
  else
    Bitmap.LoadFromFile(Value)
end;


//=============== TD2Image ===========================================

constructor TD2Image.Create(AOwner:TComponent);
begin
  inherited;
  FBitmap:=TD2Bitmap.Create(0,0);
  FBitmap.OnThreadLoaded:=DoBitmapLoaded;
  FBitmap.OnBitmapDestroy:=DoBitmapDestroy;
  FBitmap.OnChange:=DoBitmapChanged;
  FBitmapMargins:=TD2Bounds.Create(d2Rect(0,0,0,0));
  FWrapMode:=d2ImageFit;
end;

destructor TD2Image.Destroy;
begin
  FreeAndNil(FBitmapMargins);
  FBitmap.Free;
  if FBuffer <> nil then
    FreeAndNil(FBuffer);
  inherited;
end;

function TD2Image.GetData:Variant;
begin
  Result:=ObjectToVariant(FBitmap);
end;

procedure TD2Image.SetData(const Value:Variant);
begin
  if VarIsObject(Value) then
  begin
    if VariantToObject(Value) is TPersistent then
      FBitmap.Assign(TPersistent(VariantToObject(Value)));
  end
  else
    FBitmap.LoadFromFile(Value)
end;

procedure TD2Image.DoBitmapLoaded(Sender:TObject);
begin
  { Update buffer }
  if Assigned(FOnBitmapLoaded) then
    FOnBitmapLoaded(Self);
  if not (csDestroying in ComponentState) then
    Repaint;
end;

procedure TD2Image.DoBitmapDestroy(Sender:TObject);
begin
end;

procedure TD2Image.DoBitmapChanged(Sender:TObject);
begin
  FUpdateEffects:=true;
  Repaint;
end;

procedure TD2Image.Loaded;
begin
  inherited;
end;

procedure TD2Image.Realign;
begin
  inherited;
end;

procedure TD2Image.Paint;
var
  R:TD2Rect;
  State:integer;
  i,j:integer;
  B:TD2Bitmap;
begin
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
    Canvas.StrokeDash:=d2DashSolid;
  end;

  B:=FBitmap;
  if FBitmap.ResourceBitmap <> nil then
    B:=FBitmap.ResourceBitmap;
  if B.IsEmpty then Exit;

  if not FBitmapMargins.MarginEmpty then
  begin
    { lefttop }
    R:=d2Rect(0,0,FBitmapMargins.Left,FBitmapMargins.Top);
    Canvas.DrawBitmap(B,d2Rect(0,0,FBitmapMargins.Left,FBitmapMargins.Top),R,AbsoluteOpacity,true);
    { top }
    R:=d2Rect(FBitmapMargins.Left,0,Width - FBitmapMargins.Right,FBitmapMargins.Top);
    Canvas.DrawBitmap(B,d2Rect(FBitmapMargins.Left,0,B.Width - FBitmapMargins.Right,FBitmapMargins.Top),R,AbsoluteOpacity,true);
    { righttop }
    R:=d2Rect(Width - FBitmapMargins.Right,0,Width,FBitmapMargins.Top);
    Canvas.DrawBitmap(B,d2Rect(B.Width - FBitmapMargins.Right,0,B.Width,FBitmapMargins.Top),R,AbsoluteOpacity,true);
    { left }
    R:=d2Rect(0,FBitmapMargins.Top,FBitmapMargins.Left,Height - FBitmapMargins.Bottom);
    Canvas.DrawBitmap(B,d2Rect(0,FBitmapMargins.Top,FBitmapMargins.Left,B.Height - FBitmapMargins.Bottom),R,AbsoluteOpacity,true);
    { center }
    R:=d2Rect(FBitmapMargins.Left,FBitmapMargins.Top,Width - FBitmapMargins.Right,Height - FBitmapMargins.Bottom);
    Canvas.DrawBitmap(B,d2Rect(FBitmapMargins.Left,FBitmapMargins.Top,B.Width - FBitmapMargins.Right,B.Height - FBitmapMargins.Bottom),R,AbsoluteOpacity,true);
    { right }
    R:=d2Rect(Width - FBitmapMargins.Right,FBitmapMargins.Top,Width,Height - FBitmapMargins.Bottom);
    Canvas.DrawBitmap(B,d2Rect(B.Width - FBitmapMargins.Right,FBitmapMargins.Top,B.Width,B.Height - FBitmapMargins.Bottom),R,AbsoluteOpacity,true);
    { leftbottom }
    R:=d2Rect(0,Height - FBitmapMargins.Bottom,FBitmapMargins.Left,Height);
    Canvas.DrawBitmap(B,d2Rect(0,B.Height - FBitmapMargins.Bottom,FBitmapMargins.Left,B.Height),R,AbsoluteOpacity,true);
    { bottom }
    R:=d2Rect(FBitmapMargins.Left,Height - FBitmapMargins.Bottom,Width - FBitmapMargins.Right,Height);
    Canvas.DrawBitmap(B,d2Rect(FBitmapMargins.Left,B.Height - FBitmapMargins.Bottom,B.Width - FBitmapMargins.Right,B.Height),R,AbsoluteOpacity,true);
    { rightbottom }
    R:=d2Rect(Width - FBitmapMargins.Right,Height - FBitmapMargins.Bottom,Width,Height);
    Canvas.DrawBitmap(B,d2Rect(B.Width - FBitmapMargins.Right,B.Height - FBitmapMargins.Bottom,B.Width,B.Height),R,AbsoluteOpacity,true);
  end
  else
  begin
    case FWrapMode of
      d2ImageOriginal:
        begin
          State:=Canvas.SaveCanvas;
          Canvas.IntersectClipRect(LocalRect);
          R:=d2Rect(0,0,B.Width,B.Height);
          Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity,true);
          Canvas.RestoreCanvas(State);
        end;
      d2ImageFit:
        begin
          R:=d2Rect(0,0,B.Width,B.Height);
          d2FitRect(R,LocalRect);
          Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity,DisableInterpolation)
        end;
      d2ImageStretch:
        begin
          R:=LocalRect;
          Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity,DisableInterpolation)
        end;
      d2ImageTile:
        begin
          State:=Canvas.SaveCanvas;
          Canvas.IntersectClipRect(LocalRect);

          for i:=0 to round(Width / B.Width) do
            for j:=0 to round(Height / B.Height) do
            begin
              R:=d2Rect(0,0,B.Width,B.Height);
              d2OffsetRect(R,i * B.Width,j * B.Height);
              Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity,true);
            end;

          Canvas.RestoreCanvas(State);
        end;
    end;
  end;
end;

procedure TD2Image.SetBitmap(const Value:TD2Bitmap);
begin
  FBitmap.Assign(Value);
  Repaint;
end;

procedure TD2Image.SetWrapMode(const Value:TD2ImageWrap);
begin
  if FWrapMode <> Value then
  begin
    FWrapMode:=Value;
    repaint;
  end;
end;
      
             
                       
