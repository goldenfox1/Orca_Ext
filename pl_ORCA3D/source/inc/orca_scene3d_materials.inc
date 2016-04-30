{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//================ TD3Material ========================================

constructor TD3Material.Create;
begin
  inherited ;
  FBitmapRect:=TD3BitmapRect.Create;
  FDiffuse:=$FF707070;
  FAmbient :=$FF303030;
  FLighting:=true;
  FShadeMode:=d3Gouraud;
  FBitmapTile:=1;
  FBitmapTileY:=1;
end;

destructor TD3Material.Destroy;
begin
  FBitmapRect.Free;
  inherited;
end;

procedure TD3Material.Assign(Source:TPersistent);
begin
  if Source is TD3Material then
  begin
    FDiffuse:=(Source as TD3Material).FDiffuse;
    FAmbient:=(Source as TD3Material).FAmbient;
    FBitmap:=(Source as TD3Material).FBitmap;
    FBitmapTile:=(Source as TD3Material).FBitmapTile;
    FBitmapTileY:=(Source as TD3Material).FBitmapTileY;
    FBitmapRect.Assign((Source as TD3Material).FBitmapRect);
    FLighting:=(Source as TD3Material).FLighting;
    FModulation:=(Source as TD3Material).FModulation;
    FShadeMode:=(Source as TD3Material).FShadeMode;
    FFillMode:=(Source as TD3Material).FFillMode;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end
  else
    inherited
end;

function TD3Material.GetBitmap:TD3Bitmap;
var
  Id3:integer;
begin
  if FTempBitmap <> nil then
    Result:=FTempBitmap
  else
  begin
    if (GvarD3BitmapList <> nil) and (FBitmap <> '') then
    begin
      Id3:=GvarD3BitmapList.IndexOf(FBitmap);
      if Id3 >= 0 then
      begin
        if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapStream then
          Result:=TD3BitmapStream(GvarD3BitmapList.Objects[Id3]).Bitmap
        else
          if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapObject then
            Result:=TD3BitmapObject(GvarD3BitmapList.Objects[Id3]).Bitmap
          else
            if TObject(GvarD3BitmapList.Objects[Id3]) is TD3CustomBufferLayer then
              Result:=TD3CustomBufferLayer(GvarD3BitmapList.Objects[Id3]).FBuffer
      end
      else
        Result:=nil;
    end
    else
      Result:=nil;
  end;
end;

procedure TD3Material.SetModulation(const Value:TD3TexMode);
begin
  if FModulation <> Value then
  begin
    FModulation:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetLighting(const Value:boolean);
begin
  if FLighting <> Value then
  begin
    FLighting:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetBitmap(const Value:string);
begin
  if FBitmap <> Value then
  begin
    FBitmap:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetTempBitmap(const Value:TD3Bitmap);
begin
  if FTempBitmap <> Value then
  begin
    FTempBitmap:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

function TD3Material.GetAmbient:string;
begin
  Result:=d3ColorToStr(FAmbient);
end;

procedure TD3Material.SetAmbient(const Value:string);
begin
  if FAmbient <> d3StrToColor(Value) then
  begin
    FAmbient:=d3StrToColor(Value);
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

function TD3Material.GetDiffuse:string;
begin
  Result:=d3ColorToStr(FDiffuse);
end;

procedure TD3Material.SetNativeDiffuse(const Value:TD3Color);
begin
  if FDiffuse <> Value then
  begin
    FDiffuse:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetNativeAmbient(const Value:TD3Color);
begin
  if FAmbient <> Value then
  begin
    FAmbient:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetBitmapTile(const Value:single);
begin
  if FBitmapTile <> Value then
  begin
    FBitmapTile:=Value;
    if FBitmapTile < 0.01 then
      FBitmapTile:=0.01;
    if FBitmapTile > 10 then
      FBitmapTile:=10;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetBitmapTileY(const Value:single);
begin
  if FBitmapTileY <> Value then
  begin
    FBitmapTileY:=Value;
    if FBitmapTileY < 0.01 then
      FBitmapTileY:=0.01;
    if FBitmapTileY > 10 then
      FBitmapTileY:=10;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetShadeMode(const Value:TD3ShadeMode);
begin
  if FShadeMode <> Value then
  begin
    FShadeMode:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetFillMode(const Value:TD3FillMode);
begin
  if FFillMode <> Value then
  begin
    FFillMode:=Value;
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetDiffuse(const Value:string);
begin
  if FDiffuse <> d3StrToColor(Value) then
  begin
    FDiffuse:=d3StrToColor(Value);
    if Assigned(FOnChanged) then
      FOnChanged(Self);
  end;
end;

procedure TD3Material.SetBitmapRect(const Value:TD3BitmapRect);
begin
  FBitmapRect.Assign(Value);
end;
       
         
           
