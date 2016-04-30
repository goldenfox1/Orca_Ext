{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Brush ===================================================================}

constructor TD2Brush.Create;
begin
  inherited Create;
  FDefaultStyle:=ADefaultStyle;
  FDefaultColor:=ADefaultColor;
  FColor:=ADefaultColor;
  FStyle:=FDefaultStyle;
  FGradient:=TD2Gradient.Create;
  FGradient.OnChanged:=GradientChanged;
  FVisual:=TD2Visual.Create;
  FVisual.OnChanged:=VisualChanged;
  FResource:=TD2BrushResource.Create;
  FResource.OnChanged:=ResourceChanged;
  FBitmap:=TD2BrushBitmap.Create;
  FBitmap.OnChanged:=BitmapChanged;
end;

destructor TD2Brush.Destroy;
begin
  FBitmap.Free;
  FVisual.Free;
  FResource.Free;
  FGradient.Free;
  inherited;
end;

procedure TD2Brush.Assign(Source:TPersistent);
var
  SaveChange:TNotifyEvent;
begin
  if Source is TD2Brush then
  begin
    SaveChange:=FOnChanged;
    FOnChanged:=nil;
    FDefaultStyle:=(Source as TD2Brush).FDefaultStyle;
    FDefaultColor:=(Source as TD2Brush).FDefaultColor;
    FColor:=(Source as TD2Brush).SolidColor;
    FStyle:=(Source as TD2Brush).Style;
    case FStyle of
      d2BrushGradient:FGradient.Assign((Source as TD2Brush).Gradient);
      d2BrushResource:FResource.Assign((Source as TD2Brush).Resource);
      d2BrushVisual:FVisual.Assign((Source as TD2Brush).Visual);
      d2BrushBitmap:FBitmap.Assign((Source as TD2Brush).Bitmap);
    end;
    FOnChanged:=SaveChange;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
    inherited;
end;

procedure TD2Brush.GradientChanged(Sender:TObject);
begin
  if Assigned(FOnChanged) then FOnChanged(Self);
end;

procedure TD2Brush.VisualChanged(Sender:TObject);
begin
  if Assigned(FOnChanged) then FOnChanged(Self);
end;

procedure TD2Brush.ResourceChanged(Sender:TObject);
begin
  if Assigned(FOnChanged) then FOnChanged(Self);
end;

procedure TD2Brush.BitmapChanged(Sender:TObject);
begin
  if Assigned(FOnChanged) then FOnChanged(Self);
end;

function TD2Brush.isBitmapStored:boolean;
begin
  Result:=(FStyle=d2BrushBitmap);
end;

function TD2Brush.isColorStored:boolean;
begin
  Result:=(FStyle=d2BrushSolid) and (FColor <> FDefaultColor);
end;

function TD2Brush.isGradientStored:boolean;
begin
  Result:=FStyle=d2BrushGradient;
end;

function TD2Brush.isStyleStored:boolean;
begin
  Result:=FStyle <> FDefaultStyle;
end;

function TD2Brush.isVisualStored:boolean;
begin
  Result:=FStyle=d2BrushVisual;
end;

function TD2Brush.isResourceStored:boolean;
begin
  Result:=FStyle=d2BrushResource;
end;

procedure TD2Brush.SetResource(const Value:TD2BrushResource);
begin
  FResource:=Value;
end;

procedure TD2Brush.SetGradient(const Value:TD2Gradient);
begin
  FGradient.Assign(Value);
end;

procedure TD2Brush.SetVisual(const Value:TD2Visual);
begin
  FVisual:=Value;
end;

function TD2Brush.GetColor:string;
begin
  Result:=d2ColorToStr(FColor);
end;

procedure TD2Brush.SetColor(const Value:string);
begin
  SolidColor:=d2StrToColor(Value);
  if FStyle=d2BrushGradient then
    FGradient.Color:=Value;
end;

function TD2Brush.GetSolidColor:TD2Color;
begin
  Result:=FColor;
  if (Style=d2BrushResource) and (Resource <> nil) and (Resource.Brush <> nil) then
    Result:=Resource.Brush.SolidColor;
end;

procedure TD2Brush.SetSolidColor(const Value:TD2Color);
begin
  if FColor <> Value then
  begin
    FColor:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD2Brush.SetStyle(const Value:TD2BrushStyle);
begin
  if FStyle <> Value then
  begin
    FStyle:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

//=============== TD2BrushResource ==========================================================}

destructor TD2BrushResource.Destroy;
begin
  if FResource <> nil then
  begin
    FResource.RemoveFreeNotify(Self);
    FResource:=nil;
  end;
  inherited;
end;

procedure TD2BrushResource.Assign(Source:TPersistent);
begin
  if Source is TD2BrushResource then
  begin
    Resource:=TD2BrushResource(Source).Resource;
    FResourceName:=TD2BrushResource(Source).ResourceName;
  end
  else
    inherited;
end;

procedure TD2BrushResource.SetResource(const Value:TD2BrushObject);
begin
  if FResource <> Value then
  begin
    if FResource <> nil then
      FResource.RemoveFreeNotify(Self);
    FResource:=Value;
    if FResource <> nil then
    begin
      FResourceName:=FResource.ResourceName;
      FResource.AddFreeNotify(Self);
    end;
  end;
end;

function TD2BrushResource.GetResourceName:string;
begin
  Result:=FResourceName;
end;

procedure TD2BrushResource.SetResourceName(const Value:string);
begin
  if Value <> FResourceName then
  begin
    FResourceName:=Value;
  end;
end;

function TD2BrushResource.GetBrush:TD2Brush;
var
  O:TD2Object;
begin
  Result:=nil;
  if FResource <> nil then
  begin
    Result:=TD2BrushObject(FResource).Brush;
  end
  else
  if FResourceName <> '' then
  begin
    O:=FindResource(FResourceName);
    if O is TD2BrushObject then
      Resource:=TD2BrushObject(O);
    if FResource <> nil then
      Result:=TD2BrushObject(FResource).Brush;
  end;
end;

//=============== TD2BrushBitmap =============================================

constructor TD2BrushBitmap.Create;
begin
  inherited Create;
  FBitmap:=TD2Bitmap.Create(0,0);
end;

destructor TD2BrushBitmap.Destroy;
begin
  FBitmap.Free;
  inherited;
end;

procedure TD2BrushBitmap.Assign(Source:TPersistent);
begin
  if Source is TD2BrushBitmap then
  begin
    FWrapMode:=TD2BrushBitmap(Source).FWrapMode;
    FBitmap.Assign(TD2BrushBitmap(Source).Bitmap);
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
    inherited;
end;

procedure TD2BrushBitmap.SetWrapMode(const Value:TD2WrapMode);
begin
  if FWrapMode <> Value then
  begin
    FWrapMode:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD2BrushBitmap.SetBitmap(Value:TD2Bitmap);
begin
  if Value <> nil then
    FBitmap.Assign(Value);
end;
          
                       
