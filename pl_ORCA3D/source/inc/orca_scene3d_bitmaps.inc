{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//==================== TD3Bitmap ==========================================

constructor TD3Bitmap.Create(const AWidth,AHeight:integer);
begin
  inherited Create;
  FWidth:=AWidth;
  FHeight:=AHeight;
  FCanvasList:=TList.Create;
  Recreate;
end;

procedure TD3Bitmap.SetCanvas(ACanvas:TD3Canvas);
begin
  if FCanvas <> nil then
    FreeAndNil(FCanvas);
    
  FCanvas:=ACanvas;
end;

constructor TD3Bitmap.CreateFromStream(const AStream:TStream);
begin
  inherited Create;
  FCanvasList:=TList.Create;
  LoadFromStream(AStream);
end;

destructor TD3Bitmap.Destroy;
var
  i:integer;
begin
  for i:=FCanvasList.Count - 1 downto 0 do
    TD3Canvas(FCanvasList[i]).DestroyBitmap(Self);
  FreeAndNil(FCanvasList);
  if FCanvas <> nil then
    FreeAndNil(FCanvas);
  if FBits <> nil then
    FreeMem(FBits);
  inherited;
end;

procedure TD3Bitmap.SetSize(const AWidth,AHeight:integer);
begin
  if (FWidth <> AWidth) or (FHeight <> AHeight) then
  begin
    FWidth:=AWidth;
    FHeight:=AHeight;
    Recreate;
  end;
end;

procedure TD3Bitmap.Recreate;
var
  i:integer;
begin
  for i:=FCanvasList.Count - 1 downto 0 do
    TD3Canvas(FCanvasList[i]).DestroyBitmap(Self);
  if FCanvas <> nil then
    FreeAndNil(FCanvas);
  FCanvas:=nil;
  if FBits <> nil then
    FreeMem(FBits);
  { not empty - we can't create empty bitmap }
  if FWidth < 1 then FWidth:=0;
  if FHeight < 1 then FHeight:=0;
  { create and clear }
  GetMem(FBits,FWidth * FHeight * SizeOf(TD3Color));
  d2FillLongword(FBits,FWidth * FHeight,0);
  FNeedUpdate:=true;
end;

procedure TD3Bitmap.Clear(const AColor:TD3Color);
begin
  if FBits <> nil then
  begin
    d2FillLongword(FBits,FWidth * FHeight,AColor);
    FNeedUpdate:=true;
  end;
end;

procedure TD3Bitmap.FillAlpha(const AAlpha:byte=$FF);
begin
  if FBits <> nil then
  begin
    d2FillAlpha(FBits,FWidth * FHeight,AAlpha);
    FNeedUpdate:=true;
  end;
end;

procedure TD3Bitmap.ClearRect(const ARect:TD2Rect;
  const AColor:TD3Color);
begin
  if FBits <> nil then
  begin
    d2FillLongwordRect(FBits,FWidth,FHeight,Trunc(ARect.Left),Trunc(ARect.Top),Trunc(ARect.Right),Trunc(ARect.Bottom),AColor);
    FNeedUpdate:=true;
  end;
end;

function TD3Bitmap.LockBitmapBits(var Bits:PD3ColorArray; const Write:boolean):boolean;
begin
  Bits:=nil;
  if FBits <> nil then
  begin
    Bits:=FBits;
    if Write then
      FLocked:=true;
    Result:=true;
  end
  else
    Result:=false;
end;

procedure TD3Bitmap.UnlockBitmapBits;
begin
  if FLocked then
  begin
    FNeedUpdate:=true;
    if Assigned(FOnChange) then FOnChange(Self);
  end;
  FLocked:=false;
end;

procedure TD3Bitmap.AssignTo(Dest:TPersistent);
var
  i:integer;
begin

  if Dest is TD2Bitmap then
  begin
    TD2Bitmap(Dest).SetSize(FWidth,FHeight);
    for i:=0 to FHeight - 1 do
      { flip,because TD2Bitmap have diffirent alignment }
      {$IFDEF UNIX}
      System.Move(Bits[i * FWidth],TD2Bitmap(Dest).Scanline[FHeight - i - 1]^,Width * 4);
      {$ELSE}
      System.Move(Bits[i * FWidth],TD2Bitmap(Dest).Scanline[i]^,Width * 4);
      {$ENDIF}
    if Assigned(TD2Bitmap(Dest).OnChange) then
      TD2Bitmap(Dest).OnChange(Self);
  end
  else
    inherited ;
end;

procedure TD3Bitmap.Assign(Source:TPersistent);
var
  SLine:PD2ColorArray;
  SLine24:PD2Color24Array;
  i,j:integer;
begin
  if Source is TD3Canvas then
  begin
    SetSize(TD3Canvas(Source).FWidth,TD3Canvas(Source).FHeight);
    TD3Canvas(Source).CopyBits(FBits);
    FNeedUpdate:=true;
    if Assigned(FOnChange) then FOnChange(Self);
  end
  else
  if Source is TD3Bitmap then
  begin
    SetSize(TD3Bitmap(Source).Width,TD3Bitmap(Source).Height);
    d2MoveLongword(TD3Bitmap(Source).FBits,FBits,Width * Height);
    FNeedUpdate:=true;
    if Assigned(FOnChange) then FOnChange(Self);
  end
  else
  if Source is TD2Bitmap then
  begin
    SetSize(TD2Bitmap(Source).Width,TD2Bitmap(Source).Height);
    d2MoveLongword(TD2Bitmap(Source).Startline,FBits,Width * Height);
    FNeedUpdate:=true;
    if Assigned(FOnChange) then FOnChange(Self);
  end
  else
    inherited
end;

procedure TD3Bitmap.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('PNG',ReadBitmap,WriteBitmap,FWidth * FHeight > 0);
end;

procedure TD3Bitmap.ReadBitmap(Stream:TStream);
begin
  LoadFromStream(Stream);
end;

procedure TD3Bitmap.WriteBitmap(Stream:TStream);
begin
  SaveToStream(Stream);
end;

procedure TD3Bitmap.LoadFromFile(const AFileName:string;
  const Rotate:single);
var
  Filter:TD3Filter;
begin
  Filter:=GvarD3DefaultFilterClass.Create;
  if Filter.LoadFromFile(AFileName,Rotate,Self) then
    if Assigned(FOnChange) then FOnChange(Self);
  Filter.Free;
end;

procedure TD3Bitmap.LoadThumbnailFromFile(const AFileName:string;
  const AFitWidth,AFitHeight:single; const UseEmbedded:boolean);
var
  Filter:TD3Filter;
begin
  Filter:=GvarD3DefaultFilterClass.Create;
  if Filter.LoadThumbnailFromFile(AFileName,AFitWidth,AFitHeight,UseEmbedded,Self) then
    if Assigned(FOnChange) then FOnChange(Self);
  Filter.Free;
end;

procedure TD3Bitmap.SaveToFile(const AFileName:string; const Params:string);
var
  Filter:TD3Filter;
begin
  Filter:=GvarD3DefaultFilterClass.Create;
  Filter.SaveToFile(AFileName,Self,Params);
  Filter.Free;
end;

procedure TD3Bitmap.LoadFromStream(const AStream:TStream);
var
  Filter:TD3Filter;
begin
  Filter:=GvarD3DefaultFilterClass.Create;
  if Filter.LoadFromStream(AStream,Self) then
    if Assigned(FOnChange) then FOnChange(Self);
  Filter.Free;
end;

procedure TD3Bitmap.SaveToStream(const AStream:TStream);
var
  i:integer;
  hasAlpha:boolean;
  Filter:TD3Filter;
begin
  { check alpha }
  hasAlpha:=false;
  for i:=0 to FWidth * FHeight - 1 do
    if FBits[i] and $FF000000 <> $FF000000 then
    begin
      hasAlpha:=true;
      Break;
    end;

  if hasAlpha then
  begin
    Filter:=GvarD3DefaultFilterClass.Create;
    Filter.SaveToStream(AStream,Self,'png');
    Filter.Free;
  end
  else
  begin
    Filter:=GvarD3DefaultFilterClass.Create;
    Filter.SaveToStream(AStream,Self,'jpg','quality=100');
    Filter.Free;
  end;
end;

procedure TD3Bitmap.SetHeight(const Value:integer);
begin
  if FHeight <> Value then
  begin
    FHeight:=Value;
    Recreate;
  end;
end;

procedure TD3Bitmap.SetWidth(const Value:integer);
begin
  if FWidth <> Value then
  begin
    FWidth:=Value;
    Recreate;
  end;
end;

function TD3Bitmap.GetCanvas:TD3Canvas;
begin
  if FCanvas=nil then
  begin
    FCanvas:=GvarD3DefaultCanvasClass.CreateFromBitmap(Self);
  end;
  Result:=FCanvas;
end;     

//===================== TD3BitmapStream =============================

constructor TD3BitmapStream.Create(Collection:TCollection);
begin
  inherited;
  FBitmap:=TD3Bitmap.Create(0,0);
  if GvarD3BitmapList=nil then
    GvarD3BitmapList:=TStringList.Create;
  GvarD3BitmapList.AddObject('',Self);
end;

procedure TD3BitmapStream.Assign(Source:TPersistent);
begin
  inherited;
end;

destructor TD3BitmapStream.Destroy;
begin
  if GvarD3BitmapList <> nil then
    GvarD3BitmapList.Delete(GvarD3BitmapList.IndexOfObject(Self));
  FreeAndNil(FBitmap);
  inherited;
end;

function TD3BitmapStream.GetDisplayName:string;
begin
  Result:='(Bitmap)';
end;

procedure TD3BitmapStream.SetBitmap(const Value:TD3Bitmap);
begin
  if (Value <> nil) then
  begin
    FBitmap.Assign(Value);
  end;
end;

procedure TD3BitmapStream.SetName(const Value:string);
var
  Id3:integer;
begin
  if FName <> Value then
  begin
    FName:=Value;
    Id3:=GvarD3BitmapList.IndexOfObject(Self);
    if Id3 >= 0 then
      GvarD3BitmapList[Id3]:=Name;
  end;
end;

//=================== TD3BitmapCollection ===============================

constructor TD3BitmapCollection.Create(AOwner:TD3BitmapList);
begin
  inherited Create(TD3BitmapStream);
end;

destructor TD3BitmapCollection.Destroy;
begin
  inherited;
end;

//============= TD3BitmapList =================================

constructor TD3BitmapList.Create(AOwner:TComponent);
begin
  inherited;
  FBitmaps:=TD3BitmapCollection.Create(Self);
end;

destructor TD3BitmapList.Destroy;
begin
  FBitmaps.Free;
  inherited;
end;        
     
//============ TD3BitmapObject =====================================

constructor TD3BitmapObject.Create(AOwner:TComponent);
begin
  inherited;
  FBitmap:=TD3Bitmap.Create(0,0);
  if GvarD3BitmapList=nil then
    GvarD3BitmapList:=TStringList.Create;
  GvarD3BitmapList.AddObject('',Self);
end;

destructor TD3BitmapObject.Destroy;
begin
  if GvarD3BitmapList <> nil then
    GvarD3BitmapList.Delete(GvarD3BitmapList.IndexOfObject(Self));
  FBitmap.Free;
  inherited;
end;

procedure TD3BitmapObject.SetBitmap(const Value:TD3Bitmap);
begin
  FBitmap.Assign(Value);
end;

procedure TD3BitmapObject.SetResourceName(const Value:string);
var
  Id3:integer;
begin
  if FResourceName <> Value then
  begin
    inherited ;
    if FResourceName <> '' then
    begin
      Id3:=GvarD3BitmapList.IndexOfObject(Self);
      if Id3 >= 0 then
        GvarD3BitmapList[Id3]:=Value;
    end;
  end;
end;

procedure TD3BitmapObject.SetName(const NewName:TComponentName);
var
  Id3:integer;
begin
  if Name <> NewName then
  begin
    inherited SetName(NewName);
    Id3:=GvarD3BitmapList.IndexOfObject(Self);
    if Id3 >= 0 then
      GvarD3BitmapList[Id3]:=Name;
  end;
end;

//==================== TD3BitmapRect ===========================================

constructor TD3BitmapRect.Create;
begin
  inherited Create;
end;

procedure TD3BitmapRect.Assign(Source:TPersistent);
begin
  if Source is TD3BitmapRect then
  begin
    FLeft:=TD3BitmapRect(Source).FLeft;
    FTop:=TD3BitmapRect(Source).FTop;
    FRight:=TD3BitmapRect(Source).FRight;
    FBottom:=TD3BitmapRect(Source).FBottom;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end
  else
    inherited;
end;

function TD3BitmapRect.empty:boolean;
begin
  Result:=(FLeft=0) and (FTop=0) and (FRight=0) and (FBottom=0);
end;

procedure TD3BitmapRect.SetBottom(const Value:single);
begin
  if FBottom <> Value then
  begin
    FBottom:=Value;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD3BitmapRect.SetLeft(const Value:single);
begin
  if FLeft <> Value then
  begin
    FLeft:=Value;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD3BitmapRect.SetRight(const Value:single);
begin
  if FRight <> Value then
  begin
    FRight:=Value;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD3BitmapRect.SetTop(const Value:single);
begin
  if FTop <> Value then
  begin
    FTop:=Value;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;
      
         
           
